################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../source/TSI_module.c \
../source/cb_fifo.c \
../source/cmd_handle.c \
../source/delay.c \
../source/i2c.c \
../source/leds.c \
../source/main.c \
../source/mma8451.c \
../source/mtb.c \
../source/semihost_hardfault.c \
../source/sysclock.c \
../source/systick.c \
../source/uart.c 

C_DEPS += \
./source/TSI_module.d \
./source/cb_fifo.d \
./source/cmd_handle.d \
./source/delay.d \
./source/i2c.d \
./source/leds.d \
./source/main.d \
./source/mma8451.d \
./source/mtb.d \
./source/semihost_hardfault.d \
./source/sysclock.d \
./source/systick.d \
./source/uart.d 

OBJS += \
./source/TSI_module.o \
./source/cb_fifo.o \
./source/cmd_handle.o \
./source/delay.o \
./source/i2c.o \
./source/leds.o \
./source/main.o \
./source/mma8451.o \
./source/mtb.o \
./source/semihost_hardfault.o \
./source/sysclock.o \
./source/systick.o \
./source/uart.o 


# Each subdirectory must supply rules for building sources it contributes
source/%.o: ../source/%.c source/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_MKL25Z128VLK4 -DCPU_MKL25Z128VLK4_cm0plus -DFSL_RTOS_BM -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"D:\Downloads\Desktop\pes_finalproject\board" -I"D:\Downloads\Desktop\pes_finalproject\source" -I"D:\Downloads\Desktop\pes_finalproject" -I"D:\Downloads\Desktop\pes_finalproject\drivers" -I"D:\Downloads\Desktop\pes_finalproject\CMSIS" -I"D:\Downloads\Desktop\pes_finalproject\utilities" -I"D:\Downloads\Desktop\pes_finalproject\startup" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-source

clean-source:
	-$(RM) ./source/TSI_module.d ./source/TSI_module.o ./source/cb_fifo.d ./source/cb_fifo.o ./source/cmd_handle.d ./source/cmd_handle.o ./source/delay.d ./source/delay.o ./source/i2c.d ./source/i2c.o ./source/leds.d ./source/leds.o ./source/main.d ./source/main.o ./source/mma8451.d ./source/mma8451.o ./source/mtb.d ./source/mtb.o ./source/semihost_hardfault.d ./source/semihost_hardfault.o ./source/sysclock.d ./source/sysclock.o ./source/systick.d ./source/systick.o ./source/uart.d ./source/uart.o

.PHONY: clean-source

