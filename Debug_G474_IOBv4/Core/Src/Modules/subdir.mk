################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
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
Core/Src/Modules/ADS131M04_24bitADC.o: ../Core/Src/Modules/ADS131M04_24bitADC.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/ADS131M04_24bitADC.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Modules/ICM2060x_IMU.o: ../Core/Src/Modules/ICM2060x_IMU.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/ICM2060x_IMU.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Modules/LPS25HB_Baro.o: ../Core/Src/Modules/LPS25HB_Baro.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/LPS25HB_Baro.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Modules/LoRa1276F30_Radio.o: ../Core/Src/Modules/LoRa1276F30_Radio.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/LoRa1276F30_Radio.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Modules/SAM_M8Q_GNSS.o: ../Core/Src/Modules/SAM_M8Q_GNSS.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/SAM_M8Q_GNSS.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/Modules/W25Qxx_Flash.o: ../Core/Src/Modules/W25Qxx_Flash.cpp Core/Src/Modules/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Core/Src/Modules/W25Qxx_Flash.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

