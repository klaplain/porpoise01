################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.c \
../Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.c 

OBJS += \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.o \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.o 

C_DEPS += \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.d \
./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/STM32H7xx_HAL_Driver/Src/%.o Drivers/STM32H7xx_HAL_Driver/Src/%.su Drivers/STM32H7xx_HAL_Driver/Src/%.cyclo: ../Drivers/STM32H7xx_HAL_Driver/Src/%.c Drivers/STM32H7xx_HAL_Driver/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32H725xx -DUSE_PWR_DIRECT_SMPS_SUPPLY -c -I../Core/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc -I../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src

clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src:
	-$(RM) ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_adc_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_cortex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_dma_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_exti.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_flash_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_gpio.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_hsem.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_i2c_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mdma.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_mmc_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_pwr_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rcc_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_rtc_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_sd_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_spi_ex.su
	-$(RM) ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_hal_uart_ex.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_delayblock.su ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.cyclo ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.d ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.o ./Drivers/STM32H7xx_HAL_Driver/Src/stm32h7xx_ll_sdmmc.su

.PHONY: clean-Drivers-2f-STM32H7xx_HAL_Driver-2f-Src

