#include "ql_uart.h"

static int32_t ql_uart_open_impl(ql_uart_t *self) 
{
    self->is_opened = 1;
    return ql_uart_open(self->port);
}

//static int32_t ql_uart_close_impl(ql_uart_t *self)
//{
//    // return ql_uart_close(self->port);
//    return 1;
//}

static int32_t ql_uart_write_impl(ql_uart_t *self, const uint8_t *data, uint32_t len) 
{
    if (self == NULL || data == NULL || len == 0) 
    {
        return -1;
    }

    if (!self->is_opened) 
    {
        return -1; // UART not opened
    }

    return ql_uart_write(self->port, (uint8_t *)data, len);
}

static int32_t ql_uart_read_impl(ql_uart_t *self, uint8_t *data, uint32_t len, uint32_t *o_len) 
{
    if (self == NULL || data == NULL || len == 0) 
    {
        return -1;
    }

    if (!self->is_opened) 
    {
        return -1; // UART not opened
    }

    return ql_uart_read(self->port, data, len, o_len);
}

//static uint8_t ql_uart_read_char_impl(ql_uart_t *self)
//{
//    // return ql_uart_read_char(self->port);
//    return 1;
//}
//
//static uint8_t ql_uart_write_char_impl(ql_uart_t *self, uint8_t c)
//{
//    // return ql_uart_write_char(self->port, c);
//    return 1;
//}

static uint32_t ql_uart_available_impl(ql_uart_t *self) 
{
    if (self == NULL)
    {
        return -1;
    }

    if (!self->is_opened) 
    {
        return -1; // UART not opened
    }
    
    return ql_uart_available(self->port);
}

uint32_t ql_uart_deinit(ql_uart_t *uart)
{
    if(uart != NULL) 
    {
        if (uart->is_opened) 
        {
            uart->vptr->close(uart);
        }
        sl_free(uart);
    }

    return 0;
}

static const ql_uart_vtbl_t ql_uart_vtbl = 
{
    .open = ql_uart_open_impl,
    .close = NULL,
    .write = ql_uart_write_impl,
    .read = ql_uart_read_impl,
    .read_char = NULL,
    .write_char = NULL,
    .available = ql_uart_available_impl,
    .deinit = ql_uart_deinit
};

ql_uart_t *ql_uart_create(ql_uart_port_e port) 
{
    ql_uart_t *uart = (ql_uart_t *)sl_malloc(sizeof(ql_uart_t));
    if (!uart) return NULL;

    uart->vptr = &ql_uart_vtbl;
    uart->port = port;
    uart->is_opened = 0;

    return uart;
}

