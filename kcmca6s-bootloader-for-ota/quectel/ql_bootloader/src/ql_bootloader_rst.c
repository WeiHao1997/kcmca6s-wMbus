/*
 * ql_bootloader_rst.c
 *
 *  Created on: 2025年6月3日
 *      Author: kahn.wei
 */

#include "ql_bootloader_rst.h"
#include "ql_internal_flash_impl.h"

#if defined(__GNUC__)
extern uint32_t __ResetReasonStart__;
#elif defined(__ICCARM__)
#pragma section = "BOOTLOADER_RESET_REASON"
#endif


void ql_bootloader_set_reset_reason(uint16_t resetReason)
{
#if defined(__GNUC__)
  uint32_t resetReasonBase = (uint32_t)&__ResetReasonStart__;
#elif defined(__ICCARM__)
  void *resetReasonBase =   __section_begin("BOOTLOADER_RESET_REASON");
#endif

  ql_bootloader_rst_cause_t *cause = (ql_bootloader_rst_cause_t *) (resetReasonBase);

  cause->reason = resetReason;
}

void ql_bootloader_reset_with_reason(uint16_t resetReason)
{
  ql_bootloader_set_reset_reason(resetReason);

  // Trigger a software system reset
#if defined(RMU_PRESENT)
  // Set reset mode to EXTENDED reset
  RMU->CTRL = (RMU->CTRL & ~_RMU_CTRL_SYSRMODE_MASK) | RMU_CTRL_SYSRMODE_EXTENDED;
#endif
  NVIC_SystemReset();
}

uint16_t ql_bootloader_reset_get_reset_reason(void)
{
#if defined(__GNUC__)
    uint32_t resetReasonBase = (uint32_t)&__ResetReasonStart__;
#elif defined(__ICCARM__)
    void *resetReasonBase =   __section_begin("BOOTLOADER_RESET_REASON");
#endif
    ql_bootloader_rst_cause_t* cause = (ql_bootloader_rst_cause_t*) (resetReasonBase);
    return cause->reason;
}
