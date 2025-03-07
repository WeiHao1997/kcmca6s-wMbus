/*
 * quec_uart.h
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

#include <stdio.h>

#ifndef QUECTEL_FILE_QUEC_UART_H_
#define QUECTEL_FILE_QUEC_UART_H_

void quec_at_processor_poll(void);
void quec_uart_send_string(char *string);
void quec_uart_send(char *data , int length);
void app_uart_recv_clear(void);
void app_uart_init(void);

#endif /* QUECTEL_FILE_QUEC_UART_H_ */
