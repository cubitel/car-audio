
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
ARCH=arm-none-eabi-ar
DEBUG=-g
OPTIM=-Os
LDSCRIPT=stm32_flash.ld

CFLAGS= \
-I. \
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

LINKER_FLAGS=-Xlinker -obootloader.elf -Xlinker -M -Xlinker -Map=bootloader.map

CRT0=startup.s

SRC= main.c system_stm32f4xx.c

OBJ = $(SRC:.c=.o)

COLOR = \033[1;31m
COLOR_CLEAR = \033[0m
PRINT = @printf "$(COLOR)%s$(COLOR_CLEAR)\n"

all: bootloader.bin

flash: bootloader.bin
	sudo dfu-util -a 0 -s 0x08000000 -D bootloader.bin

bootloader.bin : bootloader.elf
	$(OBJCOPY) $< -O binary $@

bootloader.elf : $(OBJ) Makefile
	$(PRINT) "> linking"
	@ $(CC) $(CFLAGS) $(OBJ) $(CRT0) $(LINKER_FLAGS)

$(OBJ) : %.o : %.c $(LDSCRIPT) Makefile
	$(PRINT) "> compiling ("$<")"
	@ $(CC) -c $(CFLAGS) $< -o $@
