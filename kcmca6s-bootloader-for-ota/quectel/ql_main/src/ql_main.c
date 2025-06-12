#include "ql_main.h"

ql_bootloader_version_info_t boot_version_info = 
{
    .BOOT_V_MAJOR = QL_BOOTLOADER_MAJOR,
    .BOOT_V_MINOR = QL_BOOTLOADER_MINOR,
    .BOOT_V_BUILD = QL_BOOTLOADER_BUILD
};

typedef struct
{
    uint16_t reset_reason;
    uint8_t *reset_reason_srt;
}ql_bootloader_reset_reason_tab_t;

ql_bootloader_reset_reason_tab_t restet_tabs[] =
{
    {BOOTLOADER_RESET_REASON_UNKNOWN, (uint8_t *)"BOOTLOADER_RESET_REASON_UNKNOWN"},
    {BOOTLOADER_RESET_REASON_UPG_SEQUENCE, (uint8_t *)"BOOTLOADER_RESET_REASON_UPG_SEQUENCE"},
    {BOOTLOADER_RESET_REASON_BADAPP, (uint8_t *)"BOOTLOADER_RESET_REASON_BADAPP"},
    {BOOTLOADER_RESET_REASON_OTA_SEQUENCE, (uint8_t *)"BOOTLOADER_RESET_REASON_OTA_SEQUENCE"}
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
static ql_bootloader_t *bootlader = NULL;

void ql_main(void)
{
    ql_uart_open(QL_UART_UART0_PORT);
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

    ql_printf("[BOOT]BOOTLOADER RESET REASON: [%s]\r\n",
                ql_bootloader_convert_reset_reason2str(ql_bootloader_reset_get_reset_reason()));

    bootlader = ql_bootloader_create();

    if (!bootlader || bootlader->vptr->init(bootlader) != 0)
    {
        ql_printf("[BOOT]QUECTEL BOOTLOADER INIT FAIL\r\n");
        bootlader->vptr->deinit(bootlader);

        ql_bootloader_reset_with_reason(BOOTLOADER_RESET_REASON_UNKNOWN);
    }
    ql_printf("[BOOT]QUECTEL BOOTLOADER INIT SUCCESSFULLY\r\n");

    if(bootlader->vptr->recognize_mode(bootlader) != 0)
    {
        ql_printf("[BOOT]QUECTEL BOOTLOADER RECOGNIZE MODE FAIL\r\n");
    }

    if(bootlader->vptr->enter_sequence(bootlader) != 0)
    {
        ql_printf("[BOOT]QUECTEL BOOTLOADER ENTER SEQUENCE FAIL\r\n");
    }

    if((BOOTLOADER_RESET_REASON_OTA_IMAGE_FAIL == bootlader->rst_reason) && (bootlader->i_reset_reason == BOOTLOADER_RESET_REASON_BADAPP))
    {
        bootlader->is_enter_mode = QL_BOOTLOADER_MODE_UPG_WIRED;
        bootlader->vptr->enter_sequence(bootlader);
        ql_printf("[BOOT]QUECTEL BOOTLOADER ENTER WIRED UPGRADE MODE\r\n");
    }
    else
    {
        uint16_t rst_reason = bootlader->rst_reason;
        bootlader->vptr->deinit(bootlader);
        ql_bootloader_reset_with_reason(rst_reason);
    }
}

void ql_main_loop(void)
{
    if(bootlader->dev->uart->vptr->available(bootlader->dev->uart) > 0)
    {
        uint8_t buf[1024] = { 0 };
        uint32_t recv_len = 0;
        uint32_t ret = bootlader->dev->uart->vptr->read(bootlader->dev->uart, buf, sizeof(buf), &recv_len);
        if(ret == 0)
        {
            bootlader->dev->uart->vptr->write(bootlader->dev->uart, buf, recv_len);
        }
    }
}
