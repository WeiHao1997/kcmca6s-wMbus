/*
 * quec_uart.c
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "app.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#include <string.h>
#include "em_eusart.h"

#include "app_debug.h"
#include "sl_iostream_eusart_vcom_config.h"
#include "quectel_file/quec_at_analysis.h"
#include "quectel_file/quec_product_test.h"
#include "sl_cli_types.h"

extern int sli_cli_io_getchar(void);

uint8_t g_rx_buffer[1024] = {0};
uint16_t g_rx_point = 0;
uint8_t g_uart_rx_complete = 0;
sl_sleeptimer_timer_handle_t g_tUartRecvTimeoutTimer;

void app_uart_recv_timer_handler(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  g_uart_rx_complete = 1;
}


void app_data_recv(void)
{
  int ch;

  ch = sli_cli_io_getchar();

  if(ch == EOF)
    return ;


  sl_sleeptimer_restart_timer(&g_tUartRecvTimeoutTimer, 20, app_uart_recv_timer_handler, (void *)NULL, 0, 0);

  g_rx_buffer[g_rx_point] = ch;
  g_rx_point++;
  if(g_rx_point >= sizeof(g_rx_buffer))
  {
      g_rx_point = 0;
  }
}

void app_uart_recv_timer_handler_first(sl_sleeptimer_timer_handle_t *handle, void *data)
{
  //nothing
}


void app_uart_init(void)
{
  sl_sleeptimer_start_timer(&g_tUartRecvTimeoutTimer, 20, app_uart_recv_timer_handler_first, (void *)NULL, 0, 0);
}



void app_uart_recv_clear(void)
{
  memset(g_rx_buffer , 0 , sizeof(g_rx_buffer));
  g_rx_point = 0;
}

void quec_at_processor_poll(void)
{
  if(EM_UART_FTM_MODE != app_quec_product_get_workmode())
  {
      app_data_recv();
      //if(NULL != strstr(g_rx_buffer , "AT") && NULL != strstr(g_rx_buffer , "\r\n"))
      if(g_uart_rx_complete)
      {
          ql_printf("uart recv:");
          for(uint16_t i = 0 ; i < strlen(g_rx_buffer) ; i++)
          {
              ql_printf("%c" , g_rx_buffer[i]);
          }

          quec_at_command_handle((char *)g_rx_buffer,strlen(g_rx_buffer));
          app_uart_recv_clear();
          g_uart_rx_complete = 0;
      }
  }
  else
  {
      sl_cli_instances_tick();
      sl_rail_test_internal_app_process_action();
  }
}



void quec_uart_send_string(char *string)
{
  for(uint32_t i = 0 ; i < strlen(string) ; i++)
  {
      EUSART_Tx(EUSART0 , string[i]);
  }
}

void quec_uart_send(char *data , int length)
{
  for(uint32_t i = 0 ; i < length ; i++)
  {
      EUSART_Tx(EUSART0 , data[i]);
  }
}


