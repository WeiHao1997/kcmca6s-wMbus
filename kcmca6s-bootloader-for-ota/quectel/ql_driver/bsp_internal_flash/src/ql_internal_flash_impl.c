#include <ql_internal_flash_impl.h>
#include "em_msc.h"
#include "em_cmu.h"

#if !defined(_SILICON_LABS_32B_SERIES_2)
static MSC_Status_TypeDef writeHalfword(uint32_t address, uint16_t data)
{
    uint32_t address32, data32;
    MSC_Status_TypeDef retval;

    address32 = address & ~3UL;

    if(address & 2UL) 
    {
        data32 = 0x0000FFFFUL | ((uint32_t)data << 16UL);
    } 
    else 
    {
        data32 = 0xFFFF0000UL | (uint32_t)data;
    }

    retval = MSC_WriteWord((uint32_t *)address32, &data32, 4U);

    return retval;
}
#endif

static bool flash_erasePage(uint32_t address)
{
#if defined(_CMU_CLKEN1_MASK)
    CMU->CLKEN1_SET = CMU_CLKEN1_MSC;
#endif

    MSC_Status_TypeDef retval = MSC_ErasePage((uint32_t *)address);

    if (retval == mscReturnOk) 
    {
        return true;
    } 
    else 
    {
        return false;
    }
}

static bool flash_writeBuffer(uint32_t address, void* data, size_t length)
{
    MSC_Status_TypeDef retval = mscReturnOk;

    if (length == 0UL)
    {
        // Attempt to write zero-length array, return immediately
        return true;
    }

#if defined(_SILICON_LABS_32B_SERIES_2)
    if ((address & 3UL) || (length & 3UL))
    {
        // Unaligned write, return early
        return false;
    }

#if defined(_CMU_CLKEN1_MASK)
    CMU->CLKEN1_SET = CMU_CLKEN1_MSC;
#endif /* _CMU_CLKEN1_MASK */

    retval = MSC_WriteWord((uint32_t *)address, data, length);
#else
    uint16_t * data16 = (uint16_t *)data;

    if((address & 1UL) || (length & 1UL))
    {
        // Unaligned write, return early
        return false;
    }

    // Flash unaligned data at start
    if(address & 2UL)
    {
        if((writeHalfword(address, *data16)) != mscReturnOk)
        {
            return false;
        }
        address += 2UL;
        length -= 2UL;
        data16++;
    }

    // Flash word-aligned data
    if(length >= 4UL)
    {
        uint32_t length16 = (length & ~3UL);
        retval = MSC_WriteWord((uint32_t *)address, data16, length16);
        data16 += length16 / sizeof(uint16_t);
        address += length16;
        length -= length16;
    }

    if(retval != mscReturnOk)
    {
        return false;
    }

    // Flash unaligned data at end
    if(length > 0UL)
    {
        retval = writeHalfword(address, *data16);
        address += 2UL;
        length -= 2UL;
    }
#endif // #if defined(_SILICON_LABS_32B_SERIES_2)

    if(retval == mscReturnOk)
    {
        return true;
    }
    else 
    {
        return false;
    }
}

int16_t ql_internal_flash_erase(uint32_t addr, uint32_t size)
{
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
    if((data == NULL) || (length == 0))
    {
        return -1;
    }

    (void)memcpy(data, (uint32_t *)addr, length);

    return 0;
}

int16_t ql_internal_flash_wirte(uint32_t addr, void *data, uint32_t length)
{
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
