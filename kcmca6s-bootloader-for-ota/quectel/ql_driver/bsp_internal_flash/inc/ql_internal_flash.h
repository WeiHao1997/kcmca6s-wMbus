/*
 * ql_internal_flash.h
 *
 *  Created on: 2025年5月27日
 *      Author: kahn.wei
 */


#ifndef __QL_INTERNAL_FLASH_H__
#define __QL_INTERNAL_FLASH_H__

#include "ql_include.h"
#include "ql_internal_flash_impl.h"
#include "sl_memory_manager.h"

typedef struct ql_internal_flash ql_internal_flash_t;

typedef struct 
{
    int32_t (*init)(ql_internal_flash_t *self);
    int32_t (*read)(ql_internal_flash_t *self, uint32_t addr, uint8_t *buf, uint32_t len);
    int32_t (*write)(ql_internal_flash_t *self, uint32_t addr, const uint8_t *buf, uint32_t len);
    int32_t (*erase)(ql_internal_flash_t *self, uint32_t addr, uint32_t len);
    int32_t (*get_size)(ql_internal_flash_t *self, uint32_t *size);
    int32_t (*get_sector_size)(ql_internal_flash_t *self, uint32_t *sector_size);
    int32_t (*check_region)(ql_internal_flash_t *self, uint32_t addr, uint32_t len);
}ql_internal_flash_vptr_t;

typedef struct
{
    uint32_t start_addr;
    uint32_t is_size;
    uint8_t is_operate;
    uint8_t is_type;
}ql_internal_flash_region_t;

struct ql_internal_flash
{
    const ql_internal_flash_vptr_t *vptr;
    uint32_t size;
    uint32_t sector_size;
    uint8_t is_initialized;
    ql_internal_flash_region_t *region;
};

#define QL_FLASH_ADD_REGION(addr, size, opt, type) \
    { .start_addr = (addr), .is_size = (size), .is_operate = (opt), .is_type = (type) }

#define READ_ONLY   0
#define READ_WRITE  1

ql_internal_flash_t *ql_internal_flash_create(void);

#endif /* __QL_INTERNAL_FLASH_H__ */
