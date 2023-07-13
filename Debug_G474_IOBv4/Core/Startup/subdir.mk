################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32g491metx.s 

S_DEPS += \
./Core/Startup/startup_stm32g491metx.d 

OBJS += \
./Core/Startup/startup_stm32g491metx.o 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32g491metx.o: ../Core/Startup/startup_stm32g491metx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -I"/home/georg/data/1_Uni/z_SpaceTeam/3_Software/firmware_liquids/Drivers/STM32G474" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32g491metx.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@" "$<"

