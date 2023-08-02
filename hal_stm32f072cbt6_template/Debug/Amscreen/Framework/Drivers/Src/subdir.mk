################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Amscreen/Framework/Drivers/Src/acp_can.c \
../Amscreen/Framework/Drivers/Src/acp_packet.c \
../Amscreen/Framework/Drivers/Src/debug.c \
../Amscreen/Framework/Drivers/Src/internal_flash_driver.c \
../Amscreen/Framework/Drivers/Src/led.c \
../Amscreen/Framework/Drivers/Src/mcp23017.c \
../Amscreen/Framework/Drivers/Src/pca9546a.c \
../Amscreen/Framework/Drivers/Src/sht4x_driver.c \
../Amscreen/Framework/Drivers/Src/uart.c 

OBJS += \
./Amscreen/Framework/Drivers/Src/acp_can.o \
./Amscreen/Framework/Drivers/Src/acp_packet.o \
./Amscreen/Framework/Drivers/Src/debug.o \
./Amscreen/Framework/Drivers/Src/internal_flash_driver.o \
./Amscreen/Framework/Drivers/Src/led.o \
./Amscreen/Framework/Drivers/Src/mcp23017.o \
./Amscreen/Framework/Drivers/Src/pca9546a.o \
./Amscreen/Framework/Drivers/Src/sht4x_driver.o \
./Amscreen/Framework/Drivers/Src/uart.o 

C_DEPS += \
./Amscreen/Framework/Drivers/Src/acp_can.d \
./Amscreen/Framework/Drivers/Src/acp_packet.d \
./Amscreen/Framework/Drivers/Src/debug.d \
./Amscreen/Framework/Drivers/Src/internal_flash_driver.d \
./Amscreen/Framework/Drivers/Src/led.d \
./Amscreen/Framework/Drivers/Src/mcp23017.d \
./Amscreen/Framework/Drivers/Src/pca9546a.d \
./Amscreen/Framework/Drivers/Src/sht4x_driver.d \
./Amscreen/Framework/Drivers/Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Amscreen/Framework/Drivers/Src/%.o Amscreen/Framework/Drivers/Src/%.su Amscreen/Framework/Drivers/Src/%.cyclo: ../Amscreen/Framework/Drivers/Src/%.c Amscreen/Framework/Drivers/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Tasks/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Task/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Drivers/Src" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Amscreen-2f-Framework-2f-Drivers-2f-Src

clean-Amscreen-2f-Framework-2f-Drivers-2f-Src:
	-$(RM) ./Amscreen/Framework/Drivers/Src/acp_can.cyclo ./Amscreen/Framework/Drivers/Src/acp_can.d ./Amscreen/Framework/Drivers/Src/acp_can.o ./Amscreen/Framework/Drivers/Src/acp_can.su ./Amscreen/Framework/Drivers/Src/acp_packet.cyclo ./Amscreen/Framework/Drivers/Src/acp_packet.d ./Amscreen/Framework/Drivers/Src/acp_packet.o ./Amscreen/Framework/Drivers/Src/acp_packet.su ./Amscreen/Framework/Drivers/Src/debug.cyclo ./Amscreen/Framework/Drivers/Src/debug.d ./Amscreen/Framework/Drivers/Src/debug.o ./Amscreen/Framework/Drivers/Src/debug.su ./Amscreen/Framework/Drivers/Src/internal_flash_driver.cyclo ./Amscreen/Framework/Drivers/Src/internal_flash_driver.d ./Amscreen/Framework/Drivers/Src/internal_flash_driver.o ./Amscreen/Framework/Drivers/Src/internal_flash_driver.su ./Amscreen/Framework/Drivers/Src/led.cyclo ./Amscreen/Framework/Drivers/Src/led.d ./Amscreen/Framework/Drivers/Src/led.o ./Amscreen/Framework/Drivers/Src/led.su ./Amscreen/Framework/Drivers/Src/mcp23017.cyclo ./Amscreen/Framework/Drivers/Src/mcp23017.d ./Amscreen/Framework/Drivers/Src/mcp23017.o ./Amscreen/Framework/Drivers/Src/mcp23017.su ./Amscreen/Framework/Drivers/Src/pca9546a.cyclo ./Amscreen/Framework/Drivers/Src/pca9546a.d ./Amscreen/Framework/Drivers/Src/pca9546a.o ./Amscreen/Framework/Drivers/Src/pca9546a.su ./Amscreen/Framework/Drivers/Src/sht4x_driver.cyclo ./Amscreen/Framework/Drivers/Src/sht4x_driver.d ./Amscreen/Framework/Drivers/Src/sht4x_driver.o ./Amscreen/Framework/Drivers/Src/sht4x_driver.su ./Amscreen/Framework/Drivers/Src/uart.cyclo ./Amscreen/Framework/Drivers/Src/uart.d ./Amscreen/Framework/Drivers/Src/uart.o ./Amscreen/Framework/Drivers/Src/uart.su

.PHONY: clean-Amscreen-2f-Framework-2f-Drivers-2f-Src

