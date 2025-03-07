/*
 * ql_wmbus_handle.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_WMBUS_HANDLE_H__
#define __QL_WMBUS_HANDLE_H__

#include "ql_include.h"

typedef enum 
{
    S_PACKET_RECEIVED,
    S_RX_PACKET_ERROR,
    S_CALIBRATION_ERROR,
    S_IDLE,

    S_AUTO_RECV_TX,     //!< Sending auto received packets
    S_SCHEDULE_TX,      //!< Sending scheduled packets
    S_RESPONSE_DELAY,   //!< Waiting for to start response listening
    S_UNLIMITED_ACCESS, //!< Stay in RX until the next TX
    S_IDLE_OR_RX,       //!< Got to sleep or idle depending on the power manager or go back to RX if unlimited access is selected
    S_TX_DONE,          //!< Sending is successfully done
} state_t;

typedef struct 
{
    uint8_t wmbus_uart_recv_data[1024];
    uint16_t wmbus_uart_recv_len;
}ql_wmbus_uart_recv_data_info_t;

typedef uint8_t wmbus_ci_field;
typedef void(*wmbus_app_func)(uint8_t *, uint32_t);

int16_t ql_wmbus_init(void);

void ql_wmbus_handle_loop(void);

extern ql_wmbus_uart_recv_data_info_t g_wmbus_uart_recv_data_info;

uint32_t ql_wmbus_get_tune_value(void);

uint8_t ql_wmbus_get_current_role(void);

#endif /* __QL_WMBUS_HANDLE_H__ */
