####################################################################
# Automatically-generated file. Do not edit!                       #
# Makefile Version 16                                              #
####################################################################

BASE_SDK_PATH = D:/SiliconLabs/SimplicityStudio/v5/offline/simplicity_sdk
BASE_PKG_PATH = C:/Users/kahn.wei.QUECTEL/.silabs/slt/installs
UNAME:=$(shell $(POSIX_TOOL_PATH)uname -s | $(POSIX_TOOL_PATH)sed -e 's/^\(CYGWIN\).*/\1/' | $(POSIX_TOOL_PATH)sed -e 's/^\(MINGW\).*/\1/')
ifeq ($(UNAME),MINGW)
# Translate "C:/super" into "/C/super" for MinGW make.
SDK_PATH := /$(shell $(POSIX_TOOL_PATH)echo $(BASE_SDK_PATH) | sed s/://)
PKG_PATH := /$(shell $(POSIX_TOOL_PATH)echo $(BASE_PKG_PATH) | sed s/://)
endif
SDK_PATH ?= $(BASE_SDK_PATH)
PKG_PATH ?= $(BASE_PKG_PATH)
COPIED_SDK_PATH ?= simplicity_sdk_2024.6.3

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
 '-DEFR32FG23A010F256GM40=1' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_CORE=core'

ASM_DEFS += \
 '-DDEBUG_EFM=1' \
 '-DEFR32FG23A010F256GM40=1' \
 '-DSL_COMPONENT_CATALOG_PRESENT=1' \
 '-DCMSIS_NVIC_VIRTUAL=1' \
 '-DCMSIS_NVIC_VIRTUAL_HEADER_FILE="cmsis_nvic_virtual.h"' \
 '-DSL_MEMORY_POOL_LIGHT=1' \
 '-DSL_CODE_COMPONENT_CORE=core'

INCLUDES += \
 -Iconfig \
 -Iautogen \
 -I. \
 -I$(COPIED_SDK_PATH)/platform/Device/SiliconLabs/EFR32FG23/Include \
 -I$(COPIED_SDK_PATH)/platform/common/inc \
 -I$(COPIED_SDK_PATH)/platform/CMSIS/Core/Include \
 -I$(COPIED_SDK_PATH)/platform/emlib/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/interrupt_manager/inc/arm \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/inc \
 -I$(COPIED_SDK_PATH)/platform/service/memory_manager/src \
 -I$(COPIED_SDK_PATH)/platform/common/toolchain/inc \
 -I$(COPIED_SDK_PATH)/platform/service/system/inc

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

$(OUTPUT_DIR)/project/app.o: app.c
	@$(POSIX_TOOL_PATH)echo 'Building app.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ app.c
CDEPS += $(OUTPUT_DIR)/project/app.d
OBJS += $(OUTPUT_DIR)/project/app.o

$(OUTPUT_DIR)/project/autogen/sl_event_handler.o: autogen/sl_event_handler.c
	@$(POSIX_TOOL_PATH)echo 'Building autogen/sl_event_handler.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ autogen/sl_event_handler.c
CDEPS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.d
OBJS += $(OUTPUT_DIR)/project/autogen/sl_event_handler.o

$(OUTPUT_DIR)/project/main.o: main.c
	@$(POSIX_TOOL_PATH)echo 'Building main.c'
	@$(POSIX_TOOL_PATH)mkdir -p $(@D)
	$(ECHO)$(CC) $(CFLAGS) -c -o $@ main.c
CDEPS += $(OUTPUT_DIR)/project/main.d
OBJS += $(OUTPUT_DIR)/project/main.o

# Automatically-generated Simplicity Studio Metadata
# Please do not edit or delete these lines!
# SIMPLICITY_STUDIO_METADATA=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END_SIMPLICITY_STUDIO_METADATA
# END OF METADATA