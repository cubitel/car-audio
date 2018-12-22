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

#include "hardware.h"
#include "usb_audio.h"

#define AOUT_I2SDIV             (3 | (1 << 8))
#define AOUT_ADDR               0x6A

void DMA1_Stream7_IRQHandler(void);

static uint8_t aoutVolume = 24;
static uint8_t aoutRadioMute = 0;
static int16_t aoutBuffer1[AOUT_BUFFER_SIZE];
static int16_t aoutBuffer2[AOUT_BUFFER_SIZE];

static void aoutOn(void)
{
	uint8_t buf[10];

	gpioSet(GPIOD, PD_AOUT_STBY);
	gpioSet(GPIOD, PD_AOUT_MUTE);

	vTaskDelay(5);

	buf[0] = 0x00;              /* Start register address = 0 */
	buf[1] = 0x00;              /* Mode control = 0x00 (normal operation) */
	buf[2] = 0x61;              /* Misc control 1: overtemperature 110 C, 15V output voltage  */
	buf[3] = 0x62;              /* Misc control 2: PWM is 44xFs, 45 deg phase shifting */
	buf[4] = 0x54;              /* Serial port: 48 kHz, 16 bit, I2S mode */
	buf[5] = 0x00;              /* Channel state: 1-4 ON */
	i2cWrite(AOUT_CTL, AOUT_ADDR, buf, 6);
}

static inline int16_t aoutMixer(int16_t s1, int16_t s2)
{
	uint32_t r = s1 + s2;
	if (!(s1 & 0x8000) && !(s2 & 0x8000) && (r & 0x8000)) return 0x7FFF;
	if ((s1 & 0x8000) && (s2 & 0x8000) && !(r & 0x8000)) return 0x8000;
	return r;
}

static void aoutFillBuffer(int16_t *buf1, int16_t *buf2, uint32_t words)
{
	int16_t *usbPlayBuffer = audioGetPlayBuffer(words*2);
	int16_t *radioIn = radioGetPlayPosition();

	uint32_t wordCounter;
	for (wordCounter = 0; wordCounter < (words >> 1); wordCounter++) {
		int16_t sampleL = 0;
		int16_t sampleR = 0;

		if (!aoutRadioMute) {
			sampleL = *radioIn;
			radioIn++;
			sampleR = *radioIn;
			radioIn++;
		}

		if (usbPlayBuffer) {
			sampleL = aoutMixer(sampleL >> 2, *usbPlayBuffer);
			usbPlayBuffer++;
			sampleR = aoutMixer(sampleR >> 2, *usbPlayBuffer);
			usbPlayBuffer++;
		}

		*buf1 = sampleL;
		buf1++;
		*buf1 = sampleL;
		buf1++;
		*buf2 = sampleR;
		buf2++;
		*buf2 = sampleR;
		buf2++;
	}
}

void aoutStart()
{
	/* I2S prescaler */
	SPI3->I2SPR = SPI_I2SPR_MCKOE | AOUT_I2SDIV;
	/* I2S mode, master, transmit, 16 bit */
	SPI3->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1;
	/* I2Sext, slave, transmit, 16 bit */
	I2S3ext->I2SCFGR = SPI_I2SCFGR_I2SMOD;

	/* PC7 -- I2S3_MCLK, Fast */
	gpioSetAltFunc(GPIOC, 7, 6);
	gpioSetSpeed(GPIOC, 7, 2);
	/* PC10 -- I2S3_CK, Fast */
	gpioSetAltFunc(GPIOC, 10, 6);
	gpioSetSpeed(GPIOC, 10, 2);
	/* PC11 -- I2S3ext_SD, Fast */
	gpioSetAltFunc(GPIOC, 11, 5);
	gpioSetSpeed(GPIOC, 11, 2);
	/* PC12 -- I2S3_SD, Fast */
	gpioSetAltFunc(GPIOC, 12, 6);
	gpioSetSpeed(GPIOC, 12, 2);
	/* PA15 -- I2S3_WS, Fast */
	gpioSetAltFunc(GPIOA, 15, 6);
	gpioSetSpeed(GPIOA, 15, 2);

	/* DMA setup: Channel 0, Memory to peripheral, 16 bit, circular mode, transfer complete interrupt */
	DMA1_Stream7->CR = (0 << 25) | DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0 | DMA_SxCR_MINC | DMA_SxCR_CIRC | DMA_SxCR_DIR_0 |
		DMA_SxCR_HTIE | DMA_SxCR_TCIE;
	/* DMA buffer start address */
	DMA1_Stream7->M0AR = (uint32_t)aoutBuffer1;
	/* Destination -- SPI3 DR */
	DMA1_Stream7->PAR = (uint32_t)&SPI3->DR;
	/* Data items count */
	DMA1_Stream7->NDTR = AOUT_BUFFER_SIZE;
	/* Disable direct mode */
	DMA1_Stream7->FCR = DMA_SxFCR_DMDIS;

	/* DMA setup: Channel 2, Memory to peripheral, 16 bit, circular mode */
	DMA1_Stream5->CR = (2 << 25) | DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0 | DMA_SxCR_MINC | DMA_SxCR_CIRC | DMA_SxCR_DIR_0;
	/* DMA buffer start address */
	DMA1_Stream5->M0AR = (uint32_t)aoutBuffer2;
	/* Destination -- SPI3ext DR */
	DMA1_Stream5->PAR = (uint32_t)&I2S3ext->DR;
	/* Data items count */
	DMA1_Stream5->NDTR = AOUT_BUFFER_SIZE;
	/* Disable direct mode */
	DMA1_Stream5->FCR = DMA_SxFCR_DMDIS;

	/* Enable DMA IRQ */
	NVIC->ISER[1] |= (1 << (DMA1_Stream7_IRQn & 0x1F));
	/* Enable I2S DMA */
	DMA1_Stream5->CR |= DMA_SxCR_EN;
	DMA1_Stream7->CR |= DMA_SxCR_EN;
	SPI3->CR2 = SPI_CR2_TXDMAEN;
	I2S3ext->CR2 = SPI_CR2_TXDMAEN;

	I2S3ext->I2SCFGR |= SPI_I2SCFGR_I2SE;
	SPI3->I2SCFGR |= SPI_I2SCFGR_I2SE;

	/* Enable audio output */
	aoutOn();
}

void aoutStop(void)
{
	I2S3ext->I2SCFGR &= ~SPI_I2SCFGR_I2SE;
	SPI3->I2SCFGR &= ~SPI_I2SCFGR_I2SE;
	I2S3ext->CR2 = 0;
	SPI3->CR2 = 0;
	DMA1_Stream5->CR = 0;
	DMA1_Stream7->CR = 0;

	gpioClear(GPIOD, PD_AOUT_STBY);
	gpioClear(GPIOD, PD_AOUT_MUTE);
}


uint8_t aoutGetVolume()
{
	return aoutVolume;
}

void aoutSetVolume(uint8_t volume)
{
	uint8_t buf[10];

	aoutVolume = volume;

	if (aoutVolume == 0) {
		buf[8] = 0;
	} else {
		buf[8] = 107 + aoutVolume;
	}

	buf[0] = 0x05;
	buf[1] = buf[8];            /* CH1 Volume */
	buf[2] = buf[8];            /* CH2 Volume */
	buf[3] = buf[8];            /* CH3 Volume */
	buf[4] = buf[8];            /* CH4 Volume */
	i2cWrite(AOUT_CTL, AOUT_ADDR, buf, 5);
}

uint8_t aoutGetRadioMute(void)
{
	return aoutRadioMute;
}

void aoutSetRadioMute(uint8_t mute)
{
	aoutRadioMute = mute;
}

void DMA1_Stream7_IRQHandler()
{
	/* Half-transfer */
	if ((DMA1->HISR & DMA_HISR_HTIF7) != 0) {
		aoutFillBuffer(aoutBuffer1, aoutBuffer2, AOUT_BUFFER_SIZE / 2);
		/* Clear interrupt flag */
		DMA1->HIFCR = DMA_HIFCR_CHTIF7;
	}

	/* Full transfer */
	if ((DMA1->HISR & DMA_HISR_TCIF7) != 0) {
		aoutFillBuffer(&aoutBuffer1[AOUT_BUFFER_SIZE / 2], &aoutBuffer2[AOUT_BUFFER_SIZE / 2], AOUT_BUFFER_SIZE / 2);
		/* Clear interrupt flag */
		DMA1->HIFCR = DMA_HIFCR_CTCIF7;
	}
}
