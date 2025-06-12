/*
 * ql_bootloader_help.c
 *
 *  Created on: 2025年6月3日
 *      Author: kahn.wei
 */


#include "ql_bootloader_help.h"
#include "ql_bootloader_rst.h"
#include "ql_internal_flash.h"
#include "ql_crc.h"

#if defined (__GNUC__)
extern uint32_t __StackTop, __StackLimit;
extern uint32_t __data_start__, __data_end__;
extern uint32_t __bss_start__, __bss_end__;
#else
extern uint32_t CSTACK$$Base, CSTACK$$Limit;
extern uint32_t STACKSEAL$$Limit, HEAP$$Base;
#endif

#define QL_BOOTLOATER_FIRMWARE_MAGIC_WORD   0x55AA55AA
#define FLASH_FIRMWARE_CHECK_BUF_SZIE       2048

typedef struct 
{
    uint32_t magic_word;
    uint32_t firmware_size;
    uint32_t firmware_crc32;
    uint32_t segment_length;
}ql_bootloader_config_info_t;

static bool ql_bootlader_verify_application(uint32_t startAppBase, uint32_t appConfigBase)
{
    ql_bootloader_config_info_t *firmware_info = (ql_bootloader_config_info_t *)appConfigBase;

    if(QL_BOOTLOATER_FIRMWARE_MAGIC_WORD != firmware_info->magic_word)
    {
        return false;
    }

    if(0 >= firmware_info->firmware_size)
    {
        return false;
    }

    uint8_t is_loop = 1;
    uint32_t read_len = 0;
    uint32_t read_offset = 0;
    uint32_t is_image_size = firmware_info->firmware_size;
    uint32_t calc_crc32 = 0;
    uint8_t buffer[FLASH_FIRMWARE_CHECK_BUF_SZIE] = { 0 };

    while ((is_loop == 1) && (is_image_size > 0))
    {
        read_len = FLASH_FIRMWARE_CHECK_BUF_SZIE < is_image_size ? FLASH_FIRMWARE_CHECK_BUF_SZIE : is_image_size;
        if(0 != ql_internal_flash_read(startAppBase + read_offset, buffer, read_len))
        {
            is_loop = 0;
            return false;
        }
        calc_crc32 = Ql_Check_CRC32(calc_crc32, buffer, 0, read_len);
        is_image_size -= read_len;
        read_offset += read_len;

        if(read_offset > QL_FLASH_APPLICATION_BANK_1_SIZE)
        {
            is_loop = 0;
            return false;
        }
    }

    if(firmware_info->firmware_crc32 != calc_crc32)
    {
        return false;
    }

    return true;
}

/**
 * Check whether we should enter the bootloader
 *
 * @return True if the bootloader should be entered
 */
__STATIC_INLINE bool ql_bootloader_enter_boot(void)
{
#if defined(EMU_RSTCAUSE_SYSREQ)
    if (EMU->RSTCAUSE & EMU_RSTCAUSE_SYSREQ) 
    {
#else
    if (RMU->RSTCAUSE & RMU_RSTCAUSE_SYSREQRST) 
    {
#endif
        // Check if we were asked to run the bootloader...
        switch (ql_bootloader_reset_get_reset_reason())
        {
            case BOOTLOADER_RESET_REASON_BOOTLOAD:
            case BOOTLOADER_RESET_REASON_UPG_SEQUENCE:
            case BOOTLOADER_RESET_REASON_BADAPP:
            case BOOTLOADER_RESET_REASON_OTA_SEQUENCE:
            // Asked to go into bootloader mode
            return true;
            default:
            break;
        }
    }
    return false;
}

static inline void jumpToApplicationRoutine(uint32_t startOfAppSpace)
{
#if defined(BOOTLOADER_SECURE)

// Erase rw section
  __ASM volatile(
    "ldr r3,=0                  \n"
    "mov r1, %[rwStart]         \n" // Load start address
    "mov r2, %[rwEnd]           \n" // Load end address
#if defined(__GNUC__)
    "mov r4, %[bssStart]        \n" // Load bss section start address
    "mov r5, %[bssEnd]          \n" // Load bss section end address
#endif
    "data_clean_up_loop:        \n"
    "cmp r1, r2                 \n" // Check if we have reached the end address
    "beq data_clean_up_finished \n"
    "str r3, [r1]               \n" // Clear the RAM content pointed by R1
    "add r1, r1, #4             \n" // Increment by 4 to get to the next word address
    "b data_clean_up_loop       \n"
    "data_clean_up_finished:    \n"
#if defined(__GNUC__)
    "bss_clean_up_loop:         \n"
    "cmp r4, r5                 \n" // Check if we have reached the end address
    "beq bss_clean_up_finished  \n"
    "str r3, [r4]               \n" // Clear the RAM content pointed by R4
    "add r4, r4, #4             \n" // Increment by 4 to get to the next word address
    "b bss_clean_up_loop        \n"
    "bss_clean_up_finished:     \n"
#endif
    : //No outputs
#if defined(__GNUC__)
    :[rwStart] "r" (&__data_start__), [rwEnd] "r" (&__data_end__), [bssStart] "r" (&__bss_start__), [bssEnd] "r" (&__bss_end__) //Inputs
    : "r1", "r2", "r3", "r4", "r5" //Clobbered registers
    );
#else
    :[rwStart] "r" (&STACKSEAL$$Limit), [rwEnd] "r" (&HEAP$$Base) //Inputs
    : "r1", "r2", "r3" //Clobbered registers
    );
#endif

#if defined(TEST_BOOTLOADER_RAM_CLEAN_UP)
  __ASM volatile(
    "mov r5, %[dataSection]       \n"     // Address of the last word in the rw section
    "mov r6, %[stackSection]      \n"     // Address of the last word in the stack section
    "mov r7, #0xFEEB              \n"     // Magic word stored at the end of data and stack sections
    "sub r5, r5, #4               \n"
    "sub r6, r6, #4               \n"
    "str r7, [r5]                 \n"     // Store magic word at the end of data section
    "str r7, [r6]                 \n"     // Store magic word at the end of stack section
    : //No outputs
#if defined(__GNUC__)
    :[dataSection] "r" (&__bss_end__), [stackSection] "r" (&__StackTop) //Inputs
    : "r5", "r6", "r7", "r3" //Clobbered registers
    );
#else
    :[dataSection] "r" (&HEAP$$Base), [stackSection] "r" (&CSTACK$$Limit) //Inputs
    : "r5", "r6", "r7", "r3" //Clobbered registers
    );
#endif
#endif  // end of TEST_BOOTLOADER_RAM_CLEAN_UP

#endif // end of BOOTLOADER_SECURE

#if defined(BOOTLOADER_SECURE)
  // We should not leave any traces of the secure bootloader before jumping into the application
  // clean up entire RAM content except for the reset reason word
  __ASM volatile(
    "ldr r4,=0                 \n"
    "mov r2, %[stackBase]      \n" // Load start address
    "mov r3, %[stackTop]       \n" // Load end address
#if defined(TEST_BOOTLOADER_RAM_CLEAN_UP)
    "sub r3, r3, #4            \n" // Account for magic word at the top of stack section
#endif
    "mov r1, %[ramBase]        \n"
    "cmp r2, r1                \n" // Check if stack base is equal to SRAM_BASE
    "bne ram_clean_up_loop     \n"
    "add r2, r2, #4            \n" // Increment by 4 to omit reset reason word in SRAM
    "ram_clean_up_loop:        \n"
    "cmp r2, r3                \n" // Check if we have reached the end address
    "beq ram_clean_up_finished \n"
    "str r4, [r2]              \n" // Clear the RAM content pointed by R2
    "add r2, r2, #4            \n" // Increment by 4 to get to the next word address
    "b ram_clean_up_loop       \n"
    "ram_clean_up_finished:    \n"
#else
  // Load SP and PC of application
  __ASM volatile(
#endif // BOOTLOADER_SECURE
    "mov r0, %[startOfAppSpace]       \n" // Load address of SP into R0
    "ldr r1, [r0]                     \n" // Load SP into R1
    "msr msp, r1                      \n" // Set MSP
    "msr psp, r1                      \n" // Set PSP
    "ldr r0, [r0, #4]                 \n" // Load PC into R0
    "mov pc, r0                       \n" // Set PC
    : // No outputs
#if defined(BOOTLOADER_SECURE)
#if defined(__GNUC__)
    :[startOfAppSpace] "r" (startOfAppSpace), [stackBase] "r" (&__StackLimit), [stackTop] "r" (&__StackTop), [ramBase] "r" (SRAM_BASE)  // Inputs
    : "r0", "r1", "r2", "r3", "r4" // Clobbered registers
    );
#else
    :[startOfAppSpace] "r" (startOfAppSpace), [stackBase] "r" (&CSTACK$$Base), [stackTop] "r" (&CSTACK$$Limit), [ramBase] "r" (SRAM_BASE)
    : "r0", "r1", "r2", "r3", "r4" // Clobbered registers
    );
#endif // __GNUC__
#else
    :[startOfAppSpace] "r" (startOfAppSpace)  // Inputs
    : "r0", "r1" // Clobbered registers
    );
#endif  // BOOTLOADER_SECURE
}

/**
 * Jump to app
 */
__attribute__ ((noreturn, naked)) static void bootToApp(uint32_t startOfAppSpace)
{
    jumpToApplicationRoutine(startOfAppSpace);
    while (1);
}

void ql_system_init(void)
{
#if defined(_SILICON_LABS_32B_SERIES_2_CONFIG_5) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_6) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_8)
    configureSMU();
#endif // _SILICON_LABS_32B_SERIES_2_CONFIG_5 || _SILICON_LABS_32B_SERIES_2_CONFIG_6 || _SILICON_LABS_32B_SERIES_2_CONFIG_8

#if defined(TEST_BOOTLOADER_RAM_CLEAN_UP)
    ram_clean_up_test();
#endif

    // Assumption: We should enter the app
    volatile bool enterApp = true;
    // Assumption: The app should be verified
    volatile bool verifyApp = true;

    // Check if we came from EM4. If any other bit than the EM4 bit it set, we
    // can't know whether this was really an EM4 reset, and we need to do further
    // checking.
#if defined(RMU_RSTCAUSE_EM4RST) && defined(APPLICATION_VERIFICATION_SKIP_EM4_RST) && (APPLICATION_VERIFICATION_SKIP_EM4_RST == 1)
    if (RMU->RSTCAUSE == RMU_RSTCAUSE_EM4RST)
    {
        // We came from EM4, app doesn't need to be verified
        verifyApp = false;
    }
    else if (ql_bootloader_enter_boot())
    {
        // We want to enter the bootloader, app doesn't need to be verified
        enterApp = false;
        verifyApp = false;
    }
#else
    if (ql_bootloader_enter_boot())
    {
        // We want to enter the bootloader, app doesn't need to be verified
        enterApp = false;
        verifyApp = false;
    }
#endif
    uint32_t startOfAppSpace = (uint32_t)QL_FLASH_APPLICATION_BANK_1_BASE;
    uint32_t startOfAppConfig = (uint32_t)QL_FLASH_CONFIG_INFO_BANK_1_BASE;

    // Sanity check application program counter
#if defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"
#endif
    uint32_t pc = *(uint32_t *)(startOfAppSpace + 4);
    if (pc == 0xFFFFFFFF)
    {
        // Sanity check failed; enter the bootloader
        ql_bootloader_set_reset_reason(BOOTLOADER_RESET_REASON_BADAPP);
        enterApp = false;
        verifyApp = false;
    }
#if defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
#if defined(_SILICON_LABS_32B_SERIES_2) && defined(CRYPTOACC_PRESENT)
    // fih_delay_init is applicable for vse devices
    fih_delay_init();
#endif
    // App should be verified

    if(verifyApp)
    {
        // If app verification fails, enter bootloader instead
        enterApp = ql_bootlader_verify_application(startOfAppSpace, startOfAppConfig);
        if(!enterApp)
        {
            ql_bootloader_set_reset_reason(BOOTLOADER_RESET_REASON_BADAPP);
        }
    }
    if(enterApp)
    {
#if defined(BOOTLOADER_APPLOADER) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_5) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_6) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_8)
        configureSMUToDefault();
#endif
        // Set vector table to application's table
        SCB->VTOR = startOfAppSpace;

        bootToApp(startOfAppSpace);
    }
    // Enter bootloader
}
