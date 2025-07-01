# wM-Bus Frame Format A/B 与 CI字段关系详解

## 概述

本文档详细解析EN 13757-4标准中定义的Frame Format A和Frame Format B，以及它们与CI字段中LONG报文的关系。

## Frame Format基础概念

### ⚠️ **重要澄清: L字段的255字节限制**

**关键理解点:**
Frame Format B的L字段确实只有1字节，因此**单个Frame Format B帧的最大长度仍然是255字节**。

**那么如何处理超过255字节的数据？**

#### **解决方案1: 多帧序列传输**
```
大数据 (例如500字节) 分解为:
┌─────────────┐ ┌─────────────┐ ┌─────────────┐
│ Frame B #1  │ │ Frame B #2  │ │ Frame B #3  │
│ L=255字节   │ │ L=255字节   │ │ L=190字节   │
└─────────────┘ └─────────────┘ └─────────────┘
```

#### **解决方案2: 应用层分片**
```c
// 应用层负责数据分片和重组
typedef struct {
    uint8_t fragment_id;     // 分片ID
    uint8_t total_fragments; // 总分片数
    uint8_t fragment_data[250]; // 分片数据
} fragment_t;
```

#### **解决方案3: 使用更高层协议**
- **长传输层(LTL)**: 在传输层处理大数据分片
- **扩展链路层**: 提供序列号和重组机制

### 层次结构关系
```
┌─────────────────────────────────────┐
│          物理层帧格式               │
│     (Frame Format A/B)              │
├─────────────────────────────────────┤
│         数据链路层                  │
│    (L, C, M, A fields)              │
├─────────────────────────────────────┤
│         CI字段                      │
│   (Control Information)             │
├─────────────────────────────────────┤
│      传输层 (STL/LTL)               │
│    或 扩展链路层                    │
├─────────────────────────────────────┤
│         应用层数据                  │
│    (DIF/VIF + 数据记录)             │
└─────────────────────────────────────┘
```

### 核心区别

| 特性 | Frame Format A | Frame Format B |
|------|----------------|----------------|
| **L字段限制** | ≤ 255字节 | ≤ 255字节 (单帧限制) |
| **实际数据容量** | ≤ 255字节 | ≤ 255字节 (但通过多帧实现更大传输) |
| **CRC策略** | 单个CRC覆盖整个数据块 | 多个CRC，每块独立校验 |
| **块大小** | 第二块≤15字节 | 第二块≤115字节 |
| **多帧支持** | 不支持 | 支持多帧序列传输 |
| **适用场景** | 标准抄表，小数据量 | 中等数据传输，可靠性要求高 |
| **复杂度** | 简单，处理快速 | 复杂，但可靠性高 |

## Frame Format A 详细分析

### 结构组成

#### 第一块 (Table 27)
```
┌────────┬────────┬────────┬────────┬────────┐
│ L字段	│ C字段	│ M字段	│ A字段	│ CRC字段│
│ 1字节	│ 1字节	│ 2字节	│ 6字节	│ 2字节	│
└────────┴────────┴────────┴────────┴────────┘
总长度: 12字节
```

**字段说明:**
- **L字段**: 数据长度 (不包括L字段本身)
- **C字段**: 控制字段 (功能码等)
- **M字段**: 制造商ID (2字节)
- **A字段**: 设备地址 (6字节)
- **CRC字段**: 第一块的CRC校验

#### 第二块 (Table 28)
```
┌────────┬──────────────────────────┬────────┐
│ CI字段	│        数据字段          │ CRC字段│
│ 1字节	│ 15字节或最后块((L-9)%16-1)│ 2字节	│
└────────┴──────────────────────────┴────────┘
```

**数据字段长度计算:**
- 标准块: 15字节数据
- 最后块: (L-9) modulo 16 - 1字节

### 使用示例

```c
// Frame Format A 构建示例
typedef struct {
    uint8_t length;           // L字段
    uint8_t control;          // C字段  
    uint16_t manufacturer;    // M字段
    uint8_t address[6];       // A字段
    uint16_t crc1;           // 第一块CRC
    
    uint8_t ci_field;        // CI字段
    uint8_t data[15];        // 数据字段 (最大15字节)
    uint16_t crc2;           // 第二块CRC
} __attribute__((packed)) frame_format_a_t;

// 构建Frame Format A
uint32_t build_frame_format_a(uint8_t* buffer, uint8_t ci, 
                             const uint8_t* payload, uint8_t payload_len) {
    if (payload_len > 15) return 0;  // 超出Format A限制
    
    frame_format_a_t* frame = (frame_format_a_t*)buffer;
    
    // 第一块
    frame->length = 9 + 1 + payload_len;  // M+A+CI+数据
    frame->control = 0x44;  // 示例控制字段
    frame->manufacturer = 0x1234;
    memset(frame->address, 0x12, 6);
    frame->crc1 = calculate_crc(&frame->length, 10);
    
    // 第二块
    frame->ci_field = ci;
    memcpy(frame->data, payload, payload_len);
    frame->crc2 = calculate_crc(&frame->ci_field, 1 + payload_len);
    
    return sizeof(frame_format_a_t);
}
```

## Frame Format B 详细分析

### 结构组成

#### 第一块 (Table 30)
```
┌────────┬────────┬────────┬────────┐
│ L字段	│ C字段	│ M字段	│ A字段	│
│ 1字节	│ 1字节	│ 2字节	│ 6字节	│
└────────┴────────┴────────┴────────┘
总长度: 10字节 (无CRC)
```

#### 第二块 (Table 31)
```
┌────────┬──────────────────────────┬────────┐
│ CI字段	│        数据字段          │ CRC字段│
│ 1字节	│ 115字节或最后块(L-12)字节 │ 2字节	│
└────────┴──────────────────────────┴────────┘
```

#### 可选块 (Table 32)
```
┌──────────────────────────┬────────┐
│        数据字段          │ CRC字段│
│      (L-129)字节         │ 2字节	│
└──────────────────────────┴────────┘
```

### 多块传输机制

```c
// Frame Format B 多块传输示例
typedef struct {
    uint8_t length;           // L字段
    uint8_t control;          // C字段
    uint16_t manufacturer;    // M字段
    uint8_t address[6];       // A字段
    // 注意: 第一块无CRC
} __attribute__((packed)) frame_format_b_header_t;

typedef struct {
    uint8_t ci_field;        // CI字段 (仅第二块有)
    uint8_t data[115];       // 数据字段
    uint16_t crc;            // 块CRC
} __attribute__((packed)) frame_format_b_block_t;

// 构建Frame Format B
uint32_t build_frame_format_b(uint8_t* buffer, uint8_t ci,
                             const uint8_t* payload, uint16_t payload_len) {
    uint32_t offset = 0;
    
    // 第一块 (头部)
    frame_format_b_header_t* header = (frame_format_b_header_t*)&buffer[offset];
    header->length = 10 + 1 + payload_len;  // 头部+CI+数据
    header->control = 0x44;
    header->manufacturer = 0x1234;
    memset(header->address, 0x12, 6);
    offset += sizeof(frame_format_b_header_t);
    
    // 第二块 (CI + 数据)
    buffer[offset++] = ci;  // CI字段
    
    uint16_t remaining = payload_len;
    uint16_t data_offset = 0;
    
    // 第二块数据 (最多115字节)
    uint16_t block2_data_len = (remaining > 115) ? 115 : remaining;
    memcpy(&buffer[offset], &payload[data_offset], block2_data_len);
    offset += block2_data_len;
    data_offset += block2_data_len;
    remaining -= block2_data_len;
    
    // 第二块CRC
    uint16_t crc2 = calculate_crc(&buffer[offset - block2_data_len - 1], 
                                  1 + block2_data_len);
    *(uint16_t*)&buffer[offset] = crc2;
    offset += 2;
    
    // 可选块 (如果还有数据)
    while (remaining > 0) {
        uint16_t block_data_len = (remaining > 127) ? 127 : remaining;
        memcpy(&buffer[offset], &payload[data_offset], block_data_len);
        offset += block_data_len;
        data_offset += block_data_len;
        remaining -= block_data_len;
        
        // 块CRC
        uint16_t block_crc = calculate_crc(&buffer[offset - block_data_len], 
                                          block_data_len);
        *(uint16_t*)&buffer[offset] = block_crc;
        offset += 2;
    }
    
    return offset;
}
```

## 处理超过255字节数据的实际方案

### 方案1: 多帧序列传输

```c
// 多帧传输管理器
typedef struct {
    uint8_t sequence_id;      // 序列ID
    uint8_t frame_count;      // 帧数量
    uint8_t current_frame;    // 当前帧
    uint16_t total_length;    // 总数据长度
    uint8_t* reassembly_buffer; // 重组缓冲区
} multi_frame_manager_t;

// 发送大数据 (超过255字节)
int send_large_data(const uint8_t* data, uint16_t data_len) {
    if (data_len <= 255) {
        // 单帧传输
        return build_frame_format_b(buffer, ci, data, data_len);
    }

    // 多帧传输
    uint8_t frame_count = (data_len + 254) / 255;  // 每帧最多255字节
    uint8_t sequence_id = generate_sequence_id();

    for (uint8_t i = 0; i < frame_count; i++) {
        uint16_t offset = i * 255;
        uint16_t frame_data_len = (data_len - offset > 255) ? 255 : (data_len - offset);

        // 构建帧头信息
        multi_frame_header_t header = {
            .sequence_id = sequence_id,
            .frame_index = i,
            .total_frames = frame_count,
            .frame_length = frame_data_len
        };

        // 构建Frame Format B
        uint8_t frame_buffer[300];
        uint32_t frame_len = build_frame_format_b_with_header(
            frame_buffer, 0x8A, &header, &data[offset], frame_data_len);

        // 发送帧
        send_frame(frame_buffer, frame_len);

        // 等待确认 (可选)
        wait_for_ack(sequence_id, i);
    }

    return 0;
}

// 接收端重组
int receive_and_reassemble(const uint8_t* frame_data, uint16_t frame_len) {
    multi_frame_header_t header;
    uint8_t* payload;
    uint16_t payload_len;

    // 解析帧
    parse_frame_format_b_with_header(frame_data, frame_len,
                                    &header, &payload, &payload_len);

    // 获取或创建重组管理器
    multi_frame_manager_t* manager = get_reassembly_manager(header.sequence_id);
    if (!manager) {
        manager = create_reassembly_manager(header.sequence_id,
                                          header.total_frames);
    }

    // 存储帧数据
    uint16_t offset = header.frame_index * 255;
    memcpy(&manager->reassembly_buffer[offset], payload, payload_len);
    manager->current_frame++;

    // 检查是否完成重组
    if (manager->current_frame == manager->frame_count) {
        // 重组完成，处理完整数据
        process_complete_data(manager->reassembly_buffer, manager->total_length);
        destroy_reassembly_manager(manager);
    }

    return 0;
}
```

### 方案2: 长传输层(LTL)分片

```c
// LTL分片传输
typedef struct {
    uint8_t ci_field;         // 0x8A/8B/8C
    uint16_t fragment_id;     // 分片ID
    uint8_t fragment_index;   // 分片索引
    uint8_t total_fragments;  // 总分片数
    uint16_t fragment_length; // 分片长度
    uint8_t data[];          // 分片数据
} ltl_fragment_t;

// 使用LTL处理大数据
int send_large_data_with_ltl(const uint8_t* data, uint16_t data_len) {
    uint8_t max_fragment_size = 240;  // 留出LTL头部空间
    uint8_t fragment_count = (data_len + max_fragment_size - 1) / max_fragment_size;
    uint16_t fragment_id = generate_fragment_id();

    for (uint8_t i = 0; i < fragment_count; i++) {
        uint16_t offset = i * max_fragment_size;
        uint16_t fragment_len = (data_len - offset > max_fragment_size) ?
                               max_fragment_size : (data_len - offset);

        // 构建LTL分片
        ltl_fragment_t fragment = {
            .ci_field = 0x8A,
            .fragment_id = fragment_id,
            .fragment_index = i,
            .total_fragments = fragment_count,
            .fragment_length = fragment_len
        };

        // 复制数据
        memcpy(fragment.data, &data[offset], fragment_len);

        // 发送Frame Format B
        uint8_t frame_buffer[300];
        uint32_t frame_len = build_frame_format_b(frame_buffer, 0x8A,
                                                 (uint8_t*)&fragment,
                                                 sizeof(ltl_fragment_t) + fragment_len);
        send_frame(frame_buffer, frame_len);
    }

    return 0;
}
```

## CI字段与Frame Format的匹配策略

### 自动选择算法

```c
typedef enum {
    FRAME_FORMAT_A,
    FRAME_FORMAT_B
} frame_format_type_t;

frame_format_type_t select_frame_format(uint8_t ci_field, uint16_t payload_len) {
    uint16_t total_data_len = payload_len;
    
    // 根据CI字段调整总长度
    switch (ci_field) {
        case 0x7A:  // STL: +4字节头部
            total_data_len += 4;
            break;
        case 0x8A:  // LTL: +8字节头部
            total_data_len += 8;
            break;
        case 0x8B:  // LTL: +12字节头部
            total_data_len += 12;
            break;
        case 0x8C:  // LTL: +16字节头部
            total_data_len += 16;
            break;
        case 0x8D:  // 扩展链路层: +3字节
            total_data_len += 3;
            break;
        case 0x8E:  // 扩展链路层: +8字节
            total_data_len += 8;
            break;
        case 0x8F:  // 扩展链路层: +10字节
            total_data_len += 10;
            break;
    }
    
    // 选择Frame Format
    if (total_data_len <= 15) {
        return FRAME_FORMAT_A;  // 小数据量，使用Format A
    } else {
        return FRAME_FORMAT_B;  // 大数据量，使用Format B
    }
}
```

### 实际应用建议

#### 1. 标准抄表应用
```c
// CI=0x7A (STL) + 小量数据 → Frame Format A
if (ci_field == 0x7A && payload_len <= 11) {
    use_frame_format_a();
}
```

#### 2. 大数据传输应用
```c
// CI=0x8A/8B/8C (LTL) + 大量数据 → Frame Format B
if (ci_field >= 0x8A && ci_field <= 0x8C) {
    use_frame_format_b();
}
```

#### 3. 扩展链路层应用
```c
// CI=0x8D-0x8F (扩展链路层) → 根据数据量选择
if (ci_field >= 0x8D && ci_field <= 0x8F) {
    frame_format_type_t format = select_frame_format(ci_field, payload_len);
    if (format == FRAME_FORMAT_A) {
        use_frame_format_a();
    } else {
        use_frame_format_b();
    }
}
```

## 总结

### 关键要点
1. **Frame Format A/B** 是物理层的数据组织方式
2. **CI字段** 是传输层的控制信息，位于Frame Format的数据部分
3. **选择策略** 主要基于总数据长度和可靠性要求
4. **LONG报文** 通常需要Frame Format B来承载

### 最佳实践
- 小数据量 (≤15字节): 使用Frame Format A
- 大数据量 (>15字节): 使用Frame Format B  
- 关键数据传输: 优先选择Frame Format B (多CRC校验)
- 实时性要求高: 可选择Frame Format A (处理简单)

---

*文档版本: v1.0*
*基于EN 13757-4标准*
*创建日期: 2024-12-26*
