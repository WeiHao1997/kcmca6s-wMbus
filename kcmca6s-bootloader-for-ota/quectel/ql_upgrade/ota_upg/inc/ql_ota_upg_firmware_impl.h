/*
 * ql_ota_upg_firmware_impl.h
 *
 *  Created on: 2025年5月28日
 *      Author: kahn.wei
 */

#ifndef __QL_OTA_UPG_FIRMWARE_IMPL_H__
#define __QL_OTA_UPG_FIRMWARE_IMPL_H__

#include "ql_include.h"
#include "ql_internal_flash.h"
#include "ql_crc.h"
#include "ql_utils.h"
#include "ql_uart.h"


typedef struct
{
    uint32_t magic_word;
    uint32_t firmware_size;
    uint32_t firmware_crc32;
    uint32_t segment_length;
} ql_ota_firmware_info_t;

#define QL_OTA_FIRMWARE_MAGIC_WORD 0x55AA55AA

int32_t ql_ota_upg_firmware_init_impl(ql_internal_flash_t *internal_flash, MemoryPool *memory_pool);

int32_t ql_ota_upg_firmware_check_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len, uint32_t segment_size, MemoryPool *memory_pool);

int32_t ql_ota_upg_firmware_move_impl(ql_internal_flash_t *internal_flash, uint32_t src_addr, uint32_t dst_addr, uint32_t len, uint32_t segment_size, MemoryPool *memory_pool);

int32_t ql_ota_upg_firmware_get_info_impl(ql_internal_flash_t *internal_flash, ql_ota_firmware_info_t *info, QL_FLASH_REGION_TYPE i_region);

int32_t ql_ota_upg_firmware_erase_origin_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len);

int32_t ql_ota_upg_firmware_deinit_impl(ql_internal_flash_t *internal_flash);

int32_t ql_ota_upg_firmware_integrity_check_impl(ql_internal_flash_t *internal_flash, uint32_t addr, uint32_t len, QL_FLASH_REGION_TYPE i_region, uint32_t segment_size, MemoryPool *memory_pool);

#endif /* __QL_OTA_UPG_FIRMWARE_IMPL_H__ */
