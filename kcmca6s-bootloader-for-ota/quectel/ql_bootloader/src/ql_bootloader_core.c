/*
 * ql_bootloader_core.c
 *
 *  Created on: 2025年6月4日
 *      Author: kahn.wei
 */

#include "ql_bootloader_core.h"

static int32_t ql_bootloader_init(ql_bootloader_t *self);

static int32_t ql_bootloader_recognize_mode(ql_bootloader_t *self);

static int32_t ql_bootloader_sequence(ql_bootloader_t *self);

static int32_t ql_bootloader_deinit(ql_bootloader_t *self);

const ql_bootloader_vptr_t ql_bootloader_vtable = {

    .init = ql_bootloader_init,
    .recognize_mode = ql_bootloader_recognize_mode,
    .enter_sequence = ql_bootloader_sequence,
    .deinit = ql_bootloader_deinit,
};

ql_bootloader_t *ql_bootloader_create(void)
{
    ql_bootloader_t *bootloader = (ql_bootloader_t *)sl_malloc(sizeof(ql_bootloader_t));
    if(!bootloader)
    {
        ql_printf("[BOOT]BOOTLOADER CREATE FAIL\r\n");
        return NULL;
    }
    bootloader->vptr = &ql_bootloader_vtable;
    bootloader->dev = (ql_bootloader_dev_t *)sl_malloc(sizeof(ql_bootloader_dev_t));
    if(!bootloader->dev)
    {
        ql_printf("[BOOT]BOOTLOADER DEVICE CREATE FAIL\r\n");
        sl_free(bootloader);
        bootloader = NULL;
        return NULL;
    }
    bootloader->is_enter_mode = QL_BOOTLOADER_MODE_UNKNOW;
    return bootloader;
}

//    if (!bootlader || bootlader->vptr->init(bootlader) != 0)

static int32_t ql_bootloader_init(ql_bootloader_t *self)
{
    if(!self->dev)
    {
        ql_printf("[BOOT]BOOTLOADER DEV INIT FAIL\r\n");
        return -1;
    }

    self->dev->uart = ql_uart_create(QL_UART_UART1_PORT);
    if (!self->dev->uart || self->dev->uart->vptr->open(self->dev->uart) != 0)
    {
        ql_printf("[BOOT]USART DEVICE INITIALIZED FAIL\n");
        return -1;
    }

    self->dev->internal_flash = ql_internal_flash_create();
    if(!self->dev->internal_flash || self->dev->internal_flash->vptr->init(self->dev->internal_flash) != 0)
    {
        ql_printf("[BOOT]INTERNAL FLASH DEVICE INITIALIZED FAIL\n");
        return -1;
    }

    self->dev->ota_upg_firmware = ql_ota_upg_firmware_create();
    if(!self->dev->ota_upg_firmware || self->dev->ota_upg_firmware->vptr->init(self->dev->ota_upg_firmware, self->dev->internal_flash) != 0)
    {
        ql_printf("[BOOT]OTA UPG FIRMWARE INSTANCE INITIALIZED FAIL\n");
        return -1;
    }

    return 0;
}

static int32_t ql_bootloader_recognize_mode(ql_bootloader_t *self)
{
    self->i_reset_reason = ql_bootloader_reset_get_reset_reason();
    
    switch (self->i_reset_reason)
    {
        case BOOTLOADER_RESET_REASON_BOOTLOAD:
        {
            self->is_enter_mode = QL_BOOTLOADER_MODE_NORMAL;
        }
        break;
        case BOOTLOADER_RESET_REASON_BADAPP:
        {
            self->is_enter_mode = QL_BOOTLOADER_MODE_UPG_OTA;
        }
        break;
        case BOOTLOADER_RESET_REASON_UPG_SEQUENCE:
        {
            self->is_enter_mode = QL_BOOTLOADER_MODE_UPG_WIRED;
        }
        break;
        case BOOTLOADER_RESET_REASON_OTA_SEQUENCE:
        {
            self->is_enter_mode = QL_BOOTLOADER_MODE_UPG_OTA;
        }
        break;
        case BOOTLOADER_RESET_REASON_OTA_IMAGE_FAIL:
        {

        }
        break;
    
    default:
        break;
    }

    return 0;
}

static int32_t ql_bootloader_sequence(ql_bootloader_t *self)
{
    self->rst_reason = BOOTLOADER_RESET_REASON_UNKNOWN;
    switch (self->is_enter_mode)
    {
        case QL_BOOTLOADER_MODE_NORMAL: // READ/WRITE FLASH OPERATE
        {

        }
        break;
        case QL_BOOTLOADER_MODE_UPG_OTA:
        {
            ql_printf("[BOOT]QUECTEL BOOTLOADER OTA UPGRADE MODE\r\n");

            if(!self->dev)
            {
                ql_printf("[BOOT]DEV NOT INIT\r\n");
            }

            if(!self->dev->uart)
            {
                ql_printf("[BOOT]DEV UART NOT INIT\r\n");
            }

            if(!self->dev->internal_flash)
            {
                ql_printf("[BOOT]DEV INTERNAL_FLASH NOT INIT\r\n");
            }

            if(!self->dev->ota_upg_firmware)
            {
                ql_printf("[BOOT]DEV OTA_UPG_FIRMWARE NOT INIT\r\n");
            }

            if(self->dev->uart && self->dev->internal_flash && self->dev->ota_upg_firmware)
            {
                if(self->dev->ota_upg_firmware->vptr->integrity_check(self->dev->ota_upg_firmware, QL_FLASH_REGION_APPLICATION_BANK_2) != 0)
                {
                    ql_printf("[BOOT]OTA UPG FIRMWARE INTEGERITY FAIL\n");
                    self->rst_reason = BOOTLOADER_RESET_REASON_OTA_IMAGE_FAIL;
                    return -1;
                }

                if(self->dev->ota_upg_firmware->vptr->check_firmware(self->dev->ota_upg_firmware) != 0)
                {
                    ql_printf("[BOOT]OTA UPG FIRMWARE CHECK FAIL\n");
                    self->rst_reason = BOOTLOADER_RESET_REASON_OTA_IMAGE_FAIL;
                    return -1;
                }

                if(self->dev->ota_upg_firmware->vptr->erase_firmware(self->dev->ota_upg_firmware)!= 0)
                {
                    ql_printf("[BOOT]OTA UPG FIRMWARE ERASE FAIL\n");
                    self->rst_reason = BOOTLOADER_RESET_REASON_UNKNOWN;
                    return -1;
                }

                if(self->dev->ota_upg_firmware->vptr->move_firmware(self->dev->ota_upg_firmware) != 0)
                {
                    ql_printf("[BOOT]OTA UPG FIRMWARE MOVE FAIL\n");
                    self->rst_reason = BOOTLOADER_RESET_REASON_UNKNOWN;
                    return -1;
                }

                if(self->dev->ota_upg_firmware->vptr->integrity_check(self->dev->ota_upg_firmware, QL_FLASH_REGION_APPLICATION_BANK_1) != 0)
                {
                    ql_printf("[BOOT]OTA UPG FIRMWARE INTEGERITY APPLICATION 1 CHECK FAIL\n");
                    self->rst_reason = BOOTLOADER_RESET_REASON_UNKNOWN;
                    return -1;
                }
            }
            else
            {
                ql_printf("[BOOT]QUECTEL BOOTLOADER OTA UPGRADE MODE UNKNOWN ERROR\r\n");
                self->rst_reason = BOOTLOADER_RESET_REASON_UNKNOWN;
                return -1;
            }

            self->rst_reason = BOOTLOADER_RESET_REASON_OTA_SUCCESS;
        }
        break;
        case QL_BOOTLOADER_MODE_UPG_WIRED: // UPGRADE IN BOOTLOADER BY UART/SPI/IIC
        {

        }
        break;

    default:
        break;
    }
    return 0;
}

static int32_t ql_bootloader_deinit(ql_bootloader_t *self)
{
    if(self->dev)
    {
        if(self->dev->uart)
        {
            sl_free(self->dev->uart);
            self->dev->uart = NULL;
        }

        if(self->dev->internal_flash)
        {
            sl_free(self->dev->internal_flash);
            self->dev->internal_flash = NULL;
        }

        if(self->dev->ota_upg_firmware)
        {
            if(self->dev->ota_upg_firmware->buffer)
            {
                sl_free(self->dev->ota_upg_firmware->buffer);
                self->dev->ota_upg_firmware->buffer = NULL;
            }

            if(self->dev->ota_upg_firmware)
            {
                sl_free(self->dev->ota_upg_firmware);
                self->dev->ota_upg_firmware = NULL;
            }
        }

        sl_free(self->dev);
        self->dev = NULL;
    }
    return 0;
}
