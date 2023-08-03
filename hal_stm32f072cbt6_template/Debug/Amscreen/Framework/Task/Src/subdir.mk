################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Amscreen/Framework/Task/Src/acp_can_process.c \
../Amscreen/Framework/Task/Src/acp_notification.c 

OBJS += \
./Amscreen/Framework/Task/Src/acp_can_process.o \
./Amscreen/Framework/Task/Src/acp_notification.o 

C_DEPS += \
./Amscreen/Framework/Task/Src/acp_can_process.d \
./Amscreen/Framework/Task/Src/acp_notification.d 


# Each subdirectory must supply rules for building sources it contributes
Amscreen/Framework/Task/Src/%.o Amscreen/Framework/Task/Src/%.su Amscreen/Framework/Task/Src/%.cyclo: ../Amscreen/Framework/Task/Src/%.c Amscreen/Framework/Task/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Tasks/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Task/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Drivers/Src" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Amscreen-2f-Framework-2f-Task-2f-Src

clean-Amscreen-2f-Framework-2f-Task-2f-Src:
	-$(RM) ./Amscreen/Framework/Task/Src/acp_can_process.cyclo ./Amscreen/Framework/Task/Src/acp_can_process.d ./Amscreen/Framework/Task/Src/acp_can_process.o ./Amscreen/Framework/Task/Src/acp_can_process.su ./Amscreen/Framework/Task/Src/acp_notification.cyclo ./Amscreen/Framework/Task/Src/acp_notification.d ./Amscreen/Framework/Task/Src/acp_notification.o ./Amscreen/Framework/Task/Src/acp_notification.su

.PHONY: clean-Amscreen-2f-Framework-2f-Task-2f-Src

