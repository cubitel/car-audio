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

#include <string.h>
#include <FreeRTOS.h>
#include <task.h>

#include "usb_audio.h"
#include "usbd_conf.h"
#include "usb_dcd.h"

/***************************************************************/
/* USB audio interface                                         */
/***************************************************************/

/* Size must be multiple of AUDIO_EP_SIZE and AOUT_BUFFER_SIZE */
static __attribute__ ((aligned (4))) uint8_t audioReceiveBuffer[1536];

static uint32_t audioReceivePosition;
static uint32_t audioPlayPosition;
static uint32_t audioRxCount;

void audioDataRx(void *pdev, uint8_t epnum, uint32_t len)
{
	audioReceivePosition += len;
	if ((audioReceivePosition + AUDIO_EP_SIZE) > sizeof(audioReceiveBuffer)) {
		audioReceivePosition = 0;
	}
	((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame =
		((USB_OTG_CORE_HANDLE*)pdev)->dev.out_ep[epnum].even_odd_frame ? 0 : 1;
	DCD_EP_PrepareRx(pdev, epnum, &audioReceiveBuffer[audioReceivePosition], AUDIO_EP_SIZE);
	audioRxCount++;

	return;
}

int16_t *audioGetPlayBuffer(uint32_t len)
{
	if ((audioPlayPosition <= audioReceivePosition) && ((audioPlayPosition + len) > audioReceivePosition)) {
		return NULL;
	}

	int16_t *currentPosition = (int16_t *)(&audioReceiveBuffer[audioPlayPosition]);

	audioPlayPosition += len;
	if (audioPlayPosition >= sizeof(audioReceiveBuffer)) audioPlayPosition = 0;

	return currentPosition;
}

uint32_t audioGetRxCount()
{
	return audioRxCount;
}
