#ifndef __QL_INTERNAL_FLASH_IMPL_H__
#define __QL_INTERNAL_FLASH_IMPL_H__

#include "ql_include.h"

#define QL_FLASH_PAGE_SZIE        0x00002000

#if defined(__QL_SUPPORT_INTERNAM_FLASH_256K_SINGAL_BANK__)
#include "efr32fg23a010f256gm40.h"
#define QL_FLASH_BOOTLOADER_BASE            (0x08000000)
#define QL_FLASH_BOOTLOADER_SIZE            (0x00008000)
#define QL_FLASH_APPLICATION_BASE           (QL_FLASH_BOOTLOADER_BASE + QL_FLASH_BOOTLOADER_SIZE)
#define QL_FLASH_APPLICATION_SIZE           (0x00036000)
#define QL_FLASH_CONFIG_INFO_BASE           (0x0803E000)
#define QL_FLASH_CONFIG_INFO_SZIE           (0x00002000)

typedef enum
{
    QL_FLASH_REGION_BOOTLOADER = 0,
    QL_FLASH_REGION_APPLICATION,
    QL_FLASH_REGION_CONFIG_INFO,
    QL_FLASH_REGION_MAX
}QL_FLASH_REGION_TYPE;

#elif defined(__QL_SUPPORT_INTERNAM_FLASH_256K_DUAL_BANK__)
#include "efr32fg23a010f256gm40.h"
#define QL_FLASH_BOOTLOADER_BASE            (0x08000000)
#define QL_FLASH_BOOTLOADER_SIZE            (0x00008000)
#define QL_FLASH_APPLICATION_BANK_1_BASE    (QL_FLASH_BOOTLOADER_BASE + QL_FLASH_BOOTLOADER_SIZE)
#define QL_FLASH_APPLICATION_BANK_1_SIZE    (0x00020000)
#define QL_FLASH_APPLICATION_BANK_2_BASE    (QL_FLASH_APPLICATION_BANK_1_BASE + QL_FLASH_APPLICATION_BANK_1_SIZE)
#define QL_FLASH_APPLICATION_BANK_2_SIZE    (0x0000E000)
#define QL_FLASH_CONFIG_INFO_BANK_1_BASE    (0x0803C000)
#define QL_FLASH_CONFIG_INFO_BANK_1_SIZE    (0x00002000)
#define QL_FLASH_CONFIG_INFO_BANK_2_BASE    (0x0803E000)
#define QL_FLASH_CONFIG_INFO_BANK_2_SIZE    (0x00002000)

typedef enum
{
    QL_FLASH_REGION_BOOTLOADER = 0,
    QL_FLASH_REGION_APPLICATION_BANK_1,
    QL_FLASH_REGION_APPLICATION_BANK_2,
    QL_FLASH_REGION_CONFIG_INFO_BANK_1,
    QL_FLASH_REGION_CONFIG_INFO_BANK_2,
    QL_FLASH_REGION_MAX
}QL_FLASH_REGION_TYPE;

#elif defined(__QL_SUPPORT_INTERNAM_FLASH_512K_DUAL_BANK__)
#include "efr32fg23b010f512im48.h"
#define QL_FLASH_BOOTLOADER_BASE            (0x08000000)
#define QL_FLASH_BOOTLOADER_SIZE            (0x00008000)
#define QL_FLASH_APPLICATION_BANK_1_BASE    (QL_FLASH_BOOTLOADER_BASE + QL_FLASH_BOOTLOADER_SIZE)  // 0x08008000
#define QL_FLASH_APPLICATION_BANK_1_SIZE    (0x00038000)
#define QL_FLASH_APPLICATION_BANK_2_BASE    (QL_FLASH_APPLICATION_BANK_1_BASE + QL_FLASH_APPLICATION_BANK_1_SIZE) //0x08040000
#define QL_FLASH_APPLICATION_BANK_2_SIZE    (0x00036000)
#define QL_FLASH_CONFIG_INFO_BANK_1_BASE    (0x0807C000)
#define QL_FLASH_CONFIG_INFO_BANK_1_SIZE    (0x00002000)
#define QL_FLASH_CONFIG_INFO_BANK_2_BASE    (0x0807E000)
#define QL_FLASH_CONFIG_INFO_BANK_2_SIZE    (0x00002000)

typedef enum
{
    QL_FLASH_REGION_BOOTLOADER = 0,
    QL_FLASH_REGION_APPLICATION_BANK_1,
    QL_FLASH_REGION_APPLICATION_BANK_2,
    QL_FLASH_REGION_CONFIG_INFO_BANK_1,
    QL_FLASH_REGION_CONFIG_INFO_BANK_2,
    QL_FLASH_REGION_MAX
}QL_FLASH_REGION_TYPE;
#endif

void ql_internal_flash_init(void);

int16_t ql_internal_flash_erase(uint32_t addr, uint32_t size);

int16_t ql_internal_flash_read(uint32_t addr, void *data, uint32_t length);

int16_t ql_internal_flash_wirte(uint32_t addr, void *data, uint32_t length);

#endif /* __QL_INTERNAL_FLASH_IMPL_H__ */

/***
 *
 * FLASH SZIE -> 256K  SINGLE BANK
 *
 * 0x08000000  ->  bootloader   => max size 32K
 * 0x08008000  ->  application  => max size 184K
 * 0x08036000  ->  nvm start
 * 0x0803C000  ->  nvm end      => max size 24K
 * 0x0803E000  ->  config info  => max size 8K
 *
 *
 * FLASH SZIE -> 256K  DUAL BANK
 *
 * 0x08000000  ->  bootloader           => max size 32K
 * 0x08008000  ->  application bank 1   => max size 128K
 * 0x08028000  ->  application bank 2   => max size 56K
 * 0x08036000  ->  nvm start
 * 0x0803BFFF  ->  nvm end              => max size 24K
 * 0x0803C000  ->  config info bank 1   => max size 8K
 * 0x0803E000  ->  config info bank 2   => max size 8K
 *
 *
 * FLASH SZIE -> 512K  DUAL BANK
 *
 * 0x08000000  ->  bootloader           => max size 32K
 * 0x08008000  ->  application bank 1   => max size 224K
 * 0x08040000  ->  firmware    bank 2   => max size 216K
 * 0x08076000  ->  nvm start
 * 0x0807BFFF  ->  nvm end              => max size 24K
 * 0x0807C000  ->  config info bank 1   => max size 8K
 * 0x0807E000  ->  config info bank 2   => max size 8K
 *
*/
