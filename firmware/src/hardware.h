#ifndef _HARDWARE_H
#define _HARDWARE_H

#include <stm32f4xx.h>

#define FLASH_CONFIG_ADDRESS    0x08004000
#define FLASH_START_ADDRESS     0x08010000

#define RAM_ADDRESS_MAGIC       0x2001FFF0
#define RAM_ADDRESS_CAUSE       0x2001FFF4
#define RAM_ADDRESS_VALUE       0x2001FFF8

#define RAM_MAGIC_WORD          0x55AA00FF

#define BOOT_CAUSE_POWERON      0
#define BOOT_CAUSE_WATCHDOG     1
#define BOOT_CAUSE_UNHANDLED    2
#define BOOT_CAUSE_STACKOVF     3

#define PD_AOUT_STBY            (1 << 2)
#define PD_AOUT_MUTE            (1 << 3)
#define PD_AOUT_FAULT           (1 << 4)
#define PD_AOUT_WARN            (1 << 5)
#define PD_RADIO_RESET          (1 << 6)
#define PD_USB1_EN              (1 << 9)
#define PD_USB2_EN              (1 << 10)
#define PD_CAN_EN               (1 << 14)
#define PD_CAN_STBN             (1 << 15)

#define gpioSet(gpio, pin)      gpio->BSRRL = pin
#define gpioClear(gpio, pin)    gpio->BSRRH = pin

#define AOUT_BUFFER_SIZE        512

#define RADIO_CTL               I2C1
#define AOUT_CTL                I2C3

#define KEY_VOLUME_DOWN         1
#define KEY_VOLUME_UP           2
#define KEY_SEEK_UP             3
#define KEY_SEEK_DOWN           4
#define KEY_MODE                5

#define HW_MODE_STANDBY         0
#define HW_MODE_ACTIVE          1
#define HW_MODE_POWEROFF        2

/* Hardware requests */

#define HWRQ_AOUT_VOLUME_UP     0x0001
#define HWRQ_AOUT_VOLUME_DOWN   0x0002
#define HWRQ_AOUT_VOLUME_SET    0x0003

#define HWRQ_RADIO_TUNE         0x0101
#define HWRQ_RADIO_SEEK         0x0102
#define HWRQ_RADIO_MUTE         0x0103
#define HWRQ_RADIO_MUTE_TOGGLE  0x0104

#define HWRQ_SYSTEM_KEYPRESS    0x0201
#define HWRQ_SYSTEM_CANDUMP     0x0202

typedef struct {
	uint16_t requestId;
	uint16_t wParam;
} hw_request_t;

void usleep(uint32_t usec);

/* hw_aout.c */
void aoutStart(void);
void aoutStop(void);
uint8_t aoutGetVolume(void);
void aoutSetVolume(uint8_t volume);
uint8_t aoutGetRadioMute(void);
void aoutSetRadioMute(uint8_t mute);

/* hw_gpio.c */
void gpioSetMode(GPIO_TypeDef *gpio, uint8_t pin, uint8_t mode);
void gpioSetSpeed(GPIO_TypeDef *gpio, uint8_t pin, uint8_t speed);
void gpioSetAltFunc(GPIO_TypeDef *gpio, uint8_t pin, uint8_t fn);

/* hw_i2c.c */
void i2cInit(I2C_TypeDef *device);
int i2cRead(I2C_TypeDef *device, uint8_t addr, uint8_t *buf, uint32_t len);
int i2cWrite(I2C_TypeDef *device, uint8_t addr, uint8_t *buf, uint32_t len);

/* hw_radio.c */
void radioStart(void);
void radioStop(void);
int16_t *radioGetPlayPosition(void);

void flashUpdateFirmware(uint8_t *data, uint32_t len);
void flashSaveConfig(uint8_t *data, uint32_t len, int poweroff);

void hwInit(void);
uint8_t hwGetMode(void);
void hwSetMode(uint8_t mode);
void hwTask(void *p);
void hwPoll(void);
void hwSendRequest(uint16_t requestId, uint16_t wParam);
uint32_t hwGetBootCause(void);

#endif
