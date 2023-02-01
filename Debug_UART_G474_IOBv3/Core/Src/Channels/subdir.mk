################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/Channels/ADCChannel.cpp \
../Core/Src/Channels/AbstractChannel.cpp \
../Core/Src/Channels/BaroChannel.cpp \
../Core/Src/Channels/Data32Channel.cpp \
../Core/Src/Channels/DigitalInChannel.cpp \
../Core/Src/Channels/DigitalOutChannel.cpp \
../Core/Src/Channels/GenericChannel.cpp \
../Core/Src/Channels/IMUChannel.cpp \
../Core/Src/Channels/PressureControlChannel.cpp \
../Core/Src/Channels/PyroChannel.cpp \
../Core/Src/Channels/RocketChannel.cpp \
../Core/Src/Channels/ServoChannel.cpp 

OBJS += \
./Core/Src/Channels/ADCChannel.o \
./Core/Src/Channels/AbstractChannel.o \
./Core/Src/Channels/BaroChannel.o \
./Core/Src/Channels/Data32Channel.o \
./Core/Src/Channels/DigitalInChannel.o \
./Core/Src/Channels/DigitalOutChannel.o \
./Core/Src/Channels/GenericChannel.o \
./Core/Src/Channels/IMUChannel.o \
./Core/Src/Channels/PressureControlChannel.o \
./Core/Src/Channels/PyroChannel.o \
./Core/Src/Channels/RocketChannel.o \
./Core/Src/Channels/ServoChannel.o 

CPP_DEPS += \
./Core/Src/Channels/ADCChannel.d \
./Core/Src/Channels/AbstractChannel.d \
./Core/Src/Channels/BaroChannel.d \
./Core/Src/Channels/Data32Channel.d \
./Core/Src/Channels/DigitalInChannel.d \
./Core/Src/Channels/DigitalOutChannel.d \
./Core/Src/Channels/GenericChannel.d \
./Core/Src/Channels/IMUChannel.d \
./Core/Src/Channels/PressureControlChannel.d \
./Core/Src/Channels/PyroChannel.d \
./Core/Src/Channels/RocketChannel.d \
./Core/Src/Channels/ServoChannel.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Channels/ADCChannel.o: ../Core/Src/Channels/ADCChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/ADCChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/AbstractChannel.o: ../Core/Src/Channels/AbstractChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/AbstractChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/BaroChannel.o: ../Core/Src/Channels/BaroChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/BaroChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/Data32Channel.o: ../Core/Src/Channels/Data32Channel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/Data32Channel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/DigitalInChannel.o: ../Core/Src/Channels/DigitalInChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/DigitalInChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/DigitalOutChannel.o: ../Core/Src/Channels/DigitalOutChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/DigitalOutChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/GenericChannel.o: ../Core/Src/Channels/GenericChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/GenericChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/IMUChannel.o: ../Core/Src/Channels/IMUChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/IMUChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/PressureControlChannel.o: ../Core/Src/Channels/PressureControlChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/PressureControlChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/PyroChannel.o: ../Core/Src/Channels/PyroChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/PyroChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/RocketChannel.o: ../Core/Src/Channels/RocketChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/RocketChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Channels/ServoChannel.o: ../Core/Src/Channels/ServoChannel.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUART_DEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv3_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Channels/ServoChannel.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

