/*
 * ql_bootloader_rst.h
 *
 *  Created on: 2025年6月3日
 *      Author: kahn.wei
 */

#ifndef __QL_BOOTLOADER_RST_H__
#define __QL_BOOTLOADER_RST_H__

#include "ql_include.h"

typedef struct
{
  uint16_t reason;
  uint16_t signature;
} ql_bootloader_rst_cause_t;

#define BOOTLOADER_RESET_REASON_UNKNOWN         0x0200u

#define BOOTLOADER_RESET_REASON_BOOTLOAD        0x0202u

#define BOOTLOADER_RESET_REASON_BADAPP          0x0208u

#define BOOTLOADER_RESET_REASON_UPG_SEQUENCE    0x0209u

#define BOOTLOADER_RESET_REASON_UPG_SUCCESS     0x0301u

#define BOOTLOADER_RESET_REASON_OTA_SEQUENCE    0x5748u

#define BOOTLOADER_RESET_REASON_OTA_SUCCESS     0x7768u

#define BOOTLOADER_RESET_REASON_OTA_IMAGE_FAIL  0xFF01u


void ql_bootloader_set_reset_reason(uint16_t resetReason);

void ql_bootloader_reset_with_reason(uint16_t resetReason);

uint16_t ql_bootloader_reset_get_reset_reason(void);

#endif /* __QL_BOOTLOADER_RST_H__ */
