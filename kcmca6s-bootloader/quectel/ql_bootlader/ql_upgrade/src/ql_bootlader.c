#include "ql_bootloader.h"

#define FLASH_FIRMWARE_CHECK_BUF_SZIE 2048

ql_bootloader_version_info_t boot_version_info = 
{
    .BOOT_V_MAJOR = QL_BOOTLOADER_MAJOR,
    .BOOT_V_MINOR = QL_BOOTLOADER_MINOR,
    .BOOT_V_BUILD = QL_BOOTLOADER_BUILD
};

static void ql_bootloader_query_build_time(ql_build_time_t *time)
{
   char mon_str[5] = { 0 };
   static char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
   // hh:mm:ss
   sscanf(__TIME__, "%hd:%hd:%hd", &time->Hour, &time->Min, &time->Sec);

   // Mmm dd yyyy
   sscanf(__DATE__, "%s %hd %hd", mon_str, &time->Day, &time->Year);
   time->Mon = ((strstr(month_names, mon_str) - &month_names[0]) / 3 + 1);
}

void ql_bootloader_init(void)
{
    sl_memory_init();
    sl_sleeptimer_init();
    ql_internal_flash_init();
    ql_uart_open(QL_UART_UART0_PORT);
    ql_uart_open(QL_UART_UART1_PORT);
    ql_printf("\r\n[BOOT]QUECTEL BOOTLOADER INIT\r\n");
    ql_printf("[BOOT]QUECTEL BOOTLOADER CURRENT VERSION: [V%02d_%02d_%02d]\r\n", 
                boot_version_info.BOOT_V_MAJOR, boot_version_info.BOOT_V_MINOR, boot_version_info.BOOT_V_BUILD);

    ql_build_time_t build_time;
    ql_bootloader_query_build_time(&build_time);
    ql_printf("[BOOT]QUECTEL BOOTLOADER BUILD TIME: [%04d/%02d/%02d,%02d:%02d:%02d]\r\n",
            build_time.Year,
            build_time.Mon,
            build_time.Day,
            build_time.Hour,
            build_time.Min,
            build_time.Sec
    );
}

typedef struct
{
    uint16_t reset_reason;
    uint8_t *reset_reason_srt;
}ql_bootloader_reset_reason_tab_t;

ql_bootloader_reset_reason_tab_t restet_tabs[] = 
{
    {BOOTLOADER_RESET_REASON_UNKNOWN, (uint8_t *)"BOOTLOADER_RESET_REASON_UNKNOWN"},
    {BOOTLOADER_RESET_REASON_UPGRADE, (uint8_t *)"BOOTLOADER_RESET_REASON_UPGRADE"},
    {BOOTLOADER_RESET_REASON_BADAPP, (uint8_t *)"BOOTLOADER_RESET_REASON_BADAPP"}
};

#define BOOTLOADER_RESEST_REASON_SIZE (sizeof(restet_tabs) / sizeof(ql_bootloader_reset_reason_tab_t))

uint8_t *ql_bootloader_convert_reset_reason2str(uint16_t reason_hex)
{
    for (size_t i = 0; i < BOOTLOADER_RESEST_REASON_SIZE; i++)
    {
        if(restet_tabs[i].reset_reason == reason_hex)
        {
            return restet_tabs[i].reset_reason_srt;
        }
    }
    
    return restet_tabs[0].reset_reason_srt;
}

void ql_bootloader_send_upg_error_info(void)
{
    uint8_t out_buf[256] = { 0 };
    uint32_t out_len = 0;

    uint8_t cus_cmd = 0x22;
    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x05;
    out_buf[out_len++] = 0x04;

    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x08;
    out_buf[out_len++] = 0x05;

    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x0A;
    out_buf[out_len++] = 0x80;  

    ql_frame_build_send(cus_cmd, out_buf, out_len);
}

void ql_bootloader_sequence_start(ql_bootloader_info_t *boot_info)
{
    ql_printf("[BOOT]QUECTEL BOOTLOADER SEQUENCE START\r\n");

    ql_printf("[BOOT]APP BASE: [0x%08X], FLASH SZIE: [0x%08X]\r\n", 
                boot_info->appStartAddr, boot_info->flashMaxSize);

    ql_printf("[BOOT]RESET REASON: [%s]\r\n", 
                ql_bootloader_convert_reset_reason2str(boot_info->resetReason));
    
    if(boot_info->resetReason == BOOTLOADER_RESET_REASON_UPGRADE)
    {
        ql_bootloader_upgrade_enter();
        reset_resetWithReason(BOOTLOADER_RESET_REASON_UPG_SUCCESS);
        while (1); 
    }

    if(boot_info->resetReason == BOOTLOADER_RESET_REASON_UNKNOWN)
    {
        ql_printf("[BOOT]QUECTEL BOOTLOADER ENTERY APP FAIL, PLEASE CHECK FIRMWARE\r\n");
        while (1); 
    }

    if(boot_info->resetReason == BOOTLOADER_RESET_REASON_BADAPP)
    {
        ql_printf("[BOOT]QUECTEL BOOTLOADER ENTERY APP FAIL, PLEASE CHECK FIRMWARE\r\n");
        ql_bootloader_send_upg_error_info();
        ql_printf("[BOOT]QUECTEL MODULE SILENT STATUS ENTER\r\n");
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        sl_sleeptimer_delay_millisecond(60000);
        ql_printf("[BOOT]QUECTEL MODULE SILENT STATUS EXIT\r\n");
        ql_bootloader_upgrade_enter();
        sl_sleeptimer_delay_millisecond(1000); 
        reset_resetWithReason(BOOTLOADER_RESET_REASON_UPG_SUCCESS);
        while (1); 
    }
    ql_printf("[BOOT]QUECTEL BOOTLOADER ENTERY UNKNOW STATUS\r\n");
}

typedef struct 
{
    uint32_t head;
    uint32_t app_size;
    uint32_t app_crc32;
    uint32_t app_offset;
}ql_bootlader_firmware_info;

void ql_bootloader_check_reset_reason(bool *is_enterapp, bool *is_verifyApp)
{
    if(BOOTLOADER_RESET_REASON_UPGRADE == reset_getResetReason())
    {
        *is_enterapp = false;
        *is_verifyApp = false;
    }

    uint8_t is_loop = 1;
    ql_bootlader_firmware_info *firmware_info = (ql_bootlader_firmware_info *)QL_FLASH_STRAGE_INFO_BASE;
    uint32_t check_app_head = SWAP_UINT32(firmware_info->head); 
    int32_t check_app_size = SWAP_UINT32(firmware_info->app_size);
    uint32_t check_app_crc32 = SWAP_UINT32(firmware_info->app_crc32);
    if(check_app_head != FIRMWARM_FRAM_HEAD)
    {
        *is_enterapp = false;
        *is_verifyApp = false;
        reset_setResetReason(BOOTLOADER_RESET_REASON_BADAPP);
    }

    uint32_t read_len = 0;
    uint32_t read_offset = 0;
    uint8_t buf[FLASH_FIRMWARE_CHECK_BUF_SZIE] = { 0 };
    uint32_t cal_crc32 = 0;
    while ((is_loop == 1)  && (check_app_size > 0))
    {
        read_len = FLASH_FIRMWARE_CHECK_BUF_SZIE < check_app_size ? FLASH_FIRMWARE_CHECK_BUF_SZIE : check_app_size;
        if(0 != ql_internal_flash_read(QL_FLASH_APPLICATION_BASE + read_offset, buf, read_len))
        {
            is_loop = 0;
            reset_setResetReason(BOOTLOADER_RESET_REASON_BADAPP);
            return 0;
        }
        cal_crc32 = Ql_Check_CRC32(cal_crc32, buf, read_len);

        check_app_size -= read_len;
        read_offset += read_len;
        if(read_offset > QL_FLASH_APPLICATION_SIZE)
        {
            is_loop = 0;
        }
    }
    if(check_app_crc32 != cal_crc32)
    {   
        *is_enterapp = false;
        *is_verifyApp = false;
        reset_setResetReason(BOOTLOADER_RESET_REASON_BADAPP);
    }
}

