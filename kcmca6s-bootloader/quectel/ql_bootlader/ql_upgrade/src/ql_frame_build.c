#include "ql_frame_build.h"

static ql_frame_builder_t* frame_builder_create(uint8_t cmd, uint8_t* payload, uint16_t payload_len)
{
    ql_frame_builder_t* builder = (ql_frame_builder_t*)sl_malloc(sizeof(ql_frame_builder_t));
    
    if (!builder) 
    {
        ql_printf("builder is null, sizeof(ql_frame_builder_t):%d\r\n", sizeof(ql_frame_builder_t));
        return NULL;
    }

    builder->frame.header.head = 0x02;
    builder->frame.header.command = cmd;
    builder->frame.header.len = sizeof(uint8_t) + payload_len + sizeof(uint16_t);
    builder->frame.payload = payload;
    builder->frame.payload_len = payload_len;

    builder->buffer_size = sizeof(ql_frame_header_t) + payload_len + sizeof(uint16_t);
    builder->buffer = (uint8_t*)sl_malloc(builder->buffer_size);

    if (!builder->buffer) 
    {
        sl_free(builder);
        return NULL;
    }

    return builder;
}

static void frame_builder_destroy(ql_frame_builder_t* builder)
{
    if (builder)
     {
        if (builder->buffer) 
        {
            sl_free(builder->buffer);
        }
        sl_free(builder);
    }
}

static int frame_builder_pack(ql_frame_builder_t* builder)
{
    if (!builder || !builder->buffer) 
    {
        return -1;
    }

    uint32_t offset = 0;
    uint8_t *dst = builder->buffer;
    
    dst[offset++] = builder->frame.header.head;
    dst[offset++] = (builder->frame.header.len >> 8) & 0xFF;
    dst[offset++] = builder->frame.header.len & 0xFF;
    dst[offset++] = builder->frame.header.command;

    if (builder->frame.payload && (builder->frame.payload_len > 0)) 
    {
        memcpy(dst + offset, builder->frame.payload, builder->frame.payload_len);
        offset += builder->frame.payload_len;
    }
    
    builder->frame.crc16 = crc16_calculate((uint8_t *)&dst[1] , offset - 1);
    dst[offset++] = builder->frame.crc16 & 0xFF;
    dst[offset++] = (builder->frame.crc16 >> 8) & 0xFF;
    return 0;
}

static int frame_builder_send(ql_frame_builder_t* builder)
{
    if (!builder || !builder->buffer) 
    {
        return -1;
    }

    return ql_uart_write(QL_UART_UART1_PORT, builder->buffer, builder->buffer_size);
}

int ql_frame_build_send(uint8_t cmd, uint8_t* payload, uint16_t payload_len)
{
    ql_frame_builder_t* builder = frame_builder_create(cmd, payload, payload_len);
    if (!builder)
    {
        return -1;
    }

    int ret = frame_builder_pack(builder);

    if (ret == 0)
    {
        ret = frame_builder_send(builder);
    }

    frame_builder_destroy(builder);
    return ret;
}
