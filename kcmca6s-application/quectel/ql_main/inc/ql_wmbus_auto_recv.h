/****
 * @file ql_wmbus_auto_recv.h
 * @version 1.0
 * @date 2025-02-17
 * @author kahn.wei
 * This is a wmbus auto receive module.
 */

#ifndef __QL_WMBUS_AUTO_RECV_H__
#define __QL_WMBUS_AUTO_RECV_H__

#include "ql_include.h"


typedef struct
{
    uint8_t is_enable;
    uint32_t interval_time;
} ql_wmbus_auto_recv_config_t;

typedef struct
{
    uint16_t manufacturer;
    uint32_t id;
    uint8_t version;
    uint8_t device_type;
} __attribute__((packed))ql_wmbus_auto_recv_base_dev_info_t;

typedef struct 
{
    uint8_t length;
    uint8_t c_field;
    uint8_t ci_field;
    uint8_t *payload;
    uint32_t payload_len;
} ql_wmbus_auto_recv_data_info_t;

typedef struct 
{
    uint32_t index_id;
    ql_wmbus_auto_recv_config_t cfg;
    ql_wmbus_auto_recv_data_info_t data;
    ql_wmbus_auto_recv_base_dev_info_t dev_info;
}ql_wmbus_auto_recv_dev_all_info_t;

typedef struct _recv_node
{
    ql_wmbus_auto_recv_dev_all_info_t all_info;
    struct _recv_node *next;
} ql_wmbus_auto_recv_node_info_t;

typedef struct
{
    uint32_t node_cnt;
    ql_wmbus_auto_recv_node_info_t *node_list;
} ql_wmbus_auto_recv_dev_info_t;

typedef void (*ql_wmbus_auto_recv_callback)(const ql_wmbus_auto_recv_base_dev_info_t*, const ql_wmbus_auto_recv_data_info_t*);

int16_t ql_wmbus_auto_recv_init(void);

ql_wmbus_auto_recv_dev_info_t *ql_wmbus_get_auto_recv_dev_storage(void);

int16_t ql_wmbus_auto_recv_set_is_enable_by_index(uint32_t index, uint8_t is_enable);

uint32_t ql_wmbus_get_set_auto_recv_node_cnt(ql_wmbus_auto_recv_dev_info_t *storage);

int16_t ql_wmbus_register_auto_recv_callback(ql_wmbus_auto_recv_callback recv_callback);

int16_t add_new_wmbus_auto_recv_node_tail(ql_wmbus_auto_recv_dev_info_t *storage, ql_wmbus_auto_recv_dev_all_info_t *all_info);

int16_t recognize_wmbus_auto_recv_is_active(ql_wmbus_auto_recv_dev_info_t *storage, ql_wmbus_auto_recv_base_dev_info_t *dev_info);

int16_t ql_wmbus_auto_recv_get_single_by_index(uint32_t index, ql_wmbus_auto_recv_data_info_t *info, ql_wmbus_auto_recv_config_t *cfg);

#endif /* __QL_WMBUS_AUTO_RECV_H__ */