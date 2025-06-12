#ifndef __QL_MAIN_H__
#define __QL_MAIN_H__

#include "ql_include.h"
#include "ql_uart.h"
#include "ql_utils.h"
#include "ql_bootloader_core.h"
#include "ql_bootloader_rst.h"

#define QL_BOOTLOADER_MAJOR     0x01       //  0x00 - 0xFF
#define QL_BOOTLOADER_MINOR     0x01       //  0x00 - 0xFF
#define QL_BOOTLOADER_BUILD     0x00       //  0x00 - 0xFF

typedef struct
{
    uint8_t BOOT_V_MAJOR;
    uint8_t BOOT_V_MINOR;
    uint8_t BOOT_V_BUILD;
}ql_bootloader_version_info_t;


void ql_main(void);

void ql_main_loop(void);

#endif /* __QL_MAIN_H__ */
