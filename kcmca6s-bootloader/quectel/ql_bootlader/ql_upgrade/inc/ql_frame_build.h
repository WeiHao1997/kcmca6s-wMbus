#ifndef __QL_FRAME_BUILD_H__
#define __QL_FRAME_BUILD_H__

#include "ql_include.h"

int ql_frame_build_send(uint8_t cmd, uint8_t* payload, uint16_t payload_len);

typedef enum
{
    QL_FRAME_CMD_FW_UPG_INFO_REQ = 0xB0,
    QL_FRAME_CMD_FW_UPG_INFO_RES = 0xB8,

    QL_FRAME_CMD_FW_DATA_REQUEST = 0xD0,
    QL_FRAME_CMD_FW_DATA_RESPONSE = 0xD8,

    QL_FRAME_CMD_SET_REQUEST = 0x22,
    QL_FRAME_CMD_SET_RESPONSE = 0x33
}ql_frame_upg_commad_e;

typedef struct 
{
    uint8_t  head;
    uint8_t  command;
    uint16_t len;
} __attribute__((packed)) ql_frame_header_t;

typedef struct 
{
    ql_frame_header_t header;
    uint8_t* payload;
    uint16_t payload_len;
    uint16_t crc16;
} ql_bootloader_upg_frame_t;

typedef struct 
{
    ql_bootloader_upg_frame_t frame;
    uint8_t* buffer;
    uint32_t buffer_size;
} ql_frame_builder_t;

#endif /* __QL_FRAME_BUILD_H__ */
