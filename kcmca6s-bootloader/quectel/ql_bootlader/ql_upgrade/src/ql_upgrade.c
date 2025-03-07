#include "ql_upgrade.h"

#define FLASH_ERASE_SIZE (1024 * 8)

#define UART_DATA_MAX_SZIE (1024 * 2)

#define UART_DATA_TIMEOUT  10 // unit: S
#define UART_DATA_TIMEOUT_BASE 30000

#define RETRY_MAX_CNT  10

#define IS_DEBUG_ON 0
 
typedef struct 
{
    uint32_t app_base_addr;
    uint32_t app_size;
    uint32_t app_crc32;
    uint32_t cal_crc32;
    uint32_t write_firmware_total;
    uint32_t write_cur_flash_addr;
}ql_upgrade_flash_info_t;

typedef enum 
{
    QL_UPG_STATUS_SEND_ACK,
    QL_UPG_STATUS_SEND_NACK,
    QL_UPG_STATUS_LISTENING_DATA,
    QL_UPG_STATUS_SEND_DATA,
    QL_UPG_STATE_PARASER_RECV_DATA,
    QL_UPG_STATUS_FLASH_ERASE_WRITE,
    QL_UPG_STATUS_FLASH_WRITE_CHECK,
    QL_UPG_STATUS_COMPLETE,
    QL_UPG_STATUS_IDLE,
    QL_UPG_STATUS_MAX
}ql_upg_status_e;

typedef struct 
{
    uint8_t status_id;
    uint8_t is_running;
    uint8_t is_req_data;
    uint8_t is_start_recv;
    uint32_t req_head_cnt;
    uint32_t uart_recv_time;
    uint32_t req_data_cnt;
    uint32_t last_req_data_cnt;
    uint32_t last_req_data_repet_cnt;
    uint32_t last_tick_s;
    uint32_t retry_cnt;
    uint32_t unknow_reason_retry_cnt;
    ql_upgrade_flash_info_t flash_info;
}ql_upgrade_sequence_t;

typedef const void (*ql_upg_seq_handler)(ql_upgrade_sequence_t *upg_seq);

typedef struct 
{
    ql_upg_status_e sta_id;
    ql_upg_seq_handler seq_handle;
}ql_upgrade_sequence_handle_t;

typedef struct 
{
    uint32_t front;
    uint32_t tail;
    uint8_t *buf;
}ql_cmd_handle_uart_buf;

ql_cmd_handle_uart_buf g_cmd_uart_buf = { 0 };
ql_cmd_handle_uart_buf *cmd_uart_buf = &g_cmd_uart_buf;

uint8_t uart_buf[UART_DATA_MAX_SZIE] = { 0 };
static uint8_t recv_buf_payload_info[1024] = { 0 };

typedef ql_bootloader_upg_frame_t ql_upg_firmware_info_t;
ql_upg_firmware_info_t  g_upg_firmware_info = { 0 };

typedef struct 
{
    uint16_t total_cnt;
    uint32_t current_cnt;
    uint16_t curren_len;
    uint8_t payload_data[0];
}__attribute__((packed)) ql_upgrade_payload_parser_info;

static void handler_status_write_firmware_to_flash(ql_upgrade_sequence_t* upg_seq) 
{
    ql_upgrade_payload_parser_info *payload_info = (ql_upgrade_payload_parser_info *)recv_buf_payload_info;

    uint16_t total_cnt = SWAP_UINT16(payload_info->total_cnt);
    uint32_t current_cnt = SWAP_UINT32(payload_info->current_cnt);
    uint16_t curren_len = SWAP_UINT16(payload_info->curren_len);
    uint8_t *ptr_payload = payload_info->payload_data;

    ql_printf("[BOOT]TOTAL COUNT:%04X,CURRENT COUNT:%08X, CURRENT LENGTH:%04X\r\n", total_cnt, current_cnt, curren_len);

    if(upg_seq->req_data_cnt != current_cnt)
    {
        ql_printf("[BOOT]REQUEST DATA COUNT NOT EQU CURRENT DATA COUNT, REQUEST COUNT:%08X, CURRENT COUNT:%08X\r\n", upg_seq->req_data_cnt, current_cnt);
        upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
        upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
        return ;
    }

#if (IS_DEBUG_ON)
    for (size_t i = 0; i <curren_len; i++)
    {
        ql_printf("%02X ", payload_info->payload_data[i]);
        if((i + 1) % 16 == 0) ql_printf("\r\n");
    }
    ql_printf("\r\n");
#endif
    if(current_cnt == 0x00000000)
    {
        uint32_t head = SWAP_UINT32(*((uint32_t *)payload_info->payload_data));
        uint32_t size = SWAP_UINT32(*((uint32_t *)(payload_info->payload_data + 4)));
        uint32_t crc32 = SWAP_UINT32(*((uint32_t *)(payload_info->payload_data + 8)));
        uint32_t flash_offset = SWAP_UINT32(*((uint32_t *)(payload_info->payload_data + 12)));

        ql_internal_flash_erase(QL_FLASH_STRAGE_INFO_BASE, 0x2000);
        ql_internal_flash_wirte(QL_FLASH_STRAGE_INFO_BASE, payload_info->payload_data, 16);

        ptr_payload = &payload_info->payload_data[16];
        curren_len -= 16;
        upg_seq->flash_info.app_crc32 = crc32;
        upg_seq->flash_info.app_size = size;

        ql_printf("[BOOT]FRAME HEAD:%08X, LENGTH:%08X, CRC32:%08X, OFFSET:%08X\r\n",
            head, size, crc32, flash_offset);
        if(FIRMWARM_FRAM_HEAD != head)
        {
            ql_printf("[BOOT]FIRMWARE FRAME HEAD ERROR, PLEASE CHECK TRANSMITED FIRMWARE\r\n");
            upg_seq->req_head_cnt++;
            if(upg_seq->req_head_cnt > RETRY_MAX_CNT) 
            {
                upg_seq->is_running = 0;
                return;
            }
            upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
            upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;  
            return;
        }
    }

    uint16_t f_crc16 = crc16_calculate(ptr_payload, curren_len);

    ql_printf("[BOOT]FLASH WRITE ADDR:%08X, WRITE LENGTH:%d, PAYLOAD_WRITE_CRC:%04X\r\n", 
                upg_seq->flash_info.app_base_addr + upg_seq->flash_info.write_firmware_total, curren_len, f_crc16
                );
    int16_t ret = ql_internal_flash_wirte(upg_seq->flash_info.app_base_addr + upg_seq->flash_info.write_firmware_total, 
                                        ptr_payload, curren_len);
    if(ret != 0)
    {
        ql_printf("[BOOT]ERASE/WRITE FLASH ERROR, PLEASE CHECK FLASH STATUS\r\n");
        upg_seq->is_running = 0;
        upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
        return;
    }

    uint8_t t_read_buf[2048] = { 0 };
    ret = ql_internal_flash_read(upg_seq->flash_info.app_base_addr + upg_seq->flash_info.write_firmware_total, 
                                    t_read_buf, curren_len);
    if(ret != 0)
    {
        ql_printf("[BOOT]READ FLASH ERROR, PLEASE CHECK FLASH STATUS\r\n");
        upg_seq->is_running = 0;
        upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
        return;
    }

    uint16_t s_crc16 = crc16_calculate(t_read_buf, curren_len);
    ql_printf("[BOOT]FLASH READ ADDR:%08X, READ LENGTH:%d, PAYLOAD_READ_CRC:%04X\r\n", 
            upg_seq->flash_info.app_base_addr + upg_seq->flash_info.write_firmware_total, curren_len, s_crc16
            );
    if(f_crc16 != s_crc16)
    {
        ql_printf("[BOOT]CRC16 CHECK FAIL\r\n");
        ret = ql_internal_flash_wirte(upg_seq->flash_info.app_base_addr + upg_seq->flash_info.write_firmware_total, 
                                            ptr_payload, curren_len);
        if(ret != 0)
        {
            ql_printf("[BOOT]ERASE/WRITE FLASH ERROR, PLEASE CHECK FLASH STATUS\r\n");
            upg_seq->is_running = 0;
            upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
            return;
        }
    }

    upg_seq->flash_info.write_firmware_total += curren_len;
    upg_seq->req_data_cnt++;
    upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
    upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
    ql_printf("[BOOT]FIRMWARE RECV LENGTH:%08X, TOTAL LENGTH:%08X\r\n", upg_seq->flash_info.write_firmware_total, upg_seq->flash_info.app_size);

    /* 升级完成 */
    if(upg_seq->flash_info.write_firmware_total >= upg_seq->flash_info.app_size)
    {  
        ql_printf("[BOOT]CURRENT UPGRADE COMPLETED\r\n");
        upg_seq->is_running = 0;
        upg_seq->status_id = QL_UPG_STATUS_IDLE;
    }
}

static void handler_status_upg_complete(ql_upgrade_sequence_t* upg_seq)
{
    // ql_printf("[BOOT]EXIT UPGRADE SEQUENCE, JUMP TO APP\r\n");
    ql_printf("handler_status_upg_complete\r\n");
    /* 发送数据 02 00 06 22 50 0A 18 68 98  */
    upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
}

static void handler_status_send_ack(ql_upgrade_sequence_t* upg_seq) 
{
    uint8_t ack[10] = {0};
    uint32_t ack_len = 0;
    uint8_t cmd = 0;

    switch (upg_seq->is_req_data)
    {
        case QL_FRAME_CMD_FW_UPG_INFO_RES:
        {
            cmd = QL_FRAME_CMD_FW_UPG_INFO_RES;
            ack[ack_len++] = 0x00;
            ack[ack_len++] = 0x00;
            upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
            upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
        }
        break;
        case QL_FRAME_CMD_FW_DATA_REQUEST:
        {
            cmd = QL_FRAME_CMD_FW_DATA_REQUEST;
            for (int8_t i = (sizeof(upg_seq->req_data_cnt) - 1); i >= 0; i--)
            {
                ack[ack_len++] = *((uint8_t *)&upg_seq->req_data_cnt + i);
            }

            if(upg_seq->req_data_cnt == upg_seq->last_req_data_cnt)
            {
                upg_seq->last_req_data_repet_cnt++;
                if(upg_seq->last_req_data_repet_cnt > RETRY_MAX_CNT)
                {
                    upg_seq->is_running = 0;
                    upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
                    return;
                }
            }
            else
            {
                upg_seq->last_req_data_repet_cnt = 0;
            }

            upg_seq->last_req_data_cnt = upg_seq->req_data_cnt;

            upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
        }
        break;
    
    default:
        break;
    }

    int16_t ret = ql_frame_build_send(cmd, ack, ack_len);
    if (ret != 0)
    {
        ql_printf("[UART]UART SEND ACK ERROR\r\n");
        return;
    }

    if(cmd == QL_FRAME_CMD_FW_UPG_INFO_RES)
    {
        sl_sleeptimer_delay_millisecond(1000);   
    }
}

static void handler_status_send_nack(ql_upgrade_sequence_t* upg_seq) 
{
    uint8_t nack[10] = {0};
    uint32_t nack_len = 0;
    uint8_t cmd = QL_FRAME_CMD_FW_UPG_INFO_RES;

    nack[nack_len++] = 0x00;
    nack[nack_len++] = 0x01;

    int16_t ret = ql_frame_build_send(cmd, nack, nack_len);
    if (ret != 0)
    {
        ql_printf("[UART]UART SEND ACK ERROR\r\n");
        return;
    }

    upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
}

static void handler_status_listening_data(ql_upgrade_sequence_t* upg_seq)
{
    uint32_t tick_s = (sl_sleeptimer_get_tick_count() / UART_DATA_TIMEOUT_BASE);
    if(((tick_s % UART_DATA_TIMEOUT) == 0) && (upg_seq->last_tick_s != tick_s))
    {
        upg_seq->last_tick_s = tick_s;
        ql_printf("%dS, TIMER ALWAYS RUNING:%d,%d\r\n", UART_DATA_TIMEOUT, upg_seq->last_tick_s, tick_s);
        if(((tick_s - upg_seq->uart_recv_time) > UART_DATA_TIMEOUT) && (upg_seq->uart_recv_time > 0) && (upg_seq->is_start_recv == 1))  // 超时
        {
            upg_seq->retry_cnt++;
            if(upg_seq->retry_cnt > RETRY_MAX_CNT)
            {
                // System reboot
                reset_resetWithReason(BOOTLOADER_RESET_REASON_UPGRADE);
            }

            ql_printf("[BOOT]WAIT RECV DATA TIMEOUT!:%ld,RETRY COUNT :%d\r\n", tick_s, upg_seq->retry_cnt);
            upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
            upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
            return;
        }

        if((upg_seq->uart_recv_time == 0) && (upg_seq->is_start_recv == 0))
        {
            upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
            upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
            upg_seq->unknow_reason_retry_cnt++;
            if(upg_seq->unknow_reason_retry_cnt >= RETRY_MAX_CNT)
            {
                ql_printf("[BOOT]UNKNOW REASON RETRY COUNT:%d\r\n", upg_seq->unknow_reason_retry_cnt);
                ql_printf("[BOOT]REBOOT SYSTEM\r\n");
                upg_seq->is_running = 0;
            }
        }

        if((upg_seq->is_start_recv) == 0 && (reset_getResetReason() != BOOTLOADER_RESET_REASON_BADAPP))
        {
            upg_seq->is_running = 0;
        }
    }
    
    if(ql_uart_available(QL_UART_UART1_PORT) > 0)
    {
        uint8_t buf[UART_DATA_MAX_SZIE];
        uint32_t recv_len = 0;

        uint32_t ret = ql_uart_read(QL_UART_UART1_PORT, buf, sizeof(buf), &recv_len);
        if(ret == 0)
        {
            upg_seq->is_start_recv = 1;
            upg_seq->retry_cnt = 0;
            upg_seq->uart_recv_time = (sl_sleeptimer_get_tick_count() % UART_DATA_TIMEOUT_BASE) > 0 ? 
                                        (sl_sleeptimer_get_tick_count() / UART_DATA_TIMEOUT_BASE) : 
                                        (sl_sleeptimer_get_tick_count() / UART_DATA_TIMEOUT_BASE + 1);

            ql_printf("TIME START!:%ld\r\n", upg_seq->uart_recv_time);
            if(cmd_uart_buf->tail + recv_len < UART_DATA_MAX_SZIE)
            {
                memcpy((cmd_uart_buf->buf + cmd_uart_buf->tail), buf, recv_len);
                cmd_uart_buf->tail += recv_len;
            }
            else
            {
                memcpy((cmd_uart_buf->buf + cmd_uart_buf->tail), buf, (UART_DATA_MAX_SZIE - cmd_uart_buf->tail));

                memcpy(cmd_uart_buf->buf, (buf + (UART_DATA_MAX_SZIE - cmd_uart_buf->tail)), recv_len - (UART_DATA_MAX_SZIE - cmd_uart_buf->tail));

                cmd_uart_buf->tail = (recv_len - (UART_DATA_MAX_SZIE - cmd_uart_buf->tail));
            }
            upg_seq->status_id = QL_UPG_STATE_PARASER_RECV_DATA;
        }
    }
}

static void handler_status_paraser_recv_data(ql_upgrade_sequence_t* upg_seq)
{
    int32_t recv_len = (cmd_uart_buf->tail > cmd_uart_buf->front) ? 
    (cmd_uart_buf->tail - cmd_uart_buf->front) : ((UART_DATA_MAX_SZIE - cmd_uart_buf->front) + cmd_uart_buf->tail);

    if(recv_len <= 0)
    {
        upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
        return;
    }

    uint8_t *recv_data_buf = sl_malloc(sizeof(uint8_t) * recv_len);

    if(recv_data_buf == NULL)
    {
        upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
        return;
    }

    memset(recv_data_buf, 0x00, (sizeof(uint8_t) * recv_len));

    if((cmd_uart_buf->front + recv_len) < UART_DATA_MAX_SZIE)
    {
        memcpy(recv_data_buf, (cmd_uart_buf->buf + cmd_uart_buf->front), recv_len);
        cmd_uart_buf->front += recv_len;
    }
    else
    {
         memcpy(recv_data_buf, (cmd_uart_buf->buf + cmd_uart_buf->front), 
                (UART_DATA_MAX_SZIE - cmd_uart_buf->front));

         memcpy((recv_data_buf + (UART_DATA_MAX_SZIE - cmd_uart_buf->front)), cmd_uart_buf->buf, 
                (recv_len - (UART_DATA_MAX_SZIE - cmd_uart_buf->front)));

        cmd_uart_buf->front = cmd_uart_buf->tail;
    }

    if(recv_data_buf[0] != 0x02 || recv_data_buf[1] != 0x00)
    {
        upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
        upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
        return ;
    }

    uint16_t p_len = (((recv_data_buf[1] << 8) | recv_data_buf[2]) & 0x3FF);
    uint16_t cal_crc = crc16_calculate((uint8_t *)&recv_data_buf[1], p_len);
    uint16_t recv_crc = (recv_data_buf[p_len + sizeof(uint8_t)] | (recv_data_buf[p_len + (sizeof(uint8_t) * 2)] << 8));

#if (IS_DEBUG_ON)
    ql_printf("[BOOT]RECV FRAME LENGTH:%d, CAL_CRC:%04X, RECV_CRC:%04X\r\n", p_len, cal_crc, recv_crc);
#endif

    if(cal_crc != recv_crc)
    {
        upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
        upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
        return ;
    }

    g_upg_firmware_info.header.head = recv_data_buf[0];
    g_upg_firmware_info.header.len = p_len;
    g_upg_firmware_info.header.command = recv_data_buf[3];
    g_upg_firmware_info.payload_len = g_upg_firmware_info.header.len - 3;

    if(g_upg_firmware_info.payload == NULL)
    {
        g_upg_firmware_info.payload =  (uint8_t *)sl_malloc(sizeof(uint8_t) * (g_upg_firmware_info.payload_len));
    }

    if(g_upg_firmware_info.payload != NULL)
    {
        (void*)memcpy(g_upg_firmware_info.payload, &recv_data_buf[4], g_upg_firmware_info.payload_len);
    }

    g_upg_firmware_info.crc16 = cal_crc;
    
    switch (g_upg_firmware_info.header.command)
    {
        case QL_FRAME_CMD_FW_DATA_RESPONSE:
        {
            (void*)memcpy(recv_buf_payload_info, g_upg_firmware_info.payload, g_upg_firmware_info.payload_len);
            upg_seq->status_id = QL_UPG_STATUS_FLASH_ERASE_WRITE;
        }
        break;
        case QL_FRAME_CMD_SET_RESPONSE:
        {
            ql_printf("QL_FRAME_CMD_SET_RESPONSE\r\n");
            /* 获取到 02 00 05 33 00 00 EA C8 重启 */
            // upg_seq->is_running = 0;
            upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
        }
        break;
        case QL_FRAME_CMD_FW_UPG_INFO_REQ:
        {
            ql_printf("QL_FRAME_CMD_FW_UPG_INFO_REQ\r\n");
            upg_seq->status_id = QL_UPG_STATUS_LISTENING_DATA;
        }
        break;
    
    default:
        {
            upg_seq->status_id = QL_UPG_STATUS_SEND_NACK;
        }
        break;
    }

    if(g_upg_firmware_info.payload != NULL)
    {
        sl_free(g_upg_firmware_info.payload);

        memset(&g_upg_firmware_info, 0x00, sizeof(ql_upg_firmware_info_t));
    }

    if(recv_data_buf != NULL)
    {
        sl_free(recv_data_buf);
    }
}

ql_upgrade_sequence_handle_t seq_hanlde_task[] = 
{
    {QL_UPG_STATUS_SEND_ACK,            handler_status_send_ack},
    {QL_UPG_STATUS_SEND_NACK,           handler_status_send_nack},
    {QL_UPG_STATUS_LISTENING_DATA,      handler_status_listening_data},
    {QL_UPG_STATUS_FLASH_ERASE_WRITE,   handler_status_write_firmware_to_flash},
    {QL_UPG_STATE_PARASER_RECV_DATA,    handler_status_paraser_recv_data},
    {QL_UPG_STATUS_COMPLETE,            handler_status_upg_complete}
};

#define QL_UPG_SEQ_HANDLE_SZIE (sizeof(seq_hanlde_task) / sizeof(ql_upgrade_sequence_handle_t))

static int32_t ql_bootloader_upgrade_init(ql_upgrade_sequence_t *upg_seq)
{
    // 检查参数和状态
    memset(upg_seq, 0x00, sizeof(ql_upgrade_sequence_t));
    memset(cmd_uart_buf, 0x00, sizeof(ql_cmd_handle_uart_buf));
    memset(&g_upg_firmware_info, 0x00, sizeof(ql_upg_firmware_info_t));

    cmd_uart_buf->front = 0;
    cmd_uart_buf->tail = 0;
    cmd_uart_buf->buf = &uart_buf[0];

    upg_seq->status_id = QL_UPG_STATUS_SEND_ACK;
    upg_seq->is_running = 1;
    upg_seq->is_req_data = QL_FRAME_CMD_FW_UPG_INFO_RES;

    if(reset_getResetReason() == BOOTLOADER_RESET_REASON_BADAPP)
    {
        upg_seq->is_req_data = QL_FRAME_CMD_FW_DATA_REQUEST;
    }

    return 0;
}

static void ql_bootloader_upgrade_handle(ql_upgrade_sequence_t *upg_seq)
{
    for (size_t i = 0; i < QL_UPG_SEQ_HANDLE_SZIE; i++)
    {
        if(upg_seq->status_id == seq_hanlde_task[i].sta_id)
        {
            seq_hanlde_task[i].seq_handle(upg_seq);
        }
    }
}

static int32_t ql_bootloader_upgrade_deinit(ql_upgrade_sequence_t *upg_seq)
{
    upg_seq->is_running = 0;
    upg_seq->status_id = QL_UPG_STATUS_IDLE;
    return 0;
}

void ql_bootloader_upgrade_enter(void)
{
    ql_upgrade_sequence_t upg_seq = { 0 };
    ql_bootloader_upgrade_init(&upg_seq);
    upg_seq.flash_info.app_base_addr    = QL_FLASH_APPLICATION_BASE;

    ql_printf("[BOOT]MEMORY FREE HEAP SIZE :%d\r\n", sl_memory_get_free_heap_size());

    while (upg_seq.is_running)
    {
        ql_bootloader_upgrade_handle(&upg_seq);
    }

    ql_bootloader_upgrade_deinit(&upg_seq);
}
