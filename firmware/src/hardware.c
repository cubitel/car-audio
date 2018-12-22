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
#include <queue.h>
#include <FreeRTOS_IP.h>
#include <FreeRTOS_Sockets.h>

#include "hardware.h"
#include "netserv.h"
#include "config.h"
#include "usb_audio.h"
#include "cars/api.h"
#include "radio/api.h"

static uint8_t hwCurrentMode;
static uint32_t hwActiveCounter;
static uint32_t hwStandbyCounter;

static void hwHandleRequest(uint16_t requestId, uint16_t wParam);

/***************************************************************/
/* ADC                                                         */
/***************************************************************/

#define LASTKEY_MAX_COUNT       3
#define LASTKEY_THR_COUNT       2
#define LASTKEY_LONG_TIMEOUT    100

/* ADC value = R / (R + 5100) * 4095 */
static uint16_t keyValues[] = {
	KEY_VOLUME_DOWN, 23, 63,    /* 43 */
	KEY_VOLUME_UP, 95, 135,     /* 115 */
	KEY_SEEK_DOWN, 386, 426,    /* 406 */
	KEY_SEEK_UP, 208, 248,      /* 228 */
	KEY_MODE, 672, 712,         /* 692 */
	0, 0, 0
};

static uint16_t lastKey;
static uint8_t lastKeyCount;
static uint8_t lastKeyLongTimer;

static void keyEvent(uint16_t key)
{
	switch (key) {
	case KEY_VOLUME_UP:
		hwHandleRequest(HWRQ_AOUT_VOLUME_UP, 0);
		break;
	case KEY_VOLUME_DOWN:
		hwHandleRequest(HWRQ_AOUT_VOLUME_DOWN, 0);
		break;
	case KEY_SEEK_UP:
		hwHandleRequest(HWRQ_RADIO_SEEK, RADIO_SEEK_UP);
		break;
	case KEY_SEEK_DOWN:
		hwHandleRequest(HWRQ_RADIO_SEEK, RADIO_SEEK_DOWN);
		break;
	case KEY_MODE:
		hwHandleRequest(HWRQ_RADIO_MUTE_TOGGLE, 0);
		break;
	default:
		break;
	}
}

static void keyLongEvent(uint16_t key)
{
	switch (key) {
	case KEY_MODE:
		SCB->AIRCR = ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2));
		break;
	default:
		break;
	}
}

static void adcProcess(void)
{
	uint16_t adcValue = ADC1->DR;
	ADC1->CR2 |= ADC_CR2_SWSTART;

	uint8_t i;
	uint16_t keyCode, keyMinValue, keyMaxValue;
	for (i = 0; i < sizeof(keyValues);) {
		keyCode = keyValues[i++];
		if (keyCode == 0) break;
		keyMinValue = keyValues[i++];
		keyMaxValue = keyValues[i++];
		if ( (adcValue >= keyMinValue) && (adcValue <= keyMaxValue) ) break;
	}

	if (keyCode == lastKey) {
		if (lastKeyCount < LASTKEY_MAX_COUNT) lastKeyCount++;
		if (lastKeyCount == LASTKEY_THR_COUNT) {
			keyEvent(keyCode);
		}
		if (lastKeyLongTimer == LASTKEY_LONG_TIMEOUT) {
			lastKeyLongTimer++;
			keyLongEvent(keyCode);
		} else {
			lastKeyLongTimer++;
		}
	} else {
		lastKeyLongTimer = 0;
		if (lastKeyCount > 0) lastKeyCount--;
		if (lastKeyCount == 0) lastKey = keyCode;
	}
}

/***************************************************************/
/* FLASH                                                       */
/***************************************************************/

#define RAMCODE __attribute__ ((section (".data#")))

static RAMCODE void flashUnlock(void)
{
	FLASH->KEYR = 0x45670123;
	FLASH->KEYR = 0xCDEF89AB;
}

static RAMCODE void flashLock(void)
{
	FLASH->CR = FLASH_CR_LOCK;
}

static RAMCODE void flashErase(uint8_t sector)
{
	FLASH->CR = (2 << 8) | (sector << 3) | FLASH_CR_SER;
	FLASH->CR |= FLASH_CR_STRT;
	while (FLASH->SR & FLASH_SR_BSY);
	IWDG->KR = 0xAAAA;
}

RAMCODE void flashUpdateFirmware(uint8_t *data, uint32_t len)
{
	hwSetMode(HW_MODE_STANDBY);

	taskENTER_CRITICAL();

	NVIC->ICER[0] = 0xFFFFFFFF;
	NVIC->ICER[1] = 0xFFFFFFFF;
	NVIC->ICER[2] = 0xFFFFFFFF;
	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICPR[1] = 0xFFFFFFFF;
	NVIC->ICPR[2] = 0xFFFFFFFF;

	/* Disable and reset I/D cache */
	FLASH->ACR &= ~(FLASH_ACR_DCEN | FLASH_ACR_ICEN);
	FLASH->ACR |= FLASH_ACR_DCRST | FLASH_ACR_ICRST;

	/* Unlock flash control registers */
	flashUnlock();

	/* Erase flash */
	flashErase(4);

	/* Program flash */
	uint32_t i;
	volatile uint32_t *flash = (uint32_t *)FLASH_START_ADDRESS;
	for (i = 0; i < len; i += 4) {
		while (FLASH->SR & FLASH_SR_BSY);

		FLASH->CR = (2 << 8) | FLASH_CR_PG;
		*flash++ = *((uint32_t *)data);
		data += 4;

		while (FLASH->SR & FLASH_SR_BSY);
		FLASH->CR &= ~FLASH_CR_PG;

		IWDG->KR = 0xAAAA;
	}

	/* Reset MCU */
	SCB->AIRCR = ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2));
}

RAMCODE void flashSaveConfig(uint8_t *data, uint32_t len, int poweroff)
{
	hwSetMode(HW_MODE_STANDBY);

	taskENTER_CRITICAL();

	NVIC->ICER[0] = 0xFFFFFFFF;
	NVIC->ICER[1] = 0xFFFFFFFF;
	NVIC->ICER[2] = 0xFFFFFFFF;
	NVIC->ICPR[0] = 0xFFFFFFFF;
	NVIC->ICPR[1] = 0xFFFFFFFF;
	NVIC->ICPR[2] = 0xFFFFFFFF;

	/* Disable and reset I/D cache */
	FLASH->ACR &= ~(FLASH_ACR_DCEN | FLASH_ACR_ICEN);
	FLASH->ACR |= FLASH_ACR_DCRST | FLASH_ACR_ICRST;

	/* Unlock flash control registers */
	flashUnlock();

	/* Erase flash */
	flashErase(1);

	/* Program flash */
	uint32_t i;
	volatile uint32_t *flash = (uint32_t *)FLASH_CONFIG_ADDRESS;
	for (i = 0; i < len; i += 4) {
		while (FLASH->SR & FLASH_SR_BSY);

		FLASH->CR = (2 << 8) | FLASH_CR_PG;
		*flash++ = *((uint32_t *)data);
		data += 4;

		while (FLASH->SR & FLASH_SR_BSY);
		FLASH->CR &= ~FLASH_CR_PG;

		IWDG->KR = 0xAAAA;
	}

	if (poweroff) {
		gpioSet(GPIOD, PD_CAN_EN);
		gpioClear(GPIOD, PD_CAN_STBN);

		/* Enter infinite loop waiting power off */
		/* If system does not power off, watchdog timer will reset the device */
		while (1) {};
	} else {
		/* Reset MCU */
		SCB->AIRCR = ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2));
	}
}

/***************************************************************/
/* Hardware control API                                        */
/***************************************************************/

static QueueHandle_t hwQueue;
static uint8_t statusBuffer[1024];

static void hwSetVolume(uint8_t volume)
{
	aoutSetVolume(volume);
	configData()->has_audio = true;
	configData()->audio.has_volume = true;
	configData()->audio.volume = volume;
	configWrite();
	carEvent(CAREVENT_VOLUME_CHANGED);
}

static void hwSetRadioMute(uint8_t mute)
{
	aoutSetRadioMute(mute);
	configData()->has_audio = true;
	configData()->audio.has_muteTuner = true;
	configData()->audio.muteTuner = mute;
	configWrite();
}

static void hwHandleRequest(uint16_t requestId, uint16_t wParam)
{
	uint8_t aoutVolume;

	hwSetMode(HW_MODE_ACTIVE);

	switch (requestId) {
	case HWRQ_AOUT_VOLUME_UP:
		aoutVolume = configData()->audio.volume;
		if (aoutVolume <= 96) aoutVolume += 4;
		hwSetVolume(aoutVolume);
		break;
	case HWRQ_AOUT_VOLUME_DOWN:
		aoutVolume = configData()->audio.volume;
		if (aoutVolume >= 4) aoutVolume -= 4;
		hwSetVolume(aoutVolume);
		break;
	case HWRQ_AOUT_VOLUME_SET:
		hwSetVolume(wParam);
		break;

	case HWRQ_RADIO_TUNE:
		radioTune(wParam * 10);
		aoutSetRadioMute(0);
		break;
	case HWRQ_RADIO_SEEK:
		radioSeek(wParam);
		aoutSetRadioMute(0);
		break;
	case HWRQ_RADIO_MUTE:
		hwSetRadioMute(wParam);
		break;
	case HWRQ_RADIO_MUTE_TOGGLE:
		hwSetRadioMute(1 - configData()->audio.muteTuner);
		break;

	default:
		/* Ignore unknown requests */
		break;
	}
}

void startBootloader(void)
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

void reboot()
{
	*((uint32_t *)0x2001FFF0) = 0xDEADBEEF;
	usleep(10);
	SCB->AIRCR = ((0x5FA << 16) | (SCB->AIRCR & (0x700)) | (1 << 2));
}

void usleep(uint32_t usec)
{
	uint32_t count;
	uint32_t tms = TIM6->CNT;

	for (count = 0; count < usec; count++) {
		while (TIM6->CNT == tms) {};
		tms = TIM6->CNT;
	};
}

void hwInit()
{
	/* Enable peripheral clocks
	 * HCLK = 168 MHz
	 * APB1 = HCLK / 4 = 42 MHz
	 * APB2 = HCLK / 2 = 84 MHz
	 */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN | RCC_AHB1ENR_GPIODEN
		| RCC_AHB1ENR_DMA1EN | RCC_AHB1ENR_CRCEN | RCC_AHB1ENR_BKPSRAMEN;
	RCC->APB1ENR |= RCC_APB1ENR_CAN1EN | RCC_APB1ENR_CAN2EN | RCC_APB1ENR_TIM6EN | RCC_APB1ENR_SPI2EN | RCC_APB1ENR_SPI3EN
		| RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;

	PWR->CSR = PWR_CSR_BRE;

	/* Enable watchdog */
	IWDG->KR = 0x5555;
	IWDG->PR = 3;
	IWDG->KR = 0xCCCC;

	/* PB7,PB8 -- I2C1, Open drain */
	gpioSetAltFunc(GPIOB, 7, 4);
	gpioSetAltFunc(GPIOB, 8, 4);
	GPIOB->OTYPER |= (1 << 7) | (1 << 8);
	/* PA8,PC9 -- I2C3, Open drain */
	gpioSetAltFunc(GPIOA, 8, 4);
	gpioSetAltFunc(GPIOC, 9, 4);
	GPIOA->OTYPER |= (1 << 8);
	GPIOC->OTYPER |= (1 << 9);

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN  | RCC_APB1ENR_I2C3EN;

	/* PD0,PD1 -- CAN1 */
	gpioSetAltFunc(GPIOD, 0, 9);
	gpioSetAltFunc(GPIOD, 1, 9);
	/* PB5,PB6 -- CAN2 */
	gpioSetAltFunc(GPIOB, 5, 9);
	gpioSetAltFunc(GPIOB, 6, 9);

	/* PD2 -- AOUT_STBY */
	gpioSetMode(GPIOD, 2, 1);
	gpioClear(GPIOD, PD_AOUT_STBY);
	/* PD3 -- AOUT_MUTE */
	gpioSetMode(GPIOD, 3, 1);
	gpioClear(GPIOD, PD_AOUT_MUTE);

	/* PD6 -- RADIO_RESET */
	gpioSetMode(GPIOD, 6, 1);
	gpioClear(GPIOD, PD_RADIO_RESET);

	/* PD9, PD10 -- USBxEN */
	gpioSetMode(GPIOD, 9, 1);
	gpioSet(GPIOD, PD_USB1_EN);
	gpioSetMode(GPIOD, 10, 1);
	gpioSet(GPIOD, PD_USB2_EN);

	/* PD14 -- CAN_EN, PD15 -- CAN_STBN */
	gpioSetMode(GPIOD, 14, 1);
	gpioSetMode(GPIOD, 15, 1);
	gpioSet(GPIOD, PD_CAN_EN);
	gpioSet(GPIOD, PD_CAN_STBN);

	/* PA4 -- Analog input (ADC12_IN4, steering wheel keys) */
	gpioSetMode(GPIOA, 4, 3);

	/* TIM6 -- General clock (100 Hz) */
	TIM6->PSC = 83;
	TIM6->ARR = 10000;
	TIM6->CR1 |= 1;

	/* ADC1, select IN4 */
	ADC1->CR2 = ADC_CR2_ADON;
	ADC1->SQR1 = (1 << 20);
	ADC1->SQR3 = 4;

	/* Read config from flash */
	configRead((uint8_t *)FLASH_CONFIG_ADDRESS);
	configRead((uint8_t *)BKPSRAM_BASE);
}

uint8_t hwGetMode()
{
	return hwCurrentMode;
}

void hwSetMode(uint8_t mode)
{
	if (hwCurrentMode == mode) return;

	switch (mode) {
	case HW_MODE_STANDBY:
		gpioSet(GPIOD, PD_CAN_EN);
		gpioSet(GPIOD, PD_CAN_STBN);

		aoutStop();
		radioStop();

		hwStandbyCounter = 0;
		break;

	case HW_MODE_ACTIVE:
		gpioSet(GPIOD, PD_CAN_EN);
		gpioSet(GPIOD, PD_CAN_STBN);

		radioStart();
		aoutStart();

		aoutSetVolume(configData()->audio.volume);
		aoutSetRadioMute(configData()->audio.muteTuner);
		radioTune(configData()->tuner.frequency);

		hwActiveCounter = 0;
		break;

	case HW_MODE_POWEROFF:
		configSaveIfNeeded(1);

		gpioSet(GPIOD, PD_CAN_EN);
		gpioClear(GPIOD, PD_CAN_STBN);

		/* Enter infinite loop waiting power off */
		/* If system does not power off, watchdog timer will reset the device */
		taskENTER_CRITICAL();
		while (1) {};
		break;

	default:
		break;
	}

	hwCurrentMode = mode;
}

void hwTask(void *p)
{
	hwQueue = xQueueCreate(10, sizeof(hw_request_t));

	/* Enable I2S PLL for 48 kHz */
	RCC->PLLI2SCFGR = (3 << 28) | (258 << 6);
	RCC->CR |= RCC_CR_PLLI2SON;
	/* Wait for PLL ready */
	while (!(RCC->CR & RCC_CR_PLLI2SRDY)) {}

	hwCurrentMode = HW_MODE_STANDBY;

	i2cInit(RADIO_CTL);
	i2cInit(AOUT_CTL);

	/*CAN also initialize CAR HAL */
	canInit();

	uint8_t prevCarActiveState = 0;

	Socket_t sockStatus = FreeRTOS_socket(FREERTOS_AF_INET, FREERTOS_SOCK_DGRAM, FREERTOS_IPPROTO_UDP);
	struct freertos_sockaddr saDestAddress;
	saDestAddress.sin_addr = FreeRTOS_inet_addr_quick(192, 168, 91, 255);
	saDestAddress.sin_port = FreeRTOS_htons(2001);

	uint8_t loopCounter = 0;
	for (;;) {
		vTaskDelay(10);
		loopCounter++;
		if (loopCounter > 100) loopCounter = 0;

		adcProcess();
		canProcess(10);

		hw_request_t req;
		if (xQueueReceive(hwQueue, &req, 0) == pdTRUE) {
			hwHandleRequest(req.requestId, req.wParam);
		}

		/* 4 Hz */
		if ((loopCounter % 25) == 0) {
			if (hwCurrentMode == HW_MODE_ACTIVE) radioPoll();
		}

		/* 1 Hz */
		if (loopCounter == 0) {
			if (hwCurrentMode == HW_MODE_STANDBY) {
				if (carIsActive()) {
					/* Go ACTIVE mode if ignition is on */
					hwSetMode(HW_MODE_ACTIVE);
				}

				if (canIsActive()) {
					hwStandbyCounter = 0;
				} else {
					/* Power off if can bus inactive */
					hwStandbyCounter++;
					if (hwStandbyCounter > configData()->system.standbyTimeout) {
						hwSetMode(HW_MODE_POWEROFF);
					}
				}
			}
			if (hwCurrentMode == HW_MODE_ACTIVE) {
				uint8_t currentCarActiveState = carIsActive();
				if (currentCarActiveState != prevCarActiveState) {
					prevCarActiveState = currentCarActiveState;
					if (currentCarActiveState == 0) {
						hwSetMode(HW_MODE_STANDBY);
					}
				}

				if (carIsActive()) {
					hwActiveCounter = 0;
				} else {
					/* Go STANDBY mode after timeout if ignition is off */
					hwActiveCounter++;
					if (hwActiveCounter > configData()->system.activeTimeout) {
						hwSetMode(HW_MODE_STANDBY);
					}
				}
			}
			uint32_t len = netservMakeStatusMessage(statusBuffer, sizeof(statusBuffer));
			if (len) {
				FreeRTOS_sendto(sockStatus, statusBuffer, len, 0, &saDestAddress, sizeof(saDestAddress));
			}
		}
	}
}

void hwSendRequest(uint16_t requestId, uint16_t wParam)
{
	hw_request_t req;
	req.requestId = requestId;
	req.wParam = wParam;
	xQueueSend(hwQueue, &req, 0);
}
