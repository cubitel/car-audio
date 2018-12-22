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
#include "cars/api.h"
#include "radio/api.h"
#include "hardware.h"

#define signal2D8_Volume        8, 5

#define FLAG_VOLUME_CHANGED     1

static uint32_t flags;
static uint8_t ignitionKey;
static uint32_t engineRpm;

static void cbCanInit(can_t *can1, can_t *can2)
{
}

static void cbCanRecv(uint32_t id, uint8_t *data, uint8_t len)
{
	switch (id) {
	case 0x140:
		engineRpm = (data[6] & 0x1F) << 8 | data[7];
		break;
	case 0x285:
		ignitionKey = data[0] >> 4;
		break;
	default:
		break;
	}
}

static void cbCanSendTimer(uint8_t slot, can_t *can1, can_t *can2)
{
	uint8_t buf[10];

	/* Const packets, 1 Hz */
	if (slot == 0) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x02;
		canSend(can1, 0x2E3, buf, 1);
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x22;
		buf[1] = 0x93;
		buf[2] = 0x4C;
		buf[3] = 0x20;
		canSend(can1, 0x4D0, buf, 4);
	}

	/* Radio station info, 1 Hz */
	if (slot == 1) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x53;
		buf[1] = 0x00;
		buf[2] = 0x7B;
		buf[3] = 0x92;
		buf[4] = 0x82;
		buf[5] = 0x16;
		buf[6] = 0x48;
		canSend(can1, 0x2D5, buf, 7);
	}

	/* Radio station number */
	if (slot == 2) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x87;
		canSend(can1, 0x2DC, buf, 8);
	}

	/* Volume info, 1 Hz */
	if ( (slot == 3) || (flags & FLAG_VOLUME_CHANGED) ) {
		flags &= ~FLAG_VOLUME_CHANGED;
		memset(buf, 0, sizeof(buf));
		buf[0] = 0xC8;
		buf[1] = 0x02;
		buf[2] = 0x17;
		canSetSignal(buf, signal2D8_Volume, aoutGetVolume() / 4);
		canSend(can1, 0x2D8, buf, 3);
	}

	/* RDS */
	if (slot == 4) {
		memset(buf, 0, sizeof(buf));
		radioGetData(RADIO_DATA_RDS_PS_NAME, buf, 9);
		canSend(can1, 0x1E9, buf, 8);
	}

	if (slot == 5) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x80;
		buf[1] = 0x0C;
		buf[2] = 0xE8;
		buf[3] = 0x00;
		canSend(can1, 0x1E6, buf, 5);
	}

/*
	if (slot == 6) {
		memset(buf, 0, sizeof(buf));
		buf[1] = 0xFF;
		buf[2] = 0xFF;
		canSend(can1, 0x2D9, buf, 8);
	}

	if (slot == 6) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x3F;
		buf[1] = 0x2A;
		buf[2] = 0x02;
		buf[3] = 0x27;
		canSend(can1, 0x2DA, buf, 5);
	}

	if (slot == 7) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0xFF;
		buf[1] = 0xFF;
		buf[2] = 0xFF;
		buf[3] = 0xFF;
		canSend(can1, 0x2DB, buf, 8);
	}

	if (slot == 7) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x03;
		canSend(can1, 0x2DE, buf, 8);
	}

	if (slot == 8) {
		memset(buf, 0, sizeof(buf));
		buf[0] = 0x11;
		buf[1] = 0x12;
		buf[2] = 0x00;
		canSend(can1, 0x50C, buf, 3);
	}
*/
}

static void cbEvent(uint32_t eventId)
{
	switch (eventId) {
	case CAREVENT_VOLUME_CHANGED:
		flags |= FLAG_VOLUME_CHANGED;
		break;
	}
}

static int cbIsActive(void)
{
	return ignitionKey > 0;
}

static int cbGetData(uint32_t type, uint8_t *buf, uint32_t len)
{
	switch (type) {
	case CAR_DATA_ENGINE_RPM:
		if (len < 4) break;
		memcpy(buf, &engineRpm, 4);
		return 1;
	default:
		break;
	}
	return 0;
}

static const car_t ford = {
	cbCanInit,
	cbCanRecv,
	cbCanSendTimer,
	cbEvent,
	cbIsActive,
	cbGetData
};

const car_t *ford_cd3xx()
{
	return &ford;
}
