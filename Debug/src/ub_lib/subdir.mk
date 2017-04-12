################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ub_lib/stm32_ub_led.c \
../src/ub_lib/stm32_ub_lis3dsh.c \
../src/ub_lib/stm32_ub_spi1.c 

OBJS += \
./src/ub_lib/stm32_ub_led.o \
./src/ub_lib/stm32_ub_lis3dsh.o \
./src/ub_lib/stm32_ub_spi1.o 

C_DEPS += \
./src/ub_lib/stm32_ub_led.d \
./src/ub_lib/stm32_ub_lis3dsh.d \
./src/ub_lib/stm32_ub_spi1.d 


# Each subdirectory must supply rules for building sources it contributes
src/ub_lib/%.o: ../src/ub_lib/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F407VGTx -DSTM32F4 -DSTM32F4DISCOVERY -DSTM32 -DDEBUG -DUSE_STDPERIPH_DRIVER -DSTM32F40XX -DSTM32F40_41xxx -I"/Users/domenicoamelino/Documents/openstm32/workspace/acc_test/inc" -I"/Users/domenicoamelino/Documents/openstm32/workspace/acc_test/CMSIS/core" -I"/Users/domenicoamelino/Documents/openstm32/workspace/acc_test/CMSIS/device" -I"/Users/domenicoamelino/Documents/openstm32/workspace/acc_test/StdPeriph_Driver/inc" -I"/Users/domenicoamelino/Documents/openstm32/workspace/acc_test/Utilities" -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


