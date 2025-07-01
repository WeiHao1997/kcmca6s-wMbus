# wM-Bus 扩展链路层使用示例

## 概述

扩展链路层为wM-Bus协议提供了高级功能，特别适用于无线通信场景。本文档提供了详细的使用示例和最佳实践。

## 基础概念

### 扩展链路层的作用
- **通信控制**: 同步、流控制、重传管理
- **安全功能**: 加密、认证、序列号管理
- **网络功能**: 目标地址、路由控制
- **服务质量**: 优先级、确认机制

### CI字段映射
| CI值 | 长度 | 主要用途 | 适用场景 |
|------|------|----------|----------|
| 0x8C | 3-20字节 | 可选择服务 | 灵活配置 |
| 0x8D | 3字节 | 基本控制 | 简单同步 |
| 0x8E | 8字节 | 安全通信 | 加密传输 |
| 0x8F | 10字节 | 网络路由 | 多跳通信 |

## 实际应用示例

### 示例1: 基本同步通信 (CI=0x8D)

```c
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// 场景：简单的主从同步通信
void example_basic_sync_communication() {
    uint8_t frame_buffer[256];
    uint32_t offset = 0;
    
    // 构建wM-Bus帧头
    frame_buffer[offset++] = 0x68;  // 起始字符
    frame_buffer[offset++] = 0x0B;  // 长度 (11字节)
    frame_buffer[offset++] = 0x0B;  // 长度重复
    frame_buffer[offset++] = 0x68;  // 起始字符重复
    
    // C字段 - 主机请求数据
    frame_buffer[offset++] = 0x53;  // REQ-UD1
    
    // 地址字段 (6字节)
    frame_buffer[offset++] = 0x12;  // ID[0]
    frame_buffer[offset++] = 0x34;  // ID[1] 
    frame_buffer[offset++] = 0x56;  // ID[2]
    frame_buffer[offset++] = 0x78;  // ID[3]
    frame_buffer[offset++] = 0x01;  // 版本
    frame_buffer[offset++] = 0x07;  // 设备类型 (水表)
    
    // 扩展链路层 (CI=0x8D)
    frame_buffer[offset++] = 0x8D;  // CI字段
    
    // CC字段 - 通信控制
    uint8_t cc_field = 0x01 |      // 同步模式
                       0x08;       // 确认请求
    frame_buffer[offset++] = cc_field;
    
    // ACC字段 - 访问控制  
    uint8_t acc_field = 0x02 |     // 已认证
                        0x08;      // 会话状态
    frame_buffer[offset++] = acc_field;
    
    // 帧尾
    frame_buffer[offset++] = 0x16;
    
    printf("基本同步通信帧:\n");
    for (int i = 0; i < offset; i++) {
        printf("%02X ", frame_buffer[i]);
    }
    printf("\n");
}
```

### 示例2: 安全加密通信 (CI=0x8E)

```c
// 场景：带加密的安全通信
void example_secure_communication() {
    uint8_t frame_buffer[256];
    uint32_t offset = 0;
    
    // 构建wM-Bus帧头 (省略标准部分)
    // ... 标准帧头构建 ...
    
    // 扩展链路层 (CI=0x8E)
    frame_buffer[offset++] = 0x8E;  // CI字段
    
    // CC字段 - 高安全通信控制
    uint8_t cc_field = 0x80 |      // 扩展标志
                       0x40 |      // 高安全级别
                       0x08 |      // 确认请求
                       0x01;       // 同步模式
    frame_buffer[offset++] = cc_field;
    
    // ACC字段 - 安全访问控制
    uint8_t acc_field = 0x06 |     // 已认证 + 加密
                        0x08;      // 会话状态
    frame_buffer[offset++] = acc_field;
    
    // 序列号 (4字节) - 防重放攻击
    uint32_t sequence_number = 0x12345678;
    memcpy(&frame_buffer[offset], &sequence_number, 4);
    offset += 4;
    
    // 载荷CRC (2字节) - 数据完整性
    uint16_t payload_crc = 0xABCD;
    memcpy(&frame_buffer[offset], &payload_crc, 2);
    offset += 2;
    
    printf("安全加密通信帧:\n");
    for (int i = 0; i < offset; i++) {
        printf("%02X ", frame_buffer[i]);
    }
    printf("\n");
}
```

### 示例3: 网络路由通信 (CI=0x8F)

```c
// 场景：多跳网络路由
void example_network_routing() {
    uint8_t frame_buffer[256];
    uint32_t offset = 0;
    
    // 构建wM-Bus帧头 (省略标准部分)
    // ... 标准帧头构建 ...
    
    // 扩展链路层 (CI=0x8F)
    frame_buffer[offset++] = 0x8F;  // CI字段
    
    // CC字段 - 网络路由控制
    uint8_t cc_field = 0x20 |      // 路由控制
                       0x04 |      // 高优先级
                       0x08;       // 确认请求
    frame_buffer[offset++] = cc_field;
    
    // ACC字段 - 网络访问控制
    uint8_t acc_field = 0x20 |     // 网络状态
                        0x40;      // 扩展服务
    frame_buffer[offset++] = acc_field;
    
    // M2字段 - 制造商代码
    frame_buffer[offset++] = 0x5A;  // 目标设备制造商
    
    // A2字段 - 目标地址 (6字节)
    frame_buffer[offset++] = 0x87;  // 目标ID[0]
    frame_buffer[offset++] = 0x65;  // 目标ID[1]
    frame_buffer[offset++] = 0x43;  // 目标ID[2]
    frame_buffer[offset++] = 0x21;  // 目标ID[3]
    frame_buffer[offset++] = 0x02;  // 目标版本
    frame_buffer[offset++] = 0x03;  // 目标设备类型
    
    printf("网络路由通信帧:\n");
    for (int i = 0; i < offset; i++) {
        printf("%02X ", frame_buffer[i]);
    }
    printf("\n");
}
```

## 解析示例

### 扩展链路层解析函数

```c
typedef struct {
    uint8_t ci_value;
    uint8_t cc_field;
    uint8_t acc_field;
    uint32_t sequence_number;
    uint16_t payload_crc;
    uint8_t m2_field;
    uint8_t target_address[6];
    uint8_t* variable_data;
    uint8_t variable_length;
    uint8_t is_valid;
} extended_link_result_t;

extended_link_result_t parse_extended_link(const uint8_t* data, uint32_t length) {
    extended_link_result_t result = {0};
    
    if (length < 3) return result;
    
    uint32_t offset = 0;
    result.ci_value = data[offset++];
    result.cc_field = data[offset++];
    result.acc_field = data[offset++];
    
    switch (result.ci_value) {
        case 0x8D:
            result.is_valid = (length >= 3);
            break;
            
        case 0x8E:
            if (length >= 8) {
                memcpy(&result.sequence_number, &data[offset], 4);
                offset += 4;
                memcpy(&result.payload_crc, &data[offset], 2);
                result.is_valid = 1;
            }
            break;
            
        case 0x8F:
            if (length >= 10) {
                result.m2_field = data[offset++];
                memcpy(result.target_address, &data[offset], 6);
                result.is_valid = 1;
            }
            break;
    }
    
    return result;
}
```

## 最佳实践

### 1. 安全通信建议
- 始终使用CI=0x8E进行敏感数据传输
- 实现序列号管理防止重放攻击
- 定期更新加密密钥

### 2. 网络路由建议
- 使用CI=0x8F进行多跳通信
- 实现路由表管理
- 监控网络拓扑变化

### 3. 性能优化建议
- 根据应用需求选择合适的CI类型
- 避免不必要的扩展字段
- 实现智能重传机制

---

*文档版本: v1.0*
*基于EN 13757标准*
*创建日期: 2024-12-26*
