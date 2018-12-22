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

#include "hardware.h"
#include "radio/api.h"

#define RADIO_I2SDIV            (3 | (1 << 8))

void DMA1_Stream3_IRQHandler(void);
void DMA1_Stream5_IRQHandler(void);

static int16_t radioBuffer1[AOUT_BUFFER_SIZE];
static int16_t *radioPlayPosition = radioBuffer1;

void radioStart()
{
	/* I2S prescaler */
	SPI2->I2SPR = SPI_I2SPR_MCKOE | RADIO_I2SDIV;
	/* I2S mode, master, receive, 16 bit */
	SPI2->I2SCFGR = SPI_I2SCFGR_I2SMOD | SPI_I2SCFGR_I2SCFG_1 | SPI_I2SCFGR_I2SCFG_0;

	/* PB10 -- I2S2_CK, Fast */
	gpioSetAltFunc(GPIOB, 10, 5);
	gpioSetSpeed(GPIOB, 10, 2);
	/* PB9 -- I2S2_WS, Fast */
	gpioSetAltFunc(GPIOB, 9, 5);
	gpioSetSpeed(GPIOB, 9, 2);
	/* PC3 -- I2S2_SD, Fast */
	gpioSetAltFunc(GPIOC, 3, 5);
	gpioSetSpeed(GPIOC, 3, 2);

	/* DMA setup: Channel 0, Peripheral to memory, 16 bit, circular mode, transfer complete interrupt */
	DMA1_Stream3->CR = (0 << 25) | DMA_SxCR_PSIZE_0 | DMA_SxCR_MSIZE_0 | DMA_SxCR_MINC | DMA_SxCR_CIRC | DMA_SxCR_HTIE | DMA_SxCR_TCIE;
	/* DMA buffer start address */
	DMA1_Stream3->M0AR = (uint32_t)radioBuffer1;
	/* Source -- SPI2 DR */
	DMA1_Stream3->PAR = (uint32_t)&SPI2->DR;
	/* Data items count */
	DMA1_Stream3->NDTR = AOUT_BUFFER_SIZE;
	/* Disable direct mode */
	DMA1_Stream3->FCR = DMA_SxFCR_DMDIS;

	/* Enable DMA IRQ */
	NVIC->ISER[0] |= (1 << (DMA1_Stream3_IRQn & 0x1F));
	/* Enable I2S DMA */
	SPI2->CR2 = SPI_CR2_RXDMAEN;

	DMA1_Stream3->CR |= DMA_SxCR_EN;
	SPI2->I2SCFGR |= SPI_I2SCFGR_I2SE;

	radioInitTuner();
}

void radioStop()
{
	gpioClear(GPIOD, PD_RADIO_RESET);

	SPI2->I2SCFGR &= ~SPI_I2SCFGR_I2SE;
	SPI2->CR2 = 0;
	DMA1_Stream3->CR = 0;
}

void DMA1_Stream3_IRQHandler()
{
	/* Half-transfer */
	if ((DMA1->LISR & DMA_LISR_HTIF3) != 0) {
		radioPlayPosition = radioBuffer1;
		/* Clear interrupt flag */
		DMA1->LIFCR = DMA_LIFCR_CHTIF3;
	}

	/* Full transfer */
	if ((DMA1->LISR & DMA_LISR_TCIF3) != 0) {
		radioPlayPosition = &radioBuffer1[AOUT_BUFFER_SIZE / 2];
		/* Clear interrupt flag */
		DMA1->LIFCR = DMA_LIFCR_CTCIF3;
	}
}

int16_t *radioGetPlayPosition()
{
	return radioPlayPosition;
}
