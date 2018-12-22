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

#include <string.h>
#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>
#include <semphr.h>
#include <FreeRTOS_IP.h>
#include <FreeRTOS_IP_Private.h>
#include <NetworkBufferManagement.h>
#include <NetworkInterface.h>

#include "usb_ecm.h"
#include "usbd_conf.h"
#include "usb_dcd.h"

static xSemaphoreHandle semInterfaceRecv;
static uint8_t *recvData;
static uint32_t recvLen;

/***************************************************************/
/* USB ethernet interface                                      */
/***************************************************************/

static void *eth_dev;
static uint8_t ecmReceiveBuffer[1514+ETH_DATA_EP_SIZE];
static uint32_t ecmReceivePosition;
static uint8_t ecmNotify[8];

static void ethSendNotify(void *pdev)
{
	ecmNotify[0] = 0xA1;
	ecmNotify[1] = 0;
	ecmNotify[2] = 1;
	ecmNotify[3] = 0;
	ecmNotify[4] = 0;
	ecmNotify[5] = 0;
	ecmNotify[6] = 0;
	ecmNotify[7] = 0;
	DCD_EP_Tx(pdev, ETH_NOTIFICATION_EP, ecmNotify, 8);
}

void ethControl(void *pdev, uint8_t *buf, uint32_t len)
{
	eth_dev = pdev;
	ethSendNotify(pdev);
}

uint16_t ethControlIn(void *pdev, uint8_t *buf, uint32_t maxlen)
{
	return 0;
}

void ecmDataRx(void *pdev, uint8_t epnum, uint32_t len)
{
	if (len == ETH_DATA_EP_SIZE) {
		/* Full size packet -- more data available */
		ecmReceivePosition += len;
		if ((ecmReceivePosition+ETH_DATA_EP_SIZE) > sizeof(ecmReceiveBuffer)) {
			ecmReceivePosition = 0;
		}
		DCD_EP_PrepareRx(pdev, epnum, &ecmReceiveBuffer[ecmReceivePosition], ETH_DATA_EP_SIZE);
		return;
	}

	recvData = ecmReceiveBuffer;
	recvLen = ecmReceivePosition + len;
	if (recvLen > 0) {
		xSemaphoreGiveFromISR(semInterfaceRecv, NULL);
	}

	/* TODO: Make sure the packet is handled by receive task before receiving next one */
	ecmReceivePosition = 0;
	DCD_EP_PrepareRx(pdev, epnum, ecmReceiveBuffer, ETH_DATA_EP_SIZE);
	return;
}

void ecmDataTxCompleted(void *pdev, uint8_t epnum)
{
	return;
}

void ecmEnable()
{
}

void ecmDisable()
{
	eth_dev = NULL;
}

/***************************************************************/
/* local ethernet interface                                    */
/***************************************************************/

static void taskInterfaceRecv(void *p)
{
	xNetworkBufferDescriptor_t *pxDescriptor;
	const TickType_t xDescriptorWaitTime = pdMS_TO_TICKS(250);
	xIPStackEvent_t xRxEvent = { eNetworkRxEvent, NULL };

	vSemaphoreCreateBinary(semInterfaceRecv);

	for (;;) {
		xSemaphoreTake(semInterfaceRecv, portMAX_DELAY);

		pxDescriptor = pxGetNetworkBufferWithDescriptor(recvLen, xDescriptorWaitTime);
		if (pxDescriptor) {

			memcpy(pxDescriptor->pucEthernetBuffer, recvData, recvLen);

			/* Send descriptor to network stack */
			xRxEvent.pvData = (void *)pxDescriptor;
			if (xSendEventStructToIPTask(&xRxEvent, xDescriptorWaitTime) == pdFALSE) {
				vReleaseNetworkBufferAndDescriptor(pxDescriptor);
			}
		}
	}
}

BaseType_t xNetworkInterfaceInitialise(void)
{
	xTaskCreate(taskInterfaceRecv, "ECM Receive", 256, NULL, configMAX_PRIORITIES - 1, NULL);
	return pdPASS;
}

BaseType_t xNetworkInterfaceOutput(xNetworkBufferDescriptor_t * const pxDescriptor, BaseType_t bReleaseAfterSend)
{
	if (eth_dev) {
		taskENTER_CRITICAL();
		DCD_EP_Tx(eth_dev, ETH_IN_EP, pxDescriptor->pucEthernetBuffer, pxDescriptor->xDataLength);
		if ((pxDescriptor->xDataLength % 64) == 0) {
			DCD_EP_Tx(eth_dev, ETH_IN_EP, pxDescriptor->pucEthernetBuffer, 0);
		}
		taskEXIT_CRITICAL();
	}

	if (bReleaseAfterSend != pdFALSE) {
		vReleaseNetworkBufferAndDescriptor(pxDescriptor);
	}

	return pdPASS;
}
