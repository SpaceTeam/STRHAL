################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/Channels/ADCChannel.cpp \
../Core/Src/Channels/AbstractChannel.cpp \
../Core/Src/Channels/AbstractControlInputChannel.cpp \
../Core/Src/Channels/AbstractControlOutputChannel.cpp \
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
./Core/Src/Channels/AbstractControlInputChannel.o \
./Core/Src/Channels/AbstractControlOutputChannel.o \
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
./Core/Src/Channels/AbstractControlInputChannel.d \
./Core/Src/Channels/AbstractControlOutputChannel.d \
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
Core/Src/Channels/%.o Core/Src/Channels/%.su Core/Src/Channels/%.cyclo: ../Core/Src/Channels/%.cpp Core/Src/Channels/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"C:/Users/Zbook15 G5/Programming/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Channels

clean-Core-2f-Src-2f-Channels:
	-$(RM) ./Core/Src/Channels/ADCChannel.cyclo ./Core/Src/Channels/ADCChannel.d ./Core/Src/Channels/ADCChannel.o ./Core/Src/Channels/ADCChannel.su ./Core/Src/Channels/AbstractChannel.cyclo ./Core/Src/Channels/AbstractChannel.d ./Core/Src/Channels/AbstractChannel.o ./Core/Src/Channels/AbstractChannel.su ./Core/Src/Channels/AbstractControlInputChannel.cyclo ./Core/Src/Channels/AbstractControlInputChannel.d ./Core/Src/Channels/AbstractControlInputChannel.o ./Core/Src/Channels/AbstractControlInputChannel.su ./Core/Src/Channels/AbstractControlOutputChannel.cyclo ./Core/Src/Channels/AbstractControlOutputChannel.d ./Core/Src/Channels/AbstractControlOutputChannel.o ./Core/Src/Channels/AbstractControlOutputChannel.su ./Core/Src/Channels/BaroChannel.cyclo ./Core/Src/Channels/BaroChannel.d ./Core/Src/Channels/BaroChannel.o ./Core/Src/Channels/BaroChannel.su ./Core/Src/Channels/Data32Channel.cyclo ./Core/Src/Channels/Data32Channel.d ./Core/Src/Channels/Data32Channel.o ./Core/Src/Channels/Data32Channel.su ./Core/Src/Channels/DigitalInChannel.cyclo ./Core/Src/Channels/DigitalInChannel.d ./Core/Src/Channels/DigitalInChannel.o ./Core/Src/Channels/DigitalInChannel.su ./Core/Src/Channels/DigitalOutChannel.cyclo ./Core/Src/Channels/DigitalOutChannel.d ./Core/Src/Channels/DigitalOutChannel.o ./Core/Src/Channels/DigitalOutChannel.su ./Core/Src/Channels/GenericChannel.cyclo ./Core/Src/Channels/GenericChannel.d ./Core/Src/Channels/GenericChannel.o ./Core/Src/Channels/GenericChannel.su ./Core/Src/Channels/IMUChannel.cyclo ./Core/Src/Channels/IMUChannel.d ./Core/Src/Channels/IMUChannel.o ./Core/Src/Channels/IMUChannel.su ./Core/Src/Channels/PressureControlChannel.cyclo ./Core/Src/Channels/PressureControlChannel.d ./Core/Src/Channels/PressureControlChannel.o ./Core/Src/Channels/PressureControlChannel.su ./Core/Src/Channels/PyroChannel.cyclo ./Core/Src/Channels/PyroChannel.d ./Core/Src/Channels/PyroChannel.o ./Core/Src/Channels/PyroChannel.su ./Core/Src/Channels/RocketChannel.cyclo ./Core/Src/Channels/RocketChannel.d ./Core/Src/Channels/RocketChannel.o ./Core/Src/Channels/RocketChannel.su ./Core/Src/Channels/ServoChannel.cyclo ./Core/Src/Channels/ServoChannel.d ./Core/Src/Channels/ServoChannel.o ./Core/Src/Channels/ServoChannel.su

.PHONY: clean-Core-2f-Src-2f-Channels

