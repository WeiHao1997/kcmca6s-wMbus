/*
 * ql_include.h
 *
 *  Created on: 2024年10月28日
 *      Author: Administrator
 */

#ifndef __QL_INCLUDE_H__
#define __QL_INCLUDE_H__

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <malloc.h>

#include "ql_bootloader.h"
#include "ql_crc16.h"
#include "ql_uart.h"
#include "ql_internal_flash.h"
#include "ql_frame_build.h"
#include "ql_upgrade.h"
#include "ql_time.h"

#include "em_usart.h"
#include "em_ldma.h"
#include "em_gpio.h"
#include "em_cmu.h"
#include "em_timer.h"
#include "em_msc.h"

#include "core/btl_reset.h"
#include "api/btl_reset_info.h"
#include "core/flash/btl_internal_flash.h"
#include "sl_uartdrv_instances.h"

#include "sl_memory_manager_region.h"
#include "sl_memory_manager.h"

#endif /* __QL_INCLUDE_H__ */
