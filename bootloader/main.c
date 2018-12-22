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

#include "stm32f4xx.h"

static void startApplication(void)
{
	__asm volatile (
		"LDR     R0, =0x08010000\n"
		"LDR     SP,[R0, #0]\n"
		"LDR     R0,[R0, #4]\n"
		"BX      R0\n"
	);
}

static void startBootloader(void)
{
	/* Enable SYSCFG clock */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	/* Remap address 0 to bootloader */
	SYSCFG->MEMRMP = 1;

	__asm volatile (
		"LDR     R0, =0x1FFF0000\n"
		"LDR     SP,[R0, #0]\n"
		"LDR     R0,[R0, #4]\n"
		"BX      R0\n"
	);
}

int main(void)
{
	if ( *((uint32_t *)0x08010000) == 0xFFFFFFFF) startBootloader();

	startApplication();
	return 0;
}
