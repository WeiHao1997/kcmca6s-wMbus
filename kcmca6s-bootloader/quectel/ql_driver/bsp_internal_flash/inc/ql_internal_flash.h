#ifndef __QL_INTERNAL_FLASH_H__
#define __QL_INTERNAL_FLASH_H__


#include "ql_include.h"

#define QL_FLASH_APPLICATION_BASE (0x08008000)
#define QL_FLASH_APPLICATION_SIZE 0x00036000
#define QL_FLASH_PAGE_SZIE        0x00002000
#define QL_FLASH_STRAGE_INFO_BASE (0x0803E000)

void ql_internal_flash_init(void);

int16_t ql_internal_flash_erase(uint32_t addr, uint32_t size);

int16_t ql_internal_flash_read(uint32_t addr, void *data, uint32_t length);

int16_t ql_internal_flash_wirte(uint32_t addr, void *data, uint32_t length);


#endif /* __QL_INTERNAL_FLASH_H__ */
