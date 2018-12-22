
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
ARCH=arm-none-eabi-ar
DEBUG=-g
OPTIM=-Os
LDSCRIPT=stm32/stm32_flash.ld

CFLAGS= \
-Isrc -Isrc/protobuf -Istm32 -Istm32usb -IFreeRTOS/Source/include -IFreeRTOS/Source/portable/GCC/ARM_CM4F \
-IFreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/include -IFreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/Compiler/GCC \
-mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork -mfloat-abi=hard -mfpu=fpv4-sp-d16 \
-Wall  \
-Wextra  \
-Wstrict-prototypes  \
-Wmissing-prototypes  \
-Wmissing-declarations  \
-Wno-strict-aliasing  \
-Wno-unused-parameter \
-Wno-unused-function \
-D GCC_ARM7_ECLIPSE  \
-D THUMB_INTERWORK \
-T$(LDSCRIPT) \
$(DEBUG)  \
$(OPTIM) \
-fomit-frame-pointer

LINKER_FLAGS=-Xlinker -ofirmware.elf -Xlinker -M -Xlinker -Map=firmware.map

CRT0=src/startup.s

SRC= \
  src/main.c \
  src/config.c \
  src/hardware.c \
  src/hw/aout.c \
  src/hw/can.c \
  src/hw/gpio.c \
  src/hw/i2c.c \
  src/hw/radio.c \
  src/netserv.c \
  src/sha1.c \
  src/isotp.c \
  src/usb.c \
  src/usb_audio.c \
  src/usb_ecm.c \
  src/usb_class.c \
  src/usbd_desc.c \
  src/protobuf/main.pb.c \
  src/protobuf/config.pb.c \
  src/protobuf/pb_common.c \
  src/protobuf/pb_encode.c \
  src/protobuf/pb_decode.c \
  src/cars/api.c \
  src/cars/ford_cd3xx.c \
  src/radio/api.c \
  src/radio/rds.c \
  src/radio/si47xx.c \
  stm32usb/usb_core.c \
  stm32usb/usb_dcd.c \
  stm32usb/usb_dcd_int.c \
  stm32usb/usbd_core.c \
  stm32usb/usbd_req.c \
  stm32usb/usbd_ioreq.c \
  stm32/system_stm32f4xx.c \
  FreeRTOS/Source/event_groups.c \
  FreeRTOS/Source/portable/MemMang/heap_4.c \
  FreeRTOS/Source/list.c \
  FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c \
  FreeRTOS/Source/queue.c \
  FreeRTOS/Source/tasks.c \
  FreeRTOS/Source/timers.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/portable/BufferManagement/BufferAllocation_2.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_ARP.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_DHCP.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_DNS.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_IP.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_Sockets.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_Stream_Buffer.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_TCP_IP.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_TCP_WIN.c \
  FreeRTOS-Plus/Source/FreeRTOS-Plus-TCP/FreeRTOS_UDP_IP.c

OBJ = $(SRC:.c=.o)

COLOR = \033[1;31m
COLOR_CLEAR = \033[0m
PRINT = @printf "$(COLOR)%s$(COLOR_CLEAR)\n"

all: firmware.bin

flash: firmware.bin
	sudo dfu-util -a 0 -s 0x08010000:leave -D firmware.bin

update: firmware.bin
	wget http://192.168.91.11/firmware --method=PUT --body-file=firmware.bin -O /dev/null

firmware.bin : firmware.elf
	$(OBJCOPY) $< -O binary $@
	openssl sha1 -binary < $@ >> $@

firmware.elf : $(OBJ) Makefile
	$(PRINT) "> linking"
	@ $(CC) $(CFLAGS) $(OBJ) $(CRT0) $(LINKER_FLAGS)

$(OBJ) : %.o : %.c $(LDSCRIPT) Makefile
	$(PRINT) "> compiling ("$<")"
	@ $(CC) -c $(CFLAGS) $< -o $@
