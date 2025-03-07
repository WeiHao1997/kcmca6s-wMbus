#ifndef __QL_BOOTLOADER_H__
#define __QL_BOOTLOADER_H__


#include "ql_include.h"

#define QL_BOOTLOADER_MAJOR     0x01       //  0x00 - 0xFF
#define QL_BOOTLOADER_MINOR     0x00       //  0x00 - 0xFF
#define QL_BOOTLOADER_BUILD     0x03       //  0x00 - 0xFF

typedef struct
{
    short int Year;
    short int Mon;
    short int Day;
    short int Hour;
    short int Min;
    short int Sec;
    short int Millisec;
} ql_build_time_t;

typedef struct
{
    uint8_t BOOT_V_MAJOR;
    uint8_t BOOT_V_MINOR;
    uint8_t BOOT_V_BUILD;
}ql_bootloader_version_info_t;

typedef struct
{
    uint32_t appStartAddr;
    uint32_t flashMaxSize;
    uint16_t resetReason;
}ql_bootloader_info_t;

void ql_bootloader_init(void);

void ql_bootloader_sequence_start(ql_bootloader_info_t *boot_info);

void ql_bootloader_check_reset_reason(bool *is_enterapp, bool *is_verifyApp);

#endif /* __QL_BOOTLOADER_H__ */
