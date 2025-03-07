/*
 * ql_wmbus_bind.h
 *
 *  Created on: 2024年11月25日
 *      Author: kahn.wei
 */

#ifndef __QL_WMBUS_BIND_H__ 
#define __QL_WMBUS_BIND_H__

#include "ql_include.h"

typedef struct 
{
    uint32_t index_id;
    uint16_t manufacturer;
    uint32_t id;
    uint8_t version;
    uint8_t device_type;
    uint8_t is_encryption;
}__attribute__((packed)) ql_wmbus_dev_info_t;

typedef struct _bind_node
{
    ql_wmbus_dev_info_t info;
    struct _bind_node *next;
}ql_wmbus_bind_node_t;

typedef struct
{
    uint32_t node_cnt;
    ql_wmbus_bind_node_t *node_list;
}ql_wmbus_bind_info_t;

void ql_wmbus_bind_init(void);

uint32_t ql_wmbus_get_bind_cnt(void);

ql_wmbus_bind_info_t* ql_wmbus_get_dev_storage(void);

int16_t query_dev_wmbus_node_index(ql_wmbus_dev_info_t dev_wmbus_info);
int16_t query_dev_wmbus_node_index_is_exist(uint32_t index);

int8_t query_dev_wmbus_node_is_encryption_by_index(uint32_t index);
int16_t set_dev_wmbus_node_is_encryption_by_index(uint32_t index);

int16_t add_dev_wmbus_node_tail(ql_wmbus_bind_info_t* storage, ql_wmbus_dev_info_t dev_wmbus_info);
int16_t check_device_is_register(ql_wmbus_bind_info_t* storage, ql_wmbus_dev_info_t dev_wmbus_info);
void free_data_storage(ql_wmbus_bind_info_t* storage);

int16_t query_dev_wmbus_node(ql_wmbus_bind_info_t* storage, uint32_t index_id, ql_wmbus_dev_info_t *dev_wmbus_info);

int16_t ql_wmbus_bind_dev_save_to_flash(void);
#endif /* __QL_WMBUS_BIND_H__ */
