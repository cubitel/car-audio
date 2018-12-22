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


void gpioSetMode(GPIO_TypeDef *gpio, uint8_t pin, uint8_t mode)
{
	uint32_t mask;

	mask = 3 << (pin << 1);
	gpio->MODER = (gpio->MODER & ~mask) | (mode << (pin << 1));
}

void gpioSetSpeed(GPIO_TypeDef *gpio, uint8_t pin, uint8_t speed)
{
	uint32_t mask;

	mask = 3 << (pin << 1);
	gpio->OSPEEDR = (gpio->OSPEEDR & ~mask) | (speed << (pin << 1));
}

void gpioSetAltFunc(GPIO_TypeDef *gpio, uint8_t pin, uint8_t fn)
{
	uint32_t mask;

	gpioSetMode(gpio, pin, 2);

	if (pin < 8) {
		mask = 15 << (pin << 2);
		gpio->AFR[0] = (gpio->AFR[0] & ~mask) | (fn << (pin << 2));
	} else {
		pin &= 7;
		mask = 15 << (pin << 2);
		gpio->AFR[1] = (gpio->AFR[1] & ~mask) | (fn << (pin << 2));
	}
}
