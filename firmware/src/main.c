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

#include <stm32f4xx.h>

#include <FreeRTOS.h>
#include <task.h>
#include <FreeRTOS_IP.h>

#include "hardware.h"
#include "usb.h"
#include "netserv.h"

/* hard-coded network parameters */
static const uint8_t cfgIPAddress[ 4 ] = { 192, 168, 91, 11 };
static const uint8_t cfgNetMask[ 4 ] = { 255, 255, 255, 0 };
static const uint8_t cfgGatewayAddress[ 4 ] = { 192, 168, 91, 1 };
static const uint8_t cfgDNSServerAddress[ 4 ] = { 0, 0, 0, 0 };
static uint8_t cfgMACAddress[6] = { 0x00, 0xFF, 192, 168, 91, 11 };

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	( void ) pcTaskName;
	( void ) pxTask;
	taskDISABLE_INTERRUPTS();

	*((uint32_t *)RAM_ADDRESS_CAUSE) = BOOT_CAUSE_STACKOVF;

	for(;;);
}

void vApplicationIdleHook(void);

void vApplicationIdleHook(void)
{
	IWDG->KR = 0xAAAA;
}

int main(void)
{
	hwInit();

	usbInit();

	xTaskCreate(hwTask, "HW control", 512, NULL, 1, NULL);

	FreeRTOS_IPInit(cfgIPAddress, cfgNetMask, cfgGatewayAddress, cfgDNSServerAddress, cfgMACAddress);

	xTaskCreate(netservTask, "Network services", 256, NULL, 1, NULL);

	/* Start FreeRTOS */
	vTaskStartScheduler();

	/* Infinite loop */
	while (1) {
	}
}
