#include "ql_wmbus_rf_channel_cfg.h"
#include "sl_flex_rail_config.h"

#define RF_CHANNEL_OPTIONS_DEFAULT              0

#define RF_CHANNEL_OPTIONS_T_MODE_TC_COMBO     (1 << 0)  // 0: ENABLE , 1: DIASBLE
#define RF_CHANNEL_OPTIONS_C_MODE_FRAME_A_B    (1 << 1)  // 0: Frame A, 1: Frame B

#define RF_CHANNEL_T_MODE_COUNT         3
#define RF_CHANNEL_C_MODE_COUNT         4

#define RF_CHANNEL_BASE_MODE_OFFSET     0
#define RF_CHANNEL_T_MODE_OFFSET        (RF_CHANNEL_BASE_MODE_OFFSET)
#define RF_CHANNEL_C_MODE_OFFSET        (RF_CHANNEL_T_MODE_OFFSET + RF_CHANNEL_T_MODE_COUNT - 1)

// T C R S N F
#define RF_CHANNEL_SUPPORT_T_MODE_NAME                  "T-MODE"
#define RF_CHANNEL_SUPPORT_C_MODE_NAME                  "C-MODE"

#define RF_CHANNEL_SUPPORT_R_MODE_NAME                  "R-MODE"  // <! not support>
#define RF_CHANNEL_SUPPORT_S_MODE_NAME                  "S-MODE"  // <! not support>
#define RF_CHANNEL_SUPPORT_N_MODE_NAME                  "N-MODE"  // <! not support>
#define RF_CHANNEL_SUPPORT_F_MODE_NAME                  "F-MODE"  // <! not support>

#define RF_CHANNEL_T_MODE_M2O_BASE_FREQ                 "868.95 Mhz"
#define RF_CHANNEL_T_MODE_O2M_BASE_FREQ                 "868.30 Mhz"
#define RF_CHANNEL_T_MODE_M2O_TC_COMBO_BASE_FREQ        "868.95 Mhz"

#define RF_CHANNEL_C_MODE_M2O_FRAME_A_BASE_FREQ         "868.95 Mhz"
#define RF_CHANNEL_C_MODE_M2O_FRAME_B_BASE_FREQ         "868.95 Mhz"
#define RF_CHANNEL_C_MODE_O2M_FRAME_A_BASE_FREQ         "869.52 Mhz"
#define RF_CHANNEL_C_MODE_O2M_FRAME_B_BASE_FREQ         "869.52 Mhz"

#define RF_CHANNEL_T_MODE_M2O_SPEED                     "100k"
#define RF_CHANNEL_T_MODE_O2M_SPEED                     "32.768k"
#define RF_CHANNEL_T_MODE_M2O_TC_COMBO_SPEED            "100k"

#define RF_CHANNEL_C_MODE_M2O_FRAME_A_SPEED             "100k"
#define RF_CHANNEL_C_MODE_M2O_FRAME_B_SPEED             "100k"
#define RF_CHANNEL_C_MODE_O2M_FRAME_A_SPEED             "50k"
#define RF_CHANNEL_C_MODE_O2M_FRAME_B_SPEED             "50k"

#define RF_CHANNEL_T_MODE_M2O_ENCODING_TYPE             "3of6"
#define RF_CHANNEL_T_MODE_O2M_ENCODING_TYPE             "Manchester"
#define RF_CHANNEL_T_MODE_M2O_TC_COMBO_ENCODING_TYPE    "3of6"

#define RF_CHANNEL_C_MODE_M2O_FRAME_A_ENCODING_TYPE     "NRZ"
#define RF_CHANNEL_C_MODE_M2O_FRAME_B_ENCODING_TYPE     "NRZ"
#define RF_CHANNEL_C_MODE_O2M_FRAME_A_ENCODING_TYPE     "NRZ"
#define RF_CHANNEL_C_MODE_O2M_FRAME_B_ENCODING_TYPE     "NRZ"

#define RF_CHANNEL_TX_BUFFER_MAX_SIZE       1024

typedef struct 
{
    uint8_t channel_index;
    uint8_t *str_freq;
    uint8_t *str_speed;
    uint8_t *str_encoding_type;
}ql_wmbus_rf_channel_sel_t;

typedef struct 
{
    uint8_t pmode;                               //<! @see ql_wmbus_transmit_mode_e>
    uint8_t *str_name;
    uint32_t options;
    uint32_t channel_cnt;
    uint32_t channel_offset;
    ql_wmbus_rf_channel_sel_t *channel_sel;
}ql_wmbus_channel_base_cfg_t;

typedef struct 
{
    uint8_t transmit_mode;                      //<! @see ql_wmbus_transmit_mode_e>
    uint8_t mode_type;                          //<! @see ql_wmbus_transmit_mode_type_e>
    uint8_t transmit_dir;                       //<! @see ql_wmbus_transmit_direction_e>
    uint8_t support_mode_cnt;
    ql_wmbus_channel_base_cfg_t *channel_cfg;
}ql_wmbus_rf_channel_cfg_t;

typedef struct 
{
    uint8_t pmode;
    uint8_t* str_name;
}ql_wmbus_rf_channel_str_name_t;

ql_wmbus_rf_channel_str_name_t g_rf_channel_mode_str_name[] = 
{
    {TRANSMIT_MODE_T, RF_CHANNEL_SUPPORT_T_MODE_NAME},
    {TRANSMIT_MODE_C, RF_CHANNEL_SUPPORT_C_MODE_NAME},
    {TRANSMIT_MODE_R, RF_CHANNEL_SUPPORT_R_MODE_NAME},
    {TRANSMIT_MODE_S, RF_CHANNEL_SUPPORT_S_MODE_NAME},
    {TRANSMIT_MODE_N, RF_CHANNEL_SUPPORT_N_MODE_NAME},
    {TRANSMIT_MODE_F, RF_CHANNEL_SUPPORT_F_MODE_NAME}
};

typedef struct 
{
    uint8_t *buf;
    uint32_t head;
    uint32_t tail;
    uint32_t buf_max_len;
}ql_wmbus_rf_channel_tx_buffer_t;

static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t ql_wmbus_rf_transmit_tx_buffer[SL_FLEX_RAIL_TX_FIFO_SIZE] = { 0 };
static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t ql_wmbus_rf_transmit_rx_buffer[SL_FLEX_RAIL_RX_FIFO_SIZE] = { 0 };

typedef struct 
{
    uint16_t last_tx_length;
    uint8_t *rf_transmit_buf;
    uint8_t *rf_receive_buf;
}ql_wmbus_rf_channel_transmit_cfg_t;

static ql_wmbus_rf_channel_transmit_cfg_t g_wmbus_rf_transmit_buf = 
{
    .last_tx_length     = 0,
    .rf_transmit_buf    = ql_wmbus_rf_transmit_tx_buffer,
    .rf_receive_buf     = ql_wmbus_rf_transmit_rx_buffer
};

uint8_t wmbus_rf_channel_tx_buf[RF_CHANNEL_TX_BUFFER_MAX_SIZE] = { 0 };

ql_wmbus_rf_channel_tx_buffer_t g_rf_channel_tx_buf =
{
    .buf            = wmbus_rf_channel_tx_buf,
    .head           = 0,
    .tail           = 0,
    .buf_max_len    = RF_CHANNEL_TX_BUFFER_MAX_SIZE
};

const uint16_t T_M2O_TX_O2M_RX_INDX = 0;
const uint16_t T_M2O_RX_O2M_TX_INDX = 1;
const uint16_t T_M2O_RX_TC_COMMBO_INDX = 2;

const uint16_t C_FRAME_A_M2O_TX_O2M_RX_INDX = 1;
const uint16_t C_FRAME_B_M2O_TX_O2M_RX_INDX = 2;
const uint16_t C_FRAME_A_M2O_RX_O2M_TX_INDX = 3;
const uint16_t C_FRAME_B_M2O_RX_O2M_TX_INDX = 4;

const ql_wmbus_rf_channel_sel_t ql_wmbus_rf_channel_mode_t_table[] = 
{
    [MODE_T_INDEX_0] = 
    {
        .channel_index      = T_M2O_TX_O2M_RX_INDX, 
        .str_encoding_type  = RF_CHANNEL_T_MODE_M2O_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_T_MODE_M2O_BASE_FREQ,
        .str_speed          = RF_CHANNEL_T_MODE_M2O_SPEED
    },
    [MODE_T_INDEX_1] = 
    {
        .channel_index      = T_M2O_RX_O2M_TX_INDX,
        .str_encoding_type  = RF_CHANNEL_T_MODE_O2M_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_T_MODE_O2M_BASE_FREQ,
        .str_speed          = RF_CHANNEL_T_MODE_O2M_SPEED,
    },
    [MODE_T_INDEX_2] = 
    {
        .channel_index      = T_M2O_RX_TC_COMMBO_INDX,
        .str_encoding_type  = RF_CHANNEL_T_MODE_M2O_TC_COMBO_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_T_MODE_M2O_TC_COMBO_BASE_FREQ,
        .str_speed          = RF_CHANNEL_T_MODE_M2O_TC_COMBO_SPEED
    }
};

const ql_wmbus_rf_channel_sel_t ql_wmbus_rf_channel_mode_c_table[] = 
{
    [MODE_C_INDEX_0] = 
    {
        .channel_index      = C_FRAME_A_M2O_TX_O2M_RX_INDX,
        .str_encoding_type  = RF_CHANNEL_C_MODE_M2O_FRAME_A_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_C_MODE_M2O_FRAME_A_BASE_FREQ,
        .str_speed          = RF_CHANNEL_C_MODE_M2O_FRAME_A_SPEED
    },
    [MODE_C_INDEX_1] = 
    {
        .channel_index      = C_FRAME_B_M2O_TX_O2M_RX_INDX,
        .str_encoding_type  = RF_CHANNEL_C_MODE_M2O_FRAME_B_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_C_MODE_M2O_FRAME_B_BASE_FREQ,
        .str_speed          = RF_CHANNEL_C_MODE_M2O_FRAME_B_SPEED
    },
    [MODE_C_INDEX_2] = 
    {
        .channel_index      = C_FRAME_A_M2O_RX_O2M_TX_INDX,
        .str_encoding_type  = RF_CHANNEL_C_MODE_O2M_FRAME_A_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_C_MODE_O2M_FRAME_A_BASE_FREQ,
        .str_speed          = RF_CHANNEL_C_MODE_O2M_FRAME_A_SPEED
    }, 
    [MODE_C_INDEX_3] = 
    {
        .channel_index      = C_FRAME_B_M2O_RX_O2M_TX_INDX,
        .str_encoding_type  = RF_CHANNEL_C_MODE_O2M_FRAME_B_ENCODING_TYPE,
        .str_freq           = RF_CHANNEL_C_MODE_O2M_FRAME_B_BASE_FREQ,
        .str_speed          = RF_CHANNEL_C_MODE_O2M_FRAME_B_SPEED
    } 
};

ql_wmbus_channel_base_cfg_t channel_base_cfg [] = 
{
    [TRANSMIT_MODE_T] = 
    {
        .pmode              = TRANSMIT_MODE_T,
        .channel_cnt        = RF_CHANNEL_T_MODE_COUNT,
        .channel_offset     = RF_CHANNEL_T_MODE_OFFSET,
        .channel_sel        = ql_wmbus_rf_channel_mode_t_table,
        .options            = RF_CHANNEL_OPTIONS_DEFAULT,
        .str_name           = RF_CHANNEL_SUPPORT_T_MODE_NAME
    },
    [TRANSMIT_MODE_C] = 
    {
        .pmode              = TRANSMIT_MODE_C,
        .channel_cnt        = RF_CHANNEL_C_MODE_COUNT,
        .channel_offset     = RF_CHANNEL_C_MODE_OFFSET,
        .channel_sel        = ql_wmbus_rf_channel_mode_c_table,
        .options            = RF_CHANNEL_OPTIONS_DEFAULT,
        .str_name           = RF_CHANNEL_SUPPORT_C_MODE_NAME
    }
};

#define QL_WMBUS_RF_SUPPORT_MODE_COUNT (sizeof(channel_base_cfg) / sizeof(ql_wmbus_channel_base_cfg_t))

volatile ql_wmbus_rf_channel_cfg_t g_wmbus_rf_channel_config = 
{
    .channel_cfg        = channel_base_cfg,
    .mode_type          = MODE_TYPE_2,
    .transmit_mode      = TRANSMIT_MODE_T,
    .transmit_dir       = MASTER_TRANS_DIR_O2M,
    .support_mode_cnt   = QL_WMBUS_RF_SUPPORT_MODE_COUNT
};

ql_wmbus_rf_channel_cfg_t *g_ptr_wmbus_rf_channel_config = &g_wmbus_rf_channel_config;

static RAIL_Handle_t g_rail_instance_handle = 0;
static volatile ql_wmbus_rf_channel_tx_rx_t g_wmbus_rf_channel_tx_rx_index = { 0 };

static RAIL_Handle_t ql_wmbus_rf_radio_get_instance_handle(void);
static int16_t ql_wmbus_rf_channel_id_instances_management(void);
int16_t ql_wmbus_rf_channle_get_detail_info(uint16_t channel_id, uint8_t** freq, uint8_t** encoding, uint8_t** speed);

int16_t ql_wmbus_rf_channel_init(void)
{
    g_rail_instance_handle = ql_wmbus_rf_radio_get_instance_handle();

    for (size_t i = 0; i < g_ptr_wmbus_rf_channel_config->support_mode_cnt; i++)
    {
        ql_printf("[APP]QL WMBUS TRANSMIT SUPPORT MODE: [%s]\r\n", g_ptr_wmbus_rf_channel_config->channel_cfg[i].str_name);
    }

    for (size_t i = 0; i < g_ptr_wmbus_rf_channel_config->support_mode_cnt; i++)
    {
        ql_printf("[APP]START WMBUS RF CHANNEL INIT, MODE: [%s]\r\n", g_ptr_wmbus_rf_channel_config->channel_cfg[i].str_name);
        for (size_t j = 0; j < g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_cnt; j++)
        {
            RAIL_Status_t ret = RAIL_IsValidChannel(
                g_rail_instance_handle, 
                (g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].channel_index + 
                g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_offset)
            );

            if(RAIL_STATUS_NO_ERROR == ret)
            {
                ql_printf("[APP]CHANNEL ID:%d, CHANNEL CHECK OK!\r\n", 
                    (g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].channel_index + 
                    g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_offset));
            }
            else
            {
                ql_printf("[APP]CHANNEL ID:%d, CHANNEL CHECK ERROR! RET:%d\r\n", 
                    (g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].channel_index + 
                    g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_offset), ret);     
            }
        }   
    }


    // READ FLASH GET CURRENT MODE
    //
    // READ FLASH GET CURRENT MODE


    int16_t ret = ql_wmbus_rf_channel_id_instances_management();
    if(ret != 0)
    {
        ql_printf("[APP]QL WMBUS RF CHANNEL AUTO CONFIG FAIL\r\n");
        return -1;
    }

    uint8_t trans_mode = g_ptr_wmbus_rf_channel_config->transmit_mode;
    uint8_t trans_type = g_wmbus_rf_channel_config.mode_type;
    uint8_t *str_trans_name = g_ptr_wmbus_rf_channel_config->channel_cfg[trans_mode].str_name;
    uint8_t *str_freq;
    uint8_t *str_encoding;
    uint8_t *str_speed;

    if(MODE_TYPE_1 == trans_type)
    {
        ql_wmbus_rf_channle_get_detail_info(g_wmbus_rf_channel_tx_rx_index.channel_tx, &str_freq, &str_encoding, &str_speed);

        ql_printf("[APP]QL WMBUS RF CHANNEL INFO, TRANSMIT: [TX]\r\nMODE: [%s], TYPE: [%d], FREQ: [%s], ENCODEING: [%s], SPEED: [%s]\r\n",
            str_trans_name,
            trans_type,
            str_freq,
            str_encoding,
            str_speed
        );
    }
    else
    {
        ql_wmbus_rf_channle_get_detail_info(g_wmbus_rf_channel_tx_rx_index.channel_tx, &str_freq, &str_encoding, &str_speed);
        ql_printf("[APP]QL WMBUS RF CHANNEL INFO, TRANSMIT: [TX]\r\nMODE: [%s], TYPE: [%d], FREQ: [%s], ENCODEING: [%s], SPEED: [%s]\r\n",
            str_trans_name,
            trans_type,
            str_freq,
            str_encoding,
            str_speed
        );
        ql_wmbus_rf_channle_get_detail_info(g_wmbus_rf_channel_tx_rx_index.channel_rx, &str_freq, &str_encoding, &str_speed);
        ql_printf("[APP]QL WMBUS RF CHANNEL INFO, TRANSMIT: [RX]\r\nMODE: [%s], TYPE: [%d], FREQ: [%s], ENCODEING: [%s], SPEED: [%s]\r\n",
            str_trans_name,
            trans_type,
            str_freq,
            str_encoding,
            str_speed
        );
    }

    return 0;
}

static RAIL_Handle_t ql_wmbus_rf_radio_get_instance_handle(void)
{
    return sl_rail_util_get_handle(SL_RAIL_UTIL_HANDLE_INST0);
}

ql_wmbus_transmit_mode_e ql_wmbus_rf_channel_get_mode(void)
{
    return g_ptr_wmbus_rf_channel_config->transmit_mode;
}

ql_wmbus_transmit_mode_type_e ql_wmbus_rf_channel_get_type(void)
{
    return g_ptr_wmbus_rf_channel_config->mode_type;
}

ql_wmbus_transmit_direction_e ql_wmbus_rf_channel_get_direction(void)
{
    return g_ptr_wmbus_rf_channel_config->transmit_dir;
}

int16_t ql_wmbus_rf_channel_set_mode(ql_wmbus_transmit_mode_e set_mode)
{
    uint8_t is_contain = 0;
    for (size_t i = 0; i < QL_WMBUS_RF_SUPPORT_MODE_COUNT; i++)
    {
        if(strcmp(g_ptr_wmbus_rf_channel_config->channel_cfg[i].str_name, g_rf_channel_mode_str_name[set_mode].str_name) == 0)
        {
            is_contain = 1;
        }
    }

    if(!is_contain)
    {
        ql_printf("[APP]WMBUS RF CHANNEL MODE IS NOT SUPPORT: %d\r\n", g_rf_channel_mode_str_name[set_mode].str_name);
        return -1;
    }
    
    g_ptr_wmbus_rf_channel_config->transmit_mode = set_mode;
    return ql_wmbus_rf_channel_id_instances_management();
}

int16_t ql_wmbus_rf_channel_set_mode_type(ql_wmbus_transmit_mode_type_e set_type)
{
    g_ptr_wmbus_rf_channel_config->mode_type = set_type;
    return ql_wmbus_rf_channel_id_instances_management();;
}

int16_t ql_wmbus_rf_channel_set_direction(ql_wmbus_transmit_direction_e set_dir)
{
    g_ptr_wmbus_rf_channel_config->transmit_dir = set_dir;
    return ql_wmbus_rf_channel_id_instances_management();  
}

RAIL_Handle_t ql_wmbus_rf_rail_get_handle(void)
{
    return g_rail_instance_handle;
}

ql_wmbus_rf_channel_tx_rx_t ql_wmbus_rf_channel_get_instances(void)
{
    return g_wmbus_rf_channel_tx_rx_index;
}

int16_t ql_wmbus_rf_channle_get_detail_info(uint16_t channel_id, uint8_t** freq, uint8_t** encoding, uint8_t** speed)
{
    for (size_t i = 0; i < g_ptr_wmbus_rf_channel_config->support_mode_cnt; i++)
    {
        for (size_t j = 0; j < g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_cnt; j++)
        {
            if(channel_id == ((g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].channel_index + \
                g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_offset)))
            {
                *encoding = g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].str_encoding_type;
                *freq = g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].str_freq;
                *speed = g_ptr_wmbus_rf_channel_config->channel_cfg[i].channel_sel[j].str_speed;
            }
        }   
    }

    return 0;
}

static int16_t ql_wmbus_rf_channel_id_instances_management(void)
{
    ql_wmbus_rf_channel_sel_t* rf_channel_sel = NULL;
    uint32_t rf_channel_offset = 0;
    uint32_t rf_channel_options = 0;
    int16_t rf_channel = -1;
    int16_t index = -1;

    rf_channel_sel = g_ptr_wmbus_rf_channel_config->channel_cfg[g_ptr_wmbus_rf_channel_config->transmit_mode].channel_sel;
    rf_channel_offset = g_ptr_wmbus_rf_channel_config->channel_cfg[g_ptr_wmbus_rf_channel_config->transmit_mode].channel_offset;
    rf_channel_options = g_ptr_wmbus_rf_channel_config->channel_cfg[g_ptr_wmbus_rf_channel_config->transmit_mode].options;

    switch (g_ptr_wmbus_rf_channel_config->transmit_mode)
    {
        case TRANSMIT_MODE_T:
        {
            switch (g_ptr_wmbus_rf_channel_config->transmit_dir)
            {
                case MASTER_TRANS_DIR_O2M: // Master
                {
                    g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_T_INDEX_1].channel_index + rf_channel_offset);
                    if((rf_channel_options & RF_CHANNEL_OPTIONS_T_MODE_TC_COMBO) == 0) // ENABLE
                    {
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_T_INDEX_2].channel_index + rf_channel_offset);
                    }
                    else
                    {
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_T_INDEX_0].channel_index + rf_channel_offset);
                    }
                }
                break;
                case SLAVE_TRANS_DIR_M2O:
                {
                    g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_T_INDEX_2].channel_index + rf_channel_offset);
                    g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_T_INDEX_1].channel_index + rf_channel_offset);
                }
                break;
            
            default:
                break;
            }
        }
        break;
        case TRANSMIT_MODE_C:
        {
            switch (g_ptr_wmbus_rf_channel_config->transmit_dir)
            {
                case MASTER_TRANS_DIR_O2M:
                {
                    if((rf_channel_options & RF_CHANNEL_OPTIONS_C_MODE_FRAME_A_B) == 0) // FRAME A
                    {
                        g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_C_INDEX_2].channel_index + rf_channel_offset);
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_C_INDEX_0].channel_index + rf_channel_offset);
                    }
                    else
                    {
                        g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_C_INDEX_0].channel_index + rf_channel_offset);
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_C_INDEX_2].channel_index + rf_channel_offset);
                    }
                }
                break;
                case SLAVE_TRANS_DIR_M2O:
                {
                    if((rf_channel_options & RF_CHANNEL_OPTIONS_C_MODE_FRAME_A_B) == 0)
                    {
                        g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_C_INDEX_3].channel_index + rf_channel_offset);
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_C_INDEX_1].channel_index + rf_channel_offset);
                    }
                    else
                    {                      
                        g_wmbus_rf_channel_tx_rx_index.channel_tx = (rf_channel_sel[MODE_C_INDEX_1].channel_index + rf_channel_offset);
                        g_wmbus_rf_channel_tx_rx_index.channel_rx = (rf_channel_sel[MODE_C_INDEX_3].channel_index + rf_channel_offset);
                    }
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

    return 0;
}

int sprintfFloat(char *buffer, int8_t len, float f, uint8_t precision);

int16_t ql_wmbus_rf_channel_get_rssi(void)
{
    char bufRssi[10];
    uint32_t waitTimeout = RAIL_GET_RSSI_NO_WAIT;
    int16_t rssi = RAIL_GetRssiAlt(g_rail_instance_handle, waitTimeout);
    sprintfFloat(bufRssi, sizeof(bufRssi), ((float) rssi / 4), 2);
    ql_printf("RSSI:%s\r\n", bufRssi);
    return 0;
}

int16_t ql_wmbus_rf_channel_unblock_transmit(uint8_t *data, uint32_t data_len)
{
    if(g_rf_channel_tx_buf.head + data_len < g_rf_channel_tx_buf.buf_max_len)
    {
        memcpy(g_rf_channel_tx_buf.buf[g_rf_channel_tx_buf.head], data, data_len);
        g_rf_channel_tx_buf.head += data_len;
    }
    else
    {
        memcpy(g_rf_channel_tx_buf.buf[g_rf_channel_tx_buf.head], data, (g_rf_channel_tx_buf.buf_max_len - g_rf_channel_tx_buf.head));
        memcpy(g_rf_channel_tx_buf.buf, 
            data + (g_rf_channel_tx_buf.buf_max_len - g_rf_channel_tx_buf.head),
            ((g_rf_channel_tx_buf.head + data_len) - g_rf_channel_tx_buf.buf_max_len)
        );
        g_rf_channel_tx_buf.head = ((g_rf_channel_tx_buf.head + data_len) - g_rf_channel_tx_buf.buf_max_len);
    }
    return 0;
}

void ql_wmbus_rf_channel_testing_carrier_singal(void)
{
    uint8_t txData[1024] = 
    {
        0x0F, 0x0E, 0x11, 0x22, 0x33, 0x44, 0x55, 0x0F,
        0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
    };

    RAIL_WriteTxFifo(ql_wmbus_rf_rail_get_handle(), txData, 4, true);
    RAIL_TxOptions_t antOptions = RAIL_TX_OPTIONS_DEFAULT;
    RAIL_TxOptions_t concPhyIdOptions = RAIL_TX_OPTIONS_DEFAULT;
    RAIL_StreamMode_t streamMode = RAIL_STREAM_CARRIER_WAVE;

    RAIL_StartTxStreamAlt(ql_wmbus_rf_rail_get_handle(), 
    ql_wmbus_rf_channel_get_instances().channel_tx, 
    streamMode,
    concPhyIdOptions | antOptions);
}

void ql_wmbus_rf_channel_testing_stream_singal(void)
{
    uint8_t txData[1024] = 
    {
        0x0F, 0x0E, 0x11, 0x22, 0x33, 0x44, 0x55, 0x0F,
        0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
    };

    RAIL_WriteTxFifo(ql_wmbus_rf_rail_get_handle(), txData, 4, true);
    RAIL_TxOptions_t antOptions = RAIL_TX_OPTIONS_DEFAULT;
    RAIL_TxOptions_t concPhyIdOptions = RAIL_TX_OPTIONS_DEFAULT;
    RAIL_StreamMode_t streamMode = RAIL_STREAM_PN9_STREAM;

    RAIL_StartTxStreamAlt(ql_wmbus_rf_rail_get_handle(), 
    ql_wmbus_rf_channel_get_instances().channel_tx, 
    streamMode,
    concPhyIdOptions | antOptions);
}

void ql_wmbus_rf_channel_testing_receive_singal(void)
{

}

void ql_wmbus_rf_channel_transmit_loop(void)
{
    if(g_rf_channel_tx_buf.head != g_rf_channel_tx_buf.tail)
    {
        uint16_t tx_length = 0;
        if(g_rf_channel_tx_buf.head > g_rf_channel_tx_buf.tail)
        {
            memcpy(g_wmbus_rf_transmit_buf.rf_transmit_buf, g_rf_channel_tx_buf.buf[g_rf_channel_tx_buf.tail], (g_rf_channel_tx_buf.head - g_rf_channel_tx_buf.tail));
            g_rf_channel_tx_buf.tail = g_rf_channel_tx_buf.head;
            tx_length = g_rf_channel_tx_buf.head - g_rf_channel_tx_buf.tail;  
        }
        else
        {
            memcpy(g_wmbus_rf_transmit_buf.rf_transmit_buf, g_rf_channel_tx_buf.buf[g_rf_channel_tx_buf.tail], (g_rf_channel_tx_buf.buf_max_len - g_rf_channel_tx_buf.tail));
            memcpy(&g_wmbus_rf_transmit_buf.rf_transmit_buf[(g_rf_channel_tx_buf.buf_max_len - g_rf_channel_tx_buf.tail)], g_rf_channel_tx_buf.buf, g_rf_channel_tx_buf.head);
            g_rf_channel_tx_buf.tail = g_rf_channel_tx_buf.head;
            tx_length = g_rf_channel_tx_buf.head + (g_rf_channel_tx_buf.buf_max_len - g_rf_channel_tx_buf.tail);
        }

        g_wmbus_rf_transmit_buf.last_tx_length = WMBUS_phy_software(g_wmbus_rf_transmit_buf.rf_transmit_buf,
            (uint8_t) tx_length, SL_FLEX_RAIL_TX_FIFO_SIZE);

        RAIL_SetTxFifo(g_rail_instance_handle, 
            g_wmbus_rf_transmit_buf.rf_transmit_buf, 
            g_wmbus_rf_transmit_buf.last_tx_length, 
            SL_FLEX_RAIL_TX_FIFO_SIZE
        );

        if(g_wmbus_rf_transmit_buf.last_tx_length != tx_length) 
        {
            RAIL_SetFixedLength(g_rail_instance_handle, g_wmbus_rf_transmit_buf.last_tx_length);
        } 
        else 
        {
            RAIL_SetFixedLength(g_rail_instance_handle, RAIL_SETFIXEDLENGTH_INVALID);
        }

        RAIL_StartTx(g_rail_instance_handle, g_wmbus_rf_channel_tx_rx_index.channel_tx, RAIL_TX_OPTIONS_DEFAULT, NULL);
    }
}

// int16_t ql_wmbus_rf_channel_transmit(uint8_t *data, uint32_t data_len)
// {
    
//     return 0;
// }

// int16_t ql_wmbus_rf_channel_transmit()
// {

// }


int sprintfFloat(char *buffer, int8_t len, float f, uint8_t precision)
{
  int8_t isNegative = (f < 0) ? 1 : 0;

  // Buffer needs to be big enough to hold sign (if negative), 1 integral digit,
  // precision fractional digits, decimal point (if precision > 0), and \0.
  if (buffer == NULL || len < (isNegative + 1 + precision + (precision > 0) + 1)) {
    return 0;
  }

  int8_t writeIndex = len - 1;
  buffer[writeIndex] = '\0';

  for (uint8_t exp = 0; exp < precision; exp++) {
    f *= 10;
  }

  int a;
  if (isNegative != 0) {
    a = -(int)(f - 0.5); // Round toward negative infinity
  } else {
    a = (int)(f + 0.5); // Round toward positive infinity
  }
  if (a < 0) { // Sign changed, float too large!
    return 0;
  }

  buffer[writeIndex--] = '\0'; // terminate string

  int8_t digit;
  do {
    digit = a % 10;
    a = a / 10;
    buffer[writeIndex--] = '0' + digit;
    if (precision && len == writeIndex + 2 + precision) {
      buffer[writeIndex--] = '.';
    }
  } while ((a != 0 || (precision && writeIndex >= (len - precision - 3)))
           && writeIndex >= isNegative);
  if (a != 0) {
    return 0; // Number too large to represent in buffer!
  }
  if (isNegative != 0) {
    buffer[writeIndex--] = '-';
  }

  // shift up
  if (writeIndex != -1 ) {
    memmove(buffer, &buffer[writeIndex + 1], len - writeIndex - 1);
  }
  return len - writeIndex - 1;
}

