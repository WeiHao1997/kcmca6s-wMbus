/*
 * app_debug.c
 *
 *  Created on: 2024年12月5日
 *      Author: vic.cheng
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "em_gpio.h"
#include "em_usart.h"
#include "em_eusart.h"
#include "em_cmu.h"


#define QUEC_AT_UART_COM       USART0

#define QUEC_DEBUG_UART_COM    USART0
#define QUEC_DEBUG_TXPORT      gpioPortC
#define QUEC_DEBUG_TXPIN       1
#define QUEC_DEBUG_RXPORT      gpioPortC
#define QUEC_DEBUG_RXPIN       2



int32_t app_debug_init(void)
{
    CMU_ClockEnable(cmuClock_USART0, true);
    CMU_ClockEnable(cmuClock_GPIO, true);

    // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
    USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
    // Configure the USART TX pin to the board controller as an output
    GPIO_PinModeSet(QUEC_DEBUG_TXPORT, QUEC_DEBUG_TXPIN, gpioModePushPull, 1);

    // Route USART0 TX and RX to the board controller TX and RX pins
    GPIO->USARTROUTE[0].TXROUTE = (QUEC_DEBUG_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
        | (QUEC_DEBUG_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

    // Enable RX and TX signals now that they have been routed
    GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN;

    init.baudrate = 115200;
    init.enable = usartEnableTx;
    // Configure and enable USART0
    USART_InitAsync(QUEC_AT_UART_COM, &init);
    return 0;
}

int32_t app_debug_send(uint8_t *data, uint32_t len)
{
  for(uint32_t i = 0; i < len; i++)
  {
      while(!(USART0->STATUS & USART_STATUS_TXBL));
      USART0->TXDATA = data[i];
  }
  while(!(USART0->STATUS & USART_STATUS_TXC));
  return 0;
}


void ql_printf(const char *fmt, ...)
{
#if (CFG_SUPPORT_RTT)
    va_list args;
    rt_size_t length;
    static char rt_log_buf[RT_CONSOLEBUF_SIZE];

    va_start(args, fmt);
    /* the return value of vsnprintf is the number of bytes that would be
     * written to buffer had if the size of the buffer been sufficiently
     * large excluding the terminating null byte. If the output string
     * would be larger than the rt_log_buf, we have to adjust the output
     * length. */
    length = rt_vsnprintf(rt_log_buf, sizeof(rt_log_buf) - 1, fmt, args);
    if (length > RT_CONSOLEBUF_SIZE - 1)
        length = RT_CONSOLEBUF_SIZE - 1;
    rt_kprintf("%s", rt_log_buf);
    va_end(args);
#else
    va_list ap;
    char string[1024];

    va_start(ap, fmt);
    vsnprintf(string, sizeof(string) - 1, fmt, ap);
    string[1023] = '\0';
    app_debug_send((uint8_t *)string,strlen(string));
    va_end(ap);
#endif
}

