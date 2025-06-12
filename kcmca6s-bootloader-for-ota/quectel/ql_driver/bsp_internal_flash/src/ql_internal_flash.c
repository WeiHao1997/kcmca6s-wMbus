/*
 * ql_internal_flash.c
 *
 *  Created on: 2025年5月27日
 *      Author: kahn.wei
 */


#include "ql_internal_flash.h"

ql_internal_flash_region_t flash_region[] =
{
#if defined(__QL_SUPPORT_INTERNAM_FLASH_256K_SINGAL_BANK__)
    QL_FLASH_ADD_REGION(QL_FLASH_BOOTLOADER_BASE, QL_FLASH_BOOTLOADER_SIZE, READ_ONLY, QL_FLASH_REGION_BOOTLOADER),
    QL_FLASH_ADD_REGION(QL_FLASH_APPLICATION_BASE, QL_FLASH_APPLICATION_SIZE, READ_WRITE, QL_FLASH_REGION_APPLICATION),
    QL_FLASH_ADD_REGION(QL_FLASH_CONFIG_INFO_BASE, QL_FLASH_CONFIG_INFO_SZIE, READ_WRITE, QL_FLASH_REGION_CONFIG_INFO),

#elif defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__)
    QL_FLASH_ADD_REGION(QL_FLASH_BOOTLOADER_BASE, QL_FLASH_BOOTLOADER_SIZE, READ_ONLY, QL_FLASH_REGION_BOOTLOADER),
    QL_FLASH_ADD_REGION(QL_FLASH_APPLICATION_BANK_1_BASE, QL_FLASH_APPLICATION_BANK_1_SIZE, READ_WRITE, QL_FLASH_REGION_APPLICATION_BANK_1),
    QL_FLASH_ADD_REGION(QL_FLASH_APPLICATION_BANK_2_BASE, QL_FLASH_APPLICATION_BANK_2_SIZE, READ_WRITE, QL_FLASH_REGION_APPLICATION_BANK_2),
    QL_FLASH_ADD_REGION(QL_FLASH_CONFIG_INFO_BANK_1_BASE, QL_FLASH_CONFIG_INFO_BANK_1_SIZE, READ_WRITE, QL_FLASH_REGION_CONFIG_INFO_BANK_1),
    QL_FLASH_ADD_REGION(QL_FLASH_CONFIG_INFO_BANK_2_BASE, QL_FLASH_CONFIG_INFO_BANK_2_SIZE, READ_WRITE, QL_FLASH_REGION_CONFIG_INFO_BANK_2),

#elif defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__)
    QL_FLASH_ADD_REGION(QL_FLASH_BOOTLOADER_BASE, QL_FLASH_BOOTLOADER_SIZE, READ_ONLY, QL_FLASH_REGION_BOOTLOADER),
    QL_FLASH_ADD_REGION(QL_FLASH_APPLICATION_BANK_1_BASE, QL_FLASH_APPLICATION_BANK_1_SIZE, READ_WRITE, QL_FLASH_REGION_APPLICATION_BANK_1),
    QL_FLASH_ADD_REGION(QL_FLASH_APPLICATION_BANK_2_BASE, QL_FLASH_APPLICATION_BANK_2_SIZE, READ_WRITE, QL_FLASH_REGION_APPLICATION_BANK_2),
    QL_FLASH_ADD_REGION(QL_FLASH_CONFIG_INFO_BANK_1_BASE, QL_FLASH_CONFIG_INFO_BANK_1_SIZE, READ_WRITE, QL_FLASH_REGION_CONFIG_INFO_BANK_1),
    QL_FLASH_ADD_REGION(QL_FLASH_CONFIG_INFO_BANK_2_BASE, QL_FLASH_CONFIG_INFO_BANK_2_SIZE, READ_WRITE, QL_FLASH_REGION_CONFIG_INFO_BANK_2),
    
#endif
};

#define QL_INTERNAL_FLASH_REGION_SIZE (sizeof(flash_region) / sizeof(ql_internal_flash_region_t))

#define QL_INTERNAL_FLASH_SIZE FLASH_SIZE

static int32_t ql_internal_flash_init_impl(ql_internal_flash_t *self)
{
    self->is_initialized = 1;
    ql_internal_flash_init(); 
    return 0;
}

static int32_t ql_internal_flash_read_impl(ql_internal_flash_t *self, uint32_t addr, uint8_t *buf, uint32_t len)
{
    if (self == NULL || buf == NULL || len == 0) 
    {
        return -1;
    }

    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }
    
    return ql_internal_flash_read(addr, buf, len);
}

static int32_t ql_internal_flash_write_impl(ql_internal_flash_t *self, uint32_t addr, const uint8_t *buf, uint32_t len)
{
    if (self == NULL || buf == NULL || len == 0) 
    {
        return -1;
    }
        
    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }

    if(self->vptr->check_region(self, addr, len) != 0)
    {
        return -1;
    }
    
    return ql_internal_flash_wirte(addr, (void *)buf, len);
}

static int32_t ql_internal_flash_erase_impl(ql_internal_flash_t *self, uint32_t addr, uint32_t len)
{
    if (self == NULL || len == 0) 
    {
        return -1;
    }
    
    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }

    if(self->vptr->check_region(self, addr, len) != 0)
    {
        return -1;
    }

    return ql_internal_flash_erase(addr, len);
}

static int32_t ql_internal_flash_get_size_impl(ql_internal_flash_t *self, uint32_t *size)
{
    if (self == NULL || size == NULL) 
    {
        return -1;
    }
    
    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }

    *size = self->size;
    return 0;
}

static int32_t ql_internal_flash_get_sector_size_impl(ql_internal_flash_t *self, uint32_t *sector_size)
{
    if (self == NULL || sector_size == NULL)
    {
        return -1;
    }
    
    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }

    *sector_size = self->sector_size;
    return 0;
}

static int32_t ql_internal_flash_check_region(ql_internal_flash_t *self, uint32_t addr, uint32_t len)
{
    if (self == NULL || self->region == NULL)
    {
        return -1;
    }
    
    if(self->is_initialized == 0)
    {
        return -1; // Not initialized
    }

    for (size_t i = 0; i < QL_INTERNAL_FLASH_REGION_SIZE; i++)
    {
        if ((self->region[i].is_operate == READ_WRITE) &&
            (addr >= self->region[i].start_addr) &&
            (addr + len) <= (self->region[i].start_addr + self->region[i].is_size))
        {
            return 0;
        }
    }

    return -1;
}

static ql_internal_flash_vptr_t ql_internal_flash_vtable =
{
    .init = ql_internal_flash_init_impl,
    .read = ql_internal_flash_read_impl,
    .write = ql_internal_flash_write_impl,
    .erase = ql_internal_flash_erase_impl,
    .get_size = ql_internal_flash_get_size_impl,
    .get_sector_size = ql_internal_flash_get_sector_size_impl,
    .check_region = ql_internal_flash_check_region
};

ql_internal_flash_t *ql_internal_flash_create(void)
{
    ql_internal_flash_t *flash = (ql_internal_flash_t *)sl_malloc(sizeof(ql_internal_flash_t));
    if (flash == NULL) 
    {
        return NULL;
    }
    
    flash->vptr = &ql_internal_flash_vtable;
    flash->size = QL_INTERNAL_FLASH_SIZE;
    flash->sector_size = QL_FLASH_PAGE_SZIE;
    flash->is_initialized = 0;
    flash->region = flash_region;
    
    return flash;
}
