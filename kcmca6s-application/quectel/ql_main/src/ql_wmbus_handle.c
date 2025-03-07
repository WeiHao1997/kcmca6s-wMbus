#include "ql_wmbus_handle.h"
#include "sl_flex_rail_config.h"

typedef enum
{
    QL_MSG_EVENT_TYPE_CHANGE_TX_POWER       = REG_TX_POWER,
    QL_MSG_EVENT_TYPE_CHANGE_ROLE           = REG_ROLE,

    QL_MSG_EVENT_TYPE_CHANGE_DLL_M1_FIELD   = REG_DLL_M1_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_M2_FIELD   = REG_DLL_M2_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_ID1_FIELD  = REG_DLL_ID1_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_ID2_FIELD  = REG_DLL_ID2_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_ID3_FIELD  = REG_DLL_ID3_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_ID4_FIELD  = REG_DLL_ID4_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_VER_FIELD  = REG_DLL_VER_FIELD,
    QL_MSG_EVENT_TYPE_CHANGE_DLL_DEV_FIELD  = REG_DLL_REG_FIELD,

    QL_MSG_EVENT_TYPE_CHANGE_DATA_FORMAT    = REG_UART_DATA_FORMAT,
    QL_MSG_EVENT_TYPE_CHANGE_INSTALL_MODE   = REG_INSTALL_MODE,
    QL_MSG_EVENT_TYPE_CHANGE_USE_RSSI       = REG_USE_RSSI,

    QL_MSG_EVENT_TYPE_CHANGE_ENCRYPTION_KEY = 0xFF
};
 
wmbus_ci_field app_stl_ci_tab[]=
{
    WMBUS_CI_MBUS_DATA_TO_METER_SHORT,              /**< 0x5A, short frame */
    WMBUS_CI_COSEM_DATA_TO_METER_SHORT,             /**< 0x61 */
    WMBUS_CI_OBIS_DATA_TO_METER_SHORT,              /**< 0x65 */
    WMBUS_CI_EN13757_3_DATA_TO_METER_SHORT,         /**< 0x6A */
    WMBUS_CI_APPLICATION_ERROR_SHORT,               /**< 0x6E */
    WMBUS_CI_ALARM_SHORT,                           /**< 0x74 */
    WMBUS_CI_EN13757_3_APPLICATION_SHORT,           /**< 0x7A */
    WMBUS_CI_EN13757_3_APPLICATION_CF_SHORT,        /**< 0x7B */
    WMBUS_CI_COSEM_APPLICATION_SHORT,               /**< 0x7D */
    WMBUS_CI_OBIS_APPLICATION_SHORT,                /**< 0x7F */
    WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_SHORT,  /**< 0x8A */
};

wmbus_ci_field app_ltl_ci_tab[]=
{
    WMBUS_CI_MBUS_DATA_TO_METER_LONG,               /**< 0x5B, long frame */
    WMBUS_CI_COSEM_DATA_TO_METER_LONG,              /**< 0x60 */
    WMBUS_CI_OBIS_DATA_TO_METER_LONG,               /**< 0x64 */
    WMBUS_CI_EN13757_3_DATA_TO_METER_LONG,          /**< 0x6B */
    WMBUS_CI_CLOCK_SYNC_ABSOLUTE,                   /**< 0x6C */
    WMBUS_CI_CLOCK_SYNC_RELATIVE,                   /**< 0x6D */
    WMBUS_CI_APPLICATION_ERROR_LONG,                /**< 0x6F */
    WMBUS_CI_EN13757_3_APPLICATION_LONG,            /**< 0x72 */
    WMBUS_CI_EN13757_3_APPLICATION_CF_LONG,         /**< 0x73 */
    WMBUS_CI_ALARM_LONG,                            /**< 0x75 */
    WMBUS_CI_COSEM_APPLICATION_LONG,                /**< 0x7C */
    WMBUS_CI_OBIS_APPLICATION_LONG,                 /**< 0x7E */
    WMBUS_CI_EN13757_3_TRANSPORT_TO_METER_LONG,     /**< 0x80 */
    WMBUS_CI_TRANSPORT_TO_METER_MBUS_COMPACT,       /**< 0x84 */
    WMBUS_CI_EN13757_3_TRANSPORT_FROM_METER_LONG    /**< 0x8B */
};

typedef struct 
{
    wmbus_ci_field app_ci;
    wmbus_app_func app_func;
}ql_wmbus_app_function_tables_t;

static void print_blocks(const uint8_t *buffer, uint16_t length);

void ql_wmbus_applcation_short_package(uint8_t *data, uint32_t data_len);
void ql_wmbus_applcation_long_package(uint8_t *data, uint32_t data_len);

ql_wmbus_app_function_tables_t g_wmbus_app_func_tables[] =
{
    WMBUS_CI_EN13757_3_APPLICATION_SHORT,   ql_wmbus_applcation_short_package,
    WMBUS_CI_EN13757_3_APPLICATION_LONG,    ql_wmbus_applcation_long_package
};

#define QL_WMBUS_APP_TABLES_SIZE (sizeof(g_wmbus_app_func_tables) / sizeof(ql_wmbus_app_function_tables_t))

typedef struct 
{
    uint8_t is_cur_enc;
    uint8_t wmbus_role;
    uint8_t install_mode;
    uint8_t uart_data_format;
    uint8_t is_enable_rssi;
    uint8_t tx_power;
    uint8_t is_auto_recv_status;
    uint32_t tune_value;
    ql_wmbus_frame_device_info_t device_info;
}ql_wmbus_module_config_t;

ql_wmbus_module_config_t g_wmbus_entiy = { 0 };
ql_wmbus_module_config_t *wmbus_entiy = &g_wmbus_entiy;

typedef struct 
{
    uint8_t is_ltl;
    WMBUS_dll_header_t head_dll;
    union
    {
        WMBUS_stl_header_t head_stl;
        WMBUS_ltl_header_t head_ltl;
    }package_type;
}ql_wmbus_dev_data_frame_head_info_t;

ql_wmbus_dev_data_frame_head_info_t  g_wmbus_dev_data_frame_head_info = { 0 };
ql_wmbus_dev_data_frame_head_info_t *wmbus_dev_data_frame_head_info = &g_wmbus_dev_data_frame_head_info;

typedef struct 
{
    uint8_t len;
    uint8_t c_field;
    uint8_t ci_fied;
    uint32_t payload_len;
    uint8_t *payload;
}ql_wmbus_frame_field_t;

ql_wmbus_frame_field_t ql_wmbus_frame_field = { 0 };
ql_wmbus_frame_field_t *g_ql_wmbus_frame_field = &ql_wmbus_frame_field;

// typedef enum
// {
//     SLAVE_M2O_TX_CHANNEL = 0,   // 100K 3of6        868.95 Mhz ~ 868.95 Mhz
//     MASTER_O2M_TX_CHANNEL,      // 32.768K M        868.30 Mhz ~ 868.30 Mhz
//     MASTER_M2O_RX_CHANNEL,      // 100K Frame A     868.95 Mhz ~ 868.95 Mhz
//     SLAVE_O2M_RX_CHANNEL        // 32.768K M        868.30 Mhz ~ 868.30 Mhz
// }ql_community_channel_e;    

// typedef struct ql_wmbus_handle
// {
//     ql_community_channel_e com_channle;
//     uint16_t com_channel_id;
// }ql_community_channel_t;

// const uint16_t m2o_tx_channel = 0;
// const uint16_t o2m_tx_channel = 1;
// const uint16_t m2o_rx_channel = 2;
// const uint16_t o2m_rx_channel = 3; 

// static volatile ql_community_channel_t g_community_channels[] = 
// {
//     {SLAVE_M2O_TX_CHANNEL,  m2o_tx_channel},
//     {MASTER_O2M_TX_CHANNEL, o2m_tx_channel},
//     {MASTER_M2O_RX_CHANNEL, m2o_rx_channel},
//     {SLAVE_O2M_RX_CHANNEL,  o2m_rx_channel}
// };

// #define COMMUNITY_CHANNLES_SIZE (sizeof(g_community_channels) / sizeof(ql_community_channel_t))

static volatile uint64_t current_rail_err = 0;

static volatile RAIL_Status_t calibration_status = 0;

static const uint8_t cryptoKey[] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                     0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff };

static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t rx_fifo[SL_FLEX_RAIL_RX_FIFO_SIZE];

static __ALIGNED(RAIL_FIFO_ALIGNMENT) uint8_t tx_buffer[SL_FLEX_RAIL_TX_FIFO_SIZE];

static volatile state_t state = S_IDLE;

static void print_rx_packets(RAIL_Handle_t rail_handle);
void ql_wmbus_rx_data_packet_parser_handle(RAIL_Handle_t rail_handle);

static RAIL_Handle_t rail_handle;

uint8_t access_number = 0U;

RAIL_Time_t last_tx_start_time = 0;
RAIL_Time_t last_tx_end_time = 0;

static uint16_t last_tx_length = 0U;

ql_wmbus_uart_recv_data_info_t g_wmbus_uart_recv_data_info = { 0 };

RAIL_Handle_t app_init_collector(void);
RAIL_Handle_t app_init_meter(void);

uint16_t ql_wmbus_protocol_compose_complete_frame_to_transmit(void);
void send_packet_at(RAIL_Handle_t rail_handle, uint16_t length, uint32_t send_at);

uint16_t ql_wmbus_auto_recv_send_data_to_transmit(ql_wmbus_auto_recv_data_info_t *data_info)
{
    if(data_info == NULL)
    {
        return;
    }

    if(data_info->payload == NULL)
    {
        return;
    }

    if(data_info->payload_len == 0)
    {
        return;
    }

    if(data_info->payload_len > SL_FLEX_RAIL_TX_FIFO_SIZE)
    {
        return;
    }
    g_ql_wmbus_frame_field->c_field = data_info->c_field;
    g_ql_wmbus_frame_field->ci_fied = data_info->ci_field;
    g_ql_wmbus_frame_field->payload = data_info->payload;
    g_ql_wmbus_frame_field->payload_len = data_info->payload_len;

    g_ql_wmbus_frame_field->len = data_info->payload_len + 2;
    
    return ql_wmbus_protocol_compose_complete_frame_to_transmit();
}

ql_wmbus_auto_recv_data_info_t g_auto_recv_data_info = { 0 };

void ql_wmbus_auto_recv_implement_callback(const ql_wmbus_auto_recv_base_dev_info_t *dev_info, const ql_wmbus_auto_recv_data_info_t *data_info)
{
    ql_printf("AUTO RECV OBJECT DEV,DEV_TYPE:%02X, DEV_ID:%08X, DEV_MANUFACTURER:%04X, DEV_VERSION:%02X\r\n",
        dev_info->device_type,
        dev_info->id,
        dev_info->manufacturer,
        dev_info->version);
    ql_printf("DATA_LEN:%d, DATA:", data_info->payload_len);
    print_blocks(data_info->payload, data_info->payload_len);

    // send data to wmbus
    memcpy(&g_auto_recv_data_info, data_info, sizeof(ql_wmbus_auto_recv_data_info_t));

    wmbus_entiy->is_auto_recv_status = 1;
}

void ql_wmbus_applcation_short_package(uint8_t *data, uint32_t data_len)
{
    if(wmbus_dev_data_frame_head_info->package_type.head_stl.ci_field == WMBUS_CI_EN13757_3_APPLICATION_SHORT)
    {
        ql_printf("AppHeaderShort:[CI:0x%02X,AccessNr:%d,Status:0x%02X,encMode:%d,Accessibility:%02X,encBlocks:%d,sync:%d]\n",
            wmbus_dev_data_frame_head_info->package_type.head_stl.ci_field,
            wmbus_dev_data_frame_head_info->package_type.head_stl.access_number,
            wmbus_dev_data_frame_head_info->package_type.head_stl.status,
            wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.mode,
            wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.accessibility,
            wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.num_of_enc_blocks,
            wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.synchronized);

        if(ql_wmbus_get_set_auto_recv_node_cnt(ql_wmbus_get_auto_recv_dev_storage()) > 0)
        {
            ql_wmbus_auto_recv_base_dev_info_t auto_recv_dev_info = 
            { 
                .device_type = wmbus_dev_data_frame_head_info->head_dll.address.detailed.device_type,
                .id = wmbus_dev_data_frame_head_info->head_dll.address.detailed.id,
                .manufacturer = wmbus_dev_data_frame_head_info->head_dll.address.detailed.manufacturer,
                .version = wmbus_dev_data_frame_head_info->head_dll.address.detailed.version
            };

            recognize_wmbus_auto_recv_is_active(ql_wmbus_get_auto_recv_dev_storage(), &auto_recv_dev_info);
            ql_printf("RECOGNIZE WMBUS AUTO RECV IS ACTIVE\r\n");
        }

        if(g_wmbus_entiy.is_cur_enc == 1)
        {
#if 1
            ql_printf("DECRYPTION DATA:\r\n");
            if(wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.mode == 5)
            {
                uint8_t iv[16];
                memcpy(iv, &(wmbus_dev_data_frame_head_info->head_dll.address.raw), 8);
                memset(iv + 8, wmbus_dev_data_frame_head_info->package_type.head_stl.access_number, 8);
                WMBUSframe_crypto5decrypt(data, data, iv, data_len);
            }
#endif
        }
    }
    ql_printf("[LINE:%d]SHORT PACKAGE PAYLOAD DATA:\r\n", __LINE__);
    print_blocks(data, data_len);
}

void ql_wmbus_applcation_long_package(uint8_t *data, uint32_t data_len)
{
    if(wmbus_dev_data_frame_head_info->package_type.head_ltl.ci_field == WMBUS_CI_EN13757_3_APPLICATION_LONG)
    {
        ql_printf("AppHeaderLong:[CI:0x%02X,AccessNr:%d,Status:0x%02X,encMode:%d,Accessibility:%02X,encBlocks:%d,sync:%d]\n",
            wmbus_dev_data_frame_head_info->package_type.head_ltl.ci_field,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.access_number,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.status,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.conf_word.mode_0_5.mode,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.conf_word.mode_0_5.accessibility,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.conf_word.mode_0_5.num_of_enc_blocks,
            wmbus_dev_data_frame_head_info->package_type.head_ltl.conf_word.mode_0_5.synchronized);
        if(g_wmbus_entiy.is_cur_enc == 1)
        {
#if 0
            if(wmbus_dev_data_frame_head_info->package_type.head_stl.conf_word.mode_0_5.mode == 5)
            {
                uint8_t iv[16];
                memcpy(iv, &(wmbus_dev_data_frame_head_info->head_dll.address.raw), 8);
                memset(iv + 8, wmbus_dev_data_frame_head_info->package_type.head_stl.access_number, 8);
                WMBUSframe_crypto5decrypt(data, data, iv, data_len);
            }
#endif
        }
    }
    print_blocks(data, data_len);
}

int16_t ql_wmbus_tx_power_convert(ql_wmbus_tx_power_e e_tx_power)
{
    int16_t tx_power = 6;
    switch (e_tx_power)
    {
        case E_TX_POWER_1:
        {
            tx_power = 1;
        }
        break;
        case E_TX_POWER_7:
        {
            tx_power = 7;
        }
        break;
        case E_TX_POWER_10:
        {
            tx_power = 10;
        }
        break;
        case E_TX_POWER_13:
        {
            tx_power = 15;
        }
        break;
        case E_TX_POWER_14:
        {
            tx_power = 15;
        }
        break;
        case E_TX_POWER_MAX:
        {
            tx_power = 15;
        }
        break;
    
    default:
         tx_power = 15;
        break;
    }
    return tx_power * 10;
}

uint16_t ql_wmbus_current_device_info_init(ql_wmbus_module_config_t *entiy)
{
    uint8_t info_buf[64] = { 0 };
    uint8_t read_info_list[] = {
        REG_DLL_M1_FIELD,
        REG_DLL_M2_FIELD, 
        REG_DLL_ID1_FIELD, 
        REG_DLL_ID2_FIELD, 
        REG_DLL_ID3_FIELD, 
        REG_DLL_ID4_FIELD, 
        REG_DLL_VER_FIELD, 
        REG_DLL_REG_FIELD
    };

    for (size_t i = 0; i < sizeof(read_info_list) / sizeof(read_info_list[0]); i++)
    {
        uint16_t t_len = 0;
        int16_t ret = ql_wmbus_param_read_single(read_info_list[i], info_buf + i, &t_len);
        if(ret != 0)
        {
            ql_printf("[APP]READ DEVICE INFO FAIL, RET:%d, FAIL VAULE ID:%02X\r\n", ret, read_info_list[i]);
            return -1;
        }
    }

    entiy->device_info.manufacturer_id  = *((uint16_t *)((uint8_t *)info_buf));
    entiy->device_info.device_identify  = *((uint32_t *)((uint8_t *)info_buf + 2));
    entiy->device_info.version          = info_buf[6];
    entiy->device_info.device_type      = info_buf[7];

    ql_printf("[APP]INIT DEVICE INFO MANUFACTURER ID:%04X, DEVICE ID:%08X, VERSION:%02X, DEVICE TYPE:%02X\r\n",
        entiy->device_info.manufacturer_id,
        entiy->device_info.device_identify,
        entiy->device_info.version,
        entiy->device_info.device_type);

    return 0;
}

uint8_t ql_wmbus_get_current_role(void)
{
    return wmbus_entiy->wmbus_role;
}

uint32_t ql_wmbus_get_tune_value(void)
{
    return wmbus_entiy->tune_value;
}

int16_t ql_wmbus_register_change(uint32_t id_event, void* payload)
{
    ql_printf("REG CHANGE EVENT:%d, %X\r\n", id_event, *((uint8_t*)payload));
    switch (id_event)
    {
        case QL_MSG_EVENT_TYPE_CHANGE_ROLE:
        {
            wmbus_entiy->wmbus_role = (ql_wmbus_role_e)*((uint8_t*)payload);
            free_data_storage(ql_wmbus_get_dev_storage());
            ql_wmbus_bind_init();
            if(wmbus_entiy->wmbus_role == E_ROLE_MASTER)
            {
                app_init_collector();
            }
            if(wmbus_entiy->wmbus_role == E_ROLE_SLAVE)
            {
                app_init_meter();
            }
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_M1_FIELD:
        {
            wmbus_entiy->device_info.manufacturer_id &= 0xFF00;
            wmbus_entiy->device_info.manufacturer_id |= *((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_M2_FIELD:
        {
            wmbus_entiy->device_info.manufacturer_id &= 0x00FF;
            wmbus_entiy->device_info.manufacturer_id |= (*((uint8_t*)payload) << 8);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_ID1_FIELD:
        {
            wmbus_entiy->device_info.device_identify &= 0xFFFFFF00;
            wmbus_entiy->device_info.device_identify |= *((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_ID2_FIELD:
        {
            wmbus_entiy->device_info.device_identify &= 0xFFFF00FF;
            wmbus_entiy->device_info.device_identify |= (*((uint8_t*)payload) << 8);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_ID3_FIELD:
        {
            wmbus_entiy->device_info.device_identify &= 0xFF00FFFF;
            wmbus_entiy->device_info.device_identify |= (*((uint8_t*)payload) << 16);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_ID4_FIELD:
        {
            wmbus_entiy->device_info.device_identify &= 0x00FFFFFF;
            wmbus_entiy->device_info.device_identify |= (*((uint8_t*)payload) << 24);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_VER_FIELD:
        {
            wmbus_entiy->device_info.version = *((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DLL_DEV_FIELD:
        {
            wmbus_entiy->device_info.device_type = *((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_INSTALL_MODE:
        {
            wmbus_entiy->install_mode = (ql_wmbus_install_mode_e)*((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_DATA_FORMAT:
        {
            wmbus_entiy->uart_data_format = (ql_uart_data_format_e)*((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_USE_RSSI:
        {
            wmbus_entiy->is_enable_rssi = (ql_wmbus_rssi_mode_e)*((uint8_t*)payload);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_TX_POWER:
        {
            wmbus_entiy->tx_power = (ql_wmbus_tx_power_e)*((uint8_t*)payload);
            int16_t tx_power_v = ql_wmbus_tx_power_convert((ql_wmbus_tx_power_e)wmbus_entiy->tx_power);
            RAIL_Status_t ret = RAIL_SetTxPower(rail_handle, tx_power_v);
            ql_printf("[APP]TX POWER CHANGE VALUE:%d, RET:%d\r\n", tx_power_v, ret);
        }
        break;
        case QL_MSG_EVENT_TYPE_CHANGE_ENCRYPTION_KEY:
        {
            switch (wmbus_entiy->wmbus_role)
            {
                case E_ROLE_MASTER:
                {

                }
                break;
                case E_ROLE_SLAVE:
                {
                    uint8_t encryption[16] = { 0 };
                    memcpy(&encryption[0], (uint8_t *)payload, 16);
                    WMBUSframe_crypto5SetKey(encryption);
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

static uint8_t match_arrays_element(wmbus_ci_field ci_arrys[], int arrys_size, int target, int index)
{
    if(index >= arrys_size)
    {
        return 0;
    }

    if(ci_arrys[index] == target)
    {
        return 1;
    }

    return match_arrays_element(ci_arrys, arrys_size, target, (index + 1));
}

static int32_t match_func_element(ql_wmbus_app_function_tables_t g_wmbus_app_func_tables[], int arrys_size, int target, int index)
{
    if(index >= arrys_size)
    {
        return -1;
    }

    if(g_wmbus_app_func_tables[index].app_ci == target)
    {
        return index;
    }

    return match_func_element(g_wmbus_app_func_tables, arrys_size, target, (index + 1));
}

static int16_t handle_master_install_mode(ql_wmbus_module_config_t *entiy, const WMBUS_dll_header_t *dll_header, const void *package_header)
{
    uint8_t package_head_ci_field = (wmbus_dev_data_frame_head_info->is_ltl == 1) ? \
        ((WMBUS_ltl_header_t *)package_header)->ci_field : ((WMBUS_stl_header_t *)package_header)->ci_field;

    switch (entiy->install_mode)
    {
        case E_INSTALL_MODE_INSTALL:
        {
            if (((dll_header->c_field.raw & 0x0F) != WMBUS_FUNCTION_SND_IR) && 
                ((dll_header->c_field.raw & 0x0F) != WMBUS_FUNCTION_SND_NR))
            {
                return -1;
            }

            if (((dll_header->c_field.raw & 0x0F) == WMBUS_FUNCTION_SND_NR) && 
                (package_head_ci_field != WMBUS_CI_DATA_TO_METER_NONE))
            {
                return -1;
            }

            ql_printf("MODE_INSTAL, DEV_TYPE:%02X, DEV_ID:%08X, DEV_MANUFACTURER:%04X, DEV_VERSION:%02X\r\n", 
                dll_header->address.detailed.device_type, 
                dll_header->address.detailed.id, 
                dll_header->address.detailed.manufacturer, 
                dll_header->address.detailed.version);
        }
        break;
        case E_INSTALL_MODE_NORMAL:
        {
            if (ql_wmbus_get_bind_cnt() <= 0)
            {
                ql_printf("[APP]NO BIND OTHER DEVICE\r\n");
                return -1;
            }

            ql_wmbus_dev_info_t dev_info = 
            {
                .device_type = dll_header->address.detailed.device_type,
                .id = dll_header->address.detailed.id,
                .manufacturer = dll_header->address.detailed.manufacturer,
                .version = dll_header->address.detailed.version
            };

            ql_printf("device_type:%02X, id:%08X, manufacturer:%04X, version:%02X\r\n",
                dev_info.device_type, dev_info.id, dev_info.manufacturer, dev_info.version);

            if (0 != check_device_is_register(ql_wmbus_get_dev_storage(), dev_info))
            {
                return -1;
            }

            int16_t dev_inedx = query_dev_wmbus_node_index(dev_info);
            int8_t is_encryption = query_dev_wmbus_node_is_encryption_by_index(dev_inedx);
            uint16_t encry_flag = 0;
            query_encryption_flag_by_index(dev_inedx, &encry_flag);
            ql_printf("[APP]ENCRYPTION INFO, DEV INDEX:%d, IS ENCRYPTION:%d, ENCRY FLAG:%04X\r\n", dev_inedx, is_encryption, encry_flag);
            if((dev_inedx >= 0) && (is_encryption == 1) && (encry_flag == 0x8000))
            {
                uint8_t encryption[16] = { 0 };
                int8_t ret = query_encryption_by_index(dev_inedx, encryption);
                if(ret == 0)
                {
                    g_wmbus_entiy.is_cur_enc = 1;
                    WMBUSframe_crypto5SetKey(encryption);
                }
            }
            else
            {
                g_wmbus_entiy.is_cur_enc = 0;
            }
        }
        break;
        case E_INSTALL_MODE_LISTEN:
        {

        }
        break;
    }

    return 0;
}

static void handle_slave_install_mode(ql_wmbus_module_config_t *entiy)
{
    switch (entiy->install_mode)
    {
        case E_INSTALL_MODE_INSTALL:
        {

        }
        break;
        case E_INSTALL_MODE_LISTEN:
        {

        }
        break;
        case E_INSTALL_MODE_NORMAL:
        {

        }
        break;
    }
}

void ql_wmbus_protocol_parser_handle(RAIL_RxPacketDetails_t *package_detial_info, uint8_t *frame_data, uint32_t frame_length)
{
    uint8_t is_match_head = 0;
    memset((void *)wmbus_dev_data_frame_head_info, 0x00, sizeof(ql_wmbus_dev_data_frame_head_info_t));
    
    const WMBUS_dll_header_t *dll_header = (WMBUS_dll_header_t*)frame_data;
    const wmbus_ci_field APP_CI = frame_data[sizeof(WMBUS_dll_header_t)];
    const WMBUS_stl_header_t *stl_header = NULL;
    const WMBUS_ltl_header_t *ltl_header = NULL;
    uint8_t *payload_prt = NULL;
    uint32_t payload_len = 0;

    memcpy((void *)&wmbus_dev_data_frame_head_info->head_dll, dll_header, sizeof(WMBUS_dll_header_t));
    if(match_arrays_element(app_ltl_ci_tab, sizeof(app_ltl_ci_tab), APP_CI, 0) == 1)
    {
        is_match_head = 1;
        wmbus_dev_data_frame_head_info->is_ltl = 0;
        stl_header = (WMBUS_stl_header_t*)(frame_data + sizeof(WMBUS_dll_header_t));
        memcpy((void *)&wmbus_dev_data_frame_head_info->package_type.head_stl, (void *)stl_header, sizeof(WMBUS_stl_header_t));
        payload_prt = frame_data + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_stl_header_t);
        payload_len = dll_header->lField - sizeof(WMBUS_dll_header_t) - sizeof(WMBUS_stl_header_t) + 1;
    }
    else if(match_arrays_element(app_stl_ci_tab, sizeof(app_stl_ci_tab), APP_CI, 0) == 1)
    {
        is_match_head = 1;
        wmbus_dev_data_frame_head_info->is_ltl = 1;
        ltl_header = (WMBUS_ltl_header_t*)(frame_data + sizeof(WMBUS_dll_header_t));
        memcpy((void *)&wmbus_dev_data_frame_head_info->package_type.head_ltl, (void *)ltl_header, sizeof(WMBUS_ltl_header_t));
        payload_prt = frame_data + sizeof(WMBUS_dll_header_t) + sizeof(WMBUS_ltl_header_t);
        payload_len = dll_header->lField - sizeof(WMBUS_dll_header_t) - sizeof(WMBUS_ltl_header_t) + 1;
    }
    else
    {
        is_match_head = 0;
        ql_printf("[APP]CI FIELD NOT MATCH\r\n");
    }

    if(is_match_head == 1)
    {
        char mField[3];
        WMBUSframe_MField2Chars(dll_header->address.detailed.manufacturer, mField);
        ql_printf("RX:[Time:%lu]\n", package_detial_info->timeReceived.packetTime);
        ql_printf("Block-1:[L:%d,C:0x%02X,M:%c%c%c,ID:%08X,Version:0x%02X,devType:0x%02X]\n",
                    dll_header->lField,
                    dll_header->c_field.raw,
                    mField[0], mField[1], mField[2],
                    (unsigned int)dll_header->address.detailed.id,
                    (unsigned int)dll_header->address.detailed.version,
                    (unsigned int)dll_header->address.detailed.device_type);
    
        if (g_wmbus_entiy.wmbus_role == E_ROLE_MASTER)
        {
            void *packages_head = ((wmbus_dev_data_frame_head_info->is_ltl == 1) ? ltl_header : stl_header);
            if(handle_master_install_mode(wmbus_entiy, dll_header, packages_head) != 0)
            {
                return;
            }
        }
        else if (g_wmbus_entiy.wmbus_role == E_ROLE_SLAVE)
        {
            handle_slave_install_mode(wmbus_entiy);
        }
        else
        {
            // Repeat
        }
    
        int32_t match_index = -1;
        if((match_index = match_func_element(g_wmbus_app_func_tables, QL_WMBUS_APP_TABLES_SIZE, APP_CI, 0)) >= 0)
        {
            g_wmbus_app_func_tables[match_index].app_func(payload_prt, payload_len);
        }
    }

    if(wmbus_entiy->is_enable_rssi == E_RSSI_MODE_ENABLE)
    {
        frame_data[0] += 1;
        frame_data[frame_length] = package_detial_info->rssi;
        frame_length += 1;
    }

    ql_encode_data_interface(frame_data, frame_length);
}

void ql_wmbus_role_handle(ql_wmbus_module_config_t *entiy)
{
    uint8_t t_buf[10];
    uint16_t t_len = 0;
    int16_t ret = ql_wmbus_param_read_single(REG_ROLE, t_buf, &t_len);

    if(ret != 0)
    {
        ql_printf("[APP]READ MOUDLE ROLE FAIL\r\n");
        return ;
    }

    switch (t_buf[0])
    {
        case E_ROLE_MASTER:
        {
            rail_handle = app_init_collector();
            entiy->wmbus_role = E_ROLE_MASTER;
            ql_printf("[APP]ROLE:MASTER\r\n");
        }
        break;
        case E_ROLE_SLAVE:
        {
            rail_handle = app_init_meter();
            entiy->wmbus_role = E_ROLE_SLAVE;
            ql_printf("[APP]ROLE:SLAVE\r\n");
        }
        break;
        case E_ROLE_REPEATER:
        {
            entiy->wmbus_role = E_ROLE_REPEATER;
            ql_printf("[APP]ROLE:REPEATER\r\n");
        }
        break;
    
    default:
        entiy->wmbus_role = E_ROLE_MASTER;
        break;
    }
}

void ql_wmbus_is_use_rssi_handle(ql_wmbus_module_config_t *entiy)
{
    uint8_t t_buf[10];
    uint16_t t_len = 0;
    int16_t ret = ql_wmbus_param_read_single(REG_USE_RSSI, t_buf, &t_len);
    if(ret != 0)
    {
        ql_printf("[APP]READ OUTPUT POWER FAIL\r\n");
        return ;
    }

    switch (t_buf[0])
    {
        case E_RSSI_MODE_DISABLE:
        {
            entiy->is_enable_rssi = E_RSSI_MODE_DISABLE;
        }
        break;
        case E_RSSI_MODE_ENABLE:
        {
            entiy->is_enable_rssi = E_RSSI_MODE_ENABLE;
        }
        break;
    
    default:
        entiy->is_enable_rssi = E_RSSI_MODE_DISABLE;
        break;
    }

}

void ql_wmbus_tune_handle(ql_wmbus_module_config_t *entiy)
{    
    uint8_t config_info[64];
    memcpy(config_info, (uint8_t *)QL_MODULE_CONFIG_BASE_ADDR, 64);
    ql_module_config_t *module_config = &config_info;
    uint32_t default_tune = module_config->ctune_value;

    if(module_config->ctune_value > 120 || module_config->ctune_value < 90)
    {
        default_tune = 100;
    }

    RAIL_Status_t ret = RAIL_SetTune(rail_handle, default_tune);
    ql_printf("[APP]CONFIG TUNE VAULE:%d, RET:%d\r\n", default_tune, ret);
}

void ql_wmbus_output_power_handle(ql_wmbus_module_config_t *entiy)
{
    uint8_t t_buf[10] = { 0 };
    uint16_t t_len = 0;
    int16_t ret = ql_wmbus_param_read_single(REG_TX_POWER, t_buf, &t_len);

    if(ret != 0)
    {
        ql_printf("[APP]READ OUTPUT POWER FAIL\r\n");
        return ;
    }
    switch (t_buf[0])
    {
        case E_TX_POWER_FU5:
        {
            entiy->tx_power = E_TX_POWER_FU5;
        }
        break;
        case E_TX_POWER_FU2:
        {
            entiy->tx_power = E_TX_POWER_FU2;
        }
        break;
        case E_TX_POWER_1:
        {
            entiy->tx_power = E_TX_POWER_1;
        }
        break;
        case E_TX_POWER_7:
        {
            entiy->tx_power = E_TX_POWER_7;
        }
        break;
        case E_TX_POWER_10:
        {
            entiy->tx_power = E_TX_POWER_10;
        }
        break;
        case E_TX_POWER_13:
        {
            entiy->tx_power = E_TX_POWER_13;
        }
        break;
        case E_TX_POWER_14:
        {
            entiy->tx_power = E_TX_POWER_14;
        }
        break;
    default:
         entiy->tx_power = E_TX_POWER_MAX;
        break;
    }

    uint16_t tx_power_v =  ql_wmbus_tx_power_convert((ql_wmbus_tx_power_e)entiy->tx_power);
    ret = RAIL_SetTxPower(rail_handle, tx_power_v);
    ql_printf("[APP]TX POWER VALUE:%d, RET:%d\r\n", tx_power_v, ret);
}

void ql_wmbus_install_mode_handle(ql_wmbus_module_config_t *entiy)
{
    uint8_t t_buf[10] = { 0 };
    uint16_t t_len = 0;
    int16_t ret = ql_wmbus_param_read_single(REG_INSTALL_MODE, t_buf, &t_len);

    if(ret != 0)
    {
        ql_printf("[APP]READ MOUDLE INSTALL MODE FAIL\r\n");
        return ;
    }

    switch (t_buf[0])
    {
        case E_INSTALL_MODE_NORMAL:
        {
            entiy->install_mode = E_INSTALL_MODE_NORMAL;
            ql_printf("[APP]INSTALL MODE:NORMAL\r\n");
        }
        break;
        case E_INSTALL_MODE_INSTALL:
        {
            entiy->install_mode = E_INSTALL_MODE_INSTALL;
            ql_printf("[APP]INSTALL MODE:INSTALL\r\n");
        }
        break;
        case E_INSTALL_MODE_LISTEN:
        {
            entiy->install_mode = E_INSTALL_MODE_LISTEN;
            ql_printf("[APP]INSTALL MODE:LISTEN\r\n");
        }
        break;
    default:
        entiy->install_mode = E_INSTALL_MODE_LISTEN;
        break;
    }
}

void ql_wmbus_uart_data_format_handle(ql_wmbus_module_config_t *entiy)
{
    uint8_t t_buf[10] = { 0 };
    uint16_t t_len = 0;
    int16_t ret = ql_wmbus_param_read_single(REG_UART_DATA_FORMAT, t_buf, &t_len);

    if(ret != 0)
    {
        ql_printf("[APP]READ MOUDLE UART DATA FORMATE FAIL\r\n");
        return ;
    }

    switch (t_buf[0])
    {
        case E_DATA_FORMAT_DATA_ID:
        {
            entiy->uart_data_format = E_DATA_FORMAT_DATA_ID;
        }
        break;
        case E_DATA_FORMAT_APP:
        {
            entiy->uart_data_format = E_DATA_FORMAT_APP;
        }
        break;
        case E_DATA_FORMAT_START_END:
        {
            entiy->uart_data_format = E_DATA_FORMAT_START_END;
            ql_printf("[APP]DATA FORMAT:START END\r\n");
        }
        break;
        case E_DATA_FORMAT_CRC:
        {
            entiy->uart_data_format = E_DATA_FORMAT_CRC;
        }
        break;
        case E_DATA_FORMAT_START_END_CRC:
        {
            entiy->uart_data_format = E_DATA_FORMAT_START_END_CRC;
        }
        break;

    default:
            entiy->uart_data_format = E_DATA_FORMAT_START_END;
        break;
    }
}

void set_next_state(state_t next_state)
{
    state = next_state;
}

RAIL_Handle_t ql_wmbus_get_instance_rail_handle(void)
{
    // RAIL_Handle_t rail_handle = sl_rail_util_get_handle(SL_RAIL_UTIL_HANDLE_INST0);
    return ql_wmbus_rf_rail_get_handle();
}

RAIL_Handle_t app_init_collector(void)
{
    // for (size_t i = 0; i < COMMUNITY_CHANNLES_SIZE; i++)
    // {
    //     RAIL_Status_t ret = RAIL_IsValidChannel(rail_handle, g_community_channels[i].com_channel_id);
    //     if(RAIL_STATUS_NO_ERROR == ret)
    //     {
    //         ql_printf("[APP]CHANNEL ID:%d, CHANNEL CHECK OK!\r\n", i);
    //     }
    // } 

    RAIL_WMBUS_Config(rail_handle, true);
    set_next_state(S_IDLE);

    // Initialization of WMBUS
    WMBUS_Init();

    // Get the mode of WMBUS
    WMBUS_Mode_t mode = WMBUS_GetSelectedMode();

    if (get_wmbus_accessibility() == WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS) 
    {
        RAIL_StateTransitions_t transitions =
        {
            .error = RAIL_RF_STATE_RX,
            .success = RAIL_RF_STATE_RX,
        };
        RAIL_SetRxTransitions(rail_handle, &transitions);
    }

    // calibrate IR on rx channel before starting the main loop, since it takes longer than most rx operation
    // RAIL_PrepareChannel(rail_handle, g_community_channels[MASTER_M2O_RX_CHANNEL].com_channel_id);
    RAIL_PrepareChannel(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx);

    RAIL_CalibrateIr(rail_handle, NULL);

    WMBUSframe_crypto5Init();
    WMBUSframe_crypto5SetKey(cryptoKey);

    RAIL_Idle(rail_handle, RAIL_IDLE, true);
    // RAIL_StartRx(rail_handle, g_community_channels[MASTER_M2O_RX_CHANNEL].com_channel_id, NULL);
    RAIL_StartRx(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx, NULL);
    
    access_number = (uint8_t)(rand() % 256);

    //set_next_state(S_SCHEDULE_TX);
    last_tx_start_time = RAIL_GetTime();

    return rail_handle;
}

RAIL_Handle_t app_init_meter(void)
{
    // for (size_t i = 0; i < COMMUNITY_CHANNLES_SIZE; i++)
    // {
    //     RAIL_Status_t ret = RAIL_IsValidChannel(rail_handle, g_community_channels[i].com_channel_id);
    //     if(RAIL_STATUS_NO_ERROR == ret)
    //     {
    //         ql_printf("[APP]CHANNEL ID:%d, CHANNEL CHECK OK!\r\n", i);
    //     }
    // } 

    // RAIL_WMBUS_Config(rail_handle, true);
    set_next_state(S_IDLE);

    // Initialization of WMBUS
    WMBUS_Init();

    // Get the mode of WMBUS
    WMBUS_Mode_t mode = WMBUS_GetSelectedMode();

    if (get_wmbus_accessibility() == WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS) 
    {
        RAIL_StateTransitions_t transitions =
        {
            .error = RAIL_RF_STATE_RX,
            .success = RAIL_RF_STATE_RX,
        };
        RAIL_SetRxTransitions(rail_handle, &transitions);
    }

    // //calibrate IR on rx channel before starting the main loop, since it takes longer than most rx operation
    RAIL_PrepareChannel(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx);
    RAIL_CalibrateIr(rail_handle, NULL);

    WMBUSframe_crypto5Init();
    WMBUSframe_crypto5SetKey(cryptoKey);

    RAIL_Idle(rail_handle, RAIL_IDLE, true);
    RAIL_PrepareChannel(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx);

    access_number = (uint8_t)(rand() % 256);

    //set_next_state(S_SCHEDULE_TX);
    last_tx_start_time = RAIL_GetTime();

    return rail_handle;
}

int16_t ql_wmbus_init(void)
{
    ql_wmbus_rf_channel_init();
    ql_wmbus_register_change_event_register(ql_wmbus_register_change);
    ql_wmbus_register_auto_recv_callback(ql_wmbus_auto_recv_implement_callback);
    ql_wmbus_auto_recv_init();
    ql_wmbus_bind_init();
    ql_wmbus_bind_encryption_init();
    rail_handle = ql_wmbus_get_instance_rail_handle();
    ql_wmbus_install_mode_handle(wmbus_entiy);
    ql_wmbus_uart_data_format_handle(wmbus_entiy);
    ql_wmbus_output_power_handle(wmbus_entiy);
    ql_wmbus_role_handle(wmbus_entiy);
    ql_wmbus_current_device_info_init(wmbus_entiy);
    RAIL_TxPowerConfig_t txPowerConfig;
    RAIL_GetTxPowerConfig(rail_handle, &txPowerConfig);
    wmbus_entiy->tune_value = RAIL_GetTune(rail_handle);
    ql_printf("[APP]TX POWER CONIFG MODE:%d, RAMPTIME:%d, VOLTAGE:%d\r\n", txPowerConfig.mode, txPowerConfig.rampTime, txPowerConfig.voltage);
    ql_printf("[APP]RADIO IS SUPPORT SUB-G BAND:%d\r\n", RAIL_SupportsSubGHzBand(rail_handle));
    ql_printf("[APP]RADIO TX POWER:%d, TUNE:%d\r\n", RAIL_GetTxPower(rail_handle), wmbus_entiy->tune_value);
    return 0;
}

void print_arrs(uint8_t arr[], int size, int i, int type) {
    if(i == 0 && type == 0) {
        ql_printf("TRANSMIT DATA FOR WIRELESS PORT:\r\n");
    }
    if(i == 0 && type == 1) {
        ql_printf("WIRELESS PORT RECV DATA:\r\n");
    }
    if (i < size) {
        ql_printf("%02X ", arr[i]);
        print_arr(arr, size, i + 1);
    }
}

uint16_t ql_wmbus_setup_dll_frame_head(ql_wmbus_frame_device_info_t *dev_info , uint8_t *buf)
{
    if(!buf)
    {
        ql_printf("[APP]SETUP DLL FRAME HEAD FAIL, BUF IS NULL\r\n");
        return 0;
    }
    uint16_t off_set = 0;
    uint8_t *dst = buf;

    uint8_t *manuf_id_bit = (uint8_t *)&dev_info->manufacturer_id;
    dst[off_set++] = manuf_id_bit[0];
    dst[off_set++] = manuf_id_bit[1];
    
    uint8_t *dev_id_bit = (uint8_t *)&dev_info->device_identify;
    dst[off_set++] = dev_id_bit[0];
    dst[off_set++] = dev_id_bit[1];
    dst[off_set++] = dev_id_bit[2];
    dst[off_set++] = dev_id_bit[3];

    dst[off_set++] = dev_info->version;
    dst[off_set++] = dev_info->device_type;

    return off_set;
}

uint16_t ql_wmbus_protocol_compose_complete_frame_to_transmit(void)
{
    uint32_t data_len = 0;
    memset(tx_buffer, 0x00, sizeof(tx_buffer));
    tx_buffer[data_len++] = g_ql_wmbus_frame_field->len + sizeof(ql_wmbus_frame_device_info_t);
    tx_buffer[data_len++] = g_ql_wmbus_frame_field->c_field;

    data_len += ql_wmbus_setup_dll_frame_head(&wmbus_entiy->device_info, (tx_buffer + data_len));

    tx_buffer[data_len++] = g_ql_wmbus_frame_field->ci_fied;

    memcpy(tx_buffer + data_len, g_ql_wmbus_frame_field->payload, g_ql_wmbus_frame_field->payload_len);

    data_len += g_ql_wmbus_frame_field->payload_len;

    return data_len;
}

uint16_t ql_wmbus_input_protocol_recognize_and_parser(uint8_t *data, uint32_t data_len)
{
    uint8_t *p_data = data;
    uint32_t p_data_len = data_len;

    if(p_data == NULL || p_data_len <= 0)
    {
        return -1;
    }

    if(p_data_len < 2)
    {
        return -1;
    }

    uint8_t protocol_len = p_data[0];
    uint8_t c_field = p_data[1];
    uint8_t ci_field = p_data[2];

    g_ql_wmbus_frame_field->len = protocol_len;
    g_ql_wmbus_frame_field->c_field = c_field;
    g_ql_wmbus_frame_field->ci_fied = ci_field;

    g_ql_wmbus_frame_field->payload = p_data + 3;
    g_ql_wmbus_frame_field->payload_len = protocol_len - 2;

    if((ql_wmbus_get_encryption_bind_cnt() > 0) && (g_wmbus_entiy.wmbus_role == E_ROLE_SLAVE))
    {   
        uint32_t slave_index = 1;
        uint16_t encryption_flag = 0;
        int16_t ret = query_encryption_flag_by_index(slave_index, &encryption_flag);
        if(ret == 0)
        {
            
        }
    }

    ql_wmbus_frame_c_field_t *c_field_info = (ql_wmbus_frame_c_field_t *)&c_field;

    if(!match_arrays_element(app_stl_ci_tab, sizeof(app_stl_ci_tab), ci_field, 0))
    {
        WMBUS_stl_header_t *stl_header = (WMBUS_stl_header_t *)(p_data + 2);
    }
    else if(!match_arrays_element(app_ltl_ci_tab, sizeof(app_ltl_ci_tab), ci_field, 0))
    {
        WMBUS_ltl_header_t *ltl_header = (WMBUS_ltl_header_t *)(p_data + 2);
    }
    else
    {
        ql_printf("[APP]CI FIELD NOT MATCH\r\n");
    }

    ql_printf("PROTOCOL LEN:%d, C_FIELD:0x%02X, CI_FIELD:0x%02X\r\n", protocol_len, c_field, ci_field);
    ql_printf("C FIELD->FUNCTION CODE:%02X, PRIMARY:%d, FCB_ACD:%d, FCV_DCF:%d\r\n", 
        c_field_info->function_code, 
        c_field_info->primary, 
        c_field_info->fcb_acd, 
        c_field_info->fcv_dcf);

    return ql_wmbus_protocol_compose_complete_frame_to_transmit();
}

static const uint32_t wmbus_app_period_nom = 10e6; //10s
static uint32_t wmbus_app_period_acc = 2000e3;
static const uint32_t response_delay_safety_margin = 200U;

void ql_wmbus_handle_loop(void)
{
    (void) rail_handle;
    uint64_t current_rail_err_tmp = current_rail_err;
    switch (state) 
    {
        case S_PACKET_RECEIVED:
        {
            ql_printf("S_PACKET_RECEIVED\r\n");
            ql_wmbus_rx_data_packet_parser_handle(rail_handle);
            state = S_IDLE;
            if(wmbus_entiy->is_auto_recv_status == 1)
            {
                state = S_AUTO_RECV_TX;
            }
        }
        break;
        case S_RX_PACKET_ERROR:
        {
            // Handle Rx error
            //ql_printf("Radio RX Error occurred\nEvents: %ld\n", current_rail_err_tmp);
            state = S_IDLE;
        }
        break;
        case S_CALIBRATION_ERROR:
        {
            ql_printf("Radio Calibration Error occurred\nEvents: %lld\nRAIL_Calibrate() result:%d\n",
                    current_rail_err_tmp,
                    calibration_status);
            state = S_IDLE;
        }
        break;
        case S_AUTO_RECV_TX:
        {
            ql_printf("S_AUTO_RECV_TX\r\n");
            uint16_t send_len = ql_wmbus_auto_recv_send_data_to_transmit(&g_auto_recv_data_info);
            send_packet_at(rail_handle, send_len, last_tx_start_time + wmbus_app_period_acc);
            wmbus_entiy->is_auto_recv_status = 0;
            state = S_IDLE;
        }
        break;
        case S_SCHEDULE_TX:
        {
            uint16_t length = ql_wmbus_input_protocol_recognize_and_parser(g_wmbus_uart_recv_data_info.wmbus_uart_recv_data, g_wmbus_uart_recv_data_info.wmbus_uart_recv_len);
            send_packet_at(rail_handle, length, last_tx_start_time + wmbus_app_period_acc);
            state = S_IDLE;
        }
        break;
        case S_TX_DONE:
        {      //let's setup the time for the next tx.
            //     /*
            //     * from EN13757-4:
            //     * tACC=1+(|nACC-128|-64)/2048*tNOM
            //     * a bit easier to calculate:
            //     * tACC=(2048+|nACC-128|-64)*tNOM/2048
            //     */
            // access_number++;
            // wmbus_app_period_acc = access_number > 128 ? access_number - 128 : 128 - access_number;       //abs(gAccessNumber-128)
            // wmbus_app_period_acc = (uint64_t)(2048 + wmbus_app_period_acc - 64) * wmbus_app_period_nom / 2048;
            // switch (get_wmbus_accessibility()) 
            // {
            //     case WMBUS_ACCESSIBILITY_LIMITED_ACCESS: state = S_RESPONSE_DELAY;
            //     break;
            //     case WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS: state = S_UNLIMITED_ACCESS;
            //     break;
            //     // no RX between TXes, schedule the next TX immediately
            //     default:
            //     state = S_IDLE;
            //     break;
            // }
            ql_printf("SET RAIL TO IDLE\r\n");
            RAIL_Idle(rail_handle, RAIL_IDLE, true);
            RAIL_StartRx(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx, NULL);   

            // if(g_wmbus_entiy.wmbus_role == E_ROLE_MASTER)
            // {
            //     // RAIL_StartRx(rail_handle, g_community_channels[MASTER_M2O_RX_CHANNEL].com_channel_id, NULL);
            //     RAIL_StartRx(rail_handle, ql_wmbus_rf_channel_get_instances().channel_rx, NULL);
            // }
            // else if(g_wmbus_entiy.wmbus_role == E_ROLE_SLAVE)
            // {   
            //     RAIL_StartRx(rail_handle, g_community_channels[SLAVE_O2M_RX_CHANNEL].com_channel_id, NULL);
            // }
            // else
            // {

            // }

            state = S_IDLE;
        }
        break;
        // case S_RESPONSE_DELAY:
        // {
        //     RAIL_ScheduleRxConfig_t schedule = {
        //         .start = last_tx_end_time + WMBUS_getMeterLimitedAccRxStart(false) - response_delay_safety_margin,
        //         .startMode = RAIL_TIME_ABSOLUTE,
        //         .end = last_tx_end_time + WMBUS_getMeterLimitedAccRxStop(false) + response_delay_safety_margin,
        //         .endMode = RAIL_TIME_ABSOLUTE,
        //         .hardWindowEnd = 0,            // extend RX window if we're receiving
        //     };
        //     RAIL_Idle(rail_handle, RAIL_IDLE, true);
        //     RAIL_ScheduleRx(rail_handle, g_community_channles[SLAVE_O2M_RX_CHANNEL].com_channle_id, &schedule, NULL);
        //     // RX is scheduled, sleep can be enabled as RAIL deals with power manager
        //     state = S_IDLE_OR_RX;
        // }
        // break;

        case S_IDLE:
        break;
    default:
      break;
    }
}

void sl_rail_util_on_event(RAIL_Handle_t rail_handle, RAIL_Events_t events)
{
    if(g_wmbus_entiy.wmbus_role == E_ROLE_MASTER)
    {
        if (events & RAIL_EVENTS_RX_COMPLETION) 
        {
            if (events & RAIL_EVENT_RX_PACKET_RECEIVED) 
            {
                // Keep the packet in the radio buffer, download it later at the state machine
                RAIL_HoldRxPacket(rail_handle);
                state = S_PACKET_RECEIVED;
            } 
            else 
            {
                // Handle Rx error
                current_rail_err = (events & RAIL_EVENTS_RX_COMPLETION);
                state = S_RX_PACKET_ERROR;
            }
        }

        if (events & RAIL_EVENT_TX_STARTED) 
        {
            RAIL_GetTxTimePreambleStart(rail_handle, RAIL_TX_STARTED_BYTES, &last_tx_start_time);
        }

        if (events & RAIL_EVENTS_TX_COMPLETION) 
        {
            if (events & RAIL_EVENT_TX_PACKET_SENT) 
            {
                RAIL_GetTxPacketDetailsAlt(rail_handle, false, &last_tx_end_time);
                RAIL_GetTxTimeFrameEnd(rail_handle, last_tx_length, &last_tx_end_time);
            }
            state = S_TX_DONE;
        }
    }

    if(g_wmbus_entiy.wmbus_role == E_ROLE_SLAVE)
    {
        if (events & RAIL_EVENTS_RX_COMPLETION) 
        {
            if (events & RAIL_EVENT_RX_PACKET_RECEIVED) 
            {
                // Keep the packet in the radio buffer, download it later at the state machine
                RAIL_HoldRxPacket(rail_handle);
                state = S_PACKET_RECEIVED;
            } 
            else
            {
                // Handle Rx error
                current_rail_err = (events & RAIL_EVENTS_RX_COMPLETION);
                state = S_RX_PACKET_ERROR;
            }
        }

        if (events & RAIL_EVENT_TX_STARTED) 
        {
            RAIL_GetTxTimePreambleStart(rail_handle, RAIL_TX_STARTED_BYTES, &last_tx_start_time);
        }

        if (events & RAIL_EVENTS_TX_COMPLETION) 
        {
            if (events & RAIL_EVENT_TX_PACKET_SENT) 
            {
                RAIL_GetTxPacketDetailsAlt(rail_handle, false, &last_tx_end_time);
                RAIL_GetTxTimeFrameEnd(rail_handle, last_tx_length, &last_tx_end_time);
            }
            state = S_TX_DONE;
        }
    }

    // Perform all calibrations when needed
    if (events & RAIL_EVENT_CAL_NEEDED) 
    {
        calibration_status = RAIL_Calibrate(rail_handle, NULL, RAIL_CAL_ALL_PENDING);
        if (calibration_status != RAIL_STATUS_NO_ERROR) 
        {
            current_rail_err = (events & RAIL_EVENT_CAL_NEEDED);
            state = S_CALIBRATION_ERROR;
        }
    }
}

static void print_blocks(const uint8_t *buffer, uint16_t length)
{
    for (uint8_t i = 0; i < length; i++) {
        if (i % 16 == 0) {
            ql_printf("[0x%02X ", buffer[i]);
        } else if (i % 16 == 8) {
            ql_printf("| 0x%02X ", buffer[i]);
        } else if (i % 16 == 15 || i == length - 1) {
            ql_printf("0x%02X]\n", buffer[i]);
        } else {
            ql_printf("0x%02X ", buffer[i]);
        }
    }
}


void ql_encode_data_interface(uint8_t *buf, int buf_len)
{
    uint8_t uart_tx_buf[SL_FLEX_RAIL_TX_FIFO_SIZE];
    uint32_t uart_tx_len = 0;

    if (0 == buf_len || buf == NULL)
    {
        return;
    }

    switch (wmbus_entiy->uart_data_format)
    {
        case E_DATA_FORMAT_DATA_ID:
        {
            // 直接输出
        }
        break;
        case E_DATA_FORMAT_APP:
        {
            memcpy(uart_tx_buf + uart_tx_len, &buf[sizeof(WMBUS_dll_header_t)], buf_len - sizeof(WMBUS_dll_header_t));
            uart_tx_len += (buf_len - sizeof(WMBUS_dll_header_t));
        }
        break;
        case E_DATA_FORMAT_START_END:
        {
            uart_tx_buf[uart_tx_len++] = 0x68;
            memcpy(uart_tx_buf + uart_tx_len, buf, buf_len); //拷贝数据帧
            uart_tx_len += buf_len;
            uart_tx_buf[uart_tx_len++] = 0x16;
        }
        break;
        case E_DATA_FORMAT_CRC:
        {

        }
        break;
        case E_DATA_FORMAT_START_END_CRC:
        {

        }
        break;
    default:
        break;
    }   
    
    if(uart_tx_len > 0)
    {
        ql_uart_write(QL_UART_UART1_PORT, uart_tx_buf, uart_tx_len);
    }      
}

void ql_wmbus_rx_data_packet_parser_handle(RAIL_Handle_t rail_handle)
{
    uint16_t                rx_len = 0;
    RAIL_Status_t           rail_status;
    RAIL_RxPacketInfo_t     packet_info;
    RAIL_RxPacketHandle_t   rx_packet_handle;
    RAIL_RxPacketDetails_t  packet_details;

    rx_packet_handle = RAIL_GetRxPacketInfo(rail_handle, RAIL_RX_PACKET_HANDLE_OLDEST_COMPLETE, &packet_info);

    while (rx_packet_handle != RAIL_RX_PACKET_HANDLE_INVALID) 
    {
        packet_details.timeReceived.totalPacketBytes = packet_info.packetBytes;
        packet_details.timeReceived.timePosition = RAIL_PACKET_TIME_AT_SYNC_END;

        RAIL_GetRxPacketDetails(rail_handle, rx_packet_handle, &packet_details);

        rx_len = ((packet_info.lastPortionData != NULL) ? packet_info.packetBytes : packet_info.firstPortionBytes);

        RAIL_CopyRxPacket(rx_fifo, &packet_info);

        rail_status = RAIL_ReleaseRxPacket(rail_handle, rx_packet_handle);

        if (rail_status != RAIL_STATUS_NO_ERROR) 
        {
            ql_printf("RELEASE RX PACKET ERROR, ERROR CODE:%d\r\n", rail_status);
            return 0;
        }

        ql_printf("[LINE:%d] RX PACKET RECV\r\n", __LINE__);
        print_blocks(rx_fifo, rx_len);
        ql_wmbus_protocol_parser_handle(&packet_details, rx_fifo, rx_len);

        rx_packet_handle = RAIL_GetRxPacketInfo(rail_handle, RAIL_RX_PACKET_HANDLE_OLDEST_COMPLETE, &packet_info);
    }  
}

extern RAIL_Time_t mark_recv_start_time;

RAIL_Time_t mark_recv_end_time = 0;

void send_packet_at(RAIL_Handle_t rail_handle, uint16_t length, uint32_t send_at)
{
    ql_printf("[LINE:%d]TX PACKET SEND\r\n", __LINE__);
    //print_blocks(tx_buffer, length);
    
    last_tx_length = WMBUS_phy_software(tx_buffer, (uint8_t) length, SL_FLEX_RAIL_TX_FIFO_SIZE);
    RAIL_SetTxFifo(rail_handle, tx_buffer, last_tx_length, SL_FLEX_RAIL_TX_FIFO_SIZE);
    if ( last_tx_length != length ) 
    {
        RAIL_SetFixedLength(rail_handle, last_tx_length);
    } 
    else 
    {
        RAIL_SetFixedLength(rail_handle, RAIL_SETFIXEDLENGTH_INVALID);
    }

    RAIL_StartTx(rail_handle,  ql_wmbus_rf_channel_get_instances().channel_tx, RAIL_TX_OPTIONS_DEFAULT, NULL);
    // if(g_wmbus_entiy.wmbus_role == E_ROLE_MASTER)
    // {
    //     // RAIL_StartTx(rail_handle, g_community_channels[MASTER_O2M_TX_CHANNEL].com_channel_id, RAIL_TX_OPTIONS_DEFAULT, NULL);
    //     RAIL_StartTx(rail_handle,  ql_wmbus_rf_channel_get_instances().channel_tx, RAIL_TX_OPTIONS_DEFAULT, NULL);
    //     mark_recv_end_time = RAIL_GetTime();
    //     ql_printf("START:%ld us, END: %ld us, DIF: %ld us\r\n", mark_recv_start_time, mark_recv_end_time, (mark_recv_end_time - mark_recv_start_time));
    // }
    // else if(g_wmbus_entiy.wmbus_role == E_ROLE_SLAVE)
    // {
    //     ql_printf("SLAVE, RAIL_StartTx\r\n");
    //     RAIL_StartTx(rail_handle, g_community_channels[SLAVE_M2O_TX_CHANNEL].com_channel_id, RAIL_TX_OPTIONS_DEFAULT, NULL);
    // }
    // else
    // {

    // }
}