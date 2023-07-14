################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/stm32g4xx_it.c \
../Core/Src/syscalls.c \
../Core/Src/sysmem.c \
../Core/Src/system_stm32g4xx.c 

CPP_SRCS += \
../Core/Src/AbstractCom.cpp \
../Core/Src/Can.cpp \
../Core/Src/ECU.cpp \
../Core/Src/IOBv1.cpp \
../Core/Src/IOBv3.cpp \
../Core/Src/IOBv4.cpp \
../Core/Src/LCB.cpp \
../Core/Src/PMU.cpp \
../Core/Src/RCU.cpp \
../Core/Src/Radio.cpp \
../Core/Src/Speaker.cpp \
../Core/Src/Ubx.cpp \
../Core/Src/main.cpp 

C_DEPS += \
./Core/Src/stm32g4xx_it.d \
./Core/Src/syscalls.d \
./Core/Src/sysmem.d \
./Core/Src/system_stm32g4xx.d 

OBJS += \
./Core/Src/AbstractCom.o \
./Core/Src/Can.o \
./Core/Src/ECU.o \
./Core/Src/IOBv1.o \
./Core/Src/IOBv3.o \
./Core/Src/IOBv4.o \
./Core/Src/LCB.o \
./Core/Src/PMU.o \
./Core/Src/RCU.o \
./Core/Src/Radio.o \
./Core/Src/Speaker.o \
./Core/Src/Ubx.o \
./Core/Src/main.o \
./Core/Src/stm32g4xx_it.o \
./Core/Src/syscalls.o \
./Core/Src/sysmem.o \
./Core/Src/system_stm32g4xx.o 

CPP_DEPS += \
./Core/Src/AbstractCom.d \
./Core/Src/Can.d \
./Core/Src/ECU.d \
./Core/Src/IOBv1.d \
./Core/Src/IOBv3.d \
./Core/Src/IOBv4.d \
./Core/Src/LCB.d \
./Core/Src/PMU.d \
./Core/Src/RCU.d \
./Core/Src/Radio.d \
./Core/Src/Speaker.d \
./Core/Src/Ubx.d \
./Core/Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.cpp Core/Src/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -DIOBv4_BOARD -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"C:/Users/Zbook15 G5/Programming/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Core/Src/%.o Core/Src/%.su Core/Src/%.cyclo: ../Core/Src/%.c Core/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32G474xx -DUSE_FULL_LL_DRIVER -c -I../Core/Inc -I../Core/Inc/can_houbolt -I../Drivers/STM32G474/STRHAL/Inc -I../Drivers/STM32G474/CMSIS/Include -I../Drivers/STM32G474/LL_Driver/Inc -I../Drivers/STM32G474/CMSIS/Device/ST/STM32G4xx/Include -I"C:/Users/Zbook15 G5/Programming/firmware_liquids/Drivers/STM32G474" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src

clean-Core-2f-Src:
	-$(RM) ./Core/Src/AbstractCom.cyclo ./Core/Src/AbstractCom.d ./Core/Src/AbstractCom.o ./Core/Src/AbstractCom.su ./Core/Src/Can.cyclo ./Core/Src/Can.d ./Core/Src/Can.o ./Core/Src/Can.su ./Core/Src/ECU.cyclo ./Core/Src/ECU.d ./Core/Src/ECU.o ./Core/Src/ECU.su ./Core/Src/IOBv1.cyclo ./Core/Src/IOBv1.d ./Core/Src/IOBv1.o ./Core/Src/IOBv1.su ./Core/Src/IOBv3.cyclo ./Core/Src/IOBv3.d ./Core/Src/IOBv3.o ./Core/Src/IOBv3.su ./Core/Src/IOBv4.cyclo ./Core/Src/IOBv4.d ./Core/Src/IOBv4.o ./Core/Src/IOBv4.su ./Core/Src/LCB.cyclo ./Core/Src/LCB.d ./Core/Src/LCB.o ./Core/Src/LCB.su ./Core/Src/PMU.cyclo ./Core/Src/PMU.d ./Core/Src/PMU.o ./Core/Src/PMU.su ./Core/Src/RCU.cyclo ./Core/Src/RCU.d ./Core/Src/RCU.o ./Core/Src/RCU.su ./Core/Src/Radio.cyclo ./Core/Src/Radio.d ./Core/Src/Radio.o ./Core/Src/Radio.su ./Core/Src/Speaker.cyclo ./Core/Src/Speaker.d ./Core/Src/Speaker.o ./Core/Src/Speaker.su ./Core/Src/Ubx.cyclo ./Core/Src/Ubx.d ./Core/Src/Ubx.o ./Core/Src/Ubx.su ./Core/Src/main.cyclo ./Core/Src/main.d ./Core/Src/main.o ./Core/Src/main.su ./Core/Src/stm32g4xx_it.cyclo ./Core/Src/stm32g4xx_it.d ./Core/Src/stm32g4xx_it.o ./Core/Src/stm32g4xx_it.su ./Core/Src/syscalls.cyclo ./Core/Src/syscalls.d ./Core/Src/syscalls.o ./Core/Src/syscalls.su ./Core/Src/sysmem.cyclo ./Core/Src/sysmem.d ./Core/Src/sysmem.o ./Core/Src/sysmem.su ./Core/Src/system_stm32g4xx.cyclo ./Core/Src/system_stm32g4xx.d ./Core/Src/system_stm32g4xx.o ./Core/Src/system_stm32g4xx.su

.PHONY: clean-Core-2f-Src

