/*
 * ql_wmbus_encryption.h
 *
 *  Created on: 2025年01月08日
 *      Author: kahn.wei
 */

#ifndef __QL_WMBUS_ENCRYPTION_H__
#define __QL_WMBUS_ENCRYPTION_H__

#include "ql_include.h"

typedef struct 
{
    uint32_t node_index;
    uint16_t enc_type;
    uint8_t enc_key[16];
}ql_wmbus_encryption_info_t;

typedef struct _node_enc
{
    ql_wmbus_encryption_info_t enc_info;
    struct _node_enc *next;
}ql_wmbus_encryption_node_t;

typedef struct 
{
    uint32_t node_cnt;
    ql_wmbus_encryption_node_t *node_list;
}ql_wmbus_encryption_t;

void ql_wmbus_bind_encryption_init(void);

int16_t ql_wmbus_get_encryption_bind_cnt(void);

ql_wmbus_encryption_t* ql_wmbus_get_encryption_storage(void);

void free_encryption_data_storage(ql_wmbus_encryption_t* storage);

int8_t query_encryption_by_index(int32_t index, uint8_t *encryption_key);

int16_t query_encryption_flag_by_index(int32_t index, uint16_t *flag);

int16_t add_dev_wmbus_encryption_is_encryption_flag(uint32_t index, uint16_t method_encryption);

int16_t add_dev_wmbus_encryption_node_tail(ql_wmbus_encryption_t* storage, ql_wmbus_encryption_info_t dev_wmbus_encryption_info);

#endif /* __QL_WMBUS_ENCRYPTION_H__ */