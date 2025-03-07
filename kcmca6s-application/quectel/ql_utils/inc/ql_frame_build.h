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

typedef enum 
{
  QL_WMBUS_FUNCTION_SND_NKE = 0x0, /**< SND-NKE */
  QL_WMBUS_FUNCTION_SND_UD = 0x3,  /**< SND-UD/SND-UD2 */
  QL_WMBUS_FUNCTION_SND_NR = 0x4,  /**< SND-NR */
  QL_WMBUS_FUNCTION_SND_IR = 0x6,  /**< SND-IR */
  QL_WMBUS_FUNCTION_ACC_NR = 0x7,  /**< ACC-NR */
  QL_WMBUS_FUNCTION_ACC_DMD = 0x8, /**< ACC-DMD */
  QL_WMBUS_FUNCTION_REQ_UD1 = 0xA, /**< REQ-UD1 */
  QL_WMBUS_FUNCTION_REQ_UD2 = 0xB, /**< REQ-UD2 */
} ql_wmbus_function_code_e;

typedef enum 
{
  QL_WMBUS_ACCESSIBILITY_NO_ACCESS = 0,        /**< No access (unidirectional) */
  QL_WMBUS_ACCESSIBILITY_TEMPORARY_NO_ACCESS,  /**< Bidirectional, temporary no access */
  QL_WMBUS_ACCESSIBILITY_LIMITED_ACCESS,       /**< Bidirectional, limited access (short time after TX) */
  QL_WMBUS_ACCESSIBILITY_UNLIMITED_ACCESS,     /**< Bidirectional, unlimited access */
} ql_wmbus_accessibility_e;

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

typedef struct 
{
    ql_wmbus_function_code_e function_code : 4;
    bool fcv_dcf : 1;
    bool fcb_acd : 1;
    bool primary : 1; 
} ql_wmbus_frame_c_field_t;

typedef union 
{
  uint16_t raw;
  struct 
  {
    bool hopCounter : 1;
    bool repeatedAccess : 1;
    uint8_t contentOfMessage : 2;
    uint8_t num_of_enc_blocks : 4;
    uint8_t mode : 5;
    bool synchronized : 1;
    ql_wmbus_accessibility_e accessibility : 2;
  } mode_0_5;
} ql_wmbus_frame_config_word_t;

typedef struct {
  uint8_t ci_field;
  uint8_t access_number;
  uint8_t status;
  ql_wmbus_frame_config_word_t config_word;
} ql_wmbus_frame_compose_header_t;

typedef struct 
{
    uint16_t manufacturer_id;
    uint32_t device_identify;
    uint8_t version;
    uint8_t device_type;
} __attribute__((packed)) ql_wmbus_frame_device_info_t;


#endif /* __QL_FRAME_BUILD_H__ */
