################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../board/board.c \
../board/clock_config.c \
../board/peripherals.c \
../board/pin_mux.c 

C_DEPS += \
./board/board.d \
./board/clock_config.d \
./board/peripherals.d \
./board/pin_mux.d 

OBJS += \
./board/board.o \
./board/clock_config.o \
./board/peripherals.o \
./board/pin_mux.o 


# Each subdirectory must supply rules for building sources it contributes
board/%.o: ../board/%.c board/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DCPU_LPC845M301JBD48 -DCPU_LPC845M301JBD48_cm0plus -DSDK_OS_BAREMETAL -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\drivers" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\CMSIS" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\device" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\component\uart" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\utilities" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\board" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\board" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\source" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\drivers" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\CMSIS" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\device" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\component\uart" -I"C:\Users\andre\OneDrive - Hochschule Luzern\00_Modules\FS23_AEMBS\00_workspace_AEMBS_V11.7.0\32_LPC_Firmware\utilities" -I../McuLib -I../McuLib/config -I../McuLib/config/fonts -I../McuLib/fonts -I../McuLib/src -I../McuLib/FreeRTOS/Source/include -I../McuLib/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../McuLib/SEGGER_RTT -I../McuLib/SEGGER_Sysview -I../McuLib/TraceRecorder -I../McuLib/TraceRecorder/config -I../McuLib/TraceRecorder/include -I../McuLib/TraceRecorder/streamports/Jlink_RTT/include -I../McuLib/HD44780 -I../McuLib/FatFS -I../McuLib/FatFS/source -include"C:/Users/andre/OneDrive - Hochschule Luzern/00_Modules/FS23_AEMBS/00_workspace_AEMBS_V11.7.0/32_LPC_Firmware/source/IncludeMcuLibConfig.h" -O0 -fno-common -g3 -Wall -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m0plus -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-board

clean-board:
	-$(RM) ./board/board.d ./board/board.o ./board/clock_config.d ./board/clock_config.o ./board/peripherals.d ./board/peripherals.o ./board/pin_mux.d ./board/pin_mux.o

.PHONY: clean-board

