/*
 * ql_bootloader_core.h
 *
 *  Created on: 2025年6月4日
 *      Author: kahn.wei
 */

#ifndef __QL_BOOTLOADER_CORE_H__
#define __QL_BOOTLOADER_CORE_H__

#include "ql_include.h"
#include "ql_internal_flash.h"
#include "ql_crc.h"
#include "ql_ota_upg_firmware.h"
#include "ql_bootloader_rst.h"

typedef struct ql_bootloader ql_bootloader_t;

typedef struct
{
    ql_uart_t *uart;
    ql_internal_flash_t *internal_flash;
    ql_ota_upg_opeate_t *ota_upg_firmware;
}ql_bootloader_dev_t;

typedef struct
{
    int32_t (*init)(ql_bootloader_t *self);
    int32_t (*recognize_mode)(ql_bootloader_t *self);
    int32_t (*enter_sequence)(ql_bootloader_t *self);
    int32_t (*deinit)(ql_bootloader_t *self);
}ql_bootloader_vptr_t;

struct ql_bootloader
{
    const ql_bootloader_vptr_t *vptr;
    ql_bootloader_dev_t *dev;
    uint16_t i_reset_reason;
    uint8_t is_enter_mode;
    uint16_t rst_reason;
};

typedef enum
{
  QL_BOOTLOADER_MODE_UNKNOW,
  QL_BOOTLOADER_MODE_NORMAL,
  QL_BOOTLOADER_MODE_UPG_OTA,
  QL_BOOTLOADER_MODE_UPG_WIRED
}QL_BOOTLOADER_ENTERN_MODE_T;

ql_bootloader_t *ql_bootloader_create(void);

#endif /* __QL_BOOTLOADER_CORE_H__ */
