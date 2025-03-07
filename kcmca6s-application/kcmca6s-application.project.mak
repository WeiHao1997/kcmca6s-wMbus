####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 15                                              #
####################################################################

BASE_SDK_PATH = D:/SiliconLabs/SDKs/gecko-sdk
UNAME:=$(shell $(POSIX_TOOL_PATH)uname -s | $(POSIX_TOOL_PATH)sed -e 's/^\(CYGWIN\).*/\1/' | $(POSIX_TOOL_PATH)sed -e 's/^\(MINGW\).*/\1/')
ifeq ($(UNAME),MINGW)
# Translate "C:/super" into "/C/super" for MinGW make.
SDK_PATH := /$(shell $(POSIX_TOOL_PATH)echo $(BASE_SDK_PATH) | sed s/://)
endif
SDK_PATH ?= $(BASE_SDK_PATH)
COPIED_SDK_PATH ?= simplicity_sdk_2024.6.2
QUEC_SDK_PATH ?= quectel

# This uses the explicit build rules below
PROJECT_SOURCE_FILES =

C_SOURCE_FILES   += $(filter %.c, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cpp, $(PROJECT_SOURCE_FILES))
CXX_SOURCE_FILES += $(filter %.cc, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.s, $(PROJECT_SOURCE_FILES))
ASM_SOURCE_FILES += $(filter %.S, $(PROJECT_SOURCE_FILES))
LIB_FILES        += $(filter %.a, $(PROJECT_SOURCE_FILES))

C_DEFS += \
 '-DEFR32FG23B010F512IM48=1' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_2_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DSL_BOARD_NAME="BRD2600A"' \
 '-DSL_BOARD_REV="A01"' \
 '-DSL_CLOCK_MANAGER_AUTO_BAND_VALID=1' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_POWER_MANAGER=power_manager' \
 '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' \
 '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer'

ASM_DEFS += \
 '-DEFR32FG23B010F512IM48=1' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_2_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DSL_BOARD_NAME="BRD2600A"' \
 '-DSL_BOARD_REV="A01"' \
 '-DSL_CLOCK_MANAGER_AUTO_BAND_VALID=1' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_POWER_MANAGER=power_manager' \
 '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_RAIL_LIB_MULTIPROTOCOL_SUPPORT=0' \
 '-DSL_RAIL_UTIL_PA_CONFIG_HEADER=<sl_rail_util_pa_config.h>' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer'

INCLUDES += \
 -Iautogen \
 -Iconfig \
 -Iconfig/rail \
 -I. \
 -I$(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Include \
 -I$(COPIED_SDK_PATH)/app/common/util/app_assert \
 -I$(COPIED_SDK_PATH)/platform/common/inc \
 -I$(COPIED_SDK_PATH)/hardware/board/inc \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/src \
 -I$(COPIED_SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/inc \
 -I$(COPIED_SDK_PATH)/platform/driver/debug/inc \
 -I$(COPIED_SDK_PATH)/platform/service/device_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/device_init/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc/s2_signals \
 -I$(COPIED_SDK_PATH)/platform/emdrv/common/inc \
 -I$(COPIED_SDK_PATH)/platform/emlib/inc \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/fem_util \
 -I$(COPIED_SDK_PATH)/platform/peripheral/inc \
 -I$(COPIED_SDK_PATH)/platform/service/hfxo_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc/arm \
 -I$(COPIED_SDK_PATH)/platform/service/iostream/inc \
 -I$(COPIED_SDK_PATH)/platform/driver/leddrv/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config/preset \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/include \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/library \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/src \
 -I$(COPIED_SDK_PATH)/platform/service/mpu/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/nvm3/inc \
 -I$(COPIED_SDK_PATH)/platform/service/power_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/inc \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/common \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/ble \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/ieee802154 \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/wmbus \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/zwave \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/chip/efr32/efr32xg2x \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/protocol/sidewalk \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_callbacks \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/efr32xg23 \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_power_manager_init \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_protocol \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path \
 -I$(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rssi \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src \
 -I$(COPIED_SDK_PATH)/app/flex/component/rail/simple_rail_assistance \
 -I$(COPIED_SDK_PATH)/app/flex/component/rail/sl_flex_rail_channel_selector \
 -I$(COPIED_SDK_PATH)/platform/common/toolchain/inc \
 -I$(COPIED_SDK_PATH)/platform/service/system/inc \
 -I$(COPIED_SDK_PATH)/app/flex/component/rail/sl_wmbus_support \
 -I$(COPIED_SDK_PATH)/platform/service/sleeptimer/inc \
 -I$(COPIED_SDK_PATH)/platform/service/udelay/inc \
 -I$(QUEC_SDK_PATH)/ql_common/inc \
 -I$(QUEC_SDK_PATH)/ql_driver/bsp_nvm/inc \
 -I$(QUEC_SDK_PATH)/ql_driver/bsp_timer/inc \
 -I$(QUEC_SDK_PATH)/ql_driver/bsp_uart/inc \
 -I$(QUEC_SDK_PATH)/ql_driver/bsp_wdt/inc \
 -I$(QUEC_SDK_PATH)/ql_main/inc \
 -I$(QUEC_SDK_PATH)/ql_utils/inc

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \
 -lgcc \
 -lc \
 -lm \
 -lnosys \
 $(COPIED_SDK_PATH)/platform/emdrv/nvm3/lib/libnvm3_CM33_gcc.a \
 $(COPIED_SDK_PATH)/platform/radio/rail_lib/autogen/librail_release/librail_efr32xg23_gcc_release.a

LIBS += $(GROUP_START) $(PROJECT_LIBS) $(GROUP_END)

LIB_FILES += $(filter %.a, $(PROJECT_LIBS))

C_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -std=c18 \
 -Wall \
 -Wextra \
 -Og \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 --specs=nano.specs \
 -g

CXX_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -std=c++17 \
 -fno-rtti \
 -fno-exceptions \
 -Wall \
 -Wextra \
 -Og \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 --specs=nano.specs \
 -g

ASM_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -imacros sl_gcc_preinclude.h \
 -x assembler-with-cpp

LD_FLAGS += \
 -mcpu=cortex-m33 \
 -mthumb \
 -mfpu=fpv5-sp-d16 \
 -mfloat-abi=hard \
 -T"autogen/linkerfile.ld" \
 --specs=nano.specs \
 -Xlinker -Map=$(OUTPUT_DIR)/$(PROJECTNAME).map \
 -Wl,--wrap=_free_r -Wl,--wrap=_malloc_r -Wl,--wrap=_calloc_r -Wl,--wrap=_realloc_r \
 -Wl,--gc-sections \
 -Wl,--no-warn-rwx-segments


####################################################################
# Pre/Post Build Rules                                             #
####################################################################
pre-build:
	# No pre-build defined

post-build: $(OUTPUT_DIR)/$(PROJECTNAME).out
	# No post-build defined

####################################################################
# SDK Build Rules                                                  #
####################################################################
$(OUTPUT_DIR)/sdk/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.o: $(COPIED_SDK_PATH)/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.c
CDEPS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.d
OBJS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/simple_rail_assistance/simple_rail_assistance.o

$(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.o: $(COPIED_SDK_PATH)/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.c
CDEPS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.d
OBJS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_flex_rail_channel_selector/sl_flex_rail_channel_selector.o

$(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.o: $(COPIED_SDK_PATH)/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.c
CDEPS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.d
OBJS += $(OUTPUT_DIR)/sdk/app/flex/component/rail/sl_wmbus_support/sl_wmbus_support.o

$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o: $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/board/src/sl_board_control_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_control_gpio.o

$(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o: $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/board/src/sl_board_init.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/board/src/sl_board_init.o

$(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.o: $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/hardware/driver/configuration_over_swo/src/sl_cos.c
CDEPS += $(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.d
OBJS += $(OUTPUT_DIR)/sdk/hardware/driver/configuration_over_swo/src/sl_cos.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o: $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_assert.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_assert.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.o: $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_core_cortexm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_core_cortexm.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.o: $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_slist.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_slist.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_string.o: $(COPIED_SDK_PATH)/platform/common/src/sl_string.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_string.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_string.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_string.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_string.o

$(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.o: $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/common/src/sl_syscalls.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.d
OBJS += $(OUTPUT_DIR)/sdk/platform/common/src/sl_syscalls.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.o: $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/startup_efr32fg23.o

$(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.o: $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.d
OBJS += $(OUTPUT_DIR)/sdk/platform/Device/SiliconLabs/EFR32FG23/Source/system_efr32fg23.o

$(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.o: $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/debug/src/sl_debug_swo.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/debug/src/sl_debug_swo.o

$(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.o: $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_led.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_led.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_led.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_led.o

$(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.o: $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/driver/leddrv/src/sl_simple_led.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.d
OBJS += $(OUTPUT_DIR)/sdk/platform/driver/leddrv/src/sl_simple_led.o

$(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o: $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_default_common_linker.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_default_common_linker.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_hal_flash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_hal_flash.o

$(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.o: $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/nvm3/src/nvm3_lock.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/nvm3/src/nvm3_lock.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_burtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_burtc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_cmu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_cmu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_core.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_core.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_emu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_emu.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_eusart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_eusart.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpcrc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_gpcrc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_gpcrc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_gpcrc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpcrc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpcrc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_gpio.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_gpio.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_se.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_se.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_system.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_system.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_timer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_timer.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_usart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_usart.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_wdog.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_wdog.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_wdog.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_wdog.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_wdog.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_wdog.o

$(OUTPUT_DIR)/sdk/platform/peripheral/src/sl_hal_sysrtc.o: $(COPIED_SDK_PATH)/platform/peripheral/src/sl_hal_sysrtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/peripheral/src/sl_hal_sysrtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/peripheral/src/sl_hal_sysrtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/peripheral/src/sl_hal_sysrtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/peripheral/src/sl_hal_sysrtc.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/fem_util/sl_fem_util.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_conversions_efr32.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/pa-conversions/pa_curves_efr32.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_power_manager_init/sl_rail_util_power_manager_init.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_protocol/sl_rail_util_protocol.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_pti/sl_rail_util_pti.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rf_path/sl_rail_util_rf_path.o

$(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.o: $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.d
OBJS += $(OUTPUT_DIR)/sdk/platform/radio/rail_lib/plugin/rail_util_rssi/sl_rail_util_rssi.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_attestation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_entropy.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_hash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_hash.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_key_handling.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_signature.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_signature.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sl_se_manager_util.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sl_se_manager_util.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.o: $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/se_manager/src/sli_se_manager_mailbox.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/se_aes.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/se_aes.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_mbedtls_support/src/sl_mbedtls.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_common.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_psa_driver_init.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_builtin_keys.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_key_management.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_driver_signature.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_opaque_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_aead.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_cipher.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_hash.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_driver_mac.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_transparent_key_derivation.o

$(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o: $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.d
OBJS += $(OUTPUT_DIR)/sdk/platform/security/sl_component/sl_psa_driver/src/sli_se_version_dependencies.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_hal_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_hal_s2.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init.o

$(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o: $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/clock_manager/src/sl_clock_manager_init_hal_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_dcdc_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_dcdc_s2.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.o: $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/clocks/sl_device_clock_efr32xg23.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.o: $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/devices/sl_device_peripheral_hal_efr32xg23.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_clock.o: $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_clock.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_clock.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_clock.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_clock.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_clock.o

$(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_peripheral.o: $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_peripheral.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_peripheral.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_manager/src/sl_device_peripheral.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_peripheral.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_manager/src/sl_device_peripheral.o

$(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager.o: $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager.o

$(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.o: $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/hfxo_manager/src/sl_hfxo_manager_hal_s2.o

$(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o: $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_eusart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_eusart.o

$(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.o: $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/iostream/src/sl_iostream_uart.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/iostream/src/sl_iostream_uart.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_dynamic_reservation.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_pool.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_pool.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_region.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_region.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sl_memory_manager_retarget.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sl_memory_manager_retarget.o

$(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.o: $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/memory_manager/src/sli_memory_manager_common.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/memory_manager/src/sli_memory_manager_common.o

$(OUTPUT_DIR)/sdk/platform/service/mpu/src/sl_mpu.o: $(COPIED_SDK_PATH)/platform/service/mpu/src/sl_mpu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/mpu/src/sl_mpu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/mpu/src/sl_mpu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/mpu/src/sl_mpu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/mpu/src/sl_mpu.o

$(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager.o: $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager.o

$(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_debug.o: $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_debug.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_debug.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_debug.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_debug.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_debug.o

$(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_hal_s2.o: $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_hal_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_hal_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/power_manager/src/sl_power_manager_hal_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_hal_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/power_manager/src/sl_power_manager_hal_s2.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_burtc.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_sysrtc.o

$(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o: $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/sleeptimer/src/sl_sleeptimer_hal_timer.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o: $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_init.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_init.o

$(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.o: $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_process_action.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_process_action.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/system/src/sl_system_process_action.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/system/src/sl_system_process_action.o

$(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.o: $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay.o

$(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.o: $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(ASMFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/udelay/src/sl_udelay_armv6m_gcc.S
ASMDEPS_S += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/udelay/src/sl_udelay_armv6m_gcc.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/aes.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/aes.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/constant_time.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/constant_time.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/platform_util.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/platform_util.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_crypto_client.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_crypto_client.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/psa_util.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/psa_util.o

$(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.o: $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/mbedtls/library/threading.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/mbedtls/library/threading.o

$(OUTPUT_DIR)/project/autogen/rail_config.o: autogen/rail_config.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/rail_config.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/rail_config.c
CDEPS += $(OUTPUT_DIR)/project/autogen/rail_config.d
OBJS += $(OUTPUT_DIR)/project/autogen/rail_config.o

$(OUTPUT_DIR)/project/autogen/sl_board_default_init.o: autogen/sl_board_default_init.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_board_default_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_board_default_init.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_board_default_init.o

$(OUTPUT_DIR)/project/autogen/sl_event_handler.o: autogen/sl_event_handler.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_event_handler.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_event_handler.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.o

$(OUTPUT_DIR)/project/autogen/sl_iostream_handles.o: autogen/sl_iostream_handles.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_iostream_handles.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_iostream_handles.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_iostream_handles.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_iostream_handles.o

$(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.o: autogen/sl_iostream_init_eusart_instances.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_iostream_init_eusart_instances.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_iostream_init_eusart_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_iostream_init_eusart_instances.o

$(OUTPUT_DIR)/project/autogen/sl_power_manager_handler.o: autogen/sl_power_manager_handler.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_power_manager_handler.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_power_manager_handler.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_power_manager_handler.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_power_manager_handler.o

$(OUTPUT_DIR)/project/autogen/sl_rail_util_callbacks.o: autogen/sl_rail_util_callbacks.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_rail_util_callbacks.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_rail_util_callbacks.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_rail_util_callbacks.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_rail_util_callbacks.o

$(OUTPUT_DIR)/project/autogen/sl_rail_util_init.o: autogen/sl_rail_util_init.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_rail_util_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_rail_util_init.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_rail_util_init.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_rail_util_init.o

$(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.o: autogen/sl_simple_led_instances.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_simple_led_instances.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_simple_led_instances.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_simple_led_instances.o

##########################QUECTEL SUPPORT SDK##########################
$(OUTPUT_DIR)/quectel/ql_driver/bsp_nvm/src/ql_nvm.o: $(QUEC_SDK_PATH)/ql_driver/bsp_nvm/src/ql_nvm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_driver/bsp_nvm/src/ql_nvm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_driver/bsp_nvm/src/ql_nvm.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_nvm/src/ql_nvm.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_nvm/src/ql_nvm.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.o: $(QUEC_SDK_PATH)/ql_driver/bsp_timer/src/ql_time.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_driver/bsp_timer/src/ql_time.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_driver/bsp_timer/src/ql_time.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_wdt/src/ql_wdt.o: $(QUEC_SDK_PATH)/ql_driver/bsp_wdt/src/ql_wdt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_driver/bsp_wdt/src/ql_wdt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_driver/bsp_wdt/src/ql_wdt.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_wdt/src/ql_wdt.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_wdt/src/ql_wdt.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.o: $(QUEC_SDK_PATH)/ql_driver/bsp_uart/src/ql_uart.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_driver/bsp_uart/src/ql_uart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_driver/bsp_uart/src/ql_uart.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_cmd_handle.o: $(QUEC_SDK_PATH)/ql_main/src/ql_cmd_handle.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_cmd_handle.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_cmd_handle.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_cmd_handle.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_cmd_handle.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_param_cfg.o: $(QUEC_SDK_PATH)/ql_main/src/ql_param_cfg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_param_cfg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_param_cfg.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_param_cfg.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_param_cfg.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_handle.o: $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_handle.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_handle.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_handle.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_handle.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_handle.o

$(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.o: $(QUEC_SDK_PATH)/ql_utils/src/ql_crc16.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_utils/src/ql_crc16.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_utils/src/ql_crc16.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.d
OBJS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.o

$(OUTPUT_DIR)/quectel/ql_utils/src/ql_frame_build.o: $(QUEC_SDK_PATH)/ql_utils/src/ql_frame_build.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_utils/src/ql_frame_build.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_utils/src/ql_frame_build.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_frame_build.d
OBJS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_frame_build.o

$(OUTPUT_DIR)/quectel/ql_utils/src/ql_reset.o: $(QUEC_SDK_PATH)/ql_utils/src/ql_reset.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_utils/src/ql_reset.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_utils/src/ql_reset.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_reset.d
OBJS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_reset.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_version.o: $(QUEC_SDK_PATH)/ql_main/src/ql_version.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_version.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_version.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_version.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_version.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_bind.o: $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_bind.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_bind.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_bind.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_bind.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_bind.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_encryption.o: $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_encryption.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_encryption.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_encryption.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_encryption.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_encryption.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_auto_recv.o: $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_auto_recv.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_auto_recv.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_auto_recv.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_auto_recv.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_auto_recv.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_rf_channel_cfg.o: $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_rf_channel_cfg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_rf_channel_cfg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_wmbus_rf_channel_cfg.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_rf_channel_cfg.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_wmbus_rf_channel_cfg.o

$(OUTPUT_DIR)/quectel/ql_main/src/ql_main.o: $(QUEC_SDK_PATH)/ql_main/src/ql_main.c
	@$(POSIX_TOOL_PATH)echo 'Building $(QUEC_SDK_PATH)/ql_main/src/ql_main.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(QUEC_SDK_PATH)/ql_main/src/ql_main.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_main.d
OBJS += $(OUTPUT_DIR)/quectel/ql_main/src/ql_main.o
##########################QUECTEL SUPPORT SDK##########################

$(OUTPUT_DIR)/project/main.o: main.c
	@$(POSIX_TOOL_PATH)echo 'Building main.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ main.c
CDEPS += $(OUTPUT_DIR)/project/main.d
OBJS += $(OUTPUT_DIR)/project/main.o

# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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=END_SIMPLICITY_STUDIO_METADATA