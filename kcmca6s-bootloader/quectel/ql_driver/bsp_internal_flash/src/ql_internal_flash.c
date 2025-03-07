#include "ql_internal_flash.h"

int16_t ql_internal_flash_erase(uint32_t addr, uint32_t size)
{
    if((addr < QL_FLASH_APPLICATION_BASE) || ((QL_FLASH_APPLICATION_BASE + QL_FLASH_APPLICATION_SIZE) < addr))
    {
        return -1;
    }
    
    uint32_t erase_cnt = (size % QL_FLASH_PAGE_SZIE == 0) ? (size / QL_FLASH_PAGE_SZIE) : (size / QL_FLASH_PAGE_SZIE + 1);

    for (size_t i = 0; i < erase_cnt; i++)
    {
        bool ret = flash_erasePage(addr + (i * QL_FLASH_PAGE_SZIE));
        if(ret != true)
        {
            return -1;
        }
    }
    
    return 0;
}

int16_t ql_internal_flash_read(uint32_t addr, void *data, uint32_t length)
{
    if((addr < QL_FLASH_APPLICATION_BASE) || ((QL_FLASH_APPLICATION_BASE + QL_FLASH_APPLICATION_SIZE) < addr))
    {
        return -1;
    }

    if((data == NULL) || (length == 0))
    {
        return -1;
    }

    (void)memcpy(data, (uint32_t *)addr, length);

    return 0;
}

int16_t ql_internal_flash_wirte(uint32_t addr, void *data, uint32_t length)
{    
    if((addr < QL_FLASH_APPLICATION_BASE) || ((QL_FLASH_APPLICATION_BASE + QL_FLASH_APPLICATION_SIZE) < addr))
    {
        return -1;
    }

    if((data == NULL) || (length == 0))
    {
        return -1;
    }

    const uint32_t pageSize = (uint32_t)QL_FLASH_PAGE_SZIE;

    if (addr % pageSize == 0UL) 
    {
        flash_erasePage(addr);
    }

    for (uint32_t pageAddress = (addr + pageSize) & ~(pageSize - 1UL);
    pageAddress < (addr + length);
    pageAddress += pageSize) 
    {
        flash_erasePage(pageAddress);
    }

    bool ret = flash_writeBuffer(addr, data, length);
    if(ret != true)
    {
        return -1;
    }

    return 0;
}

void ql_internal_flash_init(void)
{
    MSC_Init();
}
