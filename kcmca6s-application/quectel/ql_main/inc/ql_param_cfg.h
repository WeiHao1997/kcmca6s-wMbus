/*
 * ql_param_cfg.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_PARAM_CFG_H__
#define __QL_PARAM_CFG_H__

#include "ql_include.h"


#define MAX(x, y)   (((x) < (y)) ? (y) : (x))
#define MIN(x, y)   (((x) < (y)) ? (x) : (y))

#define ARRAY_ALLOC(id, size)                   /* alloc array size */\
        uint8_t P##id[size];

#define STR_OFFSET(structure, member)           /* byte offset of member in structure*/\
        ((int) &(((structure *) 0) -> member))

#define STR_MEMB_SIZE(structure, member)        /* size of a member of a structure */\
        (sizeof (((structure *) 0) -> member))

typedef enum
{
    REG_TX_POWER        = 0x00,
    REG_WMBUS_MODE      = 0x01,
    REG_CHANNEL_NUM     = 0x02,
    REG_ROLE            = 0x03,
    REG_SLEEP_MODE      = 0x04,
    REG_T_C_COMPATIBLE  = 0x05,
    REG_RESERVE_00            ,
    REG_PREAMBLE_LEN    = 0x10,
    REG_DLL_C_FIELD     = 0x11,
    REG_RESERVE_01            ,
    REG_RF_RECV_TIMEOUT = 0x13,
    REG_RESERVE_02,
    REG_DLL_M1_FIELD    = 0x20,
    REG_DLL_M2_FIELD    = 0x21,
    REG_DLL_ID1_FIELD   = 0x22,
    REG_DLL_ID2_FIELD   = 0x23,
    REG_DLL_ID3_FIELD   = 0x24,
    REG_DLL_ID4_FIELD   = 0x25,
    REG_DLL_VER_FIELD   = 0x26,
    REG_DLL_REG_FIELD   = 0x27,
    REG_RESERVE_03            ,
    REG_UART_BAUDRATE   = 0x30,
    REG_FLOW_CONTROL    = 0x31,
    REG_RESERVE_04            ,
    REG_UART_DATA_FORMAT = 0x35,
    REG_INSTALL_MODE     = 0x36,
    REG_USE_RSSI         = 0x37,
    REG_LED_CONTROL      = 0x38,
    REG_TEMP_CALI        = 0x39,
    REG_FREQ_CALI       = 0x3A,
    REG_SLEEP_DELAY_TIME = 0x3B,
    REG_RESERVE_05             ,
    REG_MASTER_KEY      = 0x40,
    REG_INITIAL_VECTOR  = 0x50,
    REG_PRODUCT_NAME    = 0x60,
    REG_RESERVE_06            ,
    REG_SERIAL_NUM      = 0x70,
    REG_HARD_VER        = 0x78,
    REG_SOFT_VER        = 0x7C,
    REG_URC_INV_TIME    = 0x88,
    REG_RESERVE_07            ,
    REG_KEY_MGR         = 0x90,
    REG_RESERVE_08            ,
    REG_WORK_SCENE      = 0x9E,

    REG_PARAM_END
}ql_wmbus_param_reg_e;

typedef enum
{
    dt_integer = 0x0F,
    dt_unsigned = 0x11,
    dt_dont_care = 0xFF,
} ql_wmbus_param_type_e;

typedef struct
{
    uint16_t            offset;
    uint8_t             maxlen;
    uint8_t             pid;
    uint8_t             ptype;
    const char*         pinit;
    uint8_t             plen;
    uint8_t             is_save;
    const char*         pstr;
} ql_wmbus_param_default_tables_t;

typedef struct 
{
    ARRAY_ALLOC(REG_TX_POWER,           1)
    ARRAY_ALLOC(REG_WMBUS_MODE,         1)
    ARRAY_ALLOC(REG_CHANNEL_NUM,        1)
    ARRAY_ALLOC(REG_ROLE,               1)
    ARRAY_ALLOC(REG_SLEEP_MODE,         1)
    ARRAY_ALLOC(REG_T_C_COMPATIBLE,     1)
    ARRAY_ALLOC(REG_RESERVE_00,         10)
    ARRAY_ALLOC(REG_PREAMBLE_LEN,       1)
    ARRAY_ALLOC(REG_DLL_C_FIELD,        1)
    ARRAY_ALLOC(REG_RESERVE_01,         1)
    ARRAY_ALLOC(REG_RF_RECV_TIMEOUT,    1)
    ARRAY_ALLOC(REG_RESERVE_02,         12)
    ARRAY_ALLOC(REG_DLL_M1_FIELD,       1)
    ARRAY_ALLOC(REG_DLL_M2_FIELD,       1)
    ARRAY_ALLOC(REG_DLL_ID1_FIELD,      1)
    ARRAY_ALLOC(REG_DLL_ID2_FIELD,      1)
    ARRAY_ALLOC(REG_DLL_ID3_FIELD,      1)
    ARRAY_ALLOC(REG_DLL_ID4_FIELD,      1)
    ARRAY_ALLOC(REG_DLL_VER_FIELD,      1)
    ARRAY_ALLOC(REG_DLL_REG_FIELD,      1)
    ARRAY_ALLOC(REG_RESERVE_03,         8)
    ARRAY_ALLOC(REG_UART_BAUDRATE,      1)
    ARRAY_ALLOC(REG_FLOW_CONTROL,       1)
    ARRAY_ALLOC(REG_RESERVE_04,         3)
    ARRAY_ALLOC(REG_UART_DATA_FORMAT,   1)
    ARRAY_ALLOC(REG_INSTALL_MODE,       1)
    ARRAY_ALLOC(REG_USE_RSSI,           1)
    ARRAY_ALLOC(REG_LED_CONTROL,        1)
    ARRAY_ALLOC(REG_TEMP_CALI,          1)
    ARRAY_ALLOC(REG_FREQ_CALI,          1)
    ARRAY_ALLOC(REG_SLEEP_DELAY_TIME,   1)
    ARRAY_ALLOC(REG_RESERVE_05,         4)
    ARRAY_ALLOC(REG_MASTER_KEY,         16)
    ARRAY_ALLOC(REG_INITIAL_VECTOR,     16)
    ARRAY_ALLOC(REG_PRODUCT_NAME,       12)
    ARRAY_ALLOC(REG_RESERVE_06,         4)
    ARRAY_ALLOC(REG_SERIAL_NUM,         8)
    ARRAY_ALLOC(REG_HARD_VER,           4)
    ARRAY_ALLOC(REG_SOFT_VER,           4)
} __attribute__((packed)) ql_wmbus_param_array_t;

typedef struct 
{
    uint32_t param_magic;
    uint16_t param_version;
    uint16_t param_checksum;
} __attribute__((packed)) ql_wmbus_param_head_t;

typedef struct
{
    ql_wmbus_param_head_t       head;
    ql_wmbus_param_array_t      body;
} __attribute__((packed)) ql_wmbus_param_t;

#define STR_ITEM_ADD(id, type, name, init, save)                                            \
        {                                                                                   \
                STR_OFFSET(ql_wmbus_param_array_t, P##id) + sizeof(ql_wmbus_param_head_t),  \
                STR_MEMB_SIZE(ql_wmbus_param_array_t, P##id),                               \
                id,                                                                         \
                type,                                                                       \
                init,                                                                       \
                sizeof(init) - 1,                                                           \
                save,                                                                       \
                name,                                                                       \
        }

#define WMBUS_PARAM_IS_SAVE     1
#define WMBUS_PARAM_NOT_SAVE    0

#define BUFF_PARAM_SIZE         (sizeof(ql_wmbus_param_t))


typedef enum
{
    E_TX_POWER_FU5 = 0,
    E_TX_POWER_FU2,
    E_TX_POWER_1,
    E_TX_POWER_7,
    E_TX_POWER_10,
    E_TX_POWER_13,
    E_TX_POWER_14,
    E_TX_POWER_MAX
} ql_wmbus_tx_power_e;

typedef enum
{
    E_WMBUS_MODE_S2 = 0,
    E_WMBUS_MODE_T1 = 1,
    E_WMBUS_MODE_T2 = 2,
    E_WMBUS_MODE_S1 = 3,
    E_WMBUS_MODE_R = 4,
    E_WMBUS_MODE_C1 = 10,
    E_WMBUS_MODE_C2 = 11,
} ql_wmbus_mode_e;

typedef enum
{
    E_ROLE_SLAVE = 0,
    E_ROLE_MASTER = 1,
    E_ROLE_REPEATER = 0x10,
} ql_wmbus_role_e;

typedef enum
{
    E_SLEEP_MODE_DISABLE = 0,
    E_SLEEP_MODE_ENABLE = 1,
    E_SLEEP_MODE_DELAY = 3,
} ql_wmbus_sleep_mode_e;

typedef enum
{
    E_TC_COMPATIBLE_DISABLE = 0,
    E_TC_COMPATIBLE_ENABLE = 1,
} ql_wmbus_tc_compatible_e;

typedef enum
{
    E_PREAMBLE_SHORT = 0,
    E_PREAMBLE_LONG = 1,
} ql_wmbus_preamble_e;

typedef enum
{
    E_BAUD_RATE_2400 = 1,
    E_BAUD_RATE_4800 = 2,
    E_BAUD_RATE_9600 = 3,
    E_BAUD_RATE_14400 = 4,
    E_BAUD_RATE_19200 = 5,
    E_BAUD_RATE_28800 = 6,
    E_BAUD_RATE_38400 = 7,
    E_BAUD_RATE_57600 = 8,
    E_BAUD_RATE_76800 = 9,
    E_BAUD_RATE_115200 = 10,
    E_BAUD_RATE_230400 = 11,
} ql_uart_baud_rate_e;

typedef enum
{
    E_FLOW_CONTROL_DISABLE = 0,
    E_FLOW_CONTROL_CTS = 1,
    E_FLOW_CONTROL_CTS_RTS = 3,
} ql_uart_flow_control_e;

typedef enum
{
    E_DATA_FORMAT_DATA_ID = 0,
    E_DATA_FORMAT_APP = 1,
    E_DATA_FORMAT_START_END = 4,
    E_DATA_FORMAT_CRC = 8,
    E_DATA_FORMAT_START_END_CRC = 0x0C,
} ql_uart_data_format_e;

typedef enum
{
    E_INSTALL_MODE_NORMAL = 0,
    E_INSTALL_MODE_INSTALL = 1,
    E_INSTALL_MODE_LISTEN = 2,
} ql_wmbus_install_mode_e;

typedef enum
{
    E_RSSI_MODE_DISABLE = 0,
    E_RSSI_MODE_ENABLE = 1,
} ql_wmbus_rssi_mode_e;

typedef enum
{
    E_LED_CONTROL_DISABLE = 0,
    E_LED_CONTROL_RF = 1,
    E_LED_CONTROL_RF_UART = 2,
} ql_wmbus_led_control_e;


int16_t ql_param_cfg_init(void);
int16_t ql_wmbus_param_read_single(uint8_t pid, void* buf, uint16_t *len);
int16_t ql_wmbus_param_write_single(uint8_t pid, const void* buf, uint16_t len);
int16_t ql_wmbus_param_single_check_field(uint8_t pid, uint8_t *buf, uint16_t data_len);

uint8_t *ql_wmbus_param_current_datas(void);
uint32_t ql_wmbus_param_current_datas_len(void);

int16_t ql_wmbus_param_cfg_check_and_save(void);
#endif /* __QL_PARAM_CFG_H__ */
