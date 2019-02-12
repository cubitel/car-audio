/* Copyright (c) 2019 Vasily Voropaev <vvg@cubitel.org>
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
#include "hardware.h"

#define MFBTN_PREV              0x020000
#define MFBTN_NEXT              0x010000
#define MFBTN_VOLUP             0x100000
#define MFBTN_VOLDN             0x200000
#define MFBTN_PICKUP            0x400000
#define MFBTN_HANGUP            0x800000
#define MFBTN_SPKON             0x000001
#define MFBTN_SPKOFF            0x000002

static uint32_t prevButtons;
static uint8_t carActive;

static void cbCanInit(can_t *can1, can_t *can2)
{
}

static void cbCanRecv(uint32_t id, uint8_t *data, uint8_t len)
{
	switch (id) {
	case 0:
		if (data[0] >= 3) carActive = 1;
		if (data[0] == 0) carActive = 0;
		break;

	case 0x1CA:
		{
			uint32_t buttons = (data[1] << 16) | (data[3]);
			uint32_t pressedButtons = (prevButtons ^ buttons) & buttons;
			prevButtons = buttons;

			if (pressedButtons & MFBTN_VOLUP) {
				hwSendRequest(HWRQ_SYSTEM_KEYPRESS, KEY_VOLUME_UP);
			}
			if (pressedButtons & MFBTN_VOLDN) {
				hwSendRequest(HWRQ_SYSTEM_KEYPRESS, KEY_VOLUME_DOWN);
			}
			if (pressedButtons & MFBTN_PREV) {
				hwSendRequest(HWRQ_SYSTEM_KEYPRESS, KEY_SEEK_DOWN);
			}
			if (pressedButtons & MFBTN_NEXT) {
				hwSendRequest(HWRQ_SYSTEM_KEYPRESS, KEY_SEEK_UP);
			}
			if (pressedButtons & MFBTN_SPKON) {
				hwSendRequest(HWRQ_SYSTEM_KEYPRESS, KEY_MODE);
			}
		}
		break;
	default:
		break;
	}
}

static void cbCanSendTimer(uint8_t slot, can_t *can1, can_t *can2)
{
}

static void cbEvent(uint32_t eventId)
{
	switch (eventId) {
	}
}

static int cbIsActive(void)
{
	return carActive;
}

static int cbGetData(uint32_t type, uint8_t *buf, uint32_t len)
{
	switch (type) {
	default:
		break;
	}
	return 0;
}

static const car_t w164 = {
	cbCanInit,
	cbCanRecv,
	cbCanSendTimer,
	cbEvent,
	cbIsActive,
	cbGetData
};

const car_t *mercedes_w164()
{
	return &w164;
}
