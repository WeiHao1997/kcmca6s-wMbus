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
 '-DDEBUG_EFM=1' \
 '-DEFR32FG23B010F512IM48=1' \
 '-DSE_MANAGER_CONFIG_FILE="btl_aes_ctr_stream_block_cfg.h"' \
 '-DBOOTLOADER_ENABLE=1' \
 '-DBOOTLOADER_SECOND_STAGE=1' \
 '-DSL_RAMFUNC_DISABLE=1' \
 '-D__START=main' \
 '-D__STARTUP_CLEAR_BSS=1' \
 '-DBTL_PARSER_NO_SUPPORT_ENCRYPTION=1' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_2_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DSL_BOARD_NAME="BRD2600A"' \
 '-DSL_BOARD_REV="A01"' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer'

ASM_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFR32FG23B010F512IM48=1' \
 '-DSE_MANAGER_CONFIG_FILE="btl_aes_ctr_stream_block_cfg.h"' \
 '-DBOOTLOADER_ENABLE=1' \
 '-DBOOTLOADER_SECOND_STAGE=1' \
 '-DSL_RAMFUNC_DISABLE=1' \
 '-D__START=main' \
 '-D__STARTUP_CLEAR_BSS=1' \
 '-DBTL_PARSER_NO_SUPPORT_ENCRYPTION=1' \
 '-DHARDWARE_BOARD_DEFAULT_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_2_RF_BANDS=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_868=1' \
 '-DHARDWARE_BOARD_SUPPORTS_RF_BAND_915=1' \
 '-DHFXO_FREQ=39000000' \
 '-DSL_BOARD_NAME="BRD2600A"' \
 '-DSL_BOARD_REV="A01"' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DSL_CODE_COMPONENT_PERIPHERAL_SYSRTC=hal_sysrtc' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DMBEDTLS_CONFIG_FILE=<sl_mbedtls_config.h>' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CLIENT=1' \
 '-DMBEDTLS_PSA_CRYPTO_CONFIG_FILE=<psa_crypto_config.h>' \
 '-DSL_CODE_COMPONENT_CORE=core' \
 '-DSL_CODE_COMPONENT_SLEEPTIMER=sleeptimer'

INCLUDES += \
 -Iconfig \
 -Iautogen \
 -I. \
 -I$(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Include \
 -I$(COPIED_SDK_PATH)/platform/common/inc \
 -I$(COPIED_SDK_PATH)/hardware/board/inc \
 -I$(COPIED_SDK_PATH)/platform/bootloader \
 -I$(COPIED_SDK_PATH)/platform/bootloader/debug \
 -I$(COPIED_SDK_PATH)/platform/bootloader/parser \
 -I$(COPIED_SDK_PATH)/platform/bootloader/api \
 -I$(COPIED_SDK_PATH)/platform/bootloader/security \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/clock_manager/src \
 -I$(COPIED_SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(COPIED_SDK_PATH)/platform/service/device_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/device_init/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/dmadrv/inc/s2_signals \
 -I$(COPIED_SDK_PATH)/platform/emdrv/common/inc \
 -I$(COPIED_SDK_PATH)/platform/emlib/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/inc \
 -I$(COPIED_SDK_PATH)/platform/peripheral/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc/arm \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/config/preset \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_mbedtls_support/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/include \
 -I$(COPIED_SDK_PATH)/util/third_party/mbedtls/library \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/src \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/sl_psa_driver/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/security/sl_component/se_manager/src \
 -I$(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/lib/fih/inc \
 -I$(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/platform/include \
 -I$(COPIED_SDK_PATH)/platform/common/toolchain/inc \
 -I$(COPIED_SDK_PATH)/platform/service/system/inc \
 -I$(COPIED_SDK_PATH)/platform/service/sleeptimer/inc \
 -I$(COPIED_SDK_PATH)/platform/emdrv/uartdrv/inc \
 -I${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/inc \
 -I${QUEC_SDK_PATH}/ql_common/inc \
 -I${QUEC_SDK_PATH}/ql_driver/bsp_internal_flash/inc \
 -I${QUEC_SDK_PATH}/ql_driver/bsp_uart/inc \
 -I${QUEC_SDK_PATH}/ql_utils/inc \
 -I${QUEC_SDK_PATH}/ql_driver/bsp_timer/inc

GROUP_START =-Wl,--start-group
GROUP_END =-Wl,--end-group

PROJECT_LIBS = \
 -lgcc \
 -lc \
 -lm \
 -lnosys

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
 -Os \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 -Wno-ignored-qualifiers \
 -Wno-sign-compare \
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
 -Os \
 -fdata-sections \
 -ffunction-sections \
 -fomit-frame-pointer \
 -imacros sl_gcc_preinclude.h \
 -mcmse \
 -Wno-ignored-qualifiers \
 -Wno-sign-compare \
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

$(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_bootload.o: $(COPIED_SDK_PATH)/platform/bootloader/core/btl_bootload.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/btl_bootload.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/btl_bootload.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_bootload.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_bootload.o

$(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_core.o: $(COPIED_SDK_PATH)/platform/bootloader/core/btl_core.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/btl_core.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/btl_core.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_core.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_core.o

#######################################################################
#                          QUECTEL SUPPORT SDK                        #
#######################################################################
$(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_bootlader.o: ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_bootlader.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_bootlader.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_bootlader.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_bootlader.d
OBJS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_bootlader.o

$(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_frame_build.o: ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_frame_build.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_frame_build.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_frame_build.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_frame_build.d
OBJS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_frame_build.o

$(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_upgrade.o: ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_upgrade.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_upgrade.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_bootlader/ql_upgrade/src/ql_upgrade.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_upgrade.d
OBJS += $(OUTPUT_DIR)/quectel/ql_bootlader/ql_upgrade/src/ql_upgrade.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_internal_flash/src/ql_internal_flash.o: ${QUEC_SDK_PATH}/ql_driver/bsp_internal_flash/src/ql_internal_flash.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_driver/bsp_internal_flash/src/ql_internal_flash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_driver/bsp_internal_flash/src/ql_internal_flash.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_internal_flash/src/ql_internal_flash.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_internal_flash/src/ql_internal_flash.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.o: ${QUEC_SDK_PATH}/ql_driver/bsp_timer/src/ql_time.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_driver/bsp_timer/src/ql_time.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_driver/bsp_timer/src/ql_time.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_timer/src/ql_time.o

$(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.o: ${QUEC_SDK_PATH}/ql_driver/bsp_uart/src/ql_uart.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_driver/bsp_uart/src/ql_uart.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_driver/bsp_uart/src/ql_uart.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.d
OBJS += $(OUTPUT_DIR)/quectel/ql_driver/bsp_uart/src/ql_uart.o

$(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.o: ${QUEC_SDK_PATH}/ql_utils/src/ql_crc16.c
	@$(POSIX_TOOL_PATH)echo 'Building ${QUEC_SDK_PATH}/ql_utils/src/ql_crc16.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ ${QUEC_SDK_PATH}/ql_utils/src/ql_crc16.c
CDEPS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.d
OBJS += $(OUTPUT_DIR)/quectel/ql_utils/src/ql_crc16.o
#######################################################################
#                        QUECTEL SUPPORT SDK                          #
#######################################################################


$(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_main.o: $(COPIED_SDK_PATH)/platform/bootloader/core/btl_main.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/btl_main.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/btl_main.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_main.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_main.o

$(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_parse.o: $(COPIED_SDK_PATH)/platform/bootloader/core/btl_parse.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/btl_parse.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/btl_parse.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_parse.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_parse.o

$(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_reset.o: $(COPIED_SDK_PATH)/platform/bootloader/core/btl_reset.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/btl_reset.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/btl_reset.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_reset.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/btl_reset.o

$(OUTPUT_DIR)/sdk/platform/bootloader/core/flash/btl_internal_flash.o: $(COPIED_SDK_PATH)/platform/bootloader/core/flash/btl_internal_flash.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/core/flash/btl_internal_flash.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/core/flash/btl_internal_flash.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/flash/btl_internal_flash.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/core/flash/btl_internal_flash.o

$(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug.o: $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug.o

$(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug_swo.o: $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug_swo.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug_swo.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/debug/btl_debug_swo.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug_swo.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/debug/btl_debug_swo.o

$(OUTPUT_DIR)/sdk/platform/bootloader/driver/btl_driver_util.o: $(COPIED_SDK_PATH)/platform/bootloader/driver/btl_driver_util.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/driver/btl_driver_util.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/driver/btl_driver_util.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/driver/btl_driver_util.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/driver/btl_driver_util.o

$(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_custom_tags.o: $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_custom_tags.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_custom_tags.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_custom_tags.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_custom_tags.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_custom_tags.o

$(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_format.o: $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_format.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_format.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_format.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_format.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_format.o

$(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_parser.o: $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_parser.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_parser.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/parser/gbl/btl_gbl_parser.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_parser.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/parser/gbl/btl_gbl_parser.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc16.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc16.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc16.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc16.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc16.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc16.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc32.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc32.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc32.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_crc32.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc32.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_crc32.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_aes.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_aes.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_aes.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_aes.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_aes.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_aes.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_ecdsa.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_ecdsa.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_ecdsa.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_ecdsa.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_ecdsa.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_ecdsa.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_sha256.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_sha256.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_sha256.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_sha256.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_sha256.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_sha256.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_tokens.o: $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_tokens.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_tokens.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/btl_security_tokens.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_tokens.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/btl_security_tokens.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/ecc/ecc.o: $(COPIED_SDK_PATH)/platform/bootloader/security/ecc/ecc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/ecc/ecc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/ecc/ecc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/ecc/ecc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/ecc/ecc.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/btl_sha256.o: $(COPIED_SDK_PATH)/platform/bootloader/security/sha/btl_sha256.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/sha/btl_sha256.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/sha/btl_sha256.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/btl_sha256.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/btl_sha256.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/crypto_sha.o: $(COPIED_SDK_PATH)/platform/bootloader/security/sha/crypto_sha.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/sha/crypto_sha.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/sha/crypto_sha.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/crypto_sha.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/crypto_sha.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/cryptoacc_sha.o: $(COPIED_SDK_PATH)/platform/bootloader/security/sha/cryptoacc_sha.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/sha/cryptoacc_sha.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/sha/cryptoacc_sha.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/cryptoacc_sha.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/cryptoacc_sha.o

$(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/se_sha.o: $(COPIED_SDK_PATH)/platform/bootloader/security/sha/se_sha.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/bootloader/security/sha/se_sha.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/bootloader/security/sha/se_sha.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/se_sha.d
OBJS += $(OUTPUT_DIR)/sdk/platform/bootloader/security/sha/se_sha.o

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

$(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o: $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/dmadrv/src/dmadrv.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/dmadrv/src/dmadrv.o

$(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o: $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/gpiointerrupt/src/gpiointerrupt.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/gpiointerrupt/src/gpiointerrupt.o

$(OUTPUT_DIR)/sdk/platform/emdrv/uartdrv/src/uartdrv.o: $(COPIED_SDK_PATH)/platform/emdrv/uartdrv/src/uartdrv.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emdrv/uartdrv/src/uartdrv.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emdrv/uartdrv/src/uartdrv.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emdrv/uartdrv/src/uartdrv.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emdrv/uartdrv/src/uartdrv.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_acmp.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_acmp.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_acmp.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_acmp.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_acmp.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_acmp.o

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

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_dbg.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_dbg.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_dbg.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_dbg.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_dbg.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_dbg.o

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

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_i2c.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_i2c.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_i2c.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_i2c.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_i2c.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_i2c.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_iadc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_iadc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_iadc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_iadc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_iadc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_iadc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_lcd.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_lcd.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_lcd.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_lcd.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_lcd.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_lcd.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_ldma.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_ldma.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_lesense.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_lesense.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_lesense.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_lesense.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_lesense.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_lesense.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_letimer.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_letimer.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_letimer.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_letimer.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_letimer.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_letimer.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_msc.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_msc.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_opamp.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_opamp.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_opamp.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_opamp.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_opamp.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_opamp.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_pcnt.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_pcnt.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_pcnt.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_pcnt.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_pcnt.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_pcnt.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_prs.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_prs.o

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_rmu.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_rmu.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_rmu.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_rmu.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_rmu.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_rmu.o

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

$(OUTPUT_DIR)/sdk/platform/emlib/src/em_vdac.o: $(COPIED_SDK_PATH)/platform/emlib/src/em_vdac.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/emlib/src/em_vdac.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/emlib/src/em_vdac.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_vdac.d
OBJS += $(OUTPUT_DIR)/sdk/platform/emlib/src/em_vdac.o

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

$(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.o: $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/device_init/src/sl_device_init_emu_s2.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/device_init/src/sl_device_init_emu_s2.o

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

$(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o: $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.c
CDEPS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.d
OBJS += $(OUTPUT_DIR)/sdk/platform/service/interrupt_manager/src/sl_interrupt_manager_cortexm.o

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

$(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/lib/fih/src/fih.o: $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/lib/fih/src/fih.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/lib/fih/src/fih.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/lib/fih/src/fih.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/lib/fih/src/fih.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/lib/fih/src/fih.o

$(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.o: $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.c
	@$(POSIX_TOOL_PATH)echo 'Building $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ $(COPIED_SDK_PATH)/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.c
CDEPS += $(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.d
OBJS += $(OUTPUT_DIR)/sdk/util/third_party/trusted-firmware-m/platform/ext/target/siliconlabs/hse/sli_se.o

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

$(OUTPUT_DIR)/project/autogen/sl_uartdrv_init.o: autogen/sl_uartdrv_init.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_uartdrv_init.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_uartdrv_init.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_uartdrv_init.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_uartdrv_init.o

# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA