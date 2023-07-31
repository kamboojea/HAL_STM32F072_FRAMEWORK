################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Amscreen/Framework/Libs/Src/circular_buf.c \
../Amscreen/Framework/Libs/Src/millis.c 

OBJS += \
./Amscreen/Framework/Libs/Src/circular_buf.o \
./Amscreen/Framework/Libs/Src/millis.o 

C_DEPS += \
./Amscreen/Framework/Libs/Src/circular_buf.d \
./Amscreen/Framework/Libs/Src/millis.d 


# Each subdirectory must supply rules for building sources it contributes
Amscreen/Framework/Libs/Src/%.o Amscreen/Framework/Libs/Src/%.su Amscreen/Framework/Libs/Src/%.cyclo: ../Amscreen/Framework/Libs/Src/%.c Amscreen/Framework/Libs/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Drivers/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Framework/Libs/Inc" -I"C:/Projects/Firmwares/Signs/wafer/V2/HAL_template/HAL_STM32F072_FRAMEWORK/hal_stm32f072cbt6_template/Amscreen/Aplication/Tasks/Inc" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Amscreen-2f-Framework-2f-Libs-2f-Src

clean-Amscreen-2f-Framework-2f-Libs-2f-Src:
	-$(RM) ./Amscreen/Framework/Libs/Src/circular_buf.cyclo ./Amscreen/Framework/Libs/Src/circular_buf.d ./Amscreen/Framework/Libs/Src/circular_buf.o ./Amscreen/Framework/Libs/Src/circular_buf.su ./Amscreen/Framework/Libs/Src/millis.cyclo ./Amscreen/Framework/Libs/Src/millis.d ./Amscreen/Framework/Libs/Src/millis.o ./Amscreen/Framework/Libs/Src/millis.su

.PHONY: clean-Amscreen-2f-Framework-2f-Libs-2f-Src
