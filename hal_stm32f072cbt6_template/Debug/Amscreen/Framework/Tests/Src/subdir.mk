################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Amscreen/Framework/Tests/Src/can_test_task.c \
../Amscreen/Framework/Tests/Src/fan_test_task.c \
../Amscreen/Framework/Tests/Src/internal_flash_test_task.c \
../Amscreen/Framework/Tests/Src/mcp23017_test_task.c \
../Amscreen/Framework/Tests/Src/spi_flash_task.c \
../Amscreen/Framework/Tests/Src/uart_test_task.c 

OBJS += \
./Amscreen/Framework/Tests/Src/can_test_task.o \
./Amscreen/Framework/Tests/Src/fan_test_task.o \
./Amscreen/Framework/Tests/Src/internal_flash_test_task.o \
./Amscreen/Framework/Tests/Src/mcp23017_test_task.o \
./Amscreen/Framework/Tests/Src/spi_flash_task.o \
./Amscreen/Framework/Tests/Src/uart_test_task.o 

C_DEPS += \
./Amscreen/Framework/Tests/Src/can_test_task.d \
./Amscreen/Framework/Tests/Src/fan_test_task.d \
./Amscreen/Framework/Tests/Src/internal_flash_test_task.d \
./Amscreen/Framework/Tests/Src/mcp23017_test_task.d \
./Amscreen/Framework/Tests/Src/spi_flash_task.d \
./Amscreen/Framework/Tests/Src/uart_test_task.d 


# Each subdirectory must supply rules for building sources it contributes
Amscreen/Framework/Tests/Src/%.o Amscreen/Framework/Tests/Src/%.su Amscreen/Framework/Tests/Src/%.cyclo: ../Amscreen/Framework/Tests/Src/%.c Amscreen/Framework/Tests/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Tasks/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Drivers/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Amscreen-2f-Framework-2f-Tests-2f-Src

clean-Amscreen-2f-Framework-2f-Tests-2f-Src:
	-$(RM) ./Amscreen/Framework/Tests/Src/can_test_task.cyclo ./Amscreen/Framework/Tests/Src/can_test_task.d ./Amscreen/Framework/Tests/Src/can_test_task.o ./Amscreen/Framework/Tests/Src/can_test_task.su ./Amscreen/Framework/Tests/Src/fan_test_task.cyclo ./Amscreen/Framework/Tests/Src/fan_test_task.d ./Amscreen/Framework/Tests/Src/fan_test_task.o ./Amscreen/Framework/Tests/Src/fan_test_task.su ./Amscreen/Framework/Tests/Src/internal_flash_test_task.cyclo ./Amscreen/Framework/Tests/Src/internal_flash_test_task.d ./Amscreen/Framework/Tests/Src/internal_flash_test_task.o ./Amscreen/Framework/Tests/Src/internal_flash_test_task.su ./Amscreen/Framework/Tests/Src/mcp23017_test_task.cyclo ./Amscreen/Framework/Tests/Src/mcp23017_test_task.d ./Amscreen/Framework/Tests/Src/mcp23017_test_task.o ./Amscreen/Framework/Tests/Src/mcp23017_test_task.su ./Amscreen/Framework/Tests/Src/spi_flash_task.cyclo ./Amscreen/Framework/Tests/Src/spi_flash_task.d ./Amscreen/Framework/Tests/Src/spi_flash_task.o ./Amscreen/Framework/Tests/Src/spi_flash_task.su ./Amscreen/Framework/Tests/Src/uart_test_task.cyclo ./Amscreen/Framework/Tests/Src/uart_test_task.d ./Amscreen/Framework/Tests/Src/uart_test_task.o ./Amscreen/Framework/Tests/Src/uart_test_task.su

.PHONY: clean-Amscreen-2f-Framework-2f-Tests-2f-Src

