################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Core/Src/Modules/ADS131M04_24bitADC.cpp \
../Core/Src/Modules/ICM2060x_IMU.cpp \
../Core/Src/Modules/LPS25HB_Baro.cpp \
../Core/Src/Modules/LoRa1276F30_Radio.cpp \
../Core/Src/Modules/SAM_M8Q_GNSS.cpp \
../Core/Src/Modules/W25Qxx_Flash.cpp 

OBJS += \
./Core/Src/Modules/ADS131M04_24bitADC.o \
./Core/Src/Modules/ICM2060x_IMU.o \
./Core/Src/Modules/LPS25HB_Baro.o \
./Core/Src/Modules/LoRa1276F30_Radio.o \
./Core/Src/Modules/SAM_M8Q_GNSS.o \
./Core/Src/Modules/W25Qxx_Flash.o 

CPP_DEPS += \
./Core/Src/Modules/ADS131M04_24bitADC.d \
./Core/Src/Modules/ICM2060x_IMU.d \
./Core/Src/Modules/LPS25HB_Baro.d \
./Core/Src/Modules/LoRa1276F30_Radio.d \
./Core/Src/Modules/SAM_M8Q_GNSS.d \
./Core/Src/Modules/W25Qxx_Flash.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Modules/%.o Core/Src/Modules/%.su Core/Src/Modules/%.cyclo: ../Core/Src/Modules/%.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"C:/Users/Zbook15 G5/Programming/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Modules

clean-Core-2f-Src-2f-Modules:
	-$(RM) ./Core/Src/Modules/ADS131M04_24bitADC.cyclo ./Core/Src/Modules/ADS131M04_24bitADC.d ./Core/Src/Modules/ADS131M04_24bitADC.o ./Core/Src/Modules/ADS131M04_24bitADC.su ./Core/Src/Modules/ICM2060x_IMU.cyclo ./Core/Src/Modules/ICM2060x_IMU.d ./Core/Src/Modules/ICM2060x_IMU.o ./Core/Src/Modules/ICM2060x_IMU.su ./Core/Src/Modules/LPS25HB_Baro.cyclo ./Core/Src/Modules/LPS25HB_Baro.d ./Core/Src/Modules/LPS25HB_Baro.o ./Core/Src/Modules/LPS25HB_Baro.su ./Core/Src/Modules/LoRa1276F30_Radio.cyclo ./Core/Src/Modules/LoRa1276F30_Radio.d ./Core/Src/Modules/LoRa1276F30_Radio.o ./Core/Src/Modules/LoRa1276F30_Radio.su ./Core/Src/Modules/SAM_M8Q_GNSS.cyclo ./Core/Src/Modules/SAM_M8Q_GNSS.d ./Core/Src/Modules/SAM_M8Q_GNSS.o ./Core/Src/Modules/SAM_M8Q_GNSS.su ./Core/Src/Modules/W25Qxx_Flash.cyclo ./Core/Src/Modules/W25Qxx_Flash.d ./Core/Src/Modules/W25Qxx_Flash.o ./Core/Src/Modules/W25Qxx_Flash.su

.PHONY: clean-Core-2f-Src-2f-Modules

