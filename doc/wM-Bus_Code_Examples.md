# wM-Bus协议代码示例

## 目录
1. [基础数据结构](#基础数据结构)
2. [DIF/VIF解析](#difvif解析)
3. [帧格式处理](#帧格式处理)
4. [T1模式实现](#t1模式实现)
5. [C模式实现](#c模式实现)
6. [加密解密](#加密解密)
7. [错误处理](#错误处理)
8. [完整示例](#完整示例)

---

## 基础数据结构

### 设备地址结构
```c
typedef struct {
    uint16_t manufacturer_id;    // 制造商ID
    uint32_t device_id;         // 设备ID (BCD格式)
    uint8_t version;            // 版本号
    uint8_t device_type;        // 设备类型
} __attribute__((packed)) wmbus_address_t;

// 制造商ID编码/解码
uint16_t encode_manufacturer_id(const char* name) {
    if (strlen(name) != 3) return 0;
    return ((name[0] - 64) << 10) | 
           ((name[1] - 64) << 5) | 
           (name[2] - 64);
}

void decode_manufacturer_id(uint16_t id, char* name) {
    name[0] = ((id >> 10) & 0x1F) + 64;
    name[1] = ((id >> 5) & 0x1F) + 64;
    name[2] = (id & 0x1F) + 64;
    name[3] = '\0';
}
```

### 帧头结构
```c
typedef struct {
    uint8_t start_char;         // 0x68
    uint8_t length;             // 数据长度
    uint8_t length_repeat;      // 长度重复
    uint8_t start_char_repeat;  // 0x68
} __attribute__((packed)) wmbus_frame_header_t;

typedef struct {
    uint8_t control_field;      // 控制字段
    wmbus_address_t address;    // 地址字段
    uint8_t ci_field;          // CI字段
} __attribute__((packed)) wmbus_dll_header_t;

// C字段结构 (基于EN 13757标准)
typedef struct {
    uint8_t res;           // 保留位 (固定为0)
    uint8_t prm;           // 主/从标识
    uint8_t fcb_acd;       // FCB或ACD
    uint8_t fcv_dfc;       // FCV或DFC
    uint8_t function_code; // 功能码
} c_field_t;

// 构建C字段
uint8_t build_c_field(uint8_t prm, uint8_t fcb_acd,
                      uint8_t fcv_dfc, uint8_t function_code) {
    return (0 << 7) |                    // RES = 0 (固定)
           (prm << 6) |                  // PRM位
           (fcb_acd << 5) |              // FCB/ACD位
           (fcv_dfc << 4) |              // FCV/DFC位
           (function_code & 0x0F);       // 功能码(4位)
}

// 解析C字段
c_field_t parse_c_field(uint8_t c_field) {
    c_field_t parsed;
    parsed.res = (c_field >> 7) & 0x01;
    parsed.prm = (c_field >> 6) & 0x01;
    parsed.fcb_acd = (c_field >> 5) & 0x01;
    parsed.fcv_dfc = (c_field >> 4) & 0x01;
    parsed.function_code = c_field & 0x0F;
    return parsed;
}
```

---

## DIF/VIF解析

### DIF解析函数
```c
typedef struct {
    uint8_t extension_bit;      // 扩展位
    uint8_t storage_number;     // 存储号
    uint8_t data_field_coding;  // 数据字段编码
    uint8_t data_length;        // 数据长度
} dif_info_t;

dif_info_t parse_dif(uint8_t dif) {
    static const uint8_t length_table[] = {
        0, 1, 2, 3, 4, 4, 6, 8,  // 0x00-0x07
        0, 2, 4, 6, 8, 0, 12, 0  // 0x08-0x0F
    };
    
    dif_info_t info;
    info.extension_bit = (dif & 0x80) ? 1 : 0;
    info.storage_number = (dif >> 4) & 0x07;
    info.data_field_coding = dif & 0x0F;
    info.data_length = length_table[info.data_field_coding];
    
    return info;
}
```

## 扩展链路层代码示例

### 扩展链路层结构定义
```c
// 扩展链路层基础结构
typedef struct {
    uint8_t ci_field;       // CI字段 (0x8C-0x8G)
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
} __attribute__((packed)) extended_link_base_t;

// CI=0x8D - 基本扩展链路层
typedef struct {
    uint8_t ci_field;       // 0x8D
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
} __attribute__((packed)) extended_link_8d_t;

// CI=0x8E - 带加密的扩展链路层
typedef struct {
    uint8_t ci_field;       // 0x8E
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
    uint32_t sequence_number; // 序列号
    uint16_t payload_crc;   // 载荷CRC
} __attribute__((packed)) extended_link_8e_t;

// 扩展链路层解析结果
typedef struct {
    uint8_t ci_value;
    uint8_t cc_field;
    uint8_t acc_field;
    uint32_t sequence_number;
    uint16_t payload_crc;
    uint8_t* variable_data;
    uint8_t variable_length;
    uint8_t is_valid;
} extended_link_info_t;

// 解析扩展链路层
extended_link_info_t parse_extended_link_layer(const uint8_t* data, uint32_t length) {
    extended_link_info_t info = {0};

    if (length < 3) {
        return info;  // 最少需要3字节
    }

    uint32_t offset = 0;
    info.ci_value = data[offset++];
    info.cc_field = data[offset++];
    info.acc_field = data[offset++];

    switch (info.ci_value) {
        case 0x8D:
            // 基本扩展链路层
            info.is_valid = (length >= 3);
            break;

        case 0x8E:
            // 带加密的扩展链路层
            if (length >= 8) {
                memcpy(&info.sequence_number, &data[offset], 4);
                offset += 4;
                memcpy(&info.payload_crc, &data[offset], 2);
                offset += 2;
                info.is_valid = 1;
            }
            break;

        case 0x8C:
            // 可变长度扩展链路层
            if (length >= 3) {
                info.variable_length = length - 3;
                if (info.variable_length > 0) {
                    info.variable_data = (uint8_t*)&data[offset];
                }
                info.is_valid = 1;
            }
            break;
    }

    return info;
}

// 构建扩展链路层
uint32_t build_extended_link_8d(uint8_t* buffer, uint8_t cc, uint8_t acc) {
    extended_link_8d_t* ext = (extended_link_8d_t*)buffer;
    ext->ci_field = 0x8D;
    ext->cc_field = cc;
    ext->acc_field = acc;
    return sizeof(extended_link_8d_t);
}

// 构建带加密的扩展链路层
uint32_t build_extended_link_8e(uint8_t* buffer, uint8_t cc, uint8_t acc,
                               uint32_t seq_num, uint16_t crc) {
    extended_link_8e_t* ext = (extended_link_8e_t*)buffer;
    ext->ci_field = 0x8E;
    ext->cc_field = cc;
    ext->acc_field = acc;
    ext->sequence_number = seq_num;
    ext->payload_crc = crc;
    return sizeof(extended_link_8e_t);
}
```

### VIF解析函数
```c
typedef struct {
    const char* unit;           // 单位
    const char* description;    // 描述
    double multiplier;          // 倍数
    uint8_t is_known;          // 是否已知VIF
} vif_info_t;

vif_info_t parse_vif(uint8_t vif) {
    vif_info_t info = {0};
    
    // 体积 (0x10-0x17)
    if (vif >= 0x10 && vif <= 0x17) {
        static const char* units[] = {"m³", "m³", "m³", "L", "m³", "m³", "m³", "m³"};
        static const double multipliers[] = {1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1.0, 10.0};
        
        int index = vif - 0x10;
        info.unit = units[index];
        info.description = "体积";
        info.multiplier = multipliers[index];
        info.is_known = 1;
    }
    // 流量 (0x38-0x3F)
    else if (vif >= 0x38 && vif <= 0x3F) {
        static const double multipliers[] = {1e-6, 1e-5, 1e-4, 1e-3, 1e-2, 1e-1, 1.0, 10.0};
        
        int index = vif - 0x38;
        info.unit = (index == 3) ? "L/h" : "m³/h";
        info.description = "流量";
        info.multiplier = multipliers[index];
        info.is_known = 1;
    }
    else {
        info.unit = "未知";
        info.description = "未知VIF";
        info.multiplier = 1.0;
        info.is_known = 0;
    }
    
    return info;
}
```

### 数据记录解析
```c
typedef struct {
    dif_info_t dif_info;
    vif_info_t vif_info;
    uint8_t raw_data[12];
    uint32_t numeric_value;
    double scaled_value;
    uint8_t is_valid;
} data_record_t;

data_record_t parse_data_record(const uint8_t* data, uint32_t max_len) {
    data_record_t record = {0};
    
    if (max_len < 2) return record;
    
    record.dif_info = parse_dif(data[0]);
    record.vif_info = parse_vif(data[1]);
    
    uint32_t data_offset = 2;
    uint8_t data_len = record.dif_info.data_length;
    
    if (data_offset + data_len <= max_len && data_len <= sizeof(record.raw_data)) {
        memcpy(record.raw_data, &data[data_offset], data_len);
        
        // 解析数值 (小端序)
        if (data_len <= 4) {
            record.numeric_value = 0;
            memcpy(&record.numeric_value, record.raw_data, data_len);
            record.scaled_value = record.numeric_value * record.vif_info.multiplier;
        }
        
        record.is_valid = 1;
    }
    
    return record;
}
```

---

## 帧格式处理

### 帧构建函数
```c
uint32_t build_wmbus_frame(uint8_t* buffer, uint32_t buffer_size,
                          uint8_t control_field,
                          const wmbus_address_t* address,
                          uint8_t ci_field,
                          const uint8_t* payload, uint32_t payload_len) {
    
    if (buffer_size < (10 + payload_len)) return 0;
    
    uint32_t offset = 0;
    uint8_t data_length = 8 + 1 + payload_len;  // 地址+CI+载荷
    
    // 帧头
    buffer[offset++] = 0x68;                    // 起始字符
    buffer[offset++] = data_length;             // 长度
    buffer[offset++] = data_length;             // 长度重复
    buffer[offset++] = 0x68;                    // 起始字符重复
    
    // 控制字段
    buffer[offset++] = control_field;
    
    // 地址字段
    memcpy(&buffer[offset], address, sizeof(wmbus_address_t));
    offset += sizeof(wmbus_address_t);
    
    // CI字段
    buffer[offset++] = ci_field;
    
    // 载荷
    if (payload && payload_len > 0) {
        memcpy(&buffer[offset], payload, payload_len);
        offset += payload_len;
    }
    
    // 帧尾
    buffer[offset++] = 0x16;
    
    return offset;
}
```

### 帧解析函数
```c
typedef struct {
    uint8_t is_valid;
    uint8_t control_field;
    wmbus_address_t address;
    uint8_t ci_field;
    uint8_t* payload;
    uint32_t payload_length;
} wmbus_frame_t;

wmbus_frame_t parse_wmbus_frame(const uint8_t* data, uint32_t data_len) {
    wmbus_frame_t frame = {0};
    
    // 检查最小长度
    if (data_len < 12) return frame;
    
    // 检查帧头
    if (data[0] != 0x68 || data[3] != 0x68) return frame;
    
    uint8_t declared_length = data[1];
    if (data[2] != declared_length) return frame;
    
    // 检查帧尾
    if (data[4 + declared_length] != 0x16) return frame;
    
    uint32_t offset = 4;
    
    // 解析控制字段
    frame.control_field = data[offset++];
    
    // 解析地址字段
    memcpy(&frame.address, &data[offset], sizeof(wmbus_address_t));
    offset += sizeof(wmbus_address_t);
    
    // 解析CI字段
    frame.ci_field = data[offset++];
    
    // 载荷
    frame.payload_length = declared_length - 9;  // 总长度 - 控制字段 - 地址 - CI
    if (frame.payload_length > 0) {
        frame.payload = (uint8_t*)&data[offset];
    }
    
    frame.is_valid = 1;
    return frame;
}
```

---

## T1模式实现

### T1模式发送
```c
typedef struct {
    uint32_t send_interval_ms;  // 发送间隔
    uint32_t last_send_time;    // 上次发送时间
    wmbus_address_t device_addr; // 设备地址
    uint8_t access_number;      // 访问号
} t1_context_t;

void t1_mode_send_data(t1_context_t* ctx, const data_record_t* records, uint32_t record_count) {
    uint8_t frame_buffer[256];
    uint8_t payload_buffer[200];
    uint32_t payload_len = 0;
    
    // 构建STL头部
    payload_buffer[payload_len++] = ctx->access_number++;
    payload_buffer[payload_len++] = 0x00;  // 状态
    payload_buffer[payload_len++] = 0x00;  // 配置字低字节
    payload_buffer[payload_len++] = 0x00;  // 配置字高字节
    
    // 添加数据记录
    for (uint32_t i = 0; i < record_count; i++) {
        const data_record_t* record = &records[i];
        
        payload_buffer[payload_len++] = (record->dif_info.storage_number << 4) | 
                                       record->dif_info.data_field_coding;
        payload_buffer[payload_len++] = record->vif_info.raw_value;
        
        memcpy(&payload_buffer[payload_len], record->raw_data, record->dif_info.data_length);
        payload_len += record->dif_info.data_length;
    }
    
    // 构建完整帧
    uint32_t frame_len = build_wmbus_frame(frame_buffer, sizeof(frame_buffer),
                                          0x08,  // SND-NR
                                          &ctx->device_addr,
                                          0x7A,  // 短传输层
                                          payload_buffer, payload_len);
    
    // 发送帧
    if (frame_len > 0) {
        rf_transmit(frame_buffer, frame_len);
        ctx->last_send_time = get_system_time_ms();
    }
}

// T1模式主循环
void t1_mode_task(t1_context_t* ctx) {
    uint32_t current_time = get_system_time_ms();
    
    if ((current_time - ctx->last_send_time) >= ctx->send_interval_ms) {
        // 准备数据记录
        data_record_t records[3];
        uint32_t record_count = 0;
        
        // 添加体积记录
        records[record_count].dif_info.data_field_coding = 0x04;  // 32位整数
        records[record_count].dif_info.data_length = 4;
        records[record_count].vif_info.raw_value = 0x13;  // 体积(升)
        uint32_t volume = get_current_volume();
        memcpy(records[record_count].raw_data, &volume, 4);
        record_count++;
        
        // 发送数据
        t1_mode_send_data(ctx, records, record_count);
    }
}
```

---

## C模式实现

### C模式命令处理
```c
typedef struct {
    uint8_t is_listening;
    uint32_t listen_timeout_ms;
    wmbus_address_t device_addr;
    uint8_t access_number;
} c_mode_context_t;

void c_mode_handle_command(c_mode_context_t* ctx, const wmbus_frame_t* frame) {
    uint8_t response_buffer[256];
    uint32_t response_len = 0;
    
    switch (frame->control_field & 0x0F) {
        case 0x03:  // REQ-UD1
            response_len = build_read_response(response_buffer, sizeof(response_buffer));
            break;
            
        case 0x04:  // REQ-UD2
            response_len = build_read_response_2(response_buffer, sizeof(response_buffer));
            break;
            
        case 0x02:  // SND-UD
            handle_write_command(frame->payload, frame->payload_length);
            response_len = build_ack_response(response_buffer, sizeof(response_buffer));
            break;
            
        default:
            return;  // 不支持的命令
    }
    
    // 发送响应
    if (response_len > 0) {
        uint8_t frame_buffer[300];
        uint32_t frame_len = build_wmbus_frame(frame_buffer, sizeof(frame_buffer),
                                              0x05,  // RSP-UD
                                              &ctx->device_addr,
                                              0x7A,  // 短传输层
                                              response_buffer, response_len);
        
        rf_transmit(frame_buffer, frame_len);
    }
}

// C模式接收处理
void c_mode_receive_task(c_mode_context_t* ctx) {
    uint8_t rx_buffer[256];
    uint32_t rx_len;
    
    if (rf_receive(rx_buffer, sizeof(rx_buffer), &rx_len)) {
        wmbus_frame_t frame = parse_wmbus_frame(rx_buffer, rx_len);
        
        if (frame.is_valid && 
            memcmp(&frame.address, &ctx->device_addr, sizeof(wmbus_address_t)) == 0) {
            c_mode_handle_command(ctx, &frame);
        }
    }
}
```

---

## 加密解密

### AES-128加密实现
```c
#include "aes.h"  // AES库

typedef struct {
    uint8_t key[16];            // AES密钥
    uint8_t encryption_mode;    // 加密模式
} encryption_context_t;

void encrypt_payload(encryption_context_t* ctx, 
                    const wmbus_address_t* address,
                    uint8_t access_number,
                    uint8_t* data, uint32_t data_len) {
    
    if (ctx->encryption_mode != 5) return;  // 仅支持模式5
    
    // 构建初始向量 (IV)
    uint8_t iv[16];
    memcpy(iv, address, 8);                 // 设备地址
    memset(iv + 8, access_number, 8);       // 访问号填充
    
    // AES-CTR加密
    aes_context_t aes_ctx;
    aes_init(&aes_ctx, ctx->key, 128);
    
    uint8_t counter[16];
    memcpy(counter, iv, 16);
    
    for (uint32_t i = 0; i < data_len; i += 16) {
        uint8_t keystream[16];
        aes_encrypt(&aes_ctx, counter, keystream);
        
        uint32_t block_len = (data_len - i > 16) ? 16 : (data_len - i);
        for (uint32_t j = 0; j < block_len; j++) {
            data[i + j] ^= keystream[j];
        }
        
        // 递增计数器
        increment_counter(counter);
    }
}

void decrypt_payload(encryption_context_t* ctx,
                    const wmbus_address_t* address,
                    uint8_t access_number,
                    uint8_t* data, uint32_t data_len) {
    // 解密与加密相同 (CTR模式)
    encrypt_payload(ctx, address, access_number, data, data_len);
}
```

---

## 错误处理

### 错误码定义和处理
```c
typedef enum {
    WMBUS_OK = 0,
    WMBUS_ERROR_CRC = 1,
    WMBUS_ERROR_LENGTH = 2,
    WMBUS_ERROR_ADDRESS = 3,
    WMBUS_ERROR_COMMAND = 4,
    WMBUS_ERROR_TIMEOUT = 5,
    WMBUS_ERROR_ENCRYPTION = 6
} wmbus_error_t;

const char* wmbus_error_string(wmbus_error_t error) {
    static const char* error_strings[] = {
        "成功",
        "CRC错误",
        "长度错误", 
        "地址错误",
        "命令错误",
        "超时错误",
        "加密错误"
    };
    
    if (error < sizeof(error_strings) / sizeof(error_strings[0])) {
        return error_strings[error];
    }
    return "未知错误";
}

// 错误处理函数
void handle_wmbus_error(wmbus_error_t error, const uint8_t* frame_data, uint32_t frame_len) {
    printf("wM-Bus错误: %s\n", wmbus_error_string(error));
    
    switch (error) {
        case WMBUS_ERROR_CRC:
            // 请求重传
            request_retransmission();
            break;
            
        case WMBUS_ERROR_TIMEOUT:
            // 增加重试计数
            increment_retry_count();
            break;
            
        case WMBUS_ERROR_ENCRYPTION:
            // 检查密钥配置
            verify_encryption_key();
            break;
            
        default:
            // 记录错误日志
            log_error(error, frame_data, frame_len);
            break;
    }
}
```

---

## 完整示例

### 水表设备完整实现
```c
#include "wmbus_protocol.h"

typedef struct {
    wmbus_address_t address;
    t1_context_t t1_ctx;
    encryption_context_t enc_ctx;
    uint32_t current_volume;
    uint32_t current_flow;
    uint32_t error_count;
} water_meter_t;

void water_meter_init(water_meter_t* meter) {
    // 初始化设备地址
    meter->address.manufacturer_id = encode_manufacturer_id("ABC");
    meter->address.device_id = 0x12345678;
    meter->address.version = 0x01;
    meter->address.device_type = 0x07;  // 水表
    
    // 初始化T1模式
    meter->t1_ctx.device_addr = meter->address;
    meter->t1_ctx.send_interval_ms = 3600000;  // 1小时
    meter->t1_ctx.access_number = 0;
    
    // 初始化加密
    meter->enc_ctx.encryption_mode = 5;  // AES-128
    // 设置密钥 (实际应用中从安全存储读取)
    memset(meter->enc_ctx.key, 0xAB, 16);
    
    // 初始化测量值
    meter->current_volume = 0;
    meter->current_flow = 0;
    meter->error_count = 0;
}

void water_meter_task(water_meter_t* meter) {
    // 更新测量值
    meter->current_volume = read_volume_sensor();
    meter->current_flow = read_flow_sensor();
    
    // T1模式定期发送
    uint32_t current_time = get_system_time_ms();
    if ((current_time - meter->t1_ctx.last_send_time) >= meter->t1_ctx.send_interval_ms) {
        
        // 准备数据记录
        data_record_t records[2];
        
        // 体积记录
        records[0].dif_info.data_field_coding = 0x04;  // 32位整数
        records[0].dif_info.data_length = 4;
        records[0].dif_info.storage_number = 0;
        records[0].vif_info.raw_value = 0x13;  // 体积(升)
        memcpy(records[0].raw_data, &meter->current_volume, 4);
        
        // 流量记录
        records[1].dif_info.data_field_coding = 0x04;  // 32位整数
        records[1].dif_info.data_length = 4;
        records[1].dif_info.storage_number = 0;
        records[1].vif_info.raw_value = 0x3B;  // 流量(L/h)
        memcpy(records[1].raw_data, &meter->current_flow, 4);
        
        // 发送数据
        t1_mode_send_data(&meter->t1_ctx, records, 2);
    }
}

int main() {
    water_meter_t meter;
    
    // 初始化系统
    system_init();
    rf_init();
    
    // 初始化水表
    water_meter_init(&meter);
    
    printf("水表启动完成\n");
    printf("制造商: ABC\n");
    printf("设备ID: %08X\n", meter.address.device_id);
    printf("发送间隔: %d秒\n", meter.t1_ctx.send_interval_ms / 1000);
    
    // 主循环
    while (1) {
        water_meter_task(&meter);
        system_sleep_ms(1000);  // 1秒检查一次
    }
    
    return 0;
}
```

---

*代码示例 v1.0 - 基于EN 13757和OMS标准实现*
