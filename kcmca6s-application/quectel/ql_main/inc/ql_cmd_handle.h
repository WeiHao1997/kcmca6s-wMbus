/*
 * ql_cmd_handle.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_CMD_HANDLE_H__
#define __QL_CMD_HANDLE_H__

#include "ql_include.h"

typedef struct
{
    uint32_t head;
    uint32_t ctune_value;
    uint32_t sn_len;
    uint8_t sn[0];
}ql_module_config_t;

void ql_cmd_handle_loop(void);

typedef int16_t (*wmbus_register_event)(int32_t , void*);

void ql_wmbus_register_change_event_register(wmbus_register_event event);

#define QL_MODULE_CONFIG_BASE_ADDR  0x0803C000

#endif /* __QL_CMD_HANDLE_H__ */