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
#include <string.h>
#include "config.h"
#include "radio/api.h"
#include "radio/rds.h"

#define RADIO_SI47XX_ADDR       0x63
#define RADIO_SI47XX_TIMEOUT    100

static rds_t rds;
static uint8_t rssi;
static uint8_t snr;
static uint8_t stereo;
static uint8_t shouldReadFreq;

static int si47xxWaitCTS(void)
{
	uint8_t buf[2];

	int i;
	for (i = 0; i < RADIO_SI47XX_TIMEOUT; i++) {
		buf[0] = 0;

		int r = i2cRead(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 1);
		if (r < 0) return -1;

		if (buf[0] & 0x80) return buf[0];
		vTaskDelay(1);
	}

	return 0;
}

static void si47xxSetProperty(uint16_t property, uint16_t value)
{
	uint8_t buf[6];

	buf[0] = 0x12;
	buf[1] = 0x00;
	buf[2] = property >> 8;
	buf[3] = property & 0xFF;
	buf[4] = value >> 8;
	buf[5] = value & 0xFF;
	i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 6);
	si47xxWaitCTS();
}

static void si47xxTune(uint32_t freq)
{
	uint8_t buf[10];

	buf[0] = 0x20;
	buf[1] = 0x00;
	buf[2] = (freq / 10) >> 8;
	buf[3] = (freq / 10) & 0xFF;
	buf[4] = 0x00;
	i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 5);
	si47xxWaitCTS();
	rdsInit(&rds);
}

static void si47xxSeek(uint8_t seekUp)
{
	uint8_t buf[10];

	buf[0] = 0x21;
	buf[1] = seekUp ? 0x0C : 0x04;
	i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 2);
	si47xxWaitCTS();
	rdsInit(&rds);
	shouldReadFreq = 1;
}

static void si47xxPoll(void)
{
	uint8_t buf[12];

	buf[0] = 0x23;
	if (shouldReadFreq) buf[0] = 0x22;
	buf[1] = 0;
	i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 2);
	si47xxWaitCTS();

	/* Read RSSI/SNR */
	int r = i2cRead(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 8);
	if (r < 0) return;

	rssi = buf[4];
	snr = buf[5];
	if (shouldReadFreq) {
		if (buf[1] & 1) {
			/* Save frequency if we found radion station */
			shouldReadFreq = 0;
			uint32_t freq = (buf[2] << 8 | buf[3]) * 10;
			configData()->has_tuner = true;
			configData()->tuner.has_frequency = true;
			configData()->tuner.frequency = freq;
			configWrite();
		}
	} else {
		stereo = buf[3];
	}

	/* RDSINT */
	if (buf[0] & 4) {
		/* Read RDS data */
		while (1) {
			buf[0] = 0x24;
			buf[1] = 1;
			i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 2);
			si47xxWaitCTS();
			int len = i2cRead(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 12);
			if (len < 0) return;

			/* Exit if FIFO is empty */
			if (buf[3] == 0) break;

			/* Process RDS data */
			rdsProcessGroup(&rds, (buf[4] << 8) | buf[5], (buf[6] << 8) | buf[7], (buf[8] << 8) | buf[9], (buf[10] << 8) | buf[11]);
		}
	}
}

static int si47xxGetData(uint16_t type, uint8_t *buf, uint16_t len)
{
	switch (type) {
	case RADIO_DATA_RSSI:
		if (len < 1) return 0;
		*buf = rssi;
		return 1;

	case RADIO_DATA_SNR:
		if (len < 1) return 0;
		*buf = snr;
		return 1;

	case RADIO_DATA_STEREO:
		if (len < 1) return 0;
		*buf = stereo;
		return 1;

	case RADIO_DATA_RDS_PS_NAME:
		if (len < 9) return 0;
		memcpy(buf, rds.psName, 8);
		buf[8] = 0;
		return 9;

	case RADIO_DATA_RDS_GROUPS:
		if (len < 4) return 0;
		memcpy(buf, &rds.validGroupTypes, 4);
		return 4;
	}

	return 0;
}

static radio_t radio = {
	si47xxTune,
	si47xxSeek,
	si47xxPoll,
	si47xxGetData
};

radio_t *si47xxInit(void)
{
	uint8_t buf[10];

	gpioClear(GPIOD, PD_RADIO_RESET);
	vTaskDelay(1);
	gpioSet(GPIOD, PD_RADIO_RESET);
	vTaskDelay(1);

	/* Power UP */
	buf[0] = 0x01;
	buf[1] = 0x00;
	buf[2] = 0xB5;
	i2cWrite(RADIO_CTL, RADIO_SI47XX_ADDR, buf, 3);
	si47xxWaitCTS();

	/* RCLK = DCLK = 16*2*48000 = 1536000 */

	/* Set REFCLK prescaler = 47, DCLK clock source */
	si47xxSetProperty(0x0202, 0x102F);

	/* Set REFCLK freq = 32681  */
	si47xxSetProperty(0x0201, 32681);

	/* Set MAX_TUNE_ERROR = 25 kHz  */
	si47xxSetProperty(0x1108, 25);

	/* 48 kHz audio frequency */
	si47xxSetProperty(0x0104, 48000);

	/* Standard I2S, 16 bit, stereo */
	si47xxSetProperty(0x0102, 0x0000);

	/* Enable RDSRECV flag */
	si47xxSetProperty(0x1500, 0x0001);

	/* Configure RDS FIFO */
	si47xxSetProperty(0x1501, 14);

	/* Enable RDS */
	si47xxSetProperty(0x1502, 0xAA01);

	return &radio;
}
