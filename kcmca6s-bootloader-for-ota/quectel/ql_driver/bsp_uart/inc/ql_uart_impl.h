/*
 * ql_uart.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_UART_IMPL_H__
#define __QL_UART_IMPL_H__

#include "ql_include.h"

typedef void (*QL_UARTx_CallBack)(uint16_t port, void *param);

/// @brief
typedef enum
{
    QL_UART_UART0_PORT   = 0,
    QL_UART_UART1_PORT   = 1,
}ql_uart_port_e;

/// @brief
typedef enum
{
  QL_UART_FC_NONE         = 0,
  QL_UART_FC_HW_RTS       = 1,
  QL_UART_FC_HW_CTS       = 2,
  QL_UART_FC_HW_RTS_CTS   = 3
}ql_uart_flowctrl_e;

/// @brief
typedef enum
{
    QL_UART_STOPBIT_1   = 1,
    QL_UART_STOPBIT_2   = 2,
    QL_UART_STOPBIT_MAX = QL_UART_STOPBIT_2
}ql_uart_stopbit_e;

/// @brief
typedef enum
{
    QL_UART_DATABIT_5   = 5,
    QL_UART_DATABIT_6,
    QL_UART_DATABIT_7,
    QL_UART_DATABIT_8,
    QL_UART_DATABIT_MAX  = QL_UART_DATABIT_8
}ql_uart_databit_e;

/// @brief
typedef enum
{
    QL_UART_PARITY_NONE     = 0,    /**< parity disable */
    QL_UART_PARITY_ODD      = 1,    /**< odd parity enable */
    QL_UART_PARITY_EVEN     = 2,    /**< even parity enable */
    QL_UART_PARITY_FORCED1  = 3,    /**< same action with ParityOdd */
    QL_UART_PARITY_FORCED0  = 4     /**< same action with ParityEven */
}ql_uart_parity_e;

/// @brief
typedef enum
{
    QL_UART_TX_MODE_POLL   = 0,     /**< 阻塞模式*/
    QL_UART_TX_MODE_INT,            /**< 通过中断的异步模式 */
    QL_UART_TX_MODE_DMA,            /**< 通过DMA模式发送,异步发送 */
    QL_UART_TX_MODE_MAX
}ql_uart_tx_mode_e;

typedef enum
{
    QL_UART_RX_MODE_POLL    = 0,    /**< 阻塞模式,获取数据*/
    QL_UART_RX_MODE_INT,            /**< 通过中断的异步模式 */
    QL_UART_RX_MODE_DMA,            /**< 通过DMA模式接收,异步接收 */
    QL_UART_RX_MODE_MAX
}ql_uart_rx_mode_e;

/// @brief 串口波特率枚举
typedef enum
{
    QL_UART_BAUDRATE_1200       = 1200,
    QL_UART_BAUDRATE_2400       = 2400,
    QL_UART_BAUDRATE_4800       = 4800,
    QL_UART_BAUDRATE_9600       = 9600,
    QL_UART_BAUDRATE_19200      = 19200,
    QL_UART_BAUDRATE_38400      = 38400,
    QL_UART_BAUDRATE_115200     = 115200,
    QL_UART_BAUDRATE_230400     = 230400,
    QL_UART_BAUDRATE_460800     = 460800,
    QL_UART_BAUDRATE_921600     = 921600,
    QL_UART_BAUDRATE_2000000    = 2000000,
    QL_UART_BAUDRATE_MAX        = QL_UART_BAUDRATE_2000000
}ql_uart_baudrate_e;

/// @brief 串口配置信息
typedef struct _ql_uart_config_t
{
    ql_uart_baudrate_e      baudrate;
    ql_uart_databit_e       data_bit;
    ql_uart_stopbit_e       stop_bit;
    ql_uart_parity_e        parity;
    ql_uart_flowctrl_e      flow_ctrl;
}ql_uart_config_t;

typedef struct
{
  uint8_t is_available;
  uint8_t *uart_rx_buf;
  uint8_t *uart_tx_buf;
}ql_uart_lowevel_config_t;


void ql_printf(const char *fmt, ...);

int32_t ql_uart_open(ql_uart_port_e port);

int32_t ql_uart_write(ql_uart_port_e port, uint8_t *data, uint32_t len);

int32_t ql_uart_read(ql_uart_port_e port, uint8_t *data, uint32_t len, uint32_t *o_len);

uint8_t ql_uart_read_char(ql_uart_port_e port);

uint8_t ql_uart_write_char(ql_uart_port_e port, uint8_t c);

int32_t ql_uart_close(ql_uart_port_e port);

uint32_t ql_uart_available(ql_uart_port_e port);

typedef struct ql_uart ql_uart_t;

typedef struct 
{
    int32_t (*open)(ql_uart_t *self);
    int32_t (*close)(ql_uart_t *self);
    int32_t (*write)(ql_uart_t *self, const uint8_t *data, uint32_t len);
    int32_t (*read)(ql_uart_t *self, uint8_t *data, uint32_t len, uint32_t *o_len);
    uint8_t (*read_char)(ql_uart_t *self);
    uint8_t (*write_char)(ql_uart_t *self, uint8_t c);
    uint32_t (*available)(ql_uart_t *self);
    uint32_t (*deinit)(ql_uart_t *self);
} ql_uart_vtbl_t;

struct ql_uart 
{
    const ql_uart_vtbl_t *vptr;
    ql_uart_port_e port;
    uint8_t is_opened;
};

ql_uart_t *ql_uart_create(ql_uart_port_e port);
void ql_uart_destroy(ql_uart_t *uart);

#endif /* __QL_UART_IMPL_H__ */
