/* Copyright (c) 2018 Vasily Voropaev <vvg@cubitel.org>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <FreeRTOS.h>
#include <task.h>
#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>
#include <string.h>
#include <stdlib.h>

#include "netserv.h"
#include "hardware.h"
#include "sha1.h"
#include "main.pb.h"
#include "pb_decode.h"
#include "pb_encode.h"
#include "radio/api.h"
#include "cars/api.h"
#include "config.h"

typedef struct {
	Socket_t socket;
	/* RX circular buffer */
	uint16_t rxStart;
	uint16_t rxEnd;
	uint8_t rxBuffer[256];
	/* RX line */
	uint16_t rxLineSize;
	uint8_t rxLine[256];
	/* HTTP */
	uint8_t httpRequest[256];
	uint32_t httpBodySize;
	uint8_t httpBody[1024];
	/* RPC */
	Main_RPCRequest request;
	Main_RPCResponse response;
} client_t;

static int clientRecvLine(client_t *client)
{
	client->rxLineSize = 0;

	while (1) {
		if (client->rxStart == client->rxEnd) {
			BaseType_t len = FreeRTOS_recv(client->socket, &client->rxBuffer[client->rxEnd], sizeof(client->rxBuffer) - client->rxEnd, 0);
			if (len <= 0) return -1;
			client->rxEnd += len;
			if (client->rxEnd >= sizeof(client->rxBuffer)) client->rxEnd = 0;
		}

		uint8_t byte = client->rxBuffer[client->rxStart++];
		if (client->rxStart >= sizeof(client->rxBuffer)) client->rxStart = 0;

		if (byte == '\n') {
			client->rxLine[client->rxLineSize] = 0;
			return client->rxLineSize;
		} else {
			if (byte < 32) continue;
			if (client->rxLineSize < (sizeof(client->rxLine)-2)) {
				client->rxLine[client->rxLineSize++] = byte;
			}
		}
	}

	return 0;
}

static int clientRecvData(client_t *client, uint8_t *data, uint32_t size)
{
	/* Copy remaining data in circular buffer, if any */
	while (size && (client->rxStart != client->rxEnd)) {
		*data++ = client->rxBuffer[client->rxStart++];
		if (client->rxStart >= sizeof(client->rxBuffer)) client->rxStart = 0;
		size--;
	}

	while (size > 0) {
		BaseType_t len = FreeRTOS_recv(client->socket, data, size, 0);
		if (len <= 0) return -1;

		data += len;
		size -= len;
	}

	return 1;
}

static void clientHandleRequest(client_t *client)
{
	if (client->request.has_requestId) {
		client->response.has_requestId = true;
		client->response.requestId = client->request.requestId;
	}

	switch (client->request.which_request) {
	case Main_RPCRequest_audio_tag:
		if (client->request.request.audio.has_volume) {
			hwSendRequest(HWRQ_AOUT_VOLUME_SET, client->request.request.audio.volume);
		}
		if (client->request.request.audio.has_muteTuner) {
			hwSendRequest(HWRQ_RADIO_MUTE, client->request.request.audio.muteTuner);
		}
		break;

	case Main_RPCRequest_tuner_tag:
		if (client->request.request.tuner.has_frequency) {
			hwSendRequest(HWRQ_RADIO_TUNE, client->request.request.tuner.frequency / 10);
		}
		break;

	default:
		break;
	}
}

static void httpSendResponse(client_t *client, uint16_t code)
{
	static const char *http200 = "HTTP/1.0 200 OK\r\n";
	static const char *http400 = "HTTP/1.0 400 Bad request\r\n";
	static const char *http404 = "HTTP/1.0 404 Not found\r\n";
	static const char *http500 = "HTTP/1.0 500 Internal server error\r\n";
	static const char *httpHeaders = "Access-Control-Allow-Origin: *\r\nConnection: close\r\n\r\n";

	switch (code) {
	case 200:
		FreeRTOS_send(client->socket, http200, strlen(http200), 0);
		break;
	case 400:
		FreeRTOS_send(client->socket, http400, strlen(http400), 0);
		break;
	case 404:
		FreeRTOS_send(client->socket, http404, strlen(http404), 0);
		break;
	default:
		FreeRTOS_send(client->socket, http500, strlen(http500), 0);
	}

	FreeRTOS_send(client->socket, httpHeaders, strlen(httpHeaders), 0);
}

static void httpClientTask(void *p)
{
	int i;
	uint8_t do_update = 0;
	static const TickType_t xReceiveTimeOut = 10000;
	Socket_t sock = (Socket_t)p;

	client_t *client = pvPortMalloc(sizeof(client_t));
	if (!client) {
		FreeRTOS_closesocket(sock);
		vTaskDelete(NULL);
		return;
	}

	do {
		/* Initialize client structure */
		memset(client, 0, sizeof(client_t));
		client->socket = sock;

		/* Set socket timeout */
		FreeRTOS_setsockopt(sock, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof(xReceiveTimeOut));

		/* Read first line */
		if (clientRecvLine(client) < 0) break;

		int portion = 0;
		for (i = 0; i < client->rxLineSize; i++) {
			if (client->rxLine[i] == 0x20) {
				portion++;
				if (portion == 2) {
					client->httpRequest[i] = 0;
					break;
				}
			}
			client->httpRequest[i] = client->rxLine[i];
		}

		/* Read headers */
		while (clientRecvLine(client) > 0) {
			if (strtok((char *)client->rxLine, ":") == NULL) continue;
			char *value = strtok(NULL, ":");
			if (value == NULL) continue;
			while (*value == ' ') value++;

			if (!strcasecmp((char *)client->rxLine, "Content-Length")) {
				client->httpBodySize = atoi(value);
				continue;
			}
		}

		/* Dispatch request */

		if (!strcmp((char *)client->httpRequest, "PUT /firmware")) {
			if ( (client->httpBodySize < 1024) || (client->httpBodySize > 65536) ) {
				httpSendResponse(client, 400);
				break;
			}

			uint8_t *ccm = (uint8_t *)0x10000000;
			if (clientRecvData(client, ccm, client->httpBodySize) <= 0) {
				httpSendResponse(client, 500);
				break;
			}

			SHA1_CTX ctx;
			uint8_t sha1sum[20];
			sha1_init(&ctx);
			sha1_update(&ctx, ccm, client->httpBodySize-20);
			sha1_final(&ctx, sha1sum);

			if (memcmp(sha1sum, &ccm[client->httpBodySize-20], 20)) {
				httpSendResponse(client, 400);
				break;
			}

			do_update = 1;
			httpSendResponse(client, 200);
			break;
		}

		if (!strcmp((char *)client->httpRequest, "POST /control")) {
			if (client->httpBodySize > sizeof(client->httpBody)) {
				httpSendResponse(client, 400);
				break;
			}
			if (clientRecvData(client, client->httpBody, client->httpBodySize) <= 0) {
				httpSendResponse(client, 500);
				break;
			}

			pb_istream_t istream = pb_istream_from_buffer(client->httpBody, client->httpBodySize);
			if (!pb_decode(&istream, Main_RPCRequest_fields, &client->request)) {
				httpSendResponse(client, 400);
				break;
			}

			clientHandleRequest(client);

			pb_ostream_t ostream = pb_ostream_from_buffer(client->httpBody, sizeof(client->httpBody));
			pb_encode(&ostream, Main_RPCResponse_fields, &client->response);

			httpSendResponse(client, 200);
			FreeRTOS_send(client->socket, client->httpBody, ostream.bytes_written, 0);
			break;
		}

		if (!strcmp((char *)client->httpRequest, "GET /audio/volume/up")) {
			hwSendRequest(HWRQ_AOUT_VOLUME_UP, 0);
			httpSendResponse(client, 200);
			break;
		}
		if (!strcmp((char *)client->httpRequest, "GET /audio/volume/down")) {
			hwSendRequest(HWRQ_AOUT_VOLUME_DOWN, 0);
			httpSendResponse(client, 200);
			break;
		}
		if (!strcmp((char *)client->httpRequest, "GET /tuner/seek/up")) {
			hwSendRequest(HWRQ_RADIO_SEEK, 1);
			httpSendResponse(client, 200);
			break;
		}
		if (!strcmp((char *)client->httpRequest, "GET /tuner/seek/down")) {
			hwSendRequest(HWRQ_RADIO_SEEK, 0);
			httpSendResponse(client, 200);
			break;
		}
		if (!strcmp((char *)client->httpRequest, "GET /system/can/dump_enable")) {
			hwSendRequest(HWRQ_SYSTEM_CANDUMP, 1);
			httpSendResponse(client, 200);
			break;
		}
		if (!strcmp((char *)client->httpRequest, "GET /system/can/dump_disable")) {
			hwSendRequest(HWRQ_SYSTEM_CANDUMP, 0);
			httpSendResponse(client, 200);
			break;
		}

		httpSendResponse(client, 404);
	} while (0);

	/* Shutdown and close socket */
	if (FreeRTOS_shutdown(sock, FREERTOS_SHUT_RDWR) == 0) {
		for (i = 0; i < 100; i++) {
			if (FreeRTOS_recv(client->socket, client->rxBuffer, sizeof(client->rxBuffer), 0) == FREERTOS_EINVAL) break;
			vTaskDelay(5);
		}
	}
	FreeRTOS_closesocket(sock);

	if (do_update) {
		vTaskDelay(100);
		flashUpdateFirmware((uint8_t *)0x10000000, client->httpBodySize);
	}

	vPortFree(client);
	vTaskDelete(NULL);
}

void netservTask(void *p)
{
	struct freertos_sockaddr bindAddress;
	static const TickType_t xReceiveTimeOut = 10000;

	/* HTTP server */

	Socket_t sockHTTP = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_STREAM, FREERTOS_IPPROTO_TCP);
	FreeRTOS_setsockopt(sockHTTP, 0, FREERTOS_SO_RCVTIMEO, &xReceiveTimeOut, sizeof(xReceiveTimeOut));

	bindAddress.sin_port = FreeRTOS_htons(80);
	FreeRTOS_bind(sockHTTP, &bindAddress, sizeof(bindAddress));
	FreeRTOS_listen(sockHTTP, 4);

	for (;;) {
		struct freertos_sockaddr clientAddress;
		socklen_t sockSize = sizeof(clientAddress);
		Socket_t httpClient = FreeRTOS_accept(sockHTTP, &clientAddress, &sockSize);
		if ( (httpClient != FREERTOS_INVALID_SOCKET) && (httpClient != NULL) ) {
			xTaskCreate(httpClientTask, "HTTP client", 256, httpClient, 1, NULL);
		}
	}
}

uint32_t netservMakeStatusMessage(uint8_t *buf, uint32_t size)
{
	Main_StatusUpdate *msg;
	uint8_t value[8];

	msg = pvPortMalloc(sizeof(Main_StatusUpdate));
	if (!msg) return 0;
	memset(msg, 0, sizeof(Main_StatusUpdate));

	msg->has_audio = true;
	msg->audio.has_volume = true;
	msg->audio.volume = aoutGetVolume();
	msg->audio.has_muteTuner = true;
	msg->audio.muteTuner = aoutGetRadioMute();

	msg->has_tuner = true;
	msg->tuner.has_frequency = true;
	msg->tuner.frequency = configData()->tuner.frequency;
	if (radioGetData(RADIO_DATA_RSSI, (uint8_t *)&msg->tuner.rssi, 1)) {
		msg->tuner.has_rssi = true;
	}
	if (radioGetData(RADIO_DATA_SNR, (uint8_t *)&msg->tuner.snr, 1)) {
		msg->tuner.has_snr = true;
	}
	if (radioGetData(RADIO_DATA_STEREO, value, 1)) {
		msg->tuner.has_stereo = true;
		msg->tuner.stereo = (value[0] != 0);
	}
	if (radioGetData(RADIO_DATA_RDS_PS_NAME, (uint8_t *)msg->tuner.rdsPsName, 9)) {
		msg->tuner.has_rdsPsName = true;
	}
	if (radioGetData(RADIO_DATA_RDS_GROUPS, (uint8_t *)&msg->tuner.rdsGroups, 4)) {
		msg->tuner.has_rdsGroups = true;
	}

	msg->has_system = true;
	msg->system.has_active = true;
	msg->system.active = hwGetMode();
	msg->system.has_bootCause = true;
	msg->system.bootCause = hwGetBootCause();

	uint32_t faultAddress = *((uint32_t *)RAM_ADDRESS_VALUE);
	if (faultAddress) {
		msg->system.has_faultAddress = true;
		msg->system.faultAddress = faultAddress;
	}

	msg->has_engine = true;
	if (carGetData(CAR_DATA_ENGINE_RPM, (uint8_t *)&msg->engine.rpm, 4)) {
		msg->engine.has_rpm = true;
	}

	pb_ostream_t stream = pb_ostream_from_buffer(buf, size);
	pb_encode(&stream, Main_StatusUpdate_fields, msg);

	vPortFree(msg);
	return stream.bytes_written;
}
