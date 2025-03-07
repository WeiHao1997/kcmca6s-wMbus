#ifndef __QL_UPGRADE_H__
#define __QL_UPGRADE_H__

#include "ql_include.h"

#define FIRMWARM_FRAM_HEAD 0x55AA55AA

#define SWAP_UINT16(val) (((val) << 8) | ((val) >> 8))
#define SWAP_UINT32(val) ((((val) << 24) & 0xFF000000) | \
                          (((val) << 8) & 0x00FF0000) | \
                          (((val) >> 8) & 0x0000FF00) | \
                          (((val) >> 24) & 0x000000FF))

void ql_bootloader_upgrade_enter(void);

#endif /* __QL_UPGRADE_H__ */