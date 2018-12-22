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

#include "radio/api.h"

static radio_t *radio;

void radioInitTuner()
{
	radio = si47xxInit();
}

void radioTune(uint32_t frequency)
{
	if (radio && radio->radioTune) radio->radioTune(frequency);
}

void radioSeek(uint8_t seekUp)
{
	if (radio && radio->radioSeek) radio->radioSeek(seekUp);
}

void radioPoll()
{
	if (radio && radio->radioPoll) radio->radioPoll();
}

int radioGetData(uint16_t type, uint8_t *buf, uint16_t len)
{
	if (radio && radio->radioGetData) return radio->radioGetData(type, buf, len);
	return 0;
}
