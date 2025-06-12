/*
 * ql_ota_upg_remove_firmware.c
 *
 *  Created on: 2025年5月28日
 *      Author: kahn.wei
 */

#include <ql_ota_upg_firmware.h>

#if defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__)

int32_t ql_ota_upg_firmware_init(ql_ota_upg_opeate_t *self, ql_internal_flash_t *internal_flash)
{
    if (self == NULL)
    {
        ql_printf("Error: self is NULL\r\n");
        return -1; // Return error if self is NULL
    }

    if (self->is_initialized)
    {
        ql_printf("Error: ql_ota_upg_firmware is already initialized\r\n");
        return 0; // Already initialized
    }

    self->internal_flash = internal_flash;

    if (self->internal_flash == NULL)
    {
        ql_printf("ql_ota_upg_firmware_init: internal_flash is not NULL\r\n");
        return -1; // Return error if internal_flash is NULL
    }

    self->is_flash_loaded = 1; // Set to true if flash is loaded

    if(ql_ota_upg_firmware_init_impl(self->internal_flash, self->buffer) != 0)
    {
        ql_printf("Error: ql_ota_upg_firmware_init_impl failed\r\n");
        return -1; // Return error if initialization implementation fails
    }

    self->is_initialized = 1;

    return 0; // Return success
}

int32_t ql_ota_upg_firmware_deinit(ql_ota_upg_opeate_t *self)
{
    if (self == NULL || !self->is_initialized)
    {
        return -1; // Return error if self is NULL or not initialized
    }

    int32_t ret = ql_ota_upg_firmware_deinit_impl(self->internal_flash);
    if (ret != 0)
    {
        return ret; // Return error from deinitialization implementation
    }

    self->is_initialized = 0;

    if (self->buffer != NULL)
    {
        sl_free(self->buffer);
        self->buffer = NULL;
    }

    return 0; // Return success
}

int32_t ql_ota_upg_firmware_integrity_check(ql_ota_upg_opeate_t *self, QL_FLASH_REGION_TYPE i_region)
{
    if(!self->is_flash_loaded)
    {
        return -1; // Return error if flash is not loaded
    }

    if (self == NULL || !self->is_initialized)
    {
        return -1; // Return error if self is NULL or not initialized
    }

    if(self->vptr->get_firmware_info == NULL)
    {
        return -1;
    }

    if(self->internal_flash == NULL)
    {
        return -1;
    }

    if((QL_FLASH_REGION_APPLICATION_BANK_1 != i_region)  && (QL_FLASH_REGION_APPLICATION_BANK_2 != i_region))
    {
        return -1;
    }

    ql_ota_firmware_info_t info = { 0 };

    if(self->vptr->get_firmware_info(self, &info, i_region) != 0)
    {
        return -1;
    }

    uint32_t start_addr = self->internal_flash->region[i_region].start_addr;
    uint32_t check_size = info.firmware_size;
    uint32_t segment_size = info.segment_length;

    return ql_ota_upg_firmware_integrity_check_impl(self->internal_flash, start_addr, check_size, i_region, segment_size, self->buffer);
}

int32_t ql_ota_upg_firmware_get_info(ql_ota_upg_opeate_t *self, ql_ota_firmware_info_t *info, QL_FLASH_REGION_TYPE i_region)
{
    if(!self->is_flash_loaded)
    {
        return -1; // Return error if flash is not loaded
    }

    if (self == NULL || !self->is_initialized || info == NULL)
    {
        ql_printf("Error: self is NULL, not initialized, or info is NULL\r\n");
        return -1; // Return error if self is NULL, not initialized, or info is NULL
    }

    return ql_ota_upg_firmware_get_info_impl(self->internal_flash, info, i_region);
}

int32_t ql_ota_upg_firmware_check_completed(ql_ota_upg_opeate_t *self)
{
    if(!self->is_flash_loaded)
    {
        return -1; // Return error if flash is not loaded
    }

    if (self == NULL || !self->is_initialized)
    {
        return -1; // Return error if self is NULL or not initialized
    }

    if(self->is_firmware_checked)
    {
        return 0; // Firmware already checked
    }

    ql_ota_firmware_info_t info = { 0 };

    if(self->vptr->get_firmware_info(self, &info, QL_FLASH_REGION_APPLICATION_BANK_2) != 0)
    {
        return -1;
    }

    uint32_t addr = self->internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr;
    uint32_t size = info.firmware_size;
    uint32_t segment_size = info.segment_length;

    if(ql_ota_upg_firmware_check_impl(self->internal_flash, addr, size, segment_size, self->buffer) !=0)
    {
        return -1; // Return error if firmware check fails
    }

    self->is_firmware_checked = 1; // Set firmware checked flag to true

    return 0; // Return success
}

int32_t ql_ota_upg_firmware_erase(ql_ota_upg_opeate_t *self)
{
    if(!self->is_flash_loaded)
    {
        return -1; // Return error if flash is not loaded
    }

    if (self == NULL || !self->is_initialized)
    {
        return -1; // Return error if self is NULL or not initialized
    }

    if(!self->is_firmware_checked)
    {
        return -1; // Return error if firmware is already checked
    }

    if(self->is_erased)
    {
        ql_printf("Firmware already erased\r\n");
        return 0; // Firmware already erased
    }

    ql_ota_firmware_info_t info = { 0 };
    if(self->vptr->get_firmware_info(self, &info, QL_FLASH_REGION_APPLICATION_BANK_1) != 0)
    {
        return -1;
    }

    uint32_t addr = self->internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr;
    uint32_t size = info.firmware_size;

    if(ql_ota_upg_firmware_erase_origin_impl(self->internal_flash, addr, size) != 0)
    {
        return -1; // Return error if firmware erase fails
    }

    self->is_erased = 1; // Set firmware erased flag to true

    return 0;
}

int32_t ql_ota_upg_firmware_move_firmware(ql_ota_upg_opeate_t *self)
{
    if(!self->is_flash_loaded)
    {
        return -1; // Return error if flash is not loaded
    }

    if (self == NULL || !self->is_initialized)
    {
        return -1; // Return error if self is NULL or not initialized
    }

    if(!self->is_firmware_checked)
    {
        return -1; // Return error if firmware is not checked
    }

    if(!self->is_erased)
    {
        return -1; // Return error if firmware is not erased
    }

    ql_ota_firmware_info_t info = { 0 };
    if(self->vptr->get_firmware_info(self, &info, QL_FLASH_REGION_APPLICATION_BANK_2) != 0)
    {
        return -1;
    }

    uint32_t dst_addr = self->internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_1].start_addr;
    uint32_t src_addr = self->internal_flash->region[QL_FLASH_REGION_APPLICATION_BANK_2].start_addr;
    uint32_t size = info.firmware_size;
    uint32_t segment_size = info.segment_length;

    // Implement the logic to move firmware if needed
    // Currently, this function is not implemented in the vtable
    return ql_ota_upg_firmware_move_impl(self->internal_flash, src_addr, dst_addr, size, segment_size, self->buffer); // Return error as this operation is not supported
}

const ql_ota_upg_opeate_vtable_t ql_ota_upg_firmware_vtable =
{
    .init = ql_ota_upg_firmware_init,
    .integrity_check = ql_ota_upg_firmware_integrity_check,
    .check_firmware = ql_ota_upg_firmware_check_completed,
    .erase_firmware = ql_ota_upg_firmware_erase,
    .move_firmware = ql_ota_upg_firmware_move_firmware,
    .get_firmware_info = ql_ota_upg_firmware_get_info,
    .deinit = ql_ota_upg_firmware_deinit
};

ql_ota_upg_opeate_t *ql_ota_upg_firmware_create(void)
{
    ql_ota_upg_opeate_t *self = (ql_ota_upg_opeate_t *)sl_malloc(sizeof(ql_ota_upg_opeate_t));
    if (self == NULL)
    {
        return NULL;
    }

    self->vptr = &ql_ota_upg_firmware_vtable;
    self->is_initialized = 0;
    self->is_firmware_checked = 0;
    self->is_erased = 0;
    self->is_flash_loaded = 0;
    self->internal_flash = NULL;

    self->buffer = (MemoryPool *)sl_malloc(sizeof(MemoryPool));
    if (self->buffer == NULL)
    {
        sl_free(self);
        return NULL;
    }

    self->buffer_size = MEMORY_POOL_SIZE;
    return self;
}

#endif /* defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__) || defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__) */
