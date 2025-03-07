#ifndef __QL_WMBUS_RF_CHANNEL_CFG_H__
#define __QL_WMBUS_RF_CHANNEL_CFG_H__

#include "ql_include.h"
#include "rail_types.h"

// T C R S N F
typedef enum 
{
    TRANSMIT_MODE_T = 0,
    TRANSMIT_MODE_C,
    TRANSMIT_MODE_R,
    TRANSMIT_MODE_S,
    TRANSMIT_MODE_N,
    TRANSMIT_MODE_F
}ql_wmbus_transmit_mode_e;

typedef enum
{
    MODE_T_INDEX_0 = 0,
    MODE_T_INDEX_1,
    MODE_T_INDEX_2
}ql_wmbus_mode_t_index_e;

typedef enum
{
    MODE_C_INDEX_0 = 0,
    MODE_C_INDEX_1,
    MODE_C_INDEX_2,
    MODE_C_INDEX_3
}ql_wmbus_mode_c_index_e;

// T1 or T2, C1 or C2
typedef enum 
{
    MODE_TYPE_1 = 1,
    MODE_TYPE_2 = 2,
}ql_wmbus_transmit_mode_type_e;

typedef enum 
{
    MASTER_TRANS_DIR_O2M = 0,
    SLAVE_TRANS_DIR_M2O
}ql_wmbus_transmit_direction_e;

typedef enum 
{
    MODE_C_FRAME_TYPE_A = 0,
    MODE_C_FRAME_TYPE_B
}ql_wmbus_mode_c_frame_type_e;

typedef struct 
{
    uint16_t channel_tx;
    uint16_t channel_rx;
}ql_wmbus_rf_channel_tx_rx_t;

int16_t ql_wmbus_rf_channel_init(void);

ql_wmbus_transmit_mode_e ql_wmbus_rf_channel_get_mode(void);

ql_wmbus_transmit_mode_type_e ql_wmbus_rf_channel_get_type(void);

ql_wmbus_transmit_direction_e ql_wmbus_rf_channel_get_direction(void);

int16_t ql_wmbus_rf_channel_set_mode(ql_wmbus_transmit_mode_e set_mode);

int16_t ql_wmbus_rf_channel_set_mode_type(ql_wmbus_transmit_mode_type_e set_type);

int16_t ql_wmbus_rf_channel_set_direction(ql_wmbus_transmit_direction_e set_dir);

ql_wmbus_rf_channel_tx_rx_t ql_wmbus_rf_channel_get_instances(void);

RAIL_Handle_t ql_wmbus_rf_rail_get_handle(void);

int16_t ql_wmbus_rf_channel_get_rssi(void);

void ql_wmbus_rf_channel_testing_carrier_singal(void);

void ql_wmbus_rf_channel_testing_stream_singal(void);

#endif /* __QL_WMBUS_RF_CHANNEL_CFG_H__ */