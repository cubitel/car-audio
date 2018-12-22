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

#define I2C_WAIT(cond) time = 0; while ( !(cond) && (time < 1000)) { time++; usleep(10); }

#define I2C_SR1_ERRORS (I2C_SR1_TIMEOUT | I2C_SR1_AF)

void i2cInit(I2C_TypeDef *device)
{
	/* 42 MHz APB clock */
	device->CR2 = 42;
	/* 400 kHz i2c clock, 300ns rise time */
	device->CCR = I2C_CCR_FS | 35;
	device->TRISE = 15;

	device->CR1 |= I2C_CR1_PE;
}

int i2cRead(I2C_TypeDef *device, uint8_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t time;

	/* Start */
	device->CR1 |= I2C_CR1_START;
	I2C_WAIT(device->SR1 & I2C_SR1_SB);
	if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
		device->SR1 = 0;
		return -1;
	}

	device->CR1 &= ~I2C_CR1_ACK;
	if (len > 1) device->CR1 |= I2C_CR1_ACK;

	device->DR = (addr << 1) | 1;
	I2C_WAIT(device->SR1 & I2C_SR1_ADDR);
	(void)device->SR2;
	if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
		device->SR1 = 0;
		return -1;
	}

	uint32_t i;
	for (i = 0; i < len; i++) {
		I2C_WAIT(device->SR1 & I2C_SR1_RXNE);
		if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
			device->SR1 = 0;
			return -1;
		}
		/* Clear ACK before _start_ receiving last byte */
		if (i == (len-2)) device->CR1 &= ~I2C_CR1_ACK;
		*(buf++) = device->DR;
	}

	device->CR1 |= I2C_CR1_STOP;

	return len;
}

int i2cWrite(I2C_TypeDef *device, uint8_t addr, uint8_t *buf, uint32_t len)
{
	uint32_t time;

	/* Start */
	device->CR1 |= I2C_CR1_START;
	I2C_WAIT(device->SR1 & I2C_SR1_SB);
	if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
		device->SR1 = 0;
		return -1;
	}

	device->DR = addr << 1;
	I2C_WAIT(device->SR1 & I2C_SR1_ADDR);
	(void)device->SR2;
	if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
		device->SR1 = 0;
		return -1;
	}

	uint32_t i;
	for (i = 0; i < len; i++) {
		device->DR = *(buf++);
		I2C_WAIT(device->SR1 & I2C_SR1_TXE);
		if ( (device->SR1 & I2C_SR1_ERRORS) || (time == 1000)) {
			device->SR1 = 0;
			return -1;
		}
	}

	I2C_WAIT(device->SR1 & I2C_SR1_BTF);
	device->CR1 |= I2C_CR1_STOP;

	return len;
}
