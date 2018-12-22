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

#include "cars/api.h"

static car_t *car;

void carInit()
{
	car = (car_t *)ford_cd3xx();
}

void carCanInit(can_t *can1, can_t *can2)
{
	if (car && car->canInit) car->canInit(can1, can2);
}

void carCanRecv(uint32_t id, uint8_t *data, uint8_t len)
{
	if (car && car->canRecv) car->canRecv(id, data, len);
}

void carCanSendTimer(uint8_t slot, can_t *can1, can_t *can2)
{
	if (car && car->canSendTimer) car->canSendTimer(slot, can1, can2);
}

void carEvent(uint32_t eventId)
{
	if (car && car->event) car->event(eventId);
}

int carIsActive()
{
	if (car && car->isActive) return car->isActive();
	return 0;
}

int carGetData(uint32_t type, uint8_t *buf, uint32_t len)
{
	if (car && car->getData) return car->getData(type, buf, len);
	return 0;
}
