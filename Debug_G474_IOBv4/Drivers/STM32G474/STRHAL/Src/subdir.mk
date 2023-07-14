################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32G474/STRHAL/Src/STRHAL.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_Container.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.c \
../Drivers/STM32G474/STRHAL/Src/STRHAL_UART.c 

C_DEPS += \
./Drivers/STM32G474/STRHAL/Src/STRHAL.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.d \
./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.d 

OBJS += \
./Drivers/STM32G474/STRHAL/Src/STRHAL.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.o \
./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32G474/STRHAL/Src/%.o Drivers/STM32G474/STRHAL/Src/%.su Drivers/STM32G474/STRHAL/Src/%.cyclo: ../Drivers/STM32G474/STRHAL/Src/%.c Drivers/STM32G474/STRHAL/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"C:/Users/Zbook15 G5/Programming/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32G474-2f-STRHAL-2f-Src

clean-Drivers-2f-STM32G474-2f-STRHAL-2f-Src:
	-$(RM) ./Drivers/STM32G474/STRHAL/Src/STRHAL.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL.d ./Drivers/STM32G474/STRHAL/Src/STRHAL.o ./Drivers/STM32G474/STRHAL/Src/STRHAL.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_ADC.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_CAN.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_Clock.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_Container.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_GPIO.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_OPAMP.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_QSPI.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_SPI.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_SysTick.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_TIM.su ./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.cyclo ./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.d ./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.o ./Drivers/STM32G474/STRHAL/Src/STRHAL_UART.su

.PHONY: clean-Drivers-2f-STM32G474-2f-STRHAL-2f-Src

