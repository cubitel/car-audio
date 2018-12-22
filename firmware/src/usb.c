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

#include "usb_bsp.h"
#include "usbd_conf.h"
#include "usb.h"
#include "usb_dcd_int.h"
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_usr.h"
#include "usb_class.h"
#include "hardware.h"
#include "usb_ecm.h"


void OTG_FS_IRQHandler(void);
void OTG_HS_IRQHandler(void);

__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB1 __ALIGN_END;
__ALIGN_BEGIN USB_OTG_CORE_HANDLE USB2 __ALIGN_END;


void usbInit()
{
	/* Initialize USB stack */
	USBD_Init(&USB1, USB_OTG_FS_CORE_ID, &USR_desc, &USBD_CDC_cb, &USR_cb);
}

/***** STM32 USBD board support functions *****/

void USB_OTG_BSP_Init(USB_OTG_CORE_HANDLE *pdev)
{
	/* Enable USB pins */
	gpioSetAltFunc(GPIOA, 11, 10);
	gpioSetAltFunc(GPIOA, 12, 10);
	gpioSetAltFunc(GPIOB, 14, 12);
	gpioSetAltFunc(GPIOB, 15, 12);

	/* Enable USB clock */
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	RCC->AHB2ENR |= RCC_AHB2ENR_OTGFSEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_OTGHSEN;
}

void USB_OTG_BSP_EnableInterrupt(USB_OTG_CORE_HANDLE *pdev)
{
	if (pdev == &USB1) {
		/* Enable OTG FS interrupt */
		NVIC->ISER[2] |= (1 << (OTG_FS_IRQn % 32));
	}
	if (pdev == &USB2) {
		/* Enable OTG HS interrupt */
		NVIC->ISER[2] |= (1 << (OTG_HS_IRQn % 32));
	}
}

void USB_OTG_BSP_uDelay (const uint32_t usec)
{
	usleep(usec);
}

void USB_OTG_BSP_mDelay (const uint32_t msec)
{
	usleep(msec*1000);
}

void OTG_FS_IRQHandler(void)
{
	USBD_OTG_ISR_Handler(&USB1);
}

void OTG_HS_IRQHandler(void)
{
	USBD_OTG_ISR_Handler(&USB2);
}

/***** Class callbacks *****/

/* Handle all class-specific requests
 *
 */
uint16_t usb_Ctrl(void *pdev, uint32_t type, uint32_t cmd, uint32_t ifnum, uint8_t* buf, uint32_t len)
{
	/* Class, Interface, OUT */
	if (type == 0x21) {
		if (ifnum == ETH_INTERFACE) {
			ethControl(pdev, buf, len);
		}
	}

	/* Class, Interface, IN */
	if (type == 0xA1) {
		if (ifnum == ETH_INTERFACE) {
			return ethControlIn(pdev, buf, len);
		}
	}

	return 0;
}

/***** Generic USBD callbacks *****/

USBD_Usr_cb_TypeDef USR_cb =
{
  USBD_USR_Init,
  USBD_USR_DeviceReset,
  USBD_USR_DeviceConfigured,
  USBD_USR_DeviceSuspended,
  USBD_USR_DeviceResumed,
  USBD_USR_DeviceConnected,
  USBD_USR_DeviceDisconnected,
};

void USBD_USR_Init(void)
{
}

void USBD_USR_DeviceReset(uint8_t speed)
{
	ecmDisable();
}

void USBD_USR_DeviceConfigured(void)
{
}

void USBD_USR_DeviceSuspended(void)
{
	ecmDisable();
}

void USBD_USR_DeviceResumed(void)
{
}

void USBD_USR_DeviceConnected(void)
{
}

void USBD_USR_DeviceDisconnected(void)
{
}
