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
#include <timers.h>

#include "hardware.h"
#include "hw/can.h"
#include "cars/api.h"

#define HEXCHAR(b)              (((b) & 0xF) < 10) ? (0x30 | ((b) & 0xF)) : (55 + ((b) & 0xF))

static can_t can1;
static can_t can2;
static TimerHandle_t canTimer;

static void canGoInit(can_t *can)
{
	can->device->MCR &= ~CAN_MCR_SLEEP;
	can->device->MCR |= CAN_MCR_INRQ;
	can->flags |= CAN_FLAG_WAIT_INIT;
}

static void canConfigureDevice(can_t *can)
{
	can->device->IER = 0;
	can->device->MCR = CAN_MCR_NART | CAN_MCR_ABOM | CAN_MCR_AWUM | CAN_MCR_INRQ | CAN_MCR_RFLM;

	/* Calculated on http://www.bittiming.can-wiki.info/ */
	switch (can->speed) {
	case CAN_SPEED_1000:
		can->device->BTR = 0x001a0002;
		break;
	case CAN_SPEED_500:
		can->device->BTR = 0x001a0005;
		break;
	case CAN_SPEED_250:
		can->device->BTR = 0x001a000b;
		break;
	case CAN_SPEED_125:
		can->device->BTR = 0x001a0017;
		break;
	}

	if ( (can->mode == CAN_MODE_AUTOBAUD) || (can->mode == CAN_MODE_LISTEN) ) {
		can->device->BTR |= CAN_BTR_SILM;
	}

	/* Configure filters */
	CAN1->FMR = (1 << 8) | CAN_FMR_FINIT;
	CAN1->FM1R = 0;
	CAN1->FS1R = 0xFFFFFFFF;
	CAN1->sFilterRegister[0].FR1 = 0;
	CAN1->sFilterRegister[0].FR2 = 0;
	CAN1->sFilterRegister[1].FR1 = 0;
	CAN1->sFilterRegister[1].FR2 = 0;
	CAN1->FA1R = 3;
	CAN1->FMR &= ~CAN_FMR_FINIT;

	/* Clear init request and wait for a sync */
	can->device->MCR &= ~CAN_MCR_INRQ;
	can->flags |= CAN_FLAG_WAIT_NORMAL;
}

static void canSetSpeed(can_t *can, uint32_t speed)
{
	can->speed = speed;
	canGoInit(can);
}

static void canSetNextSpeed(can_t *can)
{
	uint32_t speed = can->speed + 1;
	if (speed > CAN_SPEED_MAXID) speed = 0;
	canSetSpeed(can, speed);
}

static void canProcessTimers(can_t *can, uint32_t msec)
{
	if (can->flags & CAN_FLAG_WAIT_INIT) {
		if ( !(can->device->MSR & CAN_MSR_INAK) ) return;
		can->flags &= ~CAN_FLAG_WAIT_INIT;
		canConfigureDevice(can);
	}

	if (can->flags & CAN_FLAG_WAIT_NORMAL) {
		if ( (can->device->MSR & CAN_MSR_INAK) ) return;
		can->flags &= ~CAN_FLAG_WAIT_NORMAL;
		can->rxCount = 0;
	}

	switch (can->mode) {
	case CAN_MODE_AUTOBAUD:
		can->autobaudTimer += msec;
		if (can->autobaudTimer > CAN_AUTOBAUD_TIMEOUT) {
			canSetNextSpeed(can);
			can->autobaudTimer = 0;
		}
		if (can->rxCount > 0) {
			can->mode = CAN_MODE_NORMAL;
			can->rxTimer = 0;
			/* Reconfigure CAN to enable normal mode */
			canSetSpeed(can, can->speed);
		}
		break;
	case CAN_MODE_LISTEN:
	case CAN_MODE_NORMAL:
		if (can->flags & CAN_FLAG_NO_RX_TIMEOUT)
			break;

		can->rxTimer += msec;
		if (can->rxCount) {
			can->rxTimer = 0;
			can->rxCount = 0;
		}
		if (can->rxTimer > CAN_RX_TIMEOUT) {
			can->mode = CAN_MODE_AUTOBAUD;
			can->autobaudTimer = CAN_AUTOBAUD_TIMEOUT;
		}
		break;
	}
}

static void canReadMailbox(CAN_FIFOMailBox_TypeDef *mb)
{
	uint32_t id;
	uint32_t data[2];

	if (mb->RIR & CAN_RI0R_IDE) {
		id = mb->RIR >> 3;
	} else {
		id = mb->RIR >> 21;
	}

	data[0] = mb->RDLR;
	data[1] = mb->RDHR;

	carCanRecv(id, (uint8_t *)data, mb->RDTR);
}

static void canPoll(can_t *can)
{
	if (can->device->RF0R & 3) {
		canReadMailbox(&can->device->sFIFOMailBox[0]);
		can->device->RF0R |= (1 << 5);
		can->rxCount++;
		can->rxTotalCount++;
	}
	if (can->device->RF1R & 3) {
		canReadMailbox(&can->device->sFIFOMailBox[1]);
		can->device->RF1R |= (1 << 5);
		can->rxCount++;
		can->rxTotalCount++;
	}
}

static void canTimerCallback(TimerHandle_t xTimer)
{
	static uint8_t slot;

	slot++;
	if (slot >= 10) slot = 0;
	if (hwGetMode() == HW_MODE_ACTIVE) carCanSendTimer(slot, &can1, &can2);
}

void canInit()
{
	can1.device = CAN1;
	can1.mode = CAN_MODE_AUTOBAUD;
	can2.device = CAN2;
	can2.mode = CAN_MODE_AUTOBAUD;

	carInit();
	carCanInit(&can1, &can2);

	canGoInit(&can1);
	canGoInit(&can2);

	canTimer = xTimerCreate("CAN send timer", pdMS_TO_TICKS(100), pdTRUE, 0, canTimerCallback);
	xTimerStart(canTimer, 0);
}

void canProcess(uint32_t msec)
{
	canPoll(&can1);
	canPoll(&can2);

	canProcessTimers(&can1, msec);
	canProcessTimers(&can2, msec);
}

void canSend(can_t *can, uint32_t id, uint8_t *data, uint8_t len)
{
	uint32_t data0, data1;

	data0 = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
	data1 = data[4] | (data[5] << 8) | (data[6] << 16) | (data[7] << 24);

	if (can->mode != CAN_MODE_NORMAL) return;

	if (can->device->TSR & CAN_TSR_TME0) {
		can->device->sTxMailBox[0].TDTR = len;
		can->device->sTxMailBox[0].TDLR = data0;
		can->device->sTxMailBox[0].TDHR = data1;
		can->device->sTxMailBox[0].TIR = (id << 21) | 1;
		return;
	}
	if (can->device->TSR & CAN_TSR_TME1) {
		can->device->sTxMailBox[1].TDTR = len;
		can->device->sTxMailBox[1].TDLR = data0;
		can->device->sTxMailBox[1].TDHR = data1;
		can->device->sTxMailBox[1].TIR = (id << 21) | 1;
		return;
	}
	if (can->device->TSR & CAN_TSR_TME2) {
		can->device->sTxMailBox[2].TDTR = len;
		can->device->sTxMailBox[2].TDLR = data0;
		can->device->sTxMailBox[2].TDHR = data1;
		can->device->sTxMailBox[2].TIR = (id << 21) | 1;
		return;
	}
}

uint32_t canGetSignal(uint8_t *data, uint8_t offset, uint8_t size)
{
	return 0;
}

void canSetSignal(uint8_t *data, uint8_t offset, uint8_t size, uint32_t value)
{
	/* Begin from LSB */
	offset += size - 1;

	/* Initial byte and offset */
	uint8_t byte = offset >> 3;
	uint8_t shift = 7 - (offset & 7);

	while (size > 0) {
		uint8_t bitsize = 8 - shift;
		data[byte] |= value << shift;
		if (size <= bitsize) break;

		value = value >> bitsize;
		byte--;
		size -= bitsize;
		shift = 0;
	}
}

int canIsActive()
{
	if ((can1.mode == CAN_MODE_NORMAL) || (can2.mode == CAN_MODE_NORMAL)) return 1;
	return 0;
}

void canGetStatus(can_status_t *status)
{
	status->mode = can1.mode;
	status->speed = can1.speed;
	status->flags = can1.flags;
	status->rxCount = can1.rxTotalCount;
}
