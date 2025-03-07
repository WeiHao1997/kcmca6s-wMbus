#include "ql_uart.h"
#include "em_eusart.h"

#include "efr32fg23_eusart.h"
#include "em_eusart_compat.h"

#define RING_BUFFER_SIZE 1024 * 2

#define QUEC_AT_UART_COM       EUSART1

#define QUEC_BCC_TXPORT        gpioPortA
#define QUEC_BCC_TXPIN         6
#define QUEC_BCC_RXPORT        gpioPortA
#define QUEC_BCC_RXPIN         5

#define QUEC_DEBUG_UART_COM    USART0
#define QUEC_DEBUG_TXPORT      gpioPortC
#define QUEC_DEBUG_TXPIN       1
#define QUEC_DEBUG_RXPORT      gpioPortC
#define QUEC_DEBUG_RXPIN       2

typedef struct 
{
    uint32_t UART_RX_PIN;
    uint32_t UART_RX_PORT;
    uint32_t UART_TX_PIN;
    uint32_t UART_TX_PORT;
    uint32_t UART_BASE;
}ql_uart_port_t;

typedef struct
{
    uint8_t buffer[RING_BUFFER_SIZE];
    volatile uint32_t write_index;
    volatile uint32_t read_index;
    volatile uint32_t count;
    volatile bool frame_complete;
    volatile uint32_t frame_length;
} ring_buffer_t;

#define UART_RX_BUFFER_SIZE 1024

static ring_buffer_t rxRingBuffer = {0};

static inline uint32_t ring_buffer_available(ring_buffer_t *rb)
{
    return rb->count;
}

static void ring_buffer_write_byte(ring_buffer_t *rb, uint8_t data)
{
    if(rb->count < UART_RX_BUFFER_SIZE)
    {
        rb->buffer[rb->write_index] = data;
        rb->write_index = (rb->write_index + 1) % UART_RX_BUFFER_SIZE;
        rb->count++;
    }
}

static uint32_t ring_buffer_read(ring_buffer_t *rb, uint8_t *data, uint32_t len)
{
    uint32_t read_len = 0;
    
    read_len = (len > rb->count) ? rb->count : len;
    
    for(uint32_t i = 0; i < read_len; i++)
    {
        data[i] = rb->buffer[rb->read_index];
        rb->read_index = (rb->read_index + 1) % UART_RX_BUFFER_SIZE;
    }
    
    rb->count -= read_len;
    
    return read_len;
}

void ql_uart_rx_cplt_callback(void)
{
    rxRingBuffer.frame_length = rxRingBuffer.count;
    rxRingBuffer.frame_complete = true;

    drv_timer_stop(0);
}

void TIMER0_IRQHandler(void)
{
  drv_soft_time_hook();
  TIMER_IntClear(TIMER0, true);
}

void EUSART1_RX_IRQHandler(void)
{
    uint32_t flags = EUSART1->IF;
    
    if(flags & EUSART_IF_RXFL)
    {
        uint8_t data = EUSART1->RXDATA;
        ring_buffer_write_byte(&rxRingBuffer, data);
        drv_timer_start(0, 5, ql_uart_rx_cplt_callback);
        EUSART_IntClear(QUEC_AT_UART_COM, EUSART_IF_RXFL);
    }
}

int32_t ql_uart_open(ql_uart_port_e port)
{
    if(QL_UART_UART0_PORT == port)
    {
        CMU_ClockEnable(cmuClock_USART0, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

#if 0
        // Default asynchronous initializer (961200bps, 8N1, no flow control)
        EUSART_UartInit_TypeDef init = EUSART_UART_INIT_DEFAULT_HF;
        // Configure the EUSART TX pin to the board controller as an output
        GPIO_PinModeSet(QUEC_DEBUG_TXPORT, QUEC_DEBUG_TXPIN, gpioModePushPull, 1);

        // Route EUSART0 TX and RX to the board controller TX and RX pins
        GPIO->EUSARTROUTE[0].TXROUTE = (QUEC_DEBUG_TXPORT << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
            | (QUEC_DEBUG_TXPIN << _GPIO_EUSART_TXROUTE_PIN_SHIFT);
        GPIO->EUSARTROUTE[0].RXROUTE = (QUEC_DEBUG_RXPORT << _GPIO_EUSART_RXROUTE_PORT_SHIFT)
            | (QUEC_DEBUG_RXPIN << _GPIO_EUSART_RXROUTE_PIN_SHIFT);

        // Enable RX and TX signals now that they have been routed
        GPIO->EUSARTROUTE[0].ROUTEEN = GPIO_EUSART_ROUTEEN_RXPEN | GPIO_EUSART_ROUTEEN_TXPEN;
        
        // Configure and enable EUSART0
        init.baudrate = 921600;
        EUSART_UartInitHf(EUSART0, &init);
        NVIC_ClearPendingIRQ(EUSART0_RX_IRQn);
        NVIC_EnableIRQ(EUSART0_RX_IRQn);
        EUSART_IntEnable(EUSART0, EUSART_IEN_RXFL);
#endif

        // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
        USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
        // Configure the USART TX pin to the board controller as an output
        GPIO_PinModeSet(QUEC_DEBUG_TXPORT, QUEC_DEBUG_TXPIN, gpioModePushPull, 1);

        // Route USART0 TX and RX to the board controller TX and RX pins
        GPIO->USARTROUTE[0].TXROUTE = (QUEC_DEBUG_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
            | (QUEC_DEBUG_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

        // Enable RX and TX signals now that they have been routed
        GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN;

        init.baudrate = 460800;
        init.enable = usartEnableTx;
        // Configure and enable USART0
        USART_InitAsync(USART0, &init);
    }
    else if(QL_UART_UART1_PORT == port)
    {
        // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
        EUSART_UartInit_TypeDef init = EUSART_UART_INIT_DEFAULT_HF;

        init.baudrate = 19200;
        CMU_ClockEnable(cmuClock_EUSART1, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Configure the EUSART TX pin to the board controller as an output
        GPIO_PinModeSet(QUEC_BCC_TXPORT, QUEC_BCC_TXPIN, gpioModePushPull, 1);

        // Configure the EUSART RX pin to the board controller as an input
        GPIO_PinModeSet(QUEC_BCC_RXPORT, QUEC_BCC_RXPIN, gpioModeInput, 0);

        // Route EUSART1 TX and RX to the board controller TX and RX pins
        GPIO->EUSARTROUTE[1].TXROUTE = (QUEC_BCC_TXPORT << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
            | (QUEC_BCC_TXPIN << _GPIO_EUSART_TXROUTE_PIN_SHIFT);
        GPIO->EUSARTROUTE[1].RXROUTE = (QUEC_BCC_RXPORT << _GPIO_EUSART_RXROUTE_PORT_SHIFT)
            | (QUEC_BCC_RXPIN << _GPIO_EUSART_RXROUTE_PIN_SHIFT);

        // Enable RX and TX signals now that they have been routed
        GPIO->EUSARTROUTE[1].ROUTEEN = GPIO_EUSART_ROUTEEN_RXPEN | GPIO_EUSART_ROUTEEN_TXPEN;

        // Configure and enable EUSART1
        EUSART_UartInitHf(EUSART1, &init);
        NVIC_ClearPendingIRQ(EUSART1_RX_IRQn);
        NVIC_EnableIRQ(EUSART1_RX_IRQn);
        EUSART_IntEnable(EUSART1, EUSART_IEN_RXFL);

        initTIMER();
    }

    return 0;
}

int32_t ql_uart_write(ql_uart_port_e port, uint8_t *data, uint32_t len)
{
     if(QL_UART_UART0_PORT == port)
     {
         for(uint32_t i = 0; i < len; i++)
         {
             while(!(USART0->STATUS & USART_STATUS_TXBL));
             USART0->TXDATA = data[i];
         }
         while(!(USART0->STATUS & USART_STATUS_TXC));
         return 0;
     }
     else if(QL_UART_UART1_PORT == port)
     {
         for(uint32_t i = 0; i < len; i++)
         {
             while(!(EUSART1->STATUS & EUSART_STATUS_TXFL));
             EUSART1->TXDATA = data[i];
         }
         while(!(EUSART1->STATUS & EUSART_STATUS_TXC));
         return 0;
     }
    return -1;
}

int32_t ql_uart_read(ql_uart_port_e port, uint8_t *data, uint32_t len, uint32_t *o_len)
{
    if(QL_UART_UART0_PORT == port)
    {

    }
    else if(QL_UART_UART1_PORT == port)
    {
        if(rxRingBuffer.frame_complete)
        {
            *o_len = ring_buffer_read(&rxRingBuffer, data, rxRingBuffer.frame_length);
            rxRingBuffer.frame_complete = false;
            rxRingBuffer.frame_length = 0;
            return (*o_len > 0) ? 0 : -1;
        }
        return -1;
    }
    return -1;
}

uint32_t ql_uart_available(ql_uart_port_e port)
{
    if(QL_UART_UART0_PORT == port)
    {

    }
    else if(QL_UART_UART1_PORT == port)
    {
        if(rxRingBuffer.frame_complete == true)
        {
            return ring_buffer_available(&rxRingBuffer);
        }
    }
    return 0;
}



#if 0
int32_t ql_uart_open(ql_uart_port_e port)
{
    if(QL_UART_UART0_PORT == port)
    {
        sl_iostream_eusart_init_instances();
    }
    return 0;
}

int32_t ql_uart_write(ql_uart_port_e port, uint8_t *data, uint32_t len)
{
    if(QL_UART_UART0_PORT == port)
    {
        uint32_t ret = sl_iostream_write(sl_iostream_get_default(), (const void *)data, (size_t)len);
        if(ret != 0)
        {
            return -1;
        }
    }
    return 0;
}

int32_t ql_uart_read(ql_uart_port_e port, uint8_t *data, uint32_t len, uint32_t *o_len)
{
    if(QL_UART_UART0_PORT == port)
    {
        uint32_t ret = sl_iostream_read(SL_IOSTREAM_STDOUT, (void *)data, (size_t)len, (size_t *)o_len);
        if(ret != 0)
        {
            return -1;
        }
    }
    return 0;
}

uint8_t ql_uart_read_char(ql_uart_port_e port)
{
    uint8_t recv_char;
    if(QL_UART_UART0_PORT == port)
    {
         uint32_t ret = sl_iostream_getchar(sl_iostream_get_default(), &recv_char);
    }
    return recv_char;
}

#endif

void app_debug_send(uint8_t *data , uint32_t len)
{
    ql_uart_write(QL_UART_UART0_PORT, data, len);
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