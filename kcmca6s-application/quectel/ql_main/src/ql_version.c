#include "ql_version.h"

#ifndef __CUS_SOFT_WARE_VERSION_MARKET__
const uint8_t Version[] = "KCMCA6SAAR01A02K02_BMV";
const uint8_t Subversion[] = "V02";

const uint8_t CusHardwareVersion[] = "P-ME00-WM00-1BV1.0";
const uint8_t CusSoftwareVersion[] = {0x26};

#else
const uint8_t Version[] = "KCMCA6SAAR01A10K02_BMV";
const uint8_t Subversion[] = "V01";

const uint8_t CusHardwareVersion[] = "P-ME00-WM00-1BV1.0";
const uint8_t CusSoftwareVersion[] = "E.ME00-WM00-050.QC.V0.00.02";
#endif /* __CUS_SOFT_WARE_VERSION_MARKET__ */

uint8_t out_buf[256] = { 0 };
uint32_t out_len = 0;

typedef struct 
{
    uint8_t type;
    uint8_t length;
    const uint8_t *payload;
}ql_version_detail_variable_info_t;

ql_version_detail_variable_info_t g_detail_info[] =
{
    {.type = 0x06, .length = (sizeof(CusHardwareVersion) - 1), .payload = CusHardwareVersion},
#ifndef __CUS_SOFT_WARE_VERSION_MARKET__
    {.type = 0x07, .length = (sizeof(CusSoftwareVersion)), .payload = CusSoftwareVersion}
#else
    {.type = 0x07, .length = (sizeof(CusSoftwareVersion) - 1), .payload = CusSoftwareVersion}
#endif /* __CUS_SOFT_WARE_VERSION_MARKET__ */
};

void ql_output_customer_detail_version_info(void)
{
    uint8_t cus_cmd = 0x22;
    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x05;
    out_buf[out_len++] = 0x04;

    for (size_t i = 0; i < 2; i++)
    {
        out_buf[out_len++] = 0x00;
        out_buf[out_len++] = g_detail_info[i].type;
        out_buf[out_len++] = g_detail_info[i].length;
        memcpy(&out_buf[out_len], g_detail_info[i].payload, g_detail_info[i].length);
        out_len += g_detail_info[i].length;
    }
    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x08;
    out_buf[out_len++] = 05;

    if(reset_getResetReason() == BOOTLOADER_RESET_REASON_UPG_SUCCESS)
    {
        out_buf[out_len++] = 0x00;
        out_buf[out_len++] = 0x0A;
        out_buf[out_len++] = 0x20;  
    }

    out_buf[out_len++] = 0x00;
    out_buf[out_len++] = 0x0C;
    out_buf[out_len++] = 0x04;
    out_buf[out_len++] = 0x00;

    ql_frame_build_send(cus_cmd, out_buf, out_len);
}



void ql_output_version_info(uint8_t param)
{
    uint8_t out_ver[256] = { 0 };

    switch (param)
    {
        case QL_OUTPUT_VERSION_COMPLETE:
        {
            sprintf(out_ver, "+QVERSION: %s\r\n", Version);
        }
        break;
        case QL_OUTPUT_VERSION_SHORT:
        {
            sprintf(out_ver, "%s\r\n", Version);
        }
        break;
        case QL_OUTPUT_VERSION_SUB_VER:
        {
            sprintf(out_ver, "+QSUB: %s\r\n", Subversion);
        }
        break;

    default:
        break;
    }

    ql_uart_write(QL_UART_UART1_PORT, out_ver, strlen(out_ver));
}

// 模块类型     00	05	04
// 硬件版本号   00	06	12	50	2D	4D	45	30	30	2D	57	4D	30	30	2D	31	42	56	31	2E	30
// 软件版本号   00	07	1B	45	2E	4D	45	30	30	2D	57	4D	30	30	2D	30	35	30	2E	51	43	2E	56	30	2E	30	30	2E	30	32
// 厂商类型     00	08	05
// 缓存大小     00	0C	01	00