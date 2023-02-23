################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/demo_game.c 

OBJS += \
./src/demo_game.o 

C_DEPS += \
./src/demo_game.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: C Compiler'
	$(CC) -I"pch" -D_DEBUG -I"$(PROJ_PATH)/inc" -O0 -g3 -Wall -c -fmessage-length=0 $(TC_COMPILER_MISC) $(RS_COMPILER_MISC) -fPIE --sysroot="$(SBI_SYSROOT)" -mthumb -I"$(SBI_SYSROOT)/usr/include/libxml2" -I"$(SDK_PATH)/library" -I"$(SBI_SYSROOT)/usr/include" -I"$(SBI_SYSROOT)/usr/include/appcore-agent" -I"$(SBI_SYSROOT)/usr/include/appfw" -I"$(SBI_SYSROOT)/usr/include/badge" -I"$(SBI_SYSROOT)/usr/include/base" -I"$(SBI_SYSROOT)/usr/include/cairo" -I"$(SBI_SYSROOT)/usr/include/calendar-service2" -I"$(SBI_SYSROOT)/usr/include/ckm" -I"$(SBI_SYSROOT)/usr/include/contacts-svc" -I"$(SBI_SYSROOT)/usr/include/content" -I"$(SBI_SYSROOT)/usr/include/context-manager" -I"$(SBI_SYSROOT)/usr/include/dbus-1.0" -I"$(SBI_SYSROOT)/usr/include/device" -I"$(SBI_SYSROOT)/usr/include/dlog" -I"$(SBI_SYSROOT)/usr/include/ecore-1" -I"$(SBI_SYSROOT)/usr/include/e_dbus-1" -I"$(SBI_SYSROOT)/usr/include/edje-1" -I"$(SBI_SYSROOT)/usr/include/eet-1" -I"$(SBI_SYSROOT)/usr/include/efl-extension" -I"$(SBI_SYSROOT)/usr/include/efreet-1" -I"$(SBI_SYSROOT)/usr/include/eina-1" -I"$(SBI_SYSROOT)/usr/include/eina-1/eina" -I"$(SBI_SYSROOT)/usr/include/eio-1" -I"$(SBI_SYSROOT)/usr/include/elementary-1" -I"$(SBI_SYSROOT)/usr/include/embryo-1" -I"$(SBI_SYSROOT)/usr/include/ethumb-1" -I"$(SBI_SYSROOT)/usr/include/evas-1" -I"$(SBI_SYSROOT)/usr/include/ewebkit2-0" -I"$(SBI_SYSROOT)/usr/include/fontconfig" -I"$(SBI_SYSROOT)/usr/include/freetype2" -I"$(SBI_SYSROOT)/usr/include/gio-unix-2.0" -I"$(SBI_SYSROOT)/usr/include/glib-2.0" -I"$(SBI_SYSROOT)/usr/include/json-glib-1.0" -I"$(SBI_SYSROOT)/usr/include/json-glib-1.0/json-glib" -I"$(SBI_SYSROOT)/usr/include/location" -I"$(SBI_SYSROOT)/usr/include/media" -I"$(SBI_SYSROOT)/usr/include/media-content" -I"$(SBI_SYSROOT)/usr/include/messaging" -I"$(SBI_SYSROOT)/usr/include/minizip" -I"$(SBI_SYSROOT)/usr/include/network" -I"$(SBI_SYSROOT)/usr/include/notification" -I"$(SBI_SYSROOT)/usr/include/sensor" -I"$(SBI_SYSROOT)/usr/include/shortcut" -I"$(SBI_SYSROOT)/usr/include/storage" -I"$(SBI_SYSROOT)/usr/include/system" -I"$(SBI_SYSROOT)/usr/include/telephony" -I"$(SBI_SYSROOT)/usr/include/ui" -I"$(SBI_SYSROOT)/usr/include/web" -I"$(SBI_SYSROOT)/usr/include/widget_service" -I"$(SBI_SYSROOT)/usr/include/widget_viewer_evas" -I"$(SBI_SYSROOT)/usr/include/wifi-direct" -I"$(SBI_SYSROOT)/usr/lib/dbus-1.0/include" -I"$(SBI_SYSROOT)/usr/lib/glib-2.0/include" -Werror-implicit-function-declaration -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


