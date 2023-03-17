################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_disp.c \
../McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_indev.c \
../McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_tick.c 

C_DEPS += \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_disp.d \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_indev.d \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_tick.d 

OBJS += \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_disp.o \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_indev.o \
./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_tick.o 


# Each subdirectory must supply rules for building sources it contributes
McuLib/LittlevGL/lvgl/src/lv_hal/%.o: ../McuLib/LittlevGL/lvgl/src/lv_hal/%.c McuLib/LittlevGL/lvgl/src/lv_hal/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\drivers" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\CMSIS" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\device" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\component\uart" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\utilities" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\board" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\board" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\source" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\drivers" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\CMSIS" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\device" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\component\uart" -I"C:\Users\andre\Documents\GitHub\32CleanBeaches_Firmware\32_LPC_Firmware\utilities" -I../McuLib -I../McuLib/config -I../McuLib/config/fonts -I../McuLib/fonts -I../McuLib/src -I../McuLib/FreeRTOS/Source/include -I../McuLib/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../McuLib/SEGGER_RTT -I../McuLib/SEGGER_Sysview -I../McuLib/TraceRecorder -I../McuLib/TraceRecorder/config -I../McuLib/TraceRecorder/include -I../McuLib/TraceRecorder/streamports/Jlink_RTT/include -I../McuLib/HD44780 -I../McuLib/FatFS -I../McuLib/FatFS/source -include"C:/Users/andre/Documents/GitHub/32CleanBeaches_Firmware/32_LPC_Firmware/source/IncludeMcuLibConfig.h" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-McuLib-2f-LittlevGL-2f-lvgl-2f-src-2f-lv_hal

clean-McuLib-2f-LittlevGL-2f-lvgl-2f-src-2f-lv_hal:
	-$(RM) ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_disp.d ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_disp.o ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_indev.d ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_indev.o ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_tick.d ./McuLib/LittlevGL/lvgl/src/lv_hal/lv_hal_tick.o

.PHONY: clean-McuLib-2f-LittlevGL-2f-lvgl-2f-src-2f-lv_hal

