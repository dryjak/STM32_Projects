################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/nRF24/nRF24.c 

OBJS += \
./Core/Src/nRF24/nRF24.o 

C_DEPS += \
./Core/Src/nRF24/nRF24.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/nRF24/%.o Core/Src/nRF24/%.su Core/Src/nRF24/%.cyclo: ../Core/Src/nRF24/%.c Core/Src/nRF24/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-nRF24

clean-Core-2f-Src-2f-nRF24:
	-$(RM) ./Core/Src/nRF24/nRF24.cyclo ./Core/Src/nRF24/nRF24.d ./Core/Src/nRF24/nRF24.o ./Core/Src/nRF24/nRF24.su

.PHONY: clean-Core-2f-Src-2f-nRF24

