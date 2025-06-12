#include <ql_uart_impl.h>
#include "em_eusart.h"
#include "em_usart.h"
#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_ldma.h"

#include "efr32fg23_eusart.h"
#include "em_eusart_compat.h"

#define RING_BUFFER_SIZE 1024

#define QL_MAIN_UART                USART0
#define QL_MAIN_UART_TX_PORT        gpioPortA
#define QL_MAIN_UART_TX_PIN         6
#define QL_MAIN_UART_RX_PORT        gpioPortA
#define QL_MAIN_UART_RX_PIN         5

#define QL_DEBUG_UART               EUSART1 
#define QL_DEBUG_UART_TX_PORT       gpioPortC
#define QL_DEBUG_UART_TX_PIN        1

#define USE_LDMA_TX
//#define USE_LDMA_TX_DEBUG
#define IS_ENBALE_DEBUG_LOG

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

// LDMA channels for receive and transmit servicing
#define RX_LDMA_CHANNEL 0
#define TX_LDMA_CHANNEL 1
#define TX_LDMA_CHANNEL_DEBUG 2

// LDMA descriptor and transfer configuration structures for TX channel
LDMA_Descriptor_t ldmaTXDescriptor;
LDMA_TransferCfg_t ldmaTXConfig;

// LDMA descriptor and transfer configuration structures for TX debug channel
LDMA_Descriptor_t ldmaTXDebugDescriptor;
LDMA_TransferCfg_t ldmaTXDebugConfig;

// LDMA descriptor and transfer configuration structures for RX channel
LDMA_Descriptor_t ldmaRXDescriptor;
LDMA_TransferCfg_t ldmaRXConfig;

#define UART_RX_BUFFER_SIZE 1024

#define UART_BUFFER_SIZE    1024
uint8_t UART_LDMA_TX_BUFFER[UART_BUFFER_SIZE] = {0};
uint8_t UART_LDMA_TX_BUFFER_DEBUG[UART_BUFFER_SIZE] = {0};
uint8_t UART_LDMA_RX_BUFFER[UART_BUFFER_SIZE] = {0};

static ring_buffer_t rxRingBuffer = {0};
static ring_buffer_t txRingBuffer = {0};
static ring_buffer_t txDebugRingBuffer = {0};

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
}

uint8_t tx_is_sending = 0;
uint8_t tx_is_sending_debug = 0;

void ql_uart_tx_debug_cplt_callback(void)
{
    uint32_t remain_len = ring_buffer_available(&txDebugRingBuffer);
    uint32_t read_len = UART_BUFFER_SIZE > remain_len ? remain_len : UART_BUFFER_SIZE;

    tx_is_sending_debug = 0;

    if(read_len > 0)
    {
        ring_buffer_read(&txDebugRingBuffer, UART_LDMA_TX_BUFFER_DEBUG, read_len);
        ldmaTXDebugDescriptor.xfer.xferCnt = read_len - 1; 
        LDMA_StartTransfer(TX_LDMA_CHANNEL_DEBUG, &ldmaTXDebugConfig, &ldmaTXDebugDescriptor);
        tx_is_sending_debug = 1;
    }
}

void ql_uart_tx_cplt_callback(void)
{
    uint32_t remain_len = ring_buffer_available(&txRingBuffer);
    uint32_t read_len = UART_BUFFER_SIZE > remain_len ? remain_len : UART_BUFFER_SIZE;

    tx_is_sending = 0;

    if(read_len > 0)
    {
        ring_buffer_read(&txRingBuffer, UART_LDMA_TX_BUFFER, read_len);
        ldmaTXDescriptor.xfer.xferCnt = read_len - 1; 
        LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);
        tx_is_sending = 1;
    }
}

void EUSART1_TX_IRQHandler(void)
{
    uint32_t flag = EUSART_IntGet(QL_DEBUG_UART);
    if(flag & EUSART_IF_TXC)
    {
        EUSART_IntClear(QL_DEBUG_UART, EUSART_IF_TXC);
        tx_is_sending_debug = 0;
        ql_uart_tx_debug_cplt_callback();
    }
}

void USART0_TX_IRQHandler(void)
{
    uint32_t flag = USART_IntGet(QL_MAIN_UART);

    if(flag & USART_IF_TXBL)
    {
        USART_IntClear(QL_MAIN_UART, USART_IF_TXBL);
    }

    if(flag & USART_IF_TXC)
    {
        USART_IntClear(QL_MAIN_UART, USART_IF_TXC);
        tx_is_sending = 0;
        ql_uart_tx_cplt_callback();
    }
}

void USART0_RX_IRQHandler(void)
{
    uint32_t flag = USART_IntGet(QL_MAIN_UART);

    if(flag & USART_IF_RXDATAV)
    {
        USART_IntClear(QL_MAIN_UART, USART_IF_RXDATAV);
    }

    if(flag & USART_IF_TCMP1)
    {
        USART_IntClear(QL_MAIN_UART, USART_IF_TCMP1);

        uint32_t recv_len = UART_BUFFER_SIZE - LDMA_TransferRemainingCount(RX_LDMA_CHANNEL);

        for (uint32_t i = 0; i < recv_len; i++) 
        {
            ring_buffer_write_byte(&rxRingBuffer, UART_LDMA_RX_BUFFER[i]);
        }

        ql_uart_rx_cplt_callback();
        LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);
    }
}

uint8_t ldma_rx_flag = 0;
uint8_t ldma_tx_flag = 0;
uint8_t ldma_tx_flag_debug = 0;

void LDMA_IRQHandler()
{
    uint32_t flags = LDMA_IntGet();

    // Clear the transmit channel's done flag if set
    if (flags & (1 << TX_LDMA_CHANNEL)) 
    {
        ldma_tx_flag = 1;
        LDMA_IntClear(1 << TX_LDMA_CHANNEL);
    }

    /*
    * Clear the receive channel's done flag if set and change receive
    * state to done.
    */
    if (flags & (1 << RX_LDMA_CHANNEL)) 
    {
        ldma_rx_flag = 1;
        LDMA_IntClear(1 << RX_LDMA_CHANNEL);
    }

    if (flags & (1 << TX_LDMA_CHANNEL_DEBUG)) 
    {
        ldma_tx_flag = 1;
        LDMA_IntClear(1 << TX_LDMA_CHANNEL_DEBUG);
    }

    // Stop in case there was an error
    if (flags & LDMA_IF_ERROR) 
    {
        __BKPT(0);
    }
}

int32_t ql_uart_open(ql_uart_port_e port)
{
    if(QL_UART_UART0_PORT == port)
    {
        CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

        // Select LFXO for the EUSART
        CMU_HFXOInit(&hfxoInit);
        CMU_ClockSelectSet(cmuClock_EUSART1, cmuSelect_HFXO);

        // Default asynchronous initializer (460.8 Kbps, 8N1, no flow control)
        EUSART_UartInit_TypeDef init = EUSART_UART_INIT_DEFAULT_HF;
        init.baudrate = 460800;

        CMU_ClockEnable(cmuClock_EUSART1, true);
        CMU_ClockEnable(cmuClock_GPIO, true);

        // Configure the EUSART TX pin to the board controller as an output
        GPIO_PinModeSet(QL_DEBUG_UART_TX_PORT, QL_DEBUG_UART_TX_PIN, gpioModePushPull, 1);

        // Route EUSART1 TX and RX to the board controller TX and RX pins
        GPIO->EUSARTROUTE[1].TXROUTE = (QL_DEBUG_UART_TX_PORT << _GPIO_EUSART_TXROUTE_PORT_SHIFT)
            | (QL_DEBUG_UART_TX_PIN << _GPIO_EUSART_TXROUTE_PIN_SHIFT);

        // Enable RX and TX signals now that they have been routed
        GPIO->EUSARTROUTE[1].ROUTEEN = GPIO_EUSART_ROUTEEN_TXPEN;

        EUSART_UartInitHf(QL_DEBUG_UART, &init);

#ifdef USE_LDMA_TX_DEBUG
        LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
        LDMA_Init(&ldmaInit);

        ldmaTXDebugDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(UART_LDMA_TX_BUFFER_DEBUG, &(QL_DEBUG_UART->TXDATA), UART_BUFFER_SIZE);
        ldmaTXDebugDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
        ldmaTXDebugDescriptor.xfer.size = ldmaCtrlSizeByte;

        ldmaTXDebugConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_EUSART1_TXFL);

        NVIC_EnableIRQ(LDMA_IRQn);
        NVIC_ClearPendingIRQ(LDMA_IRQn);

        NVIC_EnableIRQ(EUSART1_TX_IRQn);
        NVIC_ClearPendingIRQ(EUSART1_TX_IRQn);
#endif
#if 0
        // CMU_ClockEnable(cmuClock_USART0, true);
        // CMU_ClockEnable(cmuClock_GPIO, true);

        // // Default asynchronous initializer (460.8 Kbps, 8N1, no flow control)
        // USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
        // // Configure the USART TX pin to the board controller as an output
        // GPIO_PinModeSet(QUEC_DEBUG_TXPORT, QUEC_DEBUG_TXPIN, gpioModePushPull, 1);

        // // Route USART0 TX and RX to the board controller TX and RX pins
        // GPIO->USARTROUTE[0].TXROUTE = (QUEC_DEBUG_TXPORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
        //     | (QUEC_DEBUG_TXPIN << _GPIO_USART_TXROUTE_PIN_SHIFT);

        // // Enable RX and TX signals now that they have been routed
        // GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_TXPEN;

        // init.baudrate = 460800;
        // init.enable = usartEnableTx;
        // // Configure and enable USART0
        // USART_InitAsync(USART0, &init);
#endif

    }
    else if(QL_UART_UART1_PORT == port)
    {
        // Default asynchronous initializer (115.2 Kbps, 8N1, no flow control)
        USART_InitAsync_TypeDef init = USART_INITASYNC_DEFAULT;
        init.baudrate = 19200;

        CMU_ClockEnable(cmuClock_GPIO, true);
        CMU_ClockEnable(cmuClock_USART0, true);
        CMU_ClockEnable(cmuClock_LDMA, true);

        // Configure the USART TX pin to the board controller as an output
        GPIO_PinModeSet(QL_MAIN_UART_TX_PORT, QL_MAIN_UART_TX_PIN, gpioModePushPull, 1);
        // Configure the USART RX pin to the board controller as an input
        //GPIO_PinModeSet(BSP_BCC_RXPORT, BSP_BCC_RXPIN, gpioModeInput, 0);
        GPIO_PinModeSet(QL_MAIN_UART_RX_PORT, QL_MAIN_UART_RX_PIN, gpioModeInputPull, 1);

        // Route USART0 TX and RX to the board controller TX and RX pins
        GPIO->USARTROUTE[0].TXROUTE = (QL_MAIN_UART_TX_PORT << _GPIO_USART_TXROUTE_PORT_SHIFT)
        | (QL_MAIN_UART_TX_PIN << _GPIO_USART_TXROUTE_PIN_SHIFT);
        GPIO->USARTROUTE[0].RXROUTE = (QL_MAIN_UART_RX_PORT << _GPIO_USART_RXROUTE_PORT_SHIFT)
        | (QL_MAIN_UART_RX_PIN << _GPIO_USART_RXROUTE_PIN_SHIFT);

        // Enable RX and TX signals now that they have been routed
        GPIO->USARTROUTE[0].ROUTEEN = GPIO_USART_ROUTEEN_RXPEN | GPIO_USART_ROUTEEN_TXPEN;

        USART_InitAsync(QL_MAIN_UART, &init);

        QL_MAIN_UART->TIMECMP1 |= USART_TIMECMP1_TSTART_RXEOF;
        QL_MAIN_UART->TIMECMP1 |= USART_TIMECMP1_TSTOP_RXACT;
        QL_MAIN_UART->TIMECMP1 |= 0x64; //USART_TIMECMP1_TCMPVAL_DEFAULT;
        
        LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
        LDMA_Init(&ldmaInit);

        // Source is buffer, destination is USART0_TXDATA, and length if BUFLEN
        ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(UART_LDMA_TX_BUFFER, &(QL_MAIN_UART->TXDATA), UART_BUFFER_SIZE);
        ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
        ldmaTXDescriptor.xfer.size = ldmaCtrlSizeByte;

        // Transfer a byte on free space in the EUSART FIFO
        ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_TXBL);

        // Source is USART0_RXDATA, destination is buffer, and length if BUFLEN
        ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(QL_MAIN_UART->RXDATA), UART_LDMA_RX_BUFFER, UART_BUFFER_SIZE);
        ldmaRXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
        ldmaRXDescriptor.xfer.size = ldmaCtrlSizeByte;
        ldmaRXDescriptor.xfer.decLoopCnt = 1;

        // Transfer a byte on receive FIFO level event
        ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_USART0_RXDATAV);

        LDMA_StartTransfer(RX_LDMA_CHANNEL, &ldmaRXConfig, &ldmaRXDescriptor);

        USART_IntEnable(QL_MAIN_UART, USART_IEN_TCMP1 | USART_IEN_RXDATAV | USART_IEN_TXC);
        USART_IntClear(QL_MAIN_UART, USART_IEN_TCMP1 | USART_IEN_RXDATAV | USART_IEN_TXC);

        NVIC_EnableIRQ(LDMA_IRQn);
        NVIC_ClearPendingIRQ(LDMA_IRQn);

        NVIC_EnableIRQ(USART0_RX_IRQn);
        NVIC_ClearPendingIRQ(USART0_RX_IRQn);

        NVIC_EnableIRQ(USART0_TX_IRQn);
        NVIC_ClearPendingIRQ(USART0_TX_IRQn);

#if 0
        CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_DEFAULT;

        // Select LFXO for the EUSART
        CMU_HFXOInit(&hfxoInit);
        CMU_ClockSelectSet(cmuClock_EUSART1, cmuSelect_HFXO);

        // Default asynchronous initializer (19.2 Kbps, 8N1, no flow control)
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

        EUSART_UartInitHf(EUSART1, &init);

        // EUSART1->CFG0 &= ~(_EUSART_CFG0_SYNC_MASK);
        // EUSART1->CFG0 |= EUSART_CFG0_SYNC_ASYNC;
 
        // EUSART1->CFG1 &= ~(_EUSART_CFG1_RXTIMEOUT_MASK);
        // EUSART1->CFG1 |= EUSART_CFG1_RXTIMEOUT_SEVENFRAMES;

        // Disable EUSART1 and wait for synchronization
        // EUSART1->EN_CLR = EUSART_EN_EN;
        // while (EUSART1->EN & _EUSART_EN_DISABLING_MASK);

        // // Set RXTIMEOUT
        // EUSART1->CFG1_SET = EUSART_CFG1_RXTIMEOUT_SEVENFRAMES;
        
        // // Re-enable EUSART1 and wait for synchronization
        // EUSART1->EN_SET = EUSART_EN_EN;
        // while ((EUSART1->SYNCBUSY & _EUSART_SYNCBUSY_MASK) != 0);

        EUSART_IntEnable(EUSART1, EUSART_IEN_RXFL | EUSART_IEN_RXTO);
        EUSART_IntClear(EUSART1, EUSART_IEN_RXFL | EUSART_IEN_RXTO);

        // // First, initialize the LDMA unit itself
        LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
        LDMA_Init(&ldmaInit);

        // Source is buffer, destination is EUSART1_TXDATA, and length if BUFLEN
        ldmaTXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_M2P_BYTE(UART_LDMA_TX_BUFFER, &(EUSART1->TXDATA), UART_BUFFER_SIZE);
        ldmaTXDescriptor.xfer.blockSize = ldmaCtrlBlockSizeUnit1;
        ldmaTXDescriptor.xfer.size = ldmaCtrlSizeByte;

        // Transfer a byte on free space in the EUSART FIFO
        ldmaTXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_EUSART1_TXFL);

        // Source is EUSART1_RXDATA, destination is buffer, and length if BUFLEN
        ldmaRXDescriptor = (LDMA_Descriptor_t)LDMA_DESCRIPTOR_SINGLE_P2M_BYTE(&(EUSART1->RXDATA), UART_LDMA_RX_BUFFER, UART_BUFFER_SIZE);

        // Transfer a byte on receive FIFO level event
        ldmaRXConfig = (LDMA_TransferCfg_t)LDMA_TRANSFER_CFG_PERIPHERAL(ldmaPeripheralSignal_EUSART1_RXFL);

        NVIC_ClearPendingIRQ(EUSART1_RX_IRQn);
        NVIC_EnableIRQ(EUSART1_RX_IRQn);
        
        initTIMER();
#endif
    }

    return 0;
}

int32_t ql_uart_write(ql_uart_port_e port, uint8_t *data, uint32_t len)
{
     if(QL_UART_UART0_PORT == port)
     {
#if defined (USE_LDMA_TX_DEBUG) 

        for (uint32_t i = 0; i < len; i++)
        {
            ring_buffer_write_byte(&txDebugRingBuffer, data[i]);
        }
        
        if(tx_is_sending_debug == 0)
        {
            uint32_t remain_len = ring_buffer_available(&txDebugRingBuffer);
            uint32_t read_len = UART_BUFFER_SIZE > remain_len ? remain_len : UART_BUFFER_SIZE;

            if (read_len > 0)
            {
                ring_buffer_read(&txDebugRingBuffer, UART_LDMA_TX_BUFFER_DEBUG, read_len);

                ldmaTXDebugDescriptor.xfer.xferCnt = read_len - 1;

                LDMA_StartTransfer(TX_LDMA_CHANNEL_DEBUG, &ldmaTXDebugConfig, &ldmaTXDebugDescriptor);

                tx_is_sending_debug = 1;
            }
        }
#else
        for(uint32_t i = 0; i < len; i++)
        {
            while(!(QL_DEBUG_UART->STATUS & EUSART_STATUS_TXFL));
            QL_DEBUG_UART->TXDATA = data[i];
        }
        while(!(QL_DEBUG_UART->STATUS & EUSART_STATUS_TXC));
#endif
        return 0;
     }
     else if(QL_UART_UART1_PORT == port)
     {
#if defined (USE_LDMA_TX)
        ql_printf("[USART]USART SEND LENGTH = %d\n", len);
        for (uint32_t i = 0; i < len; i++)
        {
            ring_buffer_write_byte(&txRingBuffer, data[i]);
        }

        if (tx_is_sending == 0)
        {
            uint32_t remain_len = ring_buffer_available(&txRingBuffer);
            uint32_t read_len = UART_BUFFER_SIZE > remain_len ? remain_len : UART_BUFFER_SIZE;

            if (read_len > 0)
            {
                ring_buffer_read(&txRingBuffer, UART_LDMA_TX_BUFFER, read_len);

                ldmaTXDescriptor.xfer.xferCnt = read_len - 1;

                LDMA_StartTransfer(TX_LDMA_CHANNEL, &ldmaTXConfig, &ldmaTXDescriptor);

                tx_is_sending = 1;
            }
        }
#else
        ql_printf("[USART]USART SEND LENGTH = %d\n", len);
        for(uint32_t i = 0; i < len; i++)
        {
            while(!(USART0->STATUS & USART_STATUS_TXBL));
            USART0->TXDATA = data[i];
        }
        while(!(USART0->STATUS & USART_STATUS_TXC));
#endif

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
            uint32_t read_len  = len >= rxRingBuffer.frame_length ? rxRingBuffer.frame_length : len;
            ql_printf("[USART]USART RECEIVED LENGTH = %d\n", read_len);
            *o_len = ring_buffer_read(&rxRingBuffer, data, read_len);
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
#ifdef IS_ENBALE_DEBUG_LOG
    ql_uart_write(QL_UART_UART0_PORT, data, len);
#endif /* IS_ENBALE_DEBUG_LOG */
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

