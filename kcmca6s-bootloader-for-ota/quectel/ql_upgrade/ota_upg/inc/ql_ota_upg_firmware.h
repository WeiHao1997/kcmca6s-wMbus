/*
 * ql_ota_upg_remove_firmware.h
 *
 *  Created on: 2025年5月28日
 *      Author: kahn.wei
 */

#ifndef __QL_OTA_UPG_FIRMWARE_H__
#define __QL_OTA_UPG_FIRMWARE_H__

#include "ql_include.h"
#include "ql_internal_flash.h"
#include "ql_ota_upg_firmware_impl.h"

#if defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__)

typedef struct ql_ota_upg_opeate ql_ota_upg_opeate_t;

typedef struct
{
    int32_t (*init)(ql_ota_upg_opeate_t* self, ql_internal_flash_t *internal_flash);
    int32_t (*integrity_check)(ql_ota_upg_opeate_t* self, QL_FLASH_REGION_TYPE i_region);
    int32_t (*move_firmware)(ql_ota_upg_opeate_t* self);
    int32_t (*check_firmware)(ql_ota_upg_opeate_t* self);
    int32_t (*erase_firmware)(ql_ota_upg_opeate_t* self);
    int32_t (*get_firmware_info)(ql_ota_upg_opeate_t* self, ql_ota_firmware_info_t *info, QL_FLASH_REGION_TYPE i_region);
    int32_t (*deinit)(ql_ota_upg_opeate_t* self);
}ql_ota_upg_opeate_vtable_t;

struct ql_ota_upg_opeate
{
    const ql_ota_upg_opeate_vtable_t *vptr;
    ql_internal_flash_t *internal_flash;
    uint8_t is_initialized;
    uint8_t is_flash_loaded;
    uint8_t is_firmware_checked;
    uint8_t is_erased;
    uint32_t buffer_size;
    MemoryPool *buffer;
};

ql_ota_upg_opeate_t *ql_ota_upg_firmware_create(void);

#endif /* defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__) */

#endif /* __QL_OTA_UPG_FIRMWARE_H__ */
