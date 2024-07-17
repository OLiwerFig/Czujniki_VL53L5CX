################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Platform/platform.c 

OBJS += \
./Drivers/Platform/platform.o 

C_DEPS += \
./Drivers/Platform/platform.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Platform/%.o Drivers/Platform/%.su Drivers/Platform/%.cyclo: ../Drivers/Platform/%.c Drivers/Platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L476xx -c -I../Drivers/Platform -I../Drivers/VL53L7CX_ULP_API/Inc -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -I"/Users/oliwerfigura/STM32CubeIDE/workspace_1.15.1/Inzynierka/Drivers/VL53L7CX_ULD_API/inc" -I"/Users/oliwerfigura/STM32CubeIDE/workspace_1.15.1/Inzynierka/Drivers/Platform" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-Platform

clean-Drivers-2f-Platform:
	-$(RM) ./Drivers/Platform/platform.cyclo ./Drivers/Platform/platform.d ./Drivers/Platform/platform.o ./Drivers/Platform/platform.su

.PHONY: clean-Drivers-2f-Platform

