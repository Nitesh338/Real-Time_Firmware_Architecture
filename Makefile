PRJ = Booting
BOARD ?= stm32F446RE
CPU ?= cortex-m4
FREERTOS_PATH = ./FreeRTOS-Kernel
BUILD_DIR = build

SRC = ./SourceCode/src/OS_task.c \
	  ./SourceCode/src/OS_timer.c \
	  ./SourceCode/src/Startup.c \
	  ./SourceCode/src/OS_Init.c \
	  ./SourceCode/src/drivers/nvm_drv.c \
	  ./SourceCode/src/nvm_hwab.c \
	  ./SourceCode/src/appl_main.c \
	  $(FREERTOS_PATH)/timers.c \
	  $(FREERTOS_PATH)/tasks.c \
	  $(FREERTOS_PATH)/queue.c \
	  $(FREERTOS_PATH)/list.c \
	  $(FREERTOS_PATH)/portable/GCC/ARM_CM3/port.c \
	  $(FREERTOS_PATH)/portable/MemMang/heap_4.c

ASS = ./SourceCode/asm/boot.S
#$(shell find -iname "*.S")


OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o) $(ASS:%.S=$(BUILD_DIR)/%.o)

INC = -I$(FREERTOS_PATH)/portable/GCC/ARM_CM3/ \
	  -I$(FREERTOS_PATH)/include/ \
	  -I./SourceCode/include

$(BUILD_DIR)/%.o:%.c
	@mkdir -p $(dir $@) 
	@echo "creating object files from source files"
	@arm-none-eabi-gcc $(INC) -mthumb -mcpu=$(CPU) -g -c $^ -o $@
$(BUILD_DIR)/%.o:%.S
	@mkdir -p $(dir $@) 
	@echo "creating object files from assembly files"
	@arm-none-eabi-as -mthumb -mcpu=$(CPU) -g -c $^ -o $@

all:$(BUILD_DIR)/$(PRJ).elf

$(BUILD_DIR)/$(PRJ).elf:$(OBJ)
	@echo "Linking..."
	@arm-none-eabi-ld -T.\SourceCode\linker\map.ld $^ -o $@
	@arm-none-eabi-objdump -D -S $@ > $@.lst
	@arm-none-eabi-readelf -a $@ > $@.debug
	@echo "deleting object files"
	@rm -rf $(OBJ)


flash:
	st-flash write $(BUILD_DIR)/$(PRJ).elf 0x8000000

gdb:
	arm-none-eabi-gdb $(BUILD_DIR)/$(PRJ).elf
clean:
	@rm -rf $(OBJ) $(BUILD_DIR)
