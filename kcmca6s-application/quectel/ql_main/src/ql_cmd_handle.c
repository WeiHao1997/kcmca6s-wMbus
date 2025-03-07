#include "ql_cmd_handle.h"

#define UART_DATA_MAX_SZIE  (1 * 1024)
#define UART_DATA_TIMEOUT 15e6

#define QL_CMD_HANDLE_ENTRY_DL_MODE (1 << 0)
#define QL_CMD_HANDLE_ENTRY_FT_MODE (1 << 1)

typedef struct 
{
    uint32_t front;
    uint32_t tail;
    uint8_t *buf;
}ql_cmd_handle_uart_buf;

ql_cmd_handle_uart_buf g_cmd_uart_buf = { 0 };
ql_cmd_handle_uart_buf *cmd_uart_buf = &g_cmd_uart_buf;

wmbus_register_event ql_wmbus_register_change_event = NULL;

uint8_t uart_buf[UART_DATA_MAX_SZIE];

#define CMD_PARSER_MEMORY_POOL_SIZE 1024

typedef struct
{
    uint8_t pool[CMD_PARSER_MEMORY_POOL_SIZE];
    uint32_t offset;
} MemoryPool;

void memory_pool_init(MemoryPool **pool)
{
    *pool = (MemoryPool *)sl_malloc(sizeof(MemoryPool));
    (*pool)->offset = 0;
}

void *memory_pool_alloc(MemoryPool *pool, size_t size) 
{
    if(pool->offset + size > CMD_PARSER_MEMORY_POOL_SIZE)
    {
        return NULL;
    }

    void *ptr = pool->pool + pool->offset;
    pool->offset += size;
    return ptr;
}

void memory_pool_reset(MemoryPool *pool) 
{
    pool->offset = 0;
}

typedef enum
{
    E_CMD_STATE_LISTENING_DATA,
    E_CMD_STATE_PARASER_RECV_DATA,
    E_CMD_STATE_CONFIG_REGISTER,
    E_CMD_STATE_TIMEOUT_CHANGE_MODE,
    E_CMD_STATE_RECV_ACK,
    E_CMD_STATE_IDLE
}ql_cmd_state_e;

typedef enum
{
    E_CMD_RECV_STA_OK,
    E_CMD_RECV_STA_ERROR
}ql_cmd_recv_sta_e;

typedef enum
{
    E_CMD_TYPE_UNKNOW,
    E_CMD_TYPE_ATJ,
    E_CMD_TYPE_ATY,
    E_CMD_TYPE_ATQ,
    E_CMD_TYPE_AT0,
    E_CMD_TYPE_FRAME
}ql_cmd_type_e;

typedef enum
{
	E_CMD_CFG_STATE_NORMAL,
    E_CMD_CFG_STATE_AT_CONFIG,
    E_CMD_CFG_STATE_ATJ_CONFIG1, 
    E_CMD_CFG_STATE_ATJ_CONFIG2,
}ql_cmd_cfg_state_e;

typedef struct 
{
    uint8_t org_data[1024];
    uint32_t data_len;
    uint32_t cmd_type;
    uint32_t reg_type;
    uint32_t param_cnt;
}ql_cmd_handle_parase_t;

typedef struct 
{
    uint8_t is_ok_err;
    uint8_t is_first_run;
    uint8_t status_id;
    uint8_t cfg_status_id;
    uint8_t is_quectel_cmd;
    uint8_t is_quectel_mode;
    ql_cmd_handle_parase_t parser_info;
    RAIL_Time_t uart_recv_time;
    MemoryPool **pool;
}ql_cmd_handle_sequence_t;

ql_cmd_handle_sequence_t g_cmd_handle_seq =
{
    .is_first_run = 0,
};

typedef int16_t (*cmd_handle_seq)(ql_cmd_handle_sequence_t *handle_seq);

typedef struct
{
    ql_cmd_state_e cmd_state;
    cmd_handle_seq cmd_seq;
}ql_cmd_handle_task_seq;

typedef void (*ql_cmd_handler_typedef) (ql_cmd_handle_sequence_t *, uint8_t *, uint32_t);

typedef struct
{
    uint8_t *str;
    ql_cmd_handler_typedef cmd_handler;
}ql_cmd_support_typedef;

static void ql_cmd_exec_version(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_gmr(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_prod(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_ftm(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_egmr(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_tune(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_subver(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);
static void ql_cmd_exec_wmbus_auto_recv_config(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len);

const ql_cmd_support_typedef cmd_handle_table[] = 
{
    "AT+QVERSION",  ql_cmd_exec_version,
    "AT+QGMR",      ql_cmd_exec_gmr,
    "AT+QPROD",     ql_cmd_exec_prod,
    "AT+QFTM",      ql_cmd_exec_ftm,
    "AT+EGMR",      ql_cmd_exec_egmr,
    "AT+QCTUNE",    ql_cmd_exec_tune,
    "AT+QSUB",      ql_cmd_exec_subver,
    "AT+AUTORECVCFG", ql_cmd_exec_wmbus_auto_recv_config,
};

#define QL_CMD_HANDLE_TABLE_SIZE (sizeof(cmd_handle_table) / sizeof(ql_cmd_support_typedef))

int16_t ql_cmd_handle_find_id(uint8_t *cmd, uint32_t cmd_len, ql_cmd_support_typedef *support_cmd, int16_t *cmd_id)
{
    for (int16_t i = 0; i < QL_CMD_HANDLE_TABLE_SIZE; i++)
    {
        if(strncmp(cmd, support_cmd[i].str, strlen(support_cmd[i].str)) == 0)
        {
            *cmd_id = i;
            return 0;
        }
    }
    return -1;
}

int16_t ql_cmd_hanlde(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    int16_t cmd_id = -1;
    int16_t ret = ql_cmd_handle_find_id(cmd, cmd_len, cmd_handle_table, &cmd_id);

    if((ret != 0) || (cmd_id == -1))
    {
        return -1;
    }

    cmd_handle_table[cmd_id].cmd_handler(handle_seq, cmd, cmd_len);

    return 0;
}

#include "em_msc.h"
bool flash_writeBuffer(uint32_t       address,
                       void           *data,
                       size_t         length)
{
  MSC_Status_TypeDef retval = mscReturnOk;

  if (length == 0UL) {
    // Attempt to write zero-length array, return immediately
    return true;
  }

#if defined(_SILICON_LABS_32B_SERIES_2)
  if ((address & 3UL) || (length & 3UL)) {
    // Unaligned write, return early
    return false;
  }

#if defined(_CMU_CLKEN1_MASK)
  CMU->CLKEN1_SET = CMU_CLKEN1_MSC;
#endif

  retval = MSC_WriteWord((uint32_t *)address, data, length);
#else
  uint16_t * data16 = (uint16_t *)data;

  if ((address & 1UL) || (length & 1UL)) {
    // Unaligned write, return early
    return false;
  }

  // Flash unaligned data at start
  if (address & 2UL) {
    if ((writeHalfword(address, *data16)) != mscReturnOk) {
      return false;
    }
    address += 2UL;
    length -= 2UL;
    data16++;
  }

  // Flash word-aligned data
  if (length >= 4UL) {
    uint32_t length16 = (length & ~3UL);
    retval = MSC_WriteWord((uint32_t *)address, data16, length16);
    data16 += length16 / sizeof(uint16_t);
    address += length16;
    length -= length16;
  }

  if (retval != mscReturnOk) {
    return false;
  }

  // Flash unaligned data at end
  if (length > 0UL) {
    retval = writeHalfword(address, *data16);
    address += 2UL;
    length -= 2UL;
  }
#endif // #if defined(_SILICON_LABS_32B_SERIES_2)

  if (retval == mscReturnOk) {
    return true;
  } else {
    return false;
  }
}

typedef enum
{
    AT_CMD_OP_QUERY,
    AT_CMD_OP_CFG,
}QL_AT_CMD_OP_TYPE_E;

typedef enum
{
    AT_FOUND_A,
    AT_FOUND_AT,
    AT_FOUND_SYMBOL,
    AT_FOUND_CMD_NAME,
    AT_FOUND_CMD_OP,
    AT_FOUND_CMD_PARAM
}QL_AT_CMD_STATUS_TYPE_E;

typedef struct 
{
    uint8_t status;
    uint8_t max_param;
    uint8_t operate_type;
    uint8_t **param_str;
    uint32_t param_cnt;
}ql_at_cmd_paraer_handle_t;

/***
 * @brief AT command parameter parser
 *  AT+<CMD_NAME>=<PARAM1>,<PARAM2>,<PARAM3>...
 *  AT+<CMD_NAME>?
 * @param cmd_name AT command name
 * @param handle AT command parameter handle
 * @param cmd AT command string
 * @param cmd_len AT command string length
 * @return 0: success, -1: failed
 */
static int16_t ql_at_cmd_paraser_parameter_handle(uint8_t *cmd_name, ql_at_cmd_paraer_handle_t *handle, uint8_t *cmd, uint32_t cmd_len)
{
    uint8_t parser_flag = 1;
    uint8_t *parser_cmd = cmd;
    uint32_t parser_len = cmd_len;
    uint32_t mask_cnt = 0;
    handle->status = AT_FOUND_A;
    if (!parser_cmd)
    {
        return -1;
    }

    if(parser_len < 3)
    {
        return -1;
    }

    while (*parser_cmd != '\0' && (parser_len > 0) && (parser_flag == 1))
    {
        switch (handle->status)
        {
            case AT_FOUND_A:
            {
                if(*parser_cmd == 'A')
                {
                    handle->status = AT_FOUND_AT;
                }
            }
            break;
            case AT_FOUND_AT:
            {
                if(*parser_cmd == 'T')
                {
                    handle->status = AT_FOUND_SYMBOL;
                }
            }
            break;
            case AT_FOUND_SYMBOL:
            {
                if(*parser_cmd == '+')
                {
                    handle->status = AT_FOUND_CMD_NAME;
                }
            }
            break;
            case AT_FOUND_CMD_NAME:
            {
                uint32_t cmd_name_len = strlen(cmd_name) - 3;  // - 3 is remove "AT+"
                if(strncmp(parser_cmd, &cmd_name[3], cmd_name_len) == 0)
                {
                    handle->status = AT_FOUND_CMD_OP;
                    parser_cmd+=(cmd_name_len - 1);
                    parser_len-=(cmd_name_len - 1);
                }
            }
            break;
            case AT_FOUND_CMD_OP:
            {
                switch (*parser_cmd)
                {
                    case '=':
                    {
                        handle->operate_type = AT_CMD_OP_CFG;
                        handle->status = AT_FOUND_CMD_PARAM;
                    }
                    break;
                    case '?':
                    {
                        handle->operate_type = AT_CMD_OP_QUERY;
                        parser_flag = 0;
                        return 0;
                    }
                    break;
                
                default:
                    break;
                }
            }
            break;
            case AT_FOUND_CMD_PARAM:
            {
                if(handle->param_cnt == 0)
                {
                    handle->param_str[handle->param_cnt++] = parser_cmd;
                }

                if(*parser_cmd == ',')
                {
                    mask_cnt++;
                    if(mask_cnt + 1 > handle->max_param)
                    {
                        handle->param_cnt = 0;
                        parser_flag = 0;
                        return -1;
                    }

                    *parser_cmd = '\0';
                    handle->param_str[handle->param_cnt++] = (parser_cmd + 1);
                }

                if((*parser_cmd == '\r') || (*parser_cmd == '\n'))
                { 
                    *parser_cmd = '\0';
                    parser_flag = 0;
                    return 0;
                }
            }
            break;
        
        default:
            break;
        }

        parser_cmd++;
        parser_len--;    
    }

    return 0;
}

/**
 * @brief Convert hex character to integer
 * 
 * @param c hex character
 * @return int16_t integer
 * @retval -1: failed
 * @retval 0-15: success
 * @note 0-9: 0-9, A-F: 10-15
 * @note a-f: 10-15, A-F: 10-15
 * @note example: '0' -> 0, '9' -> 9, 'A' -> 10, 'F' -> 15, 'a' -> 10, 'f' -> 15
 * @note other: -1
 * 
 */
int hex_char_to_int(char c) 
{
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return -1;
}

/**
 * @brief Convert hex string to bytes
 * 
 * @param hex_str hex string
 * @param bytes bytes
 * @return int16_t bytes length
 * @retval -1: failed
 * @retval 0-255: success
 * @note example: "01020304" -> {0x01, 0x02, 0x03, 0x04}
 */

int16_t hex_string_to_bytes(const char *hex_str, uint8_t *bytes) 
{
    size_t hex_len = strlen(hex_str);
    if (hex_len % 2 != 0)
    {
        return -1;
    }

    for (size_t i = 1; i < (hex_len - 1); i += 2) 
    {
        int high = hex_char_to_int(hex_str[i]);
        int low = hex_char_to_int(hex_str[i + 1]);
        if (high == -1 || low == -1)
        {
            return -1;
        }
        bytes[i / 2] = (high << 4) | low;
    }
    return hex_len / 2;
}

#define IS_ENABLE_PRARAM(x) (((x) == 0) || ((x) == 1))

// enable
// AT+AUTORECVCFG=<index_id>,<is_enable>,<c_field>,<ci_field>,<payload>

//disable
// AT+AUTORECVCFG=<index_id>,<is_enable>

//query
// AT+AUTORECVCFG?
// AT+AUTORECVCFG=<index_id>
// +AUTORECVCFG:<index_id>,<is_enable>,<c_field>,<ci_field>,<payload>
static void ql_cmd_exec_wmbus_auto_recv_config(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    ql_at_cmd_paraer_handle_t at_cmd_handle = { 0 };
    uint8_t *ptr_param[5] = { 0 };
    at_cmd_handle.param_str = &ptr_param;
    at_cmd_handle.max_param = 5;

    int16_t  ret = ql_at_cmd_paraser_parameter_handle(cmd_handle_table[7].str, &at_cmd_handle, cmd, cmd_len);
    if(!ret)
    {
        switch (at_cmd_handle.operate_type)
        {
            case AT_CMD_OP_QUERY:
            {
                ql_printf("QUERY WMBUS AUTO RECV CONFIG\r\n");
                uint32_t node_cnt = ql_wmbus_get_set_auto_recv_node_cnt(ql_wmbus_get_auto_recv_dev_storage());
                ql_wmbus_auto_recv_data_info_t info = { 0 };
                ql_wmbus_auto_recv_config_t cfg = { 0 };
                uint8_t out_buf[512] = { 0 };
                for(uint32_t i = 1; i < (node_cnt + 1); i++)
                {
                    int16_t  ret = ql_wmbus_auto_recv_get_single_by_index(i, &info, &cfg);
                    sprintf(out_buf, "+AUTORECVCFG:%d,%d,%02X,%02X,\"", i, cfg.is_enable, info.c_field, info.ci_field);
                    for (size_t i = 0; i < info.payload_len; i++)
                    {
                        sprintf(out_buf + strlen(out_buf), "%02X", info.payload[i]);
                    }

                    sprintf(out_buf + strlen(out_buf), "\"\r\n");
                    ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));
                }
            }
            break;
            case AT_CMD_OP_CFG:
            {
                if(at_cmd_handle.param_cnt == 1)
                {
                    uint32_t index = atoi(at_cmd_handle.param_str[0]);
                    uint8_t out_buf[512] = { 0 };

                    ql_wmbus_auto_recv_data_info_t info = { 0 };
                    ql_wmbus_auto_recv_config_t cfg = { 0 };

                    int16_t  ret = ql_wmbus_auto_recv_get_single_by_index(index, &info, &cfg);
                    if(ret != 0)
                    {
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }
                    
                    sprintf(out_buf, "+AUTORECVCFG:%d,%d,%02X,%02X,\"", index, cfg.is_enable, info.c_field, info.ci_field);
                    for (size_t i = 0; i < info.payload_len; i++)
                    {
                        sprintf(out_buf + strlen(out_buf), "%02X", info.payload[i]);
                    }

                    sprintf(out_buf + strlen(out_buf), "\"\r\n");
                    ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));
                }

                if(at_cmd_handle.param_cnt == 2)
                {
                    uint32_t index = atoi(at_cmd_handle.param_str[0]);
                    uint32_t enable = atoi(at_cmd_handle.param_str[1]);

                    if(!IS_ENABLE_PRARAM(enable))
                    {
                        ql_printf("ENABLE PARAM ERROR\r\n");
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }

                    int16_t ret = ql_wmbus_auto_recv_set_is_enable_by_index(index, enable);
                    if(ret != 0)
                    {
                        ql_printf("SET WMBUS AUTO RECV IS ENABLE BY INDEX FAILED\r\n");
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }
                }

                if(at_cmd_handle.param_cnt == 5)
                {
                    uint32_t index = atoi(at_cmd_handle.param_str[0]);
                    uint32_t enable = atoi(at_cmd_handle.param_str[1]);
                    uint8_t c_field = (uint8_t)strtol(at_cmd_handle.param_str[2], NULL, 16);
                    uint8_t ci_field = (uint8_t)strtol(at_cmd_handle.param_str[3], NULL, 16);

                    uint32_t str_hex_len = strlen(at_cmd_handle.param_str[4]);
                    uint32_t hex_len = (str_hex_len / 2) - 1;

                    uint8_t *hex_buf = (uint8_t *)memory_pool_alloc(handle_seq->pool, hex_len * sizeof(uint8_t));
                    if (hex_buf == NULL) 
                    {
                        printf("MEMORY POOL ALLOCATION FAILED\n");
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }

                    if (hex_string_to_bytes(at_cmd_handle.param_str[4], hex_buf) == -1) 
                    {
                        ql_printf("HEX STRING CONVERSION ERROR\n");
                        free(hex_buf);
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }

                    ql_wmbus_dev_info_t dev_wmbus_info = { 0 };
                    int16_t ret = query_dev_wmbus_node(ql_wmbus_get_dev_storage(), index, &dev_wmbus_info);
                    if(ret != 0)
                    {
                        ql_printf("QUERY DEV WMBUS NODE NOT FOUND\r\n");
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }

                    ql_wmbus_auto_recv_dev_all_info_t all_info = 
                    {
                        .index_id = index,
                        .cfg = {
                            .is_enable = enable, 
                            .interval_time = 0
                            },

                        .data = { 
                            .length = hex_len + 2,
                            .c_field = c_field,
                            .ci_field = ci_field,
                            .payload = hex_buf,
                            .payload_len = hex_len,
                            },

                        .dev_info = {
                            .manufacturer = dev_wmbus_info.manufacturer,
                            .id = dev_wmbus_info.id,
                            .version = dev_wmbus_info.version,
                            .device_type = dev_wmbus_info.device_type
                        }
                    };

                    ret = add_new_wmbus_auto_recv_node_tail(ql_wmbus_get_auto_recv_dev_storage(), &all_info);
                    if(ret != 0)
                    {
                        ql_printf("ADD NEW WMBUS AUTO RECV NODE TAIL FAILED\r\n");
                        handle_seq->is_quectel_cmd = 1;
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                        return;
                    }
                    memory_pool_reset(handle_seq->pool);
                }
            }
            break;
        }
        handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
    }
    else 
    {
        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
    }
    
    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
}

static void ql_cmd_exec_subver(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    ql_output_version_info(QL_OUTPUT_VERSION_SUB_VER);

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK; 
}

static void ql_cmd_exec_tune(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    uint8_t config_info[64];
    memcpy(config_info, (uint8_t *)QL_MODULE_CONFIG_BASE_ADDR, 64);
    ql_module_config_t *module_config = &config_info;

    uint8_t out_buf[256] = { 0 };
    sprintf(out_buf, "+QCTUNE: %d\r\n", ql_wmbus_get_tune_value());
    ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK; 
}

static void ql_cmd_exec_egmr(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    uint8_t config_info[64];
    uint8_t cmd_type = cmd[8];
    memcpy(config_info, (uint8_t *)QL_MODULE_CONFIG_BASE_ADDR, 64);
    ql_module_config_t *module_config = &config_info;
    
    switch (cmd[7])
    {
        case 0x3D:
        {
            switch (atoi(&cmd_type))
            {
                case 0x00:
                {
                    uint8_t out_buf[256] = { 0 };
                    if(module_config->head == 0xA5A5A5A5)
                    {
                        if((10 <= module_config->sn_len) && (module_config->sn_len <= 20))
                        {
                            sprintf(out_buf, "+EGMR: \"%s\"\r\n", module_config->sn);
                        }
                    }
                    ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));
                }
                break;
                case 0x01:
                {
                    uint8_t buf[] = { 0x01, 0x02, 0x03 ,0x04};
                    flash_writeBuffer(QL_MODULE_CONFIG_BASE_ADDR, buf, 4);
                    ql_printf("ok\r\n");
                }
                break;
            
            default:
                break;
            }
        }
        break;
    
    default:
        break;
    }

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
}

static void ql_cmd_exec_ftm(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    switch (cmd[7])
    {
        case 0x3D:
        {
            (atoi(&cmd[8]) == 0) ? (handle_seq->is_quectel_mode &= (~QL_CMD_HANDLE_ENTRY_FT_MODE)) : 
                            (handle_seq->is_quectel_mode |= QL_CMD_HANDLE_ENTRY_FT_MODE);
        }
        break;
        case 0x3F:
        {
            uint8_t out_buf[256] = { 0 };
            sprintf(out_buf, "+QFTM: %d\r\n", (handle_seq->is_quectel_mode & QL_CMD_HANDLE_ENTRY_FT_MODE) > 0 ? 1 : 0);
            ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));
        }
        break;
    default:
        break;
    }

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
}

static void ql_cmd_exec_prod(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    switch (cmd[8])
    {
        case 0x3D:
        {
            (atoi(&cmd[9]) == 0) ? (handle_seq->is_quectel_mode &= (~QL_CMD_HANDLE_ENTRY_DL_MODE)) : 
                            (handle_seq->is_quectel_mode |= QL_CMD_HANDLE_ENTRY_DL_MODE);
        }
        break;
        case 0x3F:
        {
            uint8_t out_buf[256] = { 0 };
            sprintf(out_buf, "+QPROD: %d\r\n", handle_seq->is_quectel_mode & QL_CMD_HANDLE_ENTRY_DL_MODE);
            ql_uart_write(QL_UART_UART1_PORT, out_buf, strlen(out_buf));
        }
        break;
    default:
        break;
    }

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
}

static void ql_cmd_exec_gmr(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    ql_output_version_info(QL_OUTPUT_VERSION_SHORT);

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
}

static void ql_cmd_exec_version(ql_cmd_handle_sequence_t *handle_seq, uint8_t *cmd, uint32_t cmd_len)
{
    ql_output_version_info(QL_OUTPUT_VERSION_COMPLETE);

    handle_seq->is_quectel_cmd = 1;
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
}

RAIL_Time_t mark_recv_start_time = 0;
int16_t cmd_handle_listening_data(ql_cmd_handle_sequence_t *handle_seq)
{
    if(handle_seq->cfg_status_id != E_CMD_CFG_STATE_NORMAL)
    {
        if(RAIL_GetTime() - handle_seq->uart_recv_time > UART_DATA_TIMEOUT)
        {
            handle_seq->status_id = E_CMD_STATE_TIMEOUT_CHANGE_MODE;
            return 0;
        }
    }
    else
    {
        handle_seq->uart_recv_time = 0;
    }

    if(ql_uart_available(QL_UART_UART1_PORT) > 0)
    {
        uint8_t buf[1024];
        uint32_t recv_len = 0;

        mark_recv_start_time = RAIL_GetTime();
        uint32_t ret = ql_uart_read(QL_UART_UART1_PORT, buf, sizeof(buf), &recv_len);
        if(ret == 0)
        {
            handle_seq->uart_recv_time = RAIL_GetTime();
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
            handle_seq->status_id = E_CMD_STATE_PARASER_RECV_DATA;
        }
    }
    return 0;
}

int16_t cmd_handle_paraser_recv_data(ql_cmd_handle_sequence_t *handle_seq)
{
    int32_t recv_len = (cmd_uart_buf->tail > cmd_uart_buf->front) ? 
        (cmd_uart_buf->tail - cmd_uart_buf->front) : ((UART_DATA_MAX_SZIE - cmd_uart_buf->front) + cmd_uart_buf->tail);

    if(recv_len < 0)
    {
        handle_seq->status_id = E_CMD_STATE_LISTENING_DATA;
        return -1;
    }

    memset(&handle_seq->parser_info, 0x00, sizeof(ql_cmd_handle_parase_t));

    if((cmd_uart_buf->front + recv_len) < UART_DATA_MAX_SZIE)
    {
        memcpy(&handle_seq->parser_info.org_data[0], (cmd_uart_buf->buf + cmd_uart_buf->front), recv_len);
        cmd_uart_buf->front += recv_len;
    }
    else
    {
         memcpy(&handle_seq->parser_info.org_data[0], (cmd_uart_buf->buf + cmd_uart_buf->front), 
                (UART_DATA_MAX_SZIE - cmd_uart_buf->front));

         memcpy(&handle_seq->parser_info.org_data[UART_DATA_MAX_SZIE - cmd_uart_buf->front], cmd_uart_buf->buf, 
                (recv_len - (UART_DATA_MAX_SZIE - cmd_uart_buf->front)));

        cmd_uart_buf->front = cmd_uart_buf->tail;
    }

    handle_seq->parser_info.data_len = recv_len;
    switch (handle_seq->cfg_status_id)
    {
        case E_CMD_CFG_STATE_NORMAL:
        {
            if ((strncmp(&handle_seq->parser_info.org_data[0], "\xFF", 1) == 0) && (recv_len == 1))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT", 2) == 0) && (recv_len == 2))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_AT_CONFIG;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATJ", 3) == 0) && (recv_len == 3))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_ATJ_CONFIG1;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT1", 3) == 0) && (recv_len == 3))
            {
                ql_printf("TESTING CARRIER\r\n");
                ql_wmbus_rf_channel_testing_carrier_singal();
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT2", 3) == 0) && (recv_len == 3))
            {
                ql_printf("TESTING STREAM\r\n");
                ql_wmbus_rf_channel_testing_stream_singal();
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT3", 3) == 0) && (recv_len == 3))
            {
                ql_wmbus_rf_channel_get_rssi();
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;  
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT4", 3) == 0) && (recv_len == 3))
            {
                ql_printf("STOP TESTING CARRIER\r\n");
                RAIL_StopTxStream(ql_wmbus_rf_rail_get_handle());
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;  
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATr", 3) == 0) && (recv_len == 3))
            {
                ql_uart_write(QL_UART_UART1_PORT, "OK", 2);
                reset_resetWithReason(BOOTLOADER_RESET_SIGNATURE_VALID);
            }
            else
            {
                uint8_t reset_cmd[] = {0x02, 0x00, 0x03, 0xB0, 0x2F, 0x59};
                if((memcmp(reset_cmd, &handle_seq->parser_info.org_data[0], sizeof(reset_cmd)) == 0) && (recv_len == sizeof(reset_cmd)))
                {
                    ql_printf("[APP]RECV UPGRADE COMMAND, MCU REBOOT\r\n");
                    reset_resetWithReason(BOOTLOADER_RESET_REASON_UPGRADE);
                }
                /* 检查是否是wmbus协议帧 */
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;

                if(handle_seq->parser_info.org_data[0] == (handle_seq->parser_info.data_len - 1))
                {
                    //ql_printf("WMBUS-AIR-SEND\r\n");
                    memcpy(&g_wmbus_uart_recv_data_info.wmbus_uart_recv_data[0], &handle_seq->parser_info.org_data[0], handle_seq->parser_info.data_len);
                    g_wmbus_uart_recv_data_info.wmbus_uart_recv_len = handle_seq->parser_info.data_len;
                    extern void set_next_state(state_t next_state);
                    set_next_state(S_SCHEDULE_TX);
                }
                else
                {
                    /* 如果检查失败 返回 E_CMD_RECV_STA_ERROR */  
                    if(ql_cmd_hanlde(handle_seq, &handle_seq->parser_info.org_data[0], handle_seq->parser_info.data_len) != 0)
                    {
                        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;  
                    }
                }
            }
            
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
        break;
        case E_CMD_CFG_STATE_AT_CONFIG:
        {
            handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            if ((strncmp(&handle_seq->parser_info.org_data[0], "ATQ", 3) == 0) && (recv_len == 3))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "AT0", 3) == 0) && (recv_len == 3))
            {
                handle_seq->parser_info.cmd_type = E_CMD_TYPE_AT0;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                handle_seq->status_id = E_CMD_STATE_CONFIG_REGISTER;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATL", 3) == 0) && (recv_len == 4))
            {
                ql_wmbus_dev_info_t dev_info = { 0 };
                int16_t ret = query_dev_wmbus_node(ql_wmbus_get_dev_storage(), handle_seq->parser_info.org_data[3], &dev_info);
                if(ret != 0)
                {
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                {
                    uint8_t buf[8] = { 0 };
                    uint32_t buf_len = 0;
                    
                    memcpy(buf + buf_len, &dev_info.manufacturer, 2);
                    buf_len += 2;

                    memcpy(buf + buf_len, &dev_info.id, 4);
                    buf_len += 4;

                    memcpy(buf + buf_len, &dev_info.version, 1);
                    buf_len += 1;

                    memcpy(buf + buf_len, &dev_info.device_type, 1);
                    buf_len += 1;

                    ql_printf("DEV INFO:INDEX_ID:%d DEV_TYPE:%02X, ID:%08X, MANUF:%04X, VER:%02X\r\n",
                        dev_info.index_id, dev_info.device_type, dev_info.id, dev_info.manufacturer, dev_info.version  
                    );

                    ql_uart_write(QL_UART_UART1_PORT, buf, buf_len);
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }

                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATB", 3) == 0) && (recv_len == 12))
            {
                uint8_t index_id = handle_seq->parser_info.org_data[3];

                ql_wmbus_dev_info_t dev = 
                {
                    .index_id = index_id,
                    .manufacturer = *((uint16_t *)(uint8_t *)&handle_seq->parser_info.org_data[4]),
                    .id = *((uint32_t *)(uint8_t *)&handle_seq->parser_info.org_data[6]),
                    .version = *((uint32_t *)(uint8_t *)&handle_seq->parser_info.org_data[10]),
                    .device_type = handle_seq->parser_info.org_data[11]
                };

                ql_printf("BIND DEV INFO:INDEX_ID:%d DEV_TYPE:%02X, ID:%08X, MANUF:%04X, VER:%02X\r\n",
                    index_id, dev.device_type, dev.id, dev.manufacturer, dev.version  
                );

                int16_t ret = add_dev_wmbus_node_tail(ql_wmbus_get_dev_storage(), dev);
                if(ret != 0)
                {
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                {
                    ql_wmbus_bind_dev_save_to_flash();
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }
                
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATK", 3) == 0) && (recv_len == 20))
            {
                uint8_t index_id = handle_seq->parser_info.org_data[3];
                ql_wmbus_encryption_info_t encryption = { 0 };
                encryption.node_index = index_id;
                memcpy(&encryption.enc_key, &handle_seq->parser_info.org_data[4], sizeof(encryption.enc_key));   

                int16_t ret = add_dev_wmbus_encryption_node_tail(ql_wmbus_get_encryption_storage(), encryption);
                ql_printf("[ENCRY INFO]INDEX:%d, RET:%d\r\n", encryption.node_index, ret);
                if(ret != 0)
                {
                    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                {
                    ql_wmbus_bind_dev_save_to_flash();
                    ql_wmbus_bind_dev_encryption_save_to_flash();
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }

                if(ql_wmbus_register_change_event != NULL)
                {
                    ql_wmbus_register_change_event(0xFF, encryption.enc_key);
                }
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATA", 3) == 0) && (recv_len == 6))
            {
                uint8_t index_id = handle_seq->parser_info.org_data[3];

                uint16_t method_encryption = (uint16_t)((handle_seq->parser_info.org_data[5] << 8) | handle_seq->parser_info.org_data[4]);

                int16_t ret = add_dev_wmbus_encryption_is_encryption_flag(index_id, method_encryption);

                if(ret != 0)
                {
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                {
                    ql_wmbus_bind_dev_encryption_save_to_flash();
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATO", 3) == 0) && (recv_len == 4))
            {
                uint8_t index_id = handle_seq->parser_info.org_data[3];
                uint16_t encry_flag = 0;

                int16_t ret = query_encryption_flag_by_index(index_id, &encry_flag);
                if(ret == -1)
                {
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                {
                    ql_uart_write(QL_UART_UART1_PORT, (uint8_t *)&encry_flag, sizeof(encry_flag));
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }

                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATr", 3) == 0) && (recv_len == 3))
            {
                ql_uart_write(QL_UART_UART1_PORT, "OK", 2);
                reset_resetWithReason(BOOTLOADER_RESET_SIGNATURE_VALID);
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATY", 3) == 0) && (recv_len > 3))
            {
                ql_printf("[APP]QUERY REGISTER ID:%02X\r\n", handle_seq->parser_info.org_data[3]);
                handle_seq->parser_info.reg_type = handle_seq->parser_info.org_data[3];
                handle_seq->parser_info.cmd_type = E_CMD_TYPE_ATY;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                handle_seq->status_id = E_CMD_STATE_CONFIG_REGISTER;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATW", 3) == 0) && (recv_len > 3))
            {
                uint8_t len = handle_seq->parser_info.org_data[3];
                uint8_t index = handle_seq->parser_info.org_data[4];
            }
            else
            {
                handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
        }
        break;
        case E_CMD_CFG_STATE_ATJ_CONFIG1:
        {
            if ((strncmp(&handle_seq->parser_info.org_data[0], "ATJ", 3) == 0) && (recv_len >= 5))
            {
                uint16_t param_count = recv_len - 4;
                ql_printf("[APP]WRITE REGISTER ID:%02X, PARAM LENGTH:%d, PARAM CONTENT:", 
                        handle_seq->parser_info.org_data[3], param_count);
                for (size_t i = 4; i < recv_len; i++)
                {
                    ql_printf("%02X", handle_seq->parser_info.org_data[i]); 
                }
                ql_printf("\r\n");
                
                handle_seq->parser_info.param_cnt = param_count;
                handle_seq->parser_info.reg_type = handle_seq->parser_info.org_data[3];
                handle_seq->parser_info.cmd_type = E_CMD_TYPE_ATJ;

                handle_seq->status_id = E_CMD_STATE_CONFIG_REGISTER;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATJX", 4) == 0) && (recv_len == 4))
            {
                /* 保存寄存器的值 */
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_ATJ_CONFIG2;

                int16_t ret = ql_wmbus_param_cfg_check_and_save();
                if(ret != 0)
                {
                    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                }
                else
                { 
                    handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                }

                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATQ", 3) == 0) && (recv_len == 3))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
            else
            {
                handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
                handle_seq->status_id = E_CMD_STATE_RECV_ACK;
            }
        }
        break;
        case E_CMD_CFG_STATE_ATJ_CONFIG2:
        {
            if ((strncmp(&handle_seq->parser_info.org_data[0], "ATJ", 3) == 0) && (recv_len == 3))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_ATJ_CONFIG1;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else if ((strncmp(&handle_seq->parser_info.org_data[0], "ATQ", 3) == 0) && (recv_len == 3))
            {
                handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
                handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            }
            else
            {
                handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
            }

            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
        break;
    default:
        break;
    }

    return 0;
}

void ql_wmbus_register_change_event_register(wmbus_register_event event)
{
    ql_wmbus_register_change_event = event;
}

int16_t cmd_handle_config_register(ql_cmd_handle_sequence_t *handle_seq)
{
    handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
    if(handle_seq->parser_info.cmd_type == E_CMD_TYPE_ATJ)
    {
        /* 设置 寄存器表 */
        if(ql_wmbus_param_single_check_field(handle_seq->parser_info.reg_type,
            &handle_seq->parser_info.org_data[4],
            handle_seq->parser_info.param_cnt) == 0)
        {
            int16_t ret = ql_wmbus_param_write_single(handle_seq->parser_info.reg_type, 
                            &handle_seq->parser_info.org_data[4], 
                            handle_seq->parser_info.param_cnt);

            if(ql_wmbus_register_change_event != NULL)
            {
                ql_wmbus_register_change_event(handle_seq->parser_info.reg_type, &handle_seq->parser_info.org_data[4]);
            }

            handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
        else
        {
            handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
    }
    else if (handle_seq->parser_info.cmd_type == E_CMD_TYPE_AT0)
    {
        ql_uart_write(QL_UART_UART1_PORT,  ql_wmbus_param_current_datas(), ql_wmbus_param_current_datas_len());
        handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
        handle_seq->status_id = E_CMD_STATE_RECV_ACK;
    }
    else if (handle_seq->parser_info.cmd_type == E_CMD_TYPE_ATY)
    {
        uint8_t read_buff[10];
        uint16_t data_len = 0;

        int16_t ret = ql_wmbus_param_read_single(handle_seq->parser_info.reg_type, read_buff, &data_len);
        if(ret == 0)
        {
            ql_uart_write(QL_UART_UART1_PORT, read_buff, data_len);
            handle_seq->is_ok_err = E_CMD_RECV_STA_OK;
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
        else
        {
            handle_seq->parser_info.cmd_type = E_CMD_TYPE_UNKNOW;
            handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
            handle_seq->status_id = E_CMD_STATE_RECV_ACK;
        }
    }
    else
    {
        handle_seq->is_ok_err = E_CMD_RECV_STA_ERROR;
        handle_seq->status_id = E_CMD_STATE_RECV_ACK;
    }

    handle_seq->status_id = E_CMD_STATE_RECV_ACK;
  
    return 0;
}

int16_t cmd_handle_timeout_change_mode(ql_cmd_handle_sequence_t *handle_seq)
{
    if(handle_seq->cfg_status_id != E_CMD_CFG_STATE_NORMAL)
    {
        ql_printf("[APP]UART RECV TIMEOUT\r\n");
        handle_seq->uart_recv_time = 0;
        memset(&handle_seq->parser_info, 0x00, sizeof(ql_cmd_handle_parase_t));
        handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
        handle_seq->status_id = E_CMD_STATE_LISTENING_DATA;
    }
    return 0;
}

int16_t cmd_handle_recv_ack(ql_cmd_handle_sequence_t *handle_seq)
{

    if(handle_seq->cfg_status_id == E_CMD_TYPE_FRAME)
    {
        /* 向串口发送wmbus协议帧 */
    }
    else
    {
        if(handle_seq->is_ok_err == E_CMD_RECV_STA_OK) 
        {
            if(handle_seq->is_quectel_cmd == 1)
            {
                ql_uart_write(QL_UART_UART1_PORT, "OK\r\n", 4);
            }
            else
            {
                ql_uart_write(QL_UART_UART1_PORT, "OK", 2);
            }
        }
        else
        {
            if(handle_seq->is_quectel_cmd == 1)
            {
                
                ql_uart_write(QL_UART_UART1_PORT, "ERROR\r\n", 7);
            }
            else
            {
                ql_uart_write(QL_UART_UART1_PORT, "ERROR", 5);
            }
        }
    }
    handle_seq->is_quectel_cmd = 0;
    handle_seq->status_id = E_CMD_STATE_LISTENING_DATA;
    return 0;
}

ql_cmd_handle_task_seq cmd_handle[] = 
{
    {E_CMD_STATE_LISTENING_DATA, cmd_handle_listening_data},
    {E_CMD_STATE_PARASER_RECV_DATA, cmd_handle_paraser_recv_data},
    {E_CMD_STATE_CONFIG_REGISTER, cmd_handle_config_register},
    {E_CMD_STATE_TIMEOUT_CHANGE_MODE, cmd_handle_timeout_change_mode},
    {E_CMD_STATE_RECV_ACK, cmd_handle_recv_ack}
};

#define CMD_HANDLE_SIZE (sizeof(cmd_handle) / sizeof(ql_cmd_handle_task_seq))

void ql_cmd_handle_init(ql_cmd_handle_sequence_t *handle_seq)
{
    memory_pool_init(&handle_seq->pool);
    cmd_uart_buf->front = 0;
    cmd_uart_buf->tail = 0;
    cmd_uart_buf->buf = &uart_buf[0];
    handle_seq->cfg_status_id = E_CMD_CFG_STATE_NORMAL;
    handle_seq->status_id = E_CMD_STATE_LISTENING_DATA;
}

void ql_cmd_handle_start(ql_cmd_handle_sequence_t *handle_seq)
{
    for (size_t i = 0; i < CMD_HANDLE_SIZE; i++)
    {
        if(cmd_handle[i].cmd_state == handle_seq->status_id)
        {
            cmd_handle[i].cmd_seq(handle_seq);
        }
    }
}

void ql_cmd_handle_loop(void)
{
    if(g_cmd_handle_seq.is_first_run == 0)
    {
        g_cmd_handle_seq.is_first_run = 1;
        ql_cmd_handle_init(&g_cmd_handle_seq);
    }
    ql_cmd_handle_start(&g_cmd_handle_seq);
}
