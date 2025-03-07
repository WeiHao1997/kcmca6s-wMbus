#include "ql_param_cfg.h"

#define WMBUS_PARAM_MAGIC_WORD 0x55AA55AA
#define WMBUS_PARAM_VERSION 0x01

#define QL_NVM_SAVE_WMBUS_PARAM_KEY 0x01

uint8_t g_ql_wmbus_config_param_buff[BUFF_PARAM_SIZE] = { 0 };

const ql_wmbus_param_default_tables_t g_wmbus_param_tables[] =
{
    STR_ITEM_ADD(REG_TX_POWER            , dt_unsigned,     "TX_POWER",        "\x05", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_WMBUS_MODE          , dt_unsigned,     "WMBUS_MODE",      "\x02", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_CHANNEL_NUM         , dt_unsigned,     "CHANNEL_NUM",     "\x01", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_ROLE                , dt_unsigned,     "ROLE",            "\x01", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_SLEEP_MODE          , dt_unsigned,     "SLEEP_MODE",      "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_T_C_COMPATIBLE      , dt_unsigned,     "TC_COMPATIBLE",   "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_00          , dt_dont_care,    "NOT_USE_00",      "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_PREAMBLE_LEN        , dt_unsigned,     "PREAMBLE_LEN",    "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_C_FIELD         , dt_unsigned,     "DLL_C_FIELD",     "\x06", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_01          , dt_unsigned,     "NOT_USE_01",      "\x06", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RF_RECV_TIMEOUT     , dt_unsigned,     "RF_RECV_TIMEOUT", "\x0B", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_02          , dt_unsigned,     "NOT_USE_02",      "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_M1_FIELD        , dt_unsigned,     "DLL_M1_FIELD",    "\x0C", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_M2_FIELD        , dt_unsigned,     "DLL_M2_FIELD",    "\xAE", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_ID1_FIELD       , dt_unsigned,     "DLL_ID1_FIELD",   "\x12", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_ID2_FIELD       , dt_unsigned,     "DLL_ID2_FIELD",   "\x34", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_ID3_FIELD       , dt_unsigned,     "DLL_ID3_FIELD",   "\x56", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_ID4_FIELD       , dt_unsigned,     "DLL_ID4_FIELD",   "\x78", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_VER_FIELD       , dt_unsigned,     "DLL_VER_FIELD",   "\x01", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_DLL_REG_FIELD       , dt_unsigned,     "DLL_REG_FIELD",   "\x07", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_03          , dt_dont_care,    "NOT_USE_03",      "\xff\xff\xff\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_UART_BAUDRATE       , dt_unsigned,     "BAUD_RATE",       "\x05", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_FLOW_CONTROL        , dt_unsigned,     "FLOW_CONTROL",    "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_04          , dt_dont_care,    "NOT_USE_04",      "\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_UART_DATA_FORMAT    , dt_unsigned,     "DATA_INTERFACE",  "\x04", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_INSTALL_MODE        , dt_unsigned,     "INSTALL_MODE",    "\x02", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_USE_RSSI            , dt_unsigned,     "RSSI_MODE",       "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_LED_CONTROL         , dt_unsigned,     "LED_CONTROL",     "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_TEMP_CALI           , dt_integer,      "TEMP_CALI",       "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_FREQ_CALI           , dt_integer,      "FREQ_CALI",       "\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_SLEEP_DELAY_TIME    , dt_unsigned,     "TIMEOUT",         "\x7D", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_05          , dt_dont_care,    "NOT_USE_05",      "\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_MASTER_KEY          , dt_dont_care,    "MASTER_KEY",      "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_INITIAL_VECTOR      , dt_dont_care,    "INITIAL_VECTOR",  "\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_PRODUCT_NAME        , dt_dont_care,    "REG_PRODUCT_NAME","\x46\x43\x2d\x37\x30\x33\x43\xff\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_RESERVE_06          , dt_dont_care,    "NOT_USE_06",      "\xff\xff\xff\xff", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_SERIAL_NUM          , dt_dont_care,    "REG_SERIAL_NO",   "\x00\x00\x00\x00\x00\x00\x00\x00", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_HARD_VER            , dt_dont_care,    "REG_HARD_VER",    "\x31\x2e\x30\x33", WMBUS_PARAM_IS_SAVE),
    STR_ITEM_ADD(REG_SOFT_VER            , dt_dont_care,    "REG_SOFT_VE",     "\x33\x2e\x34\x32", WMBUS_PARAM_IS_SAVE),
};

#define ARRAY_SZIE_PRARM_DEF_TABS (sizeof(g_wmbus_param_tables) / sizeof(ql_wmbus_param_default_tables_t))

uint8_t *ql_wmbus_param_current_datas(void)
{
    return &g_ql_wmbus_config_param_buff[8];
}

uint32_t ql_wmbus_param_current_datas_len(void)
{
    return sizeof(g_ql_wmbus_config_param_buff) - 8;
}

static int16_t ql_wmbus_param_read_all_form_flash(void)
{
    uint32_t data_len = 0;
    int16_t ret = ql_nvm_read_data(QL_NVM_SAVE_WMBUS_PARAM_KEY, g_ql_wmbus_config_param_buff, &data_len);
    if(ret != 0) 
    {
        return -1;
    }
    return 0;
}

static int16_t ql_wmbus_param_write_all_form_flash(const void* buf, uint32_t buf_len)
{   
    int16_t ret = ql_nvm_write_data(QL_NVM_SAVE_WMBUS_PARAM_KEY, (uint8_t *)buf, buf_len);
    if(ret != 0) 
    {
        return -1;
    }
    return 0;
}

static const ql_wmbus_param_default_tables_t* ql_wmbus_param_tab_find_by_index(uint8_t index)
{
    if (index > ARRAY_SZIE_PRARM_DEF_TABS)
    {
        return NULL;
    }

    return &g_wmbus_param_tables[index];

    return NULL;
}

static const ql_wmbus_param_default_tables_t* ql_wmbus_param_tab_find_by_pid(uint8_t pid)
{
    if (pid > REG_PARAM_END)
    {
        return NULL;
    }

    for (size_t i = 0; i < ARRAY_SZIE_PRARM_DEF_TABS; i++)
    {
        if (g_wmbus_param_tables[i].pid == pid)
        {
            return &g_wmbus_param_tables[i];
        }
    }

    return NULL;
}

static int16_t ql_wmbus_param_head_check(const uint8_t* buf)
{
    const ql_wmbus_param_head_t* ph = (const ql_wmbus_param_head_t*)buf;

    if (ph->param_magic != WMBUS_PARAM_MAGIC_WORD)
    {
        return -1;
    }

    if (ph->param_version != WMBUS_PARAM_VERSION)
    {
        return -1;
    }

    if (ph->param_checksum != crc16_calculate(buf + sizeof(ql_wmbus_param_head_t), sizeof(ql_wmbus_param_array_t)))
    {
        return -1;
    }

    return 0;
}

static int16_t ql_wmbus_param_set_default(uint8_t index)
{
    uint8_t* buf = g_ql_wmbus_config_param_buff;
    const ql_wmbus_param_default_tables_t* ptab = ql_wmbus_param_tab_find_by_index(index);

    if (!ptab)
    {
        return -1;
    }

    if (memcmp(buf + ptab->offset, ptab->pinit, ptab->plen) != 0)
    {
        memset(buf + ptab->offset, 0xFF, ptab->maxlen);
        memcpy(buf + ptab->offset, ptab->pinit, ptab->plen);
    }

    return 0;
}

static int16_t ql_wmbus_param_check(uint8_t pid, const uint8_t* buf, uint16_t field_len)
{
    int16_t paralen = 0;
    const ql_wmbus_param_default_tables_t* ptab = NULL;

    if (!buf || !field_len)
    {
        return -1;
    }

    if ((ptab = ql_wmbus_param_tab_find_by_pid(pid)) != NULL)
    {
        paralen = ptab->plen;
    }
    else
    {
        return -1;
    }

    if (paralen > field_len)
    {
        return -1;
    }

    switch (pid)
    {
        case REG_TX_POWER:
        {
            if ((buf[0] < E_TX_POWER_FU5) || (buf[0] > E_TX_POWER_14))
            {
                paralen = -1;
            }
        }
        break;
        case REG_WMBUS_MODE:
        {
            if (((buf[0] > E_WMBUS_MODE_R) && (buf[0] < E_WMBUS_MODE_C1)) || (buf[0] > E_WMBUS_MODE_C2))                    
            {
                paralen = -1;
            } 
        }
        break;
        case REG_CHANNEL_NUM:
        {
            if (((buf[0] < 1) || (buf[0] > 10)))                    
            {
                paralen = -1;
            } 
        }
        break;
        case REG_ROLE:
        {
            if ((buf[0] != E_ROLE_SLAVE) && (buf[0] != E_ROLE_MASTER) && (buf[0] != E_ROLE_REPEATER))
            {
                paralen = -1;
            } 
        }
        break;
        case REG_SLEEP_MODE:
        {
            if ((buf[0] != E_SLEEP_MODE_DISABLE) && (buf[0] != E_SLEEP_MODE_ENABLE) && (buf[0] != E_SLEEP_MODE_DELAY))
            {
                paralen = -1;
            }
        } 
        break;
        case REG_T_C_COMPATIBLE:
        {
            if ((buf[0] > E_TC_COMPATIBLE_ENABLE))
            {
                paralen = -1;
            } 
        }
        break;
        case REG_PREAMBLE_LEN:
        {
            if ((buf[0] > E_PREAMBLE_LONG))
            {
                paralen = -1;
            } 
        }
        break;
        case REG_UART_BAUDRATE:
        {
            if ((buf[0] > E_BAUD_RATE_230400))
            {
                paralen = -1;
            }
        }
        break;
        case REG_FLOW_CONTROL:
        {
            if ((buf[0] != E_FLOW_CONTROL_DISABLE) && (buf[0] != E_FLOW_CONTROL_CTS) && (buf[0] != E_FLOW_CONTROL_CTS_RTS))
            {
                paralen = -1;
            }
        }
        break;
        case REG_UART_DATA_FORMAT:
        {if ((buf[0] != E_DATA_FORMAT_DATA_ID) && (buf[0] != E_DATA_FORMAT_APP) && (buf[0] != E_DATA_FORMAT_START_END)
            && (buf[0] != E_DATA_FORMAT_CRC) && (buf[0] != E_DATA_FORMAT_START_END_CRC))
            {
                paralen = -1;
            } 
        }
        break;
        case REG_INSTALL_MODE:
        {
            if ((buf[0] > E_INSTALL_MODE_LISTEN))
            {
                paralen = -1;
            }
        }
        break;
        case REG_USE_RSSI:
        {
            if ((buf[0] > E_RSSI_MODE_ENABLE))
            {
                paralen = -1;
            } 
        }
        break;
        case REG_LED_CONTROL:
        {
            if ((buf[0] > E_LED_CONTROL_RF_UART))
            {
                paralen = -1;
            } 
        }
        break;
    default:
        break;
    }

    if (-1 == paralen)
    {
        printf("[PARAM CONFIG]PARAMETER CHECK ERROR[%d] ERROR VALUE:%02X\n", pid, buf[0]);
    }

    return paralen;
}

static void ql_wmbus_param_check_field_format(void)
{
    const ql_wmbus_param_default_tables_t* ptab = NULL;
    uint8_t* buf = g_ql_wmbus_config_param_buff;

    for (uint16_t i = 0; i < (ARRAY_SZIE_PRARM_DEF_TABS); i++)
    {
        ptab = &g_wmbus_param_tables[i];
        if (ql_wmbus_param_check(ptab->pid, &buf[ptab->offset], ptab->plen) < 0)
        {
            ql_wmbus_param_set_default(ptab->pid);
        }
    }
}

int16_t ql_wmbus_param_read_single(uint8_t pid, void* buf, uint16_t *len)
{
    const ql_wmbus_param_default_tables_t* p = ql_wmbus_param_tab_find_by_pid(pid);

    if (!p || !buf || !len)
    {
        return -1;
    }

    uint8_t* pi = &g_ql_wmbus_config_param_buff[0];

    do
    {
        uint16_t offset = p->offset;
        uint16_t data_len = p->plen;
        (void*)memcpy(buf, pi + offset, data_len);
        *len = data_len;
    } while (0);
    
    return 0;
}

int16_t ql_wmbus_param_write_single(uint8_t pid, const void* buf, uint16_t len)
{
    const ql_wmbus_param_default_tables_t* p = ql_wmbus_param_tab_find_by_pid(pid);

    if (!p || !buf || !len)
    {
        return -1;
    }

    //just not support write id
    if ((pid == REG_PRODUCT_NAME) || (pid == REG_HARD_VER) || (pid == REG_SOFT_VER))
    {
        return -1;
    }
    uint8_t* pi = &g_ql_wmbus_config_param_buff[0];

    do
    {
        uint32_t t_len = 0, i = 0;

        while ((i++ < p->maxlen) && (t_len < p->maxlen))
        {
            if (*(pi + p->offset + i) != 0xFF) t_len++;
        }

        if ((memcmp(pi + p->offset, buf, len) == 0) && (t_len == len))
        {
            return 0;
        }
        
        if (ql_wmbus_param_check(pid, (const uint8_t *)buf, len) < 0)
        {
            return -1;
        }

        memset(pi + p->offset, 0xFF, p->maxlen);
        memmove(pi + p->offset, buf, len);

    } while (0);

    return 0;
}

int16_t ql_wmbus_param_single_check_field(uint8_t pid, uint8_t *buf, uint16_t data_len)
{
    uint16_t max_feild = 0;
    const ql_wmbus_param_default_tables_t* ptab = NULL;

    if (!buf || !data_len)
    {
        return -1;
    }

    if ((ptab = ql_wmbus_param_tab_find_by_pid(pid)) != NULL)
    {
        max_feild = ptab->maxlen;
    }
    else
    {
        return -1;
    }

    if(data_len > max_feild)
    {
        return -1;
    }

    switch (pid)
    {
        case REG_TX_POWER:
        {
            if ((buf[0] < E_TX_POWER_FU5) || (buf[0] > E_TX_POWER_14))
            {
                return -1;
            }
        }
        break;
        case REG_WMBUS_MODE:
        {
            if (((buf[0] > E_WMBUS_MODE_R) && (buf[0] < E_WMBUS_MODE_C1)) || (buf[0] > E_WMBUS_MODE_C2))                    
            {
                return -1;
            } 
        }
        break;
        case REG_CHANNEL_NUM:
        {
            if (((buf[0] < 1) || (buf[0] > 10)))                    
            {
                return -1;
            } 
        }
        break;
        case REG_ROLE:
        {
            if ((buf[0] != E_ROLE_SLAVE) && (buf[0] != E_ROLE_MASTER) && (buf[0] != E_ROLE_REPEATER))
            {
                return -1;
            } 
        }
        break;
        case REG_SLEEP_MODE:
        {
            if ((buf[0] != E_SLEEP_MODE_DISABLE) && (buf[0] != E_SLEEP_MODE_ENABLE) && (buf[0] != E_SLEEP_MODE_DELAY))
            {
                return -1;
            }
        } 
        break;
        case REG_T_C_COMPATIBLE:
        {
            if ((buf[0] > E_TC_COMPATIBLE_ENABLE))
            {
                return -1;
            } 
        }
        break;
        case REG_PREAMBLE_LEN:
        {
            if ((buf[0] > E_PREAMBLE_LONG))
            {
                return -1;
            } 
        }
        break;
        case REG_UART_BAUDRATE:
        {
            if ((buf[0] > E_BAUD_RATE_230400))
            {
                return -1;
            }
        }
        break;
        case REG_FLOW_CONTROL:
        {
            if ((buf[0] != E_FLOW_CONTROL_DISABLE) && (buf[0] != E_FLOW_CONTROL_CTS) && (buf[0] != E_FLOW_CONTROL_CTS_RTS))
            {
                return -1;
            }
        }
        break;
        case REG_UART_DATA_FORMAT:
        {if ((buf[0] != E_DATA_FORMAT_DATA_ID) && (buf[0] != E_DATA_FORMAT_APP) && (buf[0] != E_DATA_FORMAT_START_END)
            && (buf[0] != E_DATA_FORMAT_CRC) && (buf[0] != E_DATA_FORMAT_START_END_CRC))
            {
                return -1;
            } 
        }
        break;
        case REG_INSTALL_MODE:
        {
            if ((buf[0] > E_INSTALL_MODE_LISTEN))
            {
                return -1;
            }
        }
        break;
        case REG_USE_RSSI:
        {
            if ((buf[0] > E_RSSI_MODE_ENABLE))
            {
                return -1;
            } 
        }
        break;
        case REG_LED_CONTROL:
        {
            if ((buf[0] > E_LED_CONTROL_RF_UART))
            {
                return -1;
            } 
        }
        break;
    default:
        break;
    }

    return 0;

}

static int16_t ql_wmbus_param_buf_check(void)
{
    const uint8_t* buf = g_ql_wmbus_config_param_buff;
    if (0 != ql_wmbus_param_head_check(buf))
    {
        ql_printf("[PARAM CONFGI]WMBUS CHECK PARAM HEAD FAIL!\r\n");
        return -1;
    }
    return 0;
}

int16_t ql_param_cfg_init(void)
{
    memset(g_ql_wmbus_config_param_buff, 0xff, BUFF_PARAM_SIZE);

    if ((ql_wmbus_param_read_all_form_flash() != 0) || (ql_wmbus_param_buf_check() != 0))
    {
        ql_printf("[APP]FLASH READ ERROR, PARAM SET DEFAULT! TABLES SIZE:%d\r\n", ARRAY_SZIE_PRARM_DEF_TABS);
        for (uint32_t i = 0; i < ARRAY_SZIE_PRARM_DEF_TABS; i++)
        {
            ql_wmbus_param_set_default(i);
        }

        ql_wmbus_param_head_t* param_head = (ql_wmbus_param_head_t *)&(g_ql_wmbus_config_param_buff[0]);
        param_head->param_magic = WMBUS_PARAM_MAGIC_WORD;
        param_head->param_version = WMBUS_PARAM_VERSION;
        param_head->param_checksum = crc16_calculate((uint8_t*)g_ql_wmbus_config_param_buff + sizeof(ql_wmbus_param_head_t), sizeof(ql_wmbus_param_array_t));

        ql_printf("[PARAM CONFIG]PARAM CHECKSUM:%04X\r\n", param_head->param_checksum);
        int16_t ret = ql_wmbus_param_write_all_form_flash(g_ql_wmbus_config_param_buff, BUFF_PARAM_SIZE);
        if(ret != 0)
        {
            return -1;
        }
    }

    return 0;
}

int16_t ql_wmbus_param_cfg_check_and_save(void)
{
    ql_wmbus_param_check_field_format();
    ql_wmbus_param_head_t* param_head = (ql_wmbus_param_head_t *)&(g_ql_wmbus_config_param_buff[0]);
    param_head->param_magic = WMBUS_PARAM_MAGIC_WORD;
    param_head->param_version = WMBUS_PARAM_VERSION;
    param_head->param_checksum = crc16_calculate((uint8_t*)g_ql_wmbus_config_param_buff + sizeof(ql_wmbus_param_head_t), sizeof(ql_wmbus_param_array_t));

    ql_printf("[PARAM CONFIG]PARAM CHECKSUM:%04X\r\n", param_head->param_checksum);
    int16_t ret = ql_wmbus_param_write_all_form_flash(g_ql_wmbus_config_param_buff, BUFF_PARAM_SIZE);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}
