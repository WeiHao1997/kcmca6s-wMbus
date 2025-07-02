# wM-Bus协议技术规范文档

## 目录
1. [概述](#概述)
2. [协议架构](#协议架构)
3. [物理层规范](#物理层规范)
4. [数据链路层](#数据链路层)
5. [传输模式详解](#传输模式详解)
6. [P2 Companion Standard规范](#p2-companion-standard-规范)
7. [OMS规范](#oms规范)
8. [设备安装过程](#设备安装过程)
9. [数据交换过程](#数据交换过程)
10. [wM-Bus网关应用](#wm-bus网关应用)
11. [实际应用示例](#实际应用示例)

---

## 快速参考索引

### 🔧 **开发者快速查找**
| 需求 | 章节 | 页面位置 |
|------|------|----------|
| **帧格式定义** | [数据链路层](#数据链路层) | §4 |
| **安全模式配置** | [OMS安全配置文件](#oms安全配置文件-基于oms-spec-vol2-91) | §1.3 |
| **DIF/VIF编码** | [数据记录格式](#数据记录格式-基于en-13757-32018完整规范) | §10.1 |
| **OBIS代码映射** | [OBIS代码映射规范](#obis代码映射规范-基于oms-annex-a和en-13757-32018) | §附录H |
| **P2标准实现** | [P2 Companion Standard](#p2-companion-standard-规范) | §6 |
| **传输模式参数** | [传输模式详解](#传输模式详解) | §5 |
| **网关功能配置** | [wM-Bus网关应用](#wm-bus网关应用) | §10 |
| **合规性检查** | [P2合规性检查](#附录e-p2-companion-standard合规性检查清单) | §附录E |

### 📊 **技术参数快查**
| 参数类型 | 典型值 | 参考位置 |
|----------|--------|----------|
| **T模式频率** | 868.95 MHz (M→O), 868.3 MHz (O→M) | §6 |
| **C模式速率** | 100 kbps | §5.2 |
| **最大帧长** | 255字节 (P2) | §6.1 |
| **CRC多项式** | 0x3D65 | §4.3 |
| **前导码长度** | ≥19位 (标准) | §3.2 |
| **网关支持表计数** | 500+ (典型) | §10.1 |
| **双向通信模式** | T2/C2/R2 | §10.2 |

### 🛡️ **安全配置快查**
| 安全模式 | 加密方法 | 应用场景 |
|----------|----------|----------|
| **模式0** | 无加密 | 测试/安装 |
| **模式5** | AES128-CBC | Profile A |
| **模式7** | AES128-CBC+CMAC | Profile B |
| **模式9** | AES128-GCM | P2强制 |
| **模式13** | TLS 1.2 | Profile C |

---

## 概述

### wM-Bus简介
wM-Bus (Wireless Meter-Bus) 是基于EN 13757标准的无线计量设备通信协议，广泛应用于智能水表、气表、电表、热表等计量设备的远程抄表系统。

### 主要特点
- **低功耗**: 支持电池供电设备长期运行
- **多模式**: 支持T/C/S/N/R/F六种传输模式
- **标准化**: 符合欧洲EN 13757和OMS标准
- **互操作性**: 不同厂商设备间可互操作
- **安全性**: 支持多种OMS安全配置文件 (A/B/C/D)

### OMS安全配置文件 (基于OMS-Spec Vol.2 §9.1)

#### 安全配置文件概述
```c
// OMS安全配置文件定义 (Table 43)
typedef enum {
    OMS_SECURITY_PROFILE_NONE = 0,    // 无安全配置
    OMS_SECURITY_PROFILE_A,           // AES128-CBC + 持久密钥
    OMS_SECURITY_PROFILE_B,           // AES128-CBC + 临时密钥 + CMAC
    OMS_SECURITY_PROFILE_C,           // TLS 1.2 + HMAC + CMAC
    OMS_SECURITY_PROFILE_D            // AES128-CCM + 临时密钥
} oms_security_profile_t;

// 安全配置文件详细规范
typedef struct {
    oms_security_profile_t profile;
    uint8_t security_mode;            // 安全模式 (0/5/7/10/13)
    uint8_t mac_mode;                 // MAC模式 (0/5)
    uint16_t key_length_bits;         // 密钥长度
    char* encryption_method;          // 加密方法
    char* authentication_method;      // 认证方法
    bool requires_afl;                // 是否需要AFL层
    bool requires_message_counter;    // 是否需要消息计数器
} oms_security_spec_t;

// OMS安全配置文件规范表
static const oms_security_spec_t oms_security_specs[] = {
    // Security Profile A (不推荐用于新开发)
    {
        .profile = OMS_SECURITY_PROFILE_A,
        .security_mode = 5,           // AES128-CBC
        .mac_mode = 0,                // 无MAC
        .key_length_bits = 128,
        .encryption_method = "AES128-CBC",
        .authentication_method = "None",
        .requires_afl = false,
        .requires_message_counter = false
    },
    // Security Profile B (标准对称加密)
    {
        .profile = OMS_SECURITY_PROFILE_B,
        .security_mode = 7,           // AES128-CBC + 临时密钥
        .mac_mode = 5,                // CMAC (8字节截断)
        .key_length_bits = 128,
        .encryption_method = "AES128-CBC",
        .authentication_method = "CMAC-8",
        .requires_afl = true,
        .requires_message_counter = true
    },
    // Security Profile C (TLS非对称加密)
    {
        .profile = OMS_SECURITY_PROFILE_C,
        .security_mode = 13,          // TLS 1.2
        .mac_mode = 5,                // HMAC + CMAC
        .key_length_bits = 256,       // ECC 256位 (可选384位)
        .encryption_method = "TLS 1.2",
        .authentication_method = "HMAC + CMAC-8",
        .requires_afl = true,
        .requires_message_counter = true
    },
    // Security Profile D (认证加密)
    {
        .profile = OMS_SECURITY_PROFILE_D,
        .security_mode = 10,          // AES128-CCM
        .mac_mode = 1,                // CCM内置认证 (8字节)
        .key_length_bits = 128,
        .encryption_method = "AES128-CCM",
        .authentication_method = "CCM-8",
        .requires_afl = false,        // CCM模式不需要AFL
        .requires_message_counter = true
    }
};
```

### 应用场景
- 智能计量系统 (AMI/AMR)
- 楼宇自动化
- 工业监控
- 环境监测

---

## 协议架构

### OMS层模型 (基于OMS-Spec Vol.2 v5.0.1)
```
┌───────────────────┐
│ Application Layer │ APL (应用层)
│                   │   OMS应用协议 (DIF/VIF数据记录) 
├───────────────────┤
│ Transport Layer   │ TPL (传输层)
│                   │   STL/LTL传输层 (CI=7A/8A/8B/8C)
├───────────────────┤
│ Authentication    │ AFL (认证和分片层)
│  and Fragmentation│   消息认证、分片处理、消息计数器管理
├───────────────────┤
│ Link Layer        │ ELL (扩展链路层)
│                   │   EN 13757-4扩展 (CI=8Ch/8Dh/8Eh/8Fh)
├───────────────────┤
│ Data Link Layer   │ LLC (逻辑链路控制层)
│                   │   帧格式控制 (L-C-地址-CI-数据-CRC)
├───────────────────┤
│ Medium Access     │ 介质访问层 (EN 13757-4 §11.1)
│ Layer             │   前导码和同步模式
├───────────────────┤
│ Physical Layer    │ PHY (物理层)
│                   │   无线射频 (T/C/S/N/R/F模式)
└───────────────────┘
```

### 协议栈结构详解

#### 各层功能说明
- **物理层 (1)**: 射频调制、频段分配、功率控制、前导码同步
  - 支持T/C/S/N/R/F六种传输模式
  - 频段: 433MHz, 868MHz, 169MHz
  - 调制: 2-FSK, 4-GFSK等

- **数据链路层 (2)**: 帧格式定义、地址管理、控制字段
  - EN 13757-4核心: L-C-地址-CI-数据-CRC
  - P2 Companion Standard: PL-L-C-M-A-Checksum-Link_user_data-Checksum
  - OMS扩展格式: 采用P2标准增强互操作性
  - 控制字段: PRM/FCB/FCV/ACD/DFC位控制
  - Frame Format A/B支持不同数据长度

- **网络层 (3)**: 路由控制、中继管理、扩展链路层
  - 扩展链路层: CI=8C/8D/8E/8F
  - 支持多跳通信和网络路由
  - 目标地址和路径管理

- **传输层 (4)**: 数据传输控制、加密管理
  - 短传输层(STL): CI=7A, 4字节头部
  - 长传输层(LTL): CI=8A/8B/8C, 8-16字节头部
  - 访问号、状态字节、配置字管理

- **会话层 (5)**: 会话建立、维护、终止
  - 访问号序列管理
  - 设备状态监控
  - 通信会话控制

- **表示层 (6)**: 数据编码、加密解密
  - AES-128 CTR模式加密 (基于EN 13757-4 §12.2.7)
  - 数据压缩和编码
  - 密钥管理

### AES-128 CTR模式加密 (基于EN 13757-4真实标准)

#### 加密模式定义 (§12.2.6.2)
```c
// 加密子字段 (ENC-subfield) - Session Number的Bit 31-29
typedef enum {
    ENC_NONE        = 0b000,    // 无加密
    ENC_AES128_CTR  = 0b001,    // AES-128 CTR模式 (标准)
    ENC_RESERVED_2  = 0b010,    // 保留
    ENC_RESERVED_3  = 0b011,    // 保留
    ENC_RESERVED_4  = 0b100,    // 保留
    ENC_RESERVED_5  = 0b101,    // 保留
    ENC_RESERVED_6  = 0b110,    // 保留
    ENC_RESERVED_7  = 0b111,    // 保留
} encryption_mode_t;
```

#### AES-128 CTR初始计数块 (§12.2.7)
```c
// 基于EN 13757-4 Figure 11的精确定义
typedef struct {
    uint16_t m_field;     // 发送方制造商ID (2字节)
    uint8_t a_field[6];   // 发送方地址字段 (6字节)
    uint8_t cc_field;     // 通信控制字段 (1字节)
    uint32_t sn_field;    // 会话号字段 (4字节)
    uint16_t fn;          // 帧号 (2字节)
    uint8_t bc;           // 块计数器 (1字节)
} __attribute__((packed)) aes128_ctr_initial_block_t;

// AES-128 CTR加密实现
void encrypt_aes128_ctr(const uint8_t* plaintext, uint8_t* ciphertext,
                       size_t length, const uint8_t* key,
                       const aes128_ctr_initial_block_t* initial_block) {
    // 实现基于FIPS PUB 197和NIST SP800-38A
    // 加密覆盖从Payload CRC字段到帧尾(不包括链路层CRC)

    uint8_t counter_block[16];
    memcpy(counter_block, initial_block, 16);

    for (size_t i = 0; i < length; i += 16) {
        uint8_t keystream[16];
        aes_encrypt(counter_block, key, keystream);

        size_t block_size = (length - i < 16) ? (length - i) : 16;
        for (size_t j = 0; j < block_size; j++) {
            ciphertext[i + j] = plaintext[i + j] ^ keystream[j];
        }

        // 递增块计数器 (BC字段)
        counter_block[15]++;
    }
}
```

- **应用层 (7)**: 业务数据处理、OMS规范实现
  - DIF/VIF数据记录格式
  - 测量数据编码
  - 设备配置和控制

---

## 物理层规范

### 频段分配

| 模式 | 频段 | 地区 | 功率 | 调制方式 |
|------|------|------|------|----------|
| T | 868.95 MHz (M→O), 868.3 MHz (O→M, T2) | 欧洲 | ≤25mW | FSK |
| C | 868.95 MHz (M→O), 869.525 MHz (O→M) | 欧洲 | ≤25mW | FSK/GFSK |
| S | 868.0-868.6 MHz (典型868.3 MHz) | 欧洲 | ≤25mW | FSK |
| N | 169.400-169.475 MHz | 欧洲 | ≤500mW | GFSK |
| R | 868.0-868.6 MHz (典型868.33 MHz) | 欧洲 | ≤25mW | FSK |
| F | 433.050-434.790 MHz (典型433.82 MHz) | 全球 | ≤10mW | FSK |

### EN 13757-4 射频性能要求 (§10)

#### 发射器性能要求
```c
// EN 13757-4 发射器规范
typedef struct {
    float frequency_accuracy_ppm;      // 频率精度 (ppm)
    float frequency_stability_ppm;     // 频率稳定性 (ppm)
    float power_accuracy_db;           // 功率精度 (dB)
    float spurious_emission_dbm;       // 杂散发射 (dBm)
    float adjacent_channel_power_db;   // 邻道功率 (dB)
    float modulation_accuracy_percent; // 调制精度 (%)
} en13757_tx_performance_t;

// 各模式发射器性能要求
static const en13757_tx_performance_t tx_performance_specs[] = {
    // T模式 (868 MHz)
    {
        .frequency_accuracy_ppm = 10.0f,        // ±10 ppm
        .frequency_stability_ppm = 5.0f,        // ±5 ppm
        .power_accuracy_db = 2.0f,              // ±2 dB
        .spurious_emission_dbm = -36.0f,        // ≤ -36 dBm
        .adjacent_channel_power_db = -60.0f,    // ≤ -60 dB
        .modulation_accuracy_percent = 5.0f     // ±5%
    },
    // N模式 (169 MHz)
    {
        .frequency_accuracy_ppm = 100.0f,       // ±100 ppm
        .frequency_stability_ppm = 50.0f,       // ±50 ppm
        .power_accuracy_db = 3.0f,              // ±3 dB
        .spurious_emission_dbm = -30.0f,        // ≤ -30 dBm
        .adjacent_channel_power_db = -55.0f,    // ≤ -55 dB
        .modulation_accuracy_percent = 10.0f    // ±10%
    }
};
```

#### 接收器性能要求
```c
// EN 13757-4 接收器规范
typedef struct {
    float sensitivity_dbm;             // 接收灵敏度 (dBm)
    float selectivity_db;              // 选择性 (dB)
    float blocking_db;                 // 阻塞性能 (dB)
    float spurious_response_db;        // 杂散响应 (dB)
    float intermodulation_db;          // 互调性能 (dB)
    float frequency_tolerance_ppm;     // 频率容差 (ppm)
} en13757_rx_performance_t;

// 接收器性能要求
static const en13757_rx_performance_t rx_performance_specs[] = {
    // T模式接收器
    {
        .sensitivity_dbm = -105.0f,            // ≥ -105 dBm
        .selectivity_db = 60.0f,               // ≥ 60 dB
        .blocking_db = 80.0f,                  // ≥ 80 dB
        .spurious_response_db = 60.0f,         // ≥ 60 dB
        .intermodulation_db = 65.0f,           // ≥ 65 dB
        .frequency_tolerance_ppm = 20.0f       // ±20 ppm
    }
};
```

### 调制参数 (基于EN 13757-4:2013)

#### T模式 (EN 13757-4 §6)
- **频率**:
  - **M→O**: 868.95 MHz ± 25 kHz (T1/T2)
  - **O→M**: 868.3 MHz ± 25 kHz (仅T2)
- **调制**: 2-FSK (二进制频移键控)
- **频偏**: ±50 kHz (标称值)
- **数据速率**:
  - **M→O**: 66.67 kbps ("3 out of 6"编码)
  - **O→M**: 16.384 kbps (Manchester编码)
- **前导码**: ≥19chips (T1/T2), ≥15chips (T2 O→M)
- **Chip速率**: 100 kcps (M→O), 32.768 kcps (O→M)
- **发射功率**: ≤ 25mW (14 dBm)
- **接收灵敏度**: ≥ -105 dBm (典型值)

#### C模式
- **频率**: 868.95 MHz (M→O), 869.525 MHz (O→M)
- **调制**: FSK (M→O), GFSK (O→M)
- **偏移**: ±45 kHz (M→O), ±25 kHz (O→M)
- **速率**: 100 kbps (M→O), 50 kbps (O→M)
- **前导码**: 32 chips + 32 chips同步字
- **总长度**: 64 chips前导码+同步

#### S模式
- **频率**: 868.3 MHz
- **调制**: 2-FSK
- **偏移**: ±50 kHz
- **速率**: 32.768 kbps
- **前导码**: ≥19位 (帧头+同步字)

#### N模式
- **频率**: 169.4-169.475 MHz
- **调制**: 4-GFSK
- **速率**: 2.4/4.8/19.2 kbps
- **前导码**: ≥19位 (帧头+同步字)

#### R模式
- **频率**: 868.0-868.6 MHz (典型868.33 MHz)
- **调制**: 2-FSK
- **偏移**: ±50 kHz
- **Chip码率**: 4.8 kbps
- **数据速率**: 2.4 kbps (DRate:Chip/2)
- **前导码**: ≥39chips
- **码率容差**: ±1.5%

#### F模式
- **频率**: 433.82 MHz
- **调制**: 2-FSK
- **偏移**: ±50 kHz
- **Chip码率**: 2.4 kbps
- **前导码**: 39bits
- **码率容差**: ±100ppm

### EN 13757-4 帧同步和时序要求

#### 前导码要求 (EN 13757-4 §9.3)
根据EN 13757-4标准，所有模式的前导码必须满足以下要求：

```c
// EN 13757-4 前导码规范
typedef struct {
    uint16_t min_preamble_bits;     // 最小前导码长度
    uint16_t sync_word_bits;        // 同步字长度
    uint8_t sync_pattern[4];        // 同步模式
    float bit_timing_tolerance;     // 位时序容差
    uint16_t frame_header_bits;     // 帧头长度
} en13757_preamble_spec_t;

// 各模式前导码规范
static const en13757_preamble_spec_t mode_preamble_specs[] = {
    // T模式
    {
        .min_preamble_bits = 19,
        .sync_word_bits = 16,
        .sync_pattern = {0x54, 0x3D, 0x00, 0x00},  // 01010100 00111101
        .bit_timing_tolerance = 0.01f,              // ±1%
        .frame_header_bits = 8
    },
    // C模式 (特殊长前导码)
    {
        .min_preamble_bits = 512,
        .sync_word_bits = 32,
        .sync_pattern = {0x54, 0x3D, 0x54, 0xCD},  // 特殊模式
        .bit_timing_tolerance = 0.01f,              // ±1%
        .frame_header_bits = 8
    },
    // S模式
    {
        .min_preamble_bits = 19,
        .sync_word_bits = 16,
        .sync_pattern = {0x54, 0x3D, 0x00, 0x00},
        .bit_timing_tolerance = 0.015f,             // ±1.5%
        .frame_header_bits = 8
    }
};
```

#### 时序同步要求 (EN 13757-4 §9.2)
- **位同步**: 接收器必须在前导码期间实现位同步
- **帧同步**: 通过同步字检测实现帧边界识别
- **时钟恢复**: 前导码提供足够的边沿用于时钟恢复
- **同步容差**: 各模式有不同的时序容差要求


## 数据链路层

### 帧格式

### 标准对比: EN 13757-4 vs OMS/P2 Companion Standard

#### EN 13757-4 核心帧格式 (基于真实标准)

**Frame Format A (EN 60870-5-1 FT3兼容)**
```
第一块 (固定10字节 + 2字节CRC):
┌─────┬─────┬────────┬──────────────────────────┬─────┐
│ L   │ C   │ M-field│         A-field          │ CRC │
│1Byte│1Byte│ 2Byte  │ 6Byte(ID+Ver+Type)       │2Byte│
└─────┴─────┴────────┴──────────────────────────┴─────┘

第二块 (CI + 数据 + CRC):
┌─────┬─────────────────────────────────────┬─────┐
│ CI  │           Data-field                │ CRC │
│1Byte│ 15字节或最后块((L-9)%16)-1Byte        │2Byte│
└─────┴─────────────────────────────────────┴─────┘
```

**Frame Format B (更高效编码)**
```
第一块 (固定10字节，无CRC):
┌─────┬─────┬────────┬──────────────────────────┐
│  L  │  C  │ M-field│         A-field          │
│1Byte│1Byte│ 2Byte  │ 6Byte(ID+Ver+Type)       │
└─────┴─────┴────────┴──────────────────────────┘

第二块 (CI + 数据 + CRC):
┌─────┬─────────────────────────────────────┬─────┐
│ CI  │           Data-field                │ CRC │
│1Byte│   115字节或最后块(L-12)Byte           │2Byte│
└─────┴─────────────────────────────────────┴─────┘
```

### P2 Companion Standard FT3帧格式 (基于P2标准§3.1)

#### P2 FT3帧结构 (基于EN 60870-5-1)
```
┌──────────┬────┬────┬────────┬──────────────────────────┬────────┬─────────────┬────────┬──────────┐
│ Preamble │ L  │ C  │M-field │         A-field          │Checksum│ Link Data   │Checksum│Postamble │
│ (变长)   │1字节│1字节│ 2字节  │ 6字节(ID+Ver+Type)       │ 2字节  │ (变长)      │ 2字节  │ 1-2位    │
└──────────┴────┴────┴────────┴──────────────────────────┴────────┴─────────────┴────────┴──────────┘
```

#### P2特定的C字段限制 (P2标准§3.1.2)
```c
// P2允许的C字段值 (与EN 13757-4有差异)
typedef enum {
    P2_SND_NKE = 0x40,    // 链路复位
    P2_REQ_UD2 = 0x5B,    // 请求用户数据Class 2
    P2_RSP_UD  = 0x08,    // 响应用户数据
    P2_SND_UD  = 0x53,    // 发送用户数据
    P2_SND_UD2 = 0x43,    // 发送用户数据Class 2
    P2_SND_NR  = 0x44,    // 发送无回复
    P2_ACK     = 0x00,    // 确认
    P2_SND_IR  = 0x46,    // 发送安装请求
    P2_CNF_IR  = 0x06     // 确认安装请求
} p2_c_field_t;

// P2不允许的C字段值
// REQ_UD1 (0x5A) - 不允许
// ACC_NR  (0x47) - 不允许
// ACC_DMD (0x48) - 不允许

// P2特定规则
// 1. FCB位被忽略，使用访问号检测通信故障
// 2. 最大报文长度255字节
// 3. 发送方地址必须在数据链路层和控制层相同
```

#### P2安装模式流程 (P2标准§3.1.5.1)
```c
// P2安装模式状态机
typedef enum {
    P2_INSTALL_FACTORY_MODE,      // 出厂安装模式 (每小时发送SND_IR)
    P2_INSTALL_BUTTON_ACTIVATED,  // 按钮激活 (30分钟内每分钟发送)
    P2_INSTALL_HOURLY_MODE,       // 小时模式 (每小时发送)
    P2_INSTALL_BOUND,             // 已绑定 (停止安装消息)
    P2_INSTALL_FAC_ENABLED        // 频繁访问周期启用
} p2_install_state_t;

// P2安装流程处理
void p2_handle_installation_mode(p2_install_state_t* state) {
    switch (*state) {
        case P2_INSTALL_FACTORY_MODE:
            // 每小时发送SND_IR直到收到CNF_IR
            send_snd_ir_message();
            break;

        case P2_INSTALL_BUTTON_ACTIVATED:
            // 30分钟内每分钟发送，然后回到小时模式
            send_snd_ir_message();
            // 30分钟后转换到P2_INSTALL_HOURLY_MODE
            break;

        case P2_INSTALL_BOUND:
            // 设置绑定标志，启用每小时FAC
            set_binding_flag(true);
            enable_hourly_fac();
            *state = P2_INSTALL_FAC_ENABLED;
            break;

        case P2_INSTALL_FAC_ENABLED:
            // 用户密钥设置后，回到每24小时一次FAC
            if (user_key_set && security_mode_9_active) {
                disable_hourly_fac();
                enable_daily_fac();
            }
            break;
    }
}
```

### P2安全模式9 (基于P2标准§5)

#### P2强制加密要求
P2 Companion Standard要求所有应用层数据在正常操作期间必须使用安全模式9进行加密和认证。

```c
// P2安全模式9配置字结构 (P2标准Table 19)
typedef struct {
    uint8_t mode : 4;         // 模式 = 9 (1001b)
    uint8_t reserved_1 : 1;   // 保留位 = 0
    uint8_t reserved_2 : 1;   // 保留位 = 0
    uint8_t reserved_3 : 1;   // 保留位 = 0
    uint8_t reserved_4 : 1;   // 保留位 = 0
    uint8_t tag_present : 1;  // 标签存在 = 1
    uint8_t reserved_5 : 7;   // 保留位 = 0
} __attribute__((packed)) p2_mode9_config_t;

// P2固定配置字值: 2901h (部署后)
#define P2_MODE9_CONFIG_WORD    0x2901

// P2安全模式使用规则
typedef enum {
    P2_SECURITY_MODE_0,       // 无加密 (仅特定情况)
    P2_SECURITY_MODE_9        // AES-128 GCM (正常操作)
} p2_security_mode_t;

// P2安全模式0的允许情况
bool p2_is_mode0_allowed(uint8_t message_type, bool user_key_set) {
    switch (message_type) {
        case P2_CNF_IR:           // 安装确认 - 总是模式0
            return true;
        case 0x64:                // 时钟设置 - 仅在用户密钥未设置时
            return !user_key_set;
        case 0xFF:                // 密钥更换 - 总是模式0
            return true;
        default:
            return false;         // 其他消息必须使用模式9
    }
}
```

#### P2调用计数器机制 (P2标准§5.4)
```c
// P2调用计数器规范
typedef struct {
    uint32_t counter;             // 32位无符号整数
    uint32_t last_valid_counter;  // 上次验证的计数器值
    bool counter_initialized;     // 计数器是否已初始化
} p2_invocation_counter_t;

// P2调用计数器验证规则
typedef enum {
    P2_COUNTER_VALID,            // 计数器有效
    P2_COUNTER_TOO_HIGH,         // 计数器过高 (>100)
    P2_COUNTER_REPLAY,           // 重放攻击
    P2_COUNTER_OVERFLOW          // 计数器溢出
} p2_counter_validation_result_t;

// P2调用计数器验证函数
p2_counter_validation_result_t p2_validate_counter(
    p2_invocation_counter_t* ctx, uint32_t received_counter) {

    // 规则1: 接收的计数器最多比上次验证的高100
    if (received_counter > ctx->last_valid_counter + 100) {
        return P2_COUNTER_TOO_HIGH;
    }

    // 规则2: 不能小于等于上次验证的计数器 (防重放)
    if (received_counter <= ctx->last_valid_counter) {
        return P2_COUNTER_REPLAY;
    }

    // 规则3: 检查溢出 (P2设备为"限制性系统")
    if (received_counter == 0xFFFFFFFF) {
        return P2_COUNTER_OVERFLOW;
    }

    return P2_COUNTER_VALID;
}

// P2调用计数器处理
void p2_handle_counter_validation(p2_invocation_counter_t* ctx,
                                 uint32_t received_counter,
                                 p2_counter_validation_result_t result) {
    switch (result) {
        case P2_COUNTER_VALID:
            // 更新上次验证的计数器
            ctx->last_valid_counter = received_counter;
            break;

        case P2_COUNTER_TOO_HIGH:
            // E-meter: 接受消息但触发事件134 (M-Bus安全错误)
            // M-Bus设备: 拒绝消息
            trigger_security_event(134);
            ctx->last_valid_counter = received_counter; // E-meter必须接受
            break;

        case P2_COUNTER_REPLAY:
        case P2_COUNTER_OVERFLOW:
            // 拒绝消息
            reject_message();
            break;
    }
}
```

#### P2 AES-128 GCM实现 (P2标准§5.2-5.3)
```c
// P2 GCM初始化向量结构
typedef struct {
    uint8_t system_title[8];      // 系统标题 (M字段+A字段)
    uint32_t invocation_counter;  // 调用计数器 (大端序)
} __attribute__((packed)) p2_gcm_iv_t;

// P2 GCM加密实现
bool p2_encrypt_gcm(const uint8_t* plaintext, size_t plaintext_len,
                   const uint8_t* key, const p2_gcm_iv_t* iv,
                   uint8_t* ciphertext, uint8_t* auth_tag) {

    // 使用FIPS 197 AES-128和NIST SP800-38D GCM
    // 认证标签长度: 12字节 (96位)

    // 1. 设置GCM参数
    gcm_context_t gcm_ctx;
    gcm_init(&gcm_ctx, key, 16);  // 128位密钥

    // 2. 设置IV (12字节)
    gcm_set_iv(&gcm_ctx, (uint8_t*)iv, sizeof(p2_gcm_iv_t));

    // 3. 加密数据
    gcm_encrypt(&gcm_ctx, plaintext, plaintext_len, ciphertext);

    // 4. 生成认证标签
    gcm_finish(&gcm_ctx, auth_tag, 12);  // P2使用12字节标签

    return true;
}
```

### OMS安全模式13 (TLS) 完整实现 (基于OMS Annex F)

#### TLS 1.2要求和限制 (基于OMS Annex F §F.3.1)
```c
// OMS TLS 1.2实现要求
typedef struct {
    bool client_functionality;        // OED必须实现客户端功能
    bool server_functionality;        // 网关必须实现服务器功能
    uint16_t min_fragment_size;       // 最小分片大小512字节
    uint32_t max_session_lifetime_ms; // 最大会话生命周期31天
    uint64_t max_session_data_bytes;  // 最大会话数据5MB
    bool session_renegotiation;       // 不允许会话重新协商
    bool session_resumption;          // 允许会话恢复
} oms_tls_requirements_t;

// OMS支持的椭圆曲线 (基于OMS Annex F Table F.2)
typedef enum {
    OMS_CURVE_BRAINPOOL_P256R1 = 0x001A,  // 必须支持
    OMS_CURVE_BRAINPOOL_P384R1 = 0x001B,  // 推荐支持
    OMS_CURVE_BRAINPOOL_P512R1 = 0x001C,  // 推荐支持
    OMS_CURVE_SECP256R1        = 0x0017,  // 必须支持 (NIST P-256)
    OMS_CURVE_SECP384R1        = 0x0018   // 推荐支持 (NIST P-384)
} oms_supported_curves_t;

// OMS TLS密码套件要求
static const char* oms_required_cipher_suites[] = {
    "TLS_ECDHE_ECDSA_WITH_AES_128_CBC_SHA256",  // 必须支持
    "TLS_ECDHE_ECDSA_WITH_AES_256_CBC_SHA384",  // 推荐支持
    "TLS_ECDHE_ECDSA_WITH_AES_128_GCM_SHA256",  // 推荐支持
    "TLS_ECDHE_ECDSA_WITH_AES_256_GCM_SHA384"   // 推荐支持
};

// OMS TLS扩展要求
typedef struct {
    bool max_fragment_length;         // 必须支持 (最小512字节)
    bool truncated_hmac;              // 不得支持 (安全要求)
    bool encrypt_then_mac;            // 必须支持 (2025年起)
    bool supported_signatures;        // 必须支持
    bool elliptic_curves;             // 必须支持
} oms_tls_extensions_t;
```

#### TLS通道请求和防洪机制 (基于OMS Annex F §F.3.5.1)
```c
// TLS通道请求结构 (CI=5Fh, ContentType=00h)
typedef struct {
    uint8_t ci_field;                 // 5Fh
    uint8_t addr_fields[8];           // 地址字段 (可选)
    uint8_t acc;                      // 访问计数器
    uint8_t status;                   // 状态字段
    uint8_t cf[2];                    // 配置字段 (MODE 13)
    uint8_t cfe;                      // 配置字段扩展 (TLSCHAN)
    uint8_t content_type;             // 00h (TLS ChannelRequest)
    uint8_t reserved[2];              // 保留字段 (00h)
    uint16_t tls_sdu_len;             // TLS SDU长度 (00h)
    uint8_t cmac[8];                  // AFL CMAC保护 (必需)
} __attribute__((packed)) oms_tls_channel_request_t;

// TLS防洪机制
bool oms_validate_tls_channel_request(const oms_tls_channel_request_t* request) {
    // 1. 验证AFL CMAC认证
    if (!verify_afl_cmac(request, sizeof(*request) - 8, request->cmac)) {
        return false;
    }

    // 2. 验证配置字段
    if (request->cf[1] != 0x0D) {  // 安全模式13
        return false;
    }

    // 3. 验证CFE字段 (TLSCHAN = 0000b)
    if ((request->cfe & 0x0F) != 0x00) {
        return false;
    }

    // 4. 关闭现有TLS通道
    close_existing_tls_channel();

    return true;
}
```

#### TLS消息分片和AFL集成 (基于OMS Annex F §F.3.4)
```c
// TLS记录头结构 (基于RFC 5246)
typedef struct {
    uint8_t content_type;             // 14h/15h/16h/17h
    uint8_t proto_major;              // 03h (TLS 1.2)
    uint8_t proto_minor;              // 03h (TLS 1.2)
    uint16_t tls_sdu_len;             // TLS SDU长度 (大端序)
} __attribute__((packed)) oms_tls_record_header_t;

// TLS握手消息类型 (基于OMS Annex F Table F.12)
typedef enum {
    OMS_TLS_CHANGE_CIPHER_SPEC = 0x01,  // 需要CMAC保护
    OMS_TLS_CLIENT_HELLO       = 0x01,  // 需要CMAC保护
    OMS_TLS_SERVER_HELLO       = 0x02,  // 无需CMAC保护
    OMS_TLS_CERTIFICATE        = 0x0B,  // 无需CMAC保护
    OMS_TLS_SERVER_KEY_EXCHANGE = 0x0C, // 无需CMAC保护
    OMS_TLS_CERTIFICATE_REQUEST = 0x0D, // 无需CMAC保护
    OMS_TLS_SERVER_HELLO_DONE  = 0x0E,  // 无需CMAC保护
    OMS_TLS_CERTIFICATE_VERIFY = 0x0F,  // 无需CMAC保护
    OMS_TLS_CLIENT_KEY_EXCHANGE = 0x10, // 无需CMAC保护
    OMS_TLS_FINISHED           = 0x14   // 无需CMAC保护
} oms_tls_handshake_type_t;

// TLS与AFL的集成处理
bool oms_process_tls_message(const uint8_t* message, size_t length,
                            uint8_t content_type, uint8_t hs_msg_type) {
    // 根据OMS Annex F Table F.12确定是否需要CMAC保护
    bool cmac_required = false;

    if (content_type == 0x14) {  // ChangeCipherSpec
        cmac_required = true;
    } else if (content_type == 0x16 && hs_msg_type == 0x01) {  // ClientHello
        cmac_required = true;
    } else if (content_type == 0x15) {  // Alert (在无效TLS会话时)
        cmac_required = !is_tls_session_valid();
    }

    if (cmac_required) {
        return verify_afl_cmac_protection(message, length);
    }

    return process_tls_record(message, length);
}
```

#### P2 Companion Standard FT3格式
```
┌────┬────┬────┬────┬────┬──────────┬─────────────────┬──────────┐
│ PL │ L  │ C  │ M  │ A  │ Checksum │ Link user data  │ Checksum │
│    │    │    │    │    │          │ (Variable len)  │          │
└────┴────┴────┴────┴────┴──────────┴─────────────────┴──────────┘
```

#### 关键区别
| 特性 | EN 13757-4 | P2 Companion FT3 |
|------|------------|------------------|
| **前导码** | 无明确定义 | PL (Preamble) |
| **长度字段** | L (1字节) | L (1字节) |
| **校验方式** | CRC | EN 60870-5-1 Checksum |
| **校验位置** | 帧尾 | 多个位置 |
| **数据块** | 固定结构 | 可变长度链路用户数据 |
| **标准基础** | EN 13757 | EN 60870-5-1 |

#### 实际应用选择
- **纯EN 13757-4**: 用于标准合规的基础实现
- **P2 FT3格式**: 用于需要EN 60870-5-1兼容的应用
- **OMS扩展格式**: 用于商业产品，确保设备间互操作性
- **混合模式**: 根据应用场景和标准要求灵活选择

### P2数据链路层帧格式详解

#### P2帧在数据链路层的作用
P2 Companion Standard在数据链路层提供了增强的帧同步和错误检测机制，是对EN 13757-4标准的重要补充。

#### P2数据链路层帧结构
```
数据链路层视图:
┌───────────────────────────────────────────────────────────────────┐
│                    P2 Enhanced Data Link Layer                    │
├───────────────────────────────────────────────────────────────────┤
│ 68 │ LL │ LL │ 68 │ C  │ M  │ A  │ V  │ T  │ CI │ Data │ CRC │ 16 │
├────┴────┴────┴────┼────┴────┴────┴────┴────┼────┴──────┴─────┴────┤
│   P2 Frame Sync   │   EN 13757-4 Core      │   P2 Frame End       │
│   (4 bytes)       │   (6 bytes)            │   (3 bytes)          │
└───────────────────┴────────────────────────┴──────────────────────┘
```

#### P2帧同步序列 (4字节)
```
┌────┬────┬────┬────┐
│ 68 │ LL │ LL │ 68 │
└────┴────┴────┴────┘
 │    │    │    │
 │    │    │    └─ 同步确认标识
 │    │    └────── 长度字段重复 (冗余校验)
 │    └─────────── 数据长度字段
 └──────────────── 帧起始标识
```

**功能说明:**
- **双68标识**: 提供强帧同步能力，减少误同步
- **长度冗余**: LL字段重复确保长度信息可靠性
- **错误检测**: 序列完整性检查

#### P2帧结束序列 (3字节)
```
┌──────┬────┐
│ CRC  │ 16 │
└──────┴────┘
 │      │
 │      └─ 帧结束标识
 └──────── 数据完整性校验
```

**功能说明:**
- **CRC校验**: 确保数据传输完整性
- **结束标识**: 明确帧边界，便于解析

#### P2数据链路层状态机
```c
// P2数据链路层接收状态机
typedef enum {
    P2_DLL_IDLE,           // 空闲状态
    P2_DLL_SYNC_1,         // 检测到第一个68
    P2_DLL_LENGTH_1,       // 接收第一个长度字段
    P2_DLL_LENGTH_2,       // 接收第二个长度字段
    P2_DLL_SYNC_2,         // 检测到第二个68
    P2_DLL_RECEIVING,      // 接收数据
    P2_DLL_CRC_CHECK,      // CRC校验
    P2_DLL_END_CHECK,      // 结束标识检查
    P2_DLL_COMPLETE,       // 帧接收完成
    P2_DLL_ERROR           // 错误状态
} p2_dll_state_t;

// P2数据链路层处理
p2_dll_state_t process_p2_dll_byte(uint8_t byte) {
    static p2_dll_state_t state = P2_DLL_IDLE;
    static uint8_t expected_length = 0;
    static uint8_t received_length = 0;
    static uint8_t data_buffer[256];
    static uint16_t data_index = 0;

    switch (state) {
        case P2_DLL_IDLE:
            if (byte == 0x68) {
                state = P2_DLL_SYNC_1;
                data_index = 0;
            }
            break;

        case P2_DLL_SYNC_1:
            expected_length = byte;
            state = P2_DLL_LENGTH_1;
            break;

        case P2_DLL_LENGTH_1:
            if (byte == expected_length) {
                state = P2_DLL_LENGTH_2;
            } else {
                state = P2_DLL_ERROR;  // 长度不匹配
            }
            break;

        case P2_DLL_LENGTH_2:
            if (byte == 0x68) {
                state = P2_DLL_SYNC_2;
                received_length = 0;
            } else {
                state = P2_DLL_ERROR;  // 同步失败
            }
            break;

        case P2_DLL_SYNC_2:
            data_buffer[data_index++] = byte;
            received_length++;
            if (received_length >= expected_length) {
                state = P2_DLL_CRC_CHECK;
            } else {
                state = P2_DLL_RECEIVING;
            }
            break;

        case P2_DLL_RECEIVING:
            data_buffer[data_index++] = byte;
            received_length++;
            if (received_length >= expected_length) {
                state = P2_DLL_CRC_CHECK;
            }
            break;

        case P2_DLL_CRC_CHECK:
            // 接收CRC的第一个字节
            state = P2_DLL_END_CHECK;
            break;

        case P2_DLL_END_CHECK:
            // 接收CRC的第二个字节，然后检查结束标识
            if (byte == 0x16) {
                state = P2_DLL_COMPLETE;
            } else {
                state = P2_DLL_ERROR;
            }
            break;

        case P2_DLL_COMPLETE:
            // 帧处理完成，重置状态
            state = P2_DLL_IDLE;
            break;

        case P2_DLL_ERROR:
            // 错误处理，重置状态
            state = P2_DLL_IDLE;
            break;
    }

    return state;
}
```

#### P2数据链路层错误处理
```c
// P2数据链路层错误类型
typedef enum {
    P2_DLL_ERROR_NONE,
    P2_DLL_ERROR_SYNC_LOST,        // 同步丢失
    P2_DLL_ERROR_LENGTH_MISMATCH,  // 长度不匹配
    P2_DLL_ERROR_CRC_FAILED,       // CRC校验失败
    P2_DLL_ERROR_NO_END_MARKER,    // 缺少结束标识
    P2_DLL_ERROR_BUFFER_OVERFLOW   // 缓冲区溢出
} p2_dll_error_t;

// P2数据链路层错误恢复
void handle_p2_dll_error(p2_dll_error_t error) {
    switch (error) {
        case P2_DLL_ERROR_SYNC_LOST:
            // 重新启动同步检测
            reset_p2_sync_detection();
            increment_sync_error_counter();
            break;

        case P2_DLL_ERROR_LENGTH_MISMATCH:
            // 长度字段不匹配，可能是传输错误
            log_dll_error("P2 length field mismatch");
            request_frame_retransmission();
            break;

        case P2_DLL_ERROR_CRC_FAILED:
            // CRC校验失败，数据损坏
            increment_crc_error_counter();
            request_frame_retransmission();
            break;

        case P2_DLL_ERROR_NO_END_MARKER:
            // 缺少结束标识，可能帧不完整
            wait_for_timeout_or_more_data();
            break;

        case P2_DLL_ERROR_BUFFER_OVERFLOW:
            // 缓冲区溢出，帧长度异常
            reset_receive_buffer();
            log_dll_error("P2 buffer overflow");
            break;
    }
}
```

#### P2数据链路层性能特性

##### 同步性能对比
| 特性 | EN 13757-4 | P2 Companion Standard |
|------|------------|----------------------|
| **同步标识** | 无明确标识 | 双68字符 |
| **同步可靠性** | 依赖物理层 | 数据链路层增强 |
| **误同步概率** | 较高 | 显著降低 |
| **同步时间** | 较长 | 快速同步 |

##### 错误检测能力
| 检测类型 | EN 13757-4 | P2 Companion Standard |
|----------|------------|----------------------|
| **帧边界错误** | 难以检测 | 68/16标识检测 |
| **长度错误** | 单一校验 | 双重长度校验 |
| **数据完整性** | CRC校验 | CRC + 结构校验 |
| **传输错误** | 基本检测 | 多层检测 |

##### 实现复杂度
```c
// EN 13757-4 简单实现
typedef struct {
    uint8_t length;
    uint8_t control;
    uint8_t data[];
} en13757_frame_t;

// P2增强实现
typedef struct {
    uint8_t start1;      // 0x68
    uint8_t length1;     // 长度
    uint8_t length2;     // 长度重复
    uint8_t start2;      // 0x68
    uint8_t control;     // 控制字段
    uint8_t data[];      // 数据
    // uint16_t crc;     // CRC (在数据后)
    // uint8_t end;      // 0x16 (在CRC后)
} p2_frame_t;
```





### Frame Format A 与 Frame Format B 对比(EN 13757-4)

#### Frame Format A - 短数据传输
**适用场景**: 数据长度 ≤ 255字节的传输

**第一块格式 (Table 27)**:
```
┌─────────┬─────────┬─────────┬───────-─┬────────────┐
│ L Field │ C Field	│ M Field │ A Field	│ CRC Field  │
│ 1Byte   │ 1Byte	│ 2Byte	  │ 6Byte	│ 2Byte    	 │
└─────────┴─────────┴─────────┴───────-─┴────────────┘
```

**第二块格式 (Table 28)**:
```
┌───────────┬─────────────────────────────┬────────────┐
│ CI Field	│        Data Field           │ CRC Field  │
│ 1Byte	    │ 15字节或最后块((L-9)%16-1)    │ 2Byte      │
└───────────┴─────────────────────────────┴────────────┘
```

#### Frame Format B - 中等数据传输
**适用场景**: 数据长度 > 128字节但 ≤ 255字节的传输，支持多CRC校验
**重要说明**: L字段仍为1字节，单帧最大255字节。超大数据需要多帧或分片机制。

**第一块格式 (Table 30)**:
```
┌──────────┬──────────┬──────────┬──────────┐
│ L Field  │ C Field  │ M Field	 │ A Field	│
│ 1Byte	   │ 1Byte	  │ 2Byte	 │ 6Byte	│
└──────────┴──────────┴──────────┴──────────┘
```

**第二块格式 (Table 31)**:
```
┌───────────┬──────────────────────────┬──────────┐
│ CI Field	│        Data Field        │ CRC Field│
│ 1Byte	    │ 115字节或最后块(L-12)Byte  │ 2Byte    │
└───────────┴──────────────────────────┴──────────┘
```

### Frame Format与CI字段的关系

#### 关键理解点
1. **Frame Format A/B** 是**物理层/数据链路层**的帧组织方式
2. **CI字段** 是**传输层**的控制信息，位于Frame Format的数据部分
3. **LONG报文** 通常需要使用**Frame Format B**来承载

#### ⚠️ **L字段255字节限制说明**

**重要理解**:
- Frame Format A和B的L字段都只有1字节
- **单个帧的最大数据长度都是255字节**
- Frame Format B的优势在于**多CRC校验**和**更好的错误恢复**，而不是更大的单帧容量

#### 超过255字节数据的处理方案

1. **多帧序列传输**: 应用层分片，多个Frame Format B帧
2. **长传输层(LTL)分片**: 传输层处理分片和重组
3. **扩展链路层**: 提供序列号和重组机制

#### Frame Format选择规则
```c
// 根据数据长度和可靠性要求选择Frame Format
if (total_data_length <= 15) {
    use_frame_format_A();  // 小数据，简单快速
} else if (total_data_length <= 255) {
    if (reliability_required) {
        use_frame_format_B();  // 中等数据，高可靠性
    } else {
        use_frame_format_A();  // 中等数据，简单处理
    }
} else {
    // 超过255字节，需要分片
    use_multi_frame_transmission();  // 多帧传输
}
```

#### CI字段在Frame Format中的位置
```
Frame Format A:
┌─第一块─┐ ┌─────第二块─────┐
│L C M A	│ │CI	│   数据   │CRC	│
└───────┘ └───────────────┘

Frame Format B:
┌─第一块─┐ ┌─────第二块─────┐ ┌─可选块─┐
│L C M A	│ │CI	│   数据   │CRC	│ │数据	│CRC	│
└───────┘ └───────────────┘ └───────┘
```

### 字段说明

#### 标准区分说明
- **EN 13757-4核心帧格式**: L-C-M-A-CI-数据-CRC
- **OMS/P2 Companion Standard扩展**: 68-LL-LL-68-C-地址-CI-数据-CRC-16

#### EN 13757-4 核心字段 (数据链路层)
- **L**: 长度字段 (1字节) - 数据部分长度 (EN 13757-4)
- **C**: 控制字段 (1字节) - PRM/FCB/FCV/ACD/DFC控制位 (EN 13757-4)
- **地址字段** (6字节总计，EN 13757-4):
  - **M**: 制造商字段 (2字节) - 制造商ID
  - **A**: 设备地址字段 (4字节) - 设备唯一地址
  - **VER**: 版本字段 (1字节) - 设备版本
  - **TYPE**: 设备类型字段 (1字节) - 设备类型代码

#### EN 13757-4 传输层字段
- **CI**: 控制信息字段 (1字节) - 传输层控制信息

### OMS支持的CI字段 (基于OMS-Spec Vol.2 Table 1)

#### 标准CI字段定义
```c
// OMS CI字段定义 (基于OMS规范Table 1)
typedef enum {
    // 应用层协议
    CI_APPLICATION_RESET    = 0x50,   // 应用复位或选择
    CI_APPLICATION_SELECT   = 0x51,   // 应用选择

    // M-Bus应用协议
    CI_MBUS_SHORT_HEADER    = 0x7A,   // M-Bus短头部
    CI_MBUS_LONG_HEADER     = 0x8A,   // M-Bus长头部
    CI_MBUS_LONG_HEADER_LSB = 0x8B,   // M-Bus长头部(LSB)

    // 扩展链路层
    CI_ELL_SHORT            = 0x8C,   // 短扩展链路层
    CI_ELL_LONG_ENCRYPTED   = 0x8D,   // 长扩展链路层(加密)
    CI_ELL_LONG_ADDRESS     = 0x8E,   // 长扩展链路层(目标地址)
    CI_ELL_FULL             = 0x8F,   // 完整扩展链路层

    // 网络管理协议
    CI_NETWORK_MGMT_1       = 0xA0,   // 网络管理协议1
    CI_NETWORK_MGMT_2       = 0xA1,   // 网络管理协议2

    // 安全管理协议 (TLS握手)
    CI_SECURITY_TLS_1       = 0xA2,   // TLS握手1
    CI_SECURITY_TLS_2       = 0xA3,   // TLS握手2

    // 时间同步
    CI_TIME_SYNC            = 0x64,   // 时钟同步

    // 图像传输 (固件更新)
    CI_IMAGE_TRANSFER_CMD   = 0xC0,   // 图像传输命令
    CI_IMAGE_TRANSFER_RSP_S = 0xC1,   // 图像传输响应(短)
    CI_IMAGE_TRANSFER_RSP_L = 0xC2,   // 图像传输响应(长)
    CI_IMAGE_TRANSFER_CMD_S = 0xC7,   // 图像传输命令(短)

    // 安全信息传输协议 (SITP)
    CI_SITP_COMMAND_LONG    = 0xC3,   // SITP命令(长)
    CI_SITP_RESPONSE_SHORT  = 0xC4,   // SITP响应(短)
    CI_SITP_RESPONSE_LONG   = 0xC5,   // SITP响应(长)
    CI_SITP_COMMAND_SHORT   = 0xC6,   // SITP命令(短)

    // M-Bus适配层
    CI_MBUS_ADAPTATION      = 0xCF    // M-Bus适配层
} oms_ci_field_t;

// CI字段属性定义
typedef struct {
    uint8_t ci_value;
    char* protocol_name;
    char* direction;          // Up/Down/Both
    char* header_type;        // Short/Long/N/A
    char* description;
    bool wireless_mode_c;     // 是否支持无线Mode C
    bool wired_mbus;          // 是否支持有线M-Bus
    bool oms_lpwan;           // 是否支持OMS LPWAN
} ci_field_spec_t;

// CI字段规范表 (部分)
static const ci_field_spec_t ci_specs[] = {
    {0x7A, "M-Bus", "Up", "Short", "M-Bus应用数据(短头部)", true, true, true},
    {0x8A, "M-Bus", "Up", "Long", "M-Bus应用数据(长头部)", true, true, true},
    {0x8C, "ELL", "Both", "Short", "扩展链路层(基本)", true, false, false},
    {0x8D, "ELL", "Both", "Long", "扩展链路层(加密)", true, false, false},
    {0x8E, "ELL", "Both", "Long", "扩展链路层(目标地址)", true, false, false},
    {0x8F, "ELL", "Both", "Long", "扩展链路层(完整)", true, false, false},
    {0xC3, "SITP", "Down", "Long", "安全信息传输(命令)", true, true, false},
    {0xC4, "SITP", "Up", "Short", "安全信息传输(响应)", true, true, false},
    {0x64, "Time Sync", "Down", "Short", "时钟同步", true, false, false}
};
```
- **AN**: 访问号字段 (1字节) - 访问序列号 (传输层)
- **ST**: 状态字段 (1字节) - 设备状态信息 (传输层)
- **CW**: 配置字字段 (2字节) - 配置和加密信息 (传输层)

#### EN 13757-4 应用层字段
- **数据**: 应用数据 (可变长度) - 实际测量数据和命令
- **CRC**: 循环冗余校验 (2字节) - 数据完整性校验

### EN 13757-4 错误检测和纠正 (§8.4)

#### CRC计算规范
根据EN 13757-4标准，CRC计算使用以下参数：

```c
// EN 13757-4 CRC规范 (基于标准11.5.7节)
// CRC多项式: x^16 + x^13 + x^12 + x^11 + x^10 + x^8 + x^6 + x^5 + x^2 + 1
#define EN13757_CRC_POLYNOMIAL  0x3D65    // CRC-16多项式 (0x13D65)
#define EN13757_CRC_INITIAL     0x0000    // 初始值为0
#define EN13757_CRC_FINAL_XOR   0xFFFF    // 最终CRC需要取反(补码)

// EN 13757-4 CRC计算实现
uint16_t calculate_en13757_crc(const uint8_t* data, size_t length) {
    uint16_t crc = EN13757_CRC_INITIAL;

    for (size_t i = 0; i < length; i++) {
        crc ^= (uint16_t)data[i] << 8;

        for (int bit = 0; bit < 8; bit++) {
            if (crc & 0x8000) {
                crc = (crc << 1) ^ EN13757_CRC_POLYNOMIAL;
            } else {
                crc <<= 1;
            }
        }
    }

    return crc ^ EN13757_CRC_FINAL_XOR;  // 取反(补码)
}

// CRC验证函数
bool verify_en13757_frame_crc(const uint8_t* frame, size_t frame_length) {
    if (frame_length < 3) return false;  // 最小帧长度

    size_t data_length = frame_length - 2;  // 除去CRC字节
    uint16_t received_crc = (frame[frame_length-1] << 8) | frame[frame_length-2];
    uint16_t calculated_crc = calculate_en13757_crc(frame, data_length);

    return received_crc == calculated_crc;
}
```

#### 错误检测能力
- **单比特错误**: 100%检测
- **双比特错误**: 100%检测
- **突发错误**: 长度≤16位的突发错误100%检测
- **随机错误**: 检测概率 > 99.998%

#### OMS/P2 Companion Standard 扩展字段
- **68** (第1个): 起始字符 (1字节) - OMS规范扩展
- **LL** (第2个): 长度字段重复 (1字节) - OMS冗余校验
- **68** (第2个): 起始字符重复 (1字节) - OMS帧同步确认
- **16**: 结束字符 (1字节) - OMS帧结束标志

#### 完整OMS帧格式
```
68-LL-LL-68-C-M-A-VER-TYPE-CI-AN-ST-CW-数据-CRC-16
└─OMS─┘ └─────EN 13757-4核心─────┘ └─OMS─┘
```

#### 层次结构说明
- **EN 13757-4核心**: L-C-地址-CI-数据-CRC (标准协议核心)
- **OMS扩展**: 68-LL重复-68前缀 + 16后缀 (互操作性增强)
- **实际应用**: 通常使用完整OMS格式以确保设备间互操作性

### 控制字段 (C-Field) 详解 (基于EN 13757-4 §11.5.4)

#### C字段位结构 (基于真实标准)
```
MSB                                                    LSB
┌─────┬─────┬─────┬─────┬─────┬─────┬─────────────────┐
│ RES │ PRM │ FCB │ FCV │ ACD │ DFC │  Function Code  │
│  0  │  1  │  x  │  x  │  x  │  x  │    (4 bits)     │
└─────┴─────┴─────┴─────┴─────┴─────┴─────────────────┘
```

#### 位字段定义 (EN 60870-5-2标准)
- **RES**: 保留位，必须为'0'
- **PRM**: 主站标识位
  - '1' = 来自主站(发起站)的消息
  - '0' = 来自从站(响应站)的消息
- **FCB**: 帧计数位 (Frame Count Bit)
- **FCV**: 帧计数有效位 (Frame Count Valid)
- **ACD**: 访问需求位 (Access Demand)
- **DFC**: 数据流控制位 (Data Flow Control)

#### 主站功能码 (发送给表计)
```c
// EN 13757-4 Table 24: 主站功能码
typedef enum {
    SND_NKE = 0x0,    // 链路复位，清除FCB，结束频繁访问周期
    SND_UD  = 0x3,    // 发送用户数据 (需要FCB有效)
    REQ_UD2 = 0xA,    // 请求用户数据 Class 2
    REQ_UD1 = 0xB,    // 请求用户数据 Class 1
} primary_function_code_t;

// 常用C字段值 (主站到从站)
#define C_SND_NKE_PRIMARY    0x40    // 40h: SND-NKE
#define C_SND_UD_PRIMARY     0x53    // 53h: SND-UD with FCB=1, FCV=1
#define C_REQ_UD2_PRIMARY    0x5A    // 5Ah: REQ-UD2 with FCB=1, FCV=1
#define C_REQ_UD1_PRIMARY    0x5B    // 5Bh: REQ-UD1 with FCB=1, FCV=1
```

#### 从站功能码 (表计响应)
```c
// EN 13757-4 Table 25: 从站功能码
typedef enum {
    ACK     = 0x0,    // 确认
    NACK    = 0x1,    // 否定确认
    RSP_UD  = 0x8,    // 响应用户数据
    SND_NR  = 0x4,    // 发送/无回复 (表计主动发送)
    SND_IR  = 0x6,    // 发送/安装请求
    ACC_NR  = 0x7,    // 访问/无回复
    ACC_DMD = 0x8,    // 访问/需求
} secondary_function_code_t;

// 常用C字段值 (从站到主站)
#define C_ACK_SECONDARY      0x00    // 00h: ACK
#define C_NACK_SECONDARY     0x01    // 01h: NACK
#define C_RSP_UD_SECONDARY   0x08    // 08h: RSP-UD
#define C_SND_NR_SECONDARY   0x44    // 44h: SND-NR (表计主动发送)
#define C_SND_IR_SECONDARY   0x46    // 46h: SND-IR (安装消息)
```

#### C字段结构 (8位) - 基于EN 13757标准
```
MSBit                                    LSBit
  7	  6	    5	  4	    3	  2	    1	  0
┌─────┬─────┬─────┬─────┬─────┬─────┬─────┐
│ RES │ PRM	│ FCB │ FCV	│     FUNCTION    │
│     │     │ ACD │ DFC	│       CODE      │
└─────┴─────┴─────┴─────┴─────┴─────┴─────┘
       主机→从机   从机→主机
```

#### 位字段详细说明 (按照EN 13757标准)

**Bit 7 - RES (Reserved)**
- **固定为 '0'**: 保留位，必须为0

**Bit 6 - PRM (Primary Message)**
- **1**: 主机发送的帧 (Primary to Secondary)
- **0**: 从机发送的帧 (Secondary to Primary)

**Bit 5 - FCB/ACD (Frame Count Bit / Access Demand)**
- **当PRM=1 (主机→从机)**: FCB - 帧计数位
  - 用于检测重复帧和确保帧序列
  - 每发送新帧时切换 (0→1 或 1→0)
- **当PRM=0 (从机→主机)**: ACD - 访问需求
  - **1**: 从机请求主机关注 (有数据要发送)
  - **0**: 从机无特殊请求

**Bit 4 - FCV/DFC (Frame Count Valid / Data Flow Control)**
- **当PRM=1 (主机→从机)**: FCV - 帧计数有效
  - **1**: FCB位有效，需要检查帧序列
  - **0**: FCB位无效，忽略帧计数
- **当PRM=0 (从机→主机)**: DFC - 数据流控制
  - **1**: 从机缓冲区满，请求主机暂停发送
  - **0**: 从机可以接收更多数据

**Bit 3-0 - 功能码 (Function Code)**
- 4位功能码，定义帧的具体功能

#### 功能码详细定义 (基于EN 13757标准)
| 码 | 名称 | 方向 | PRM | RES | 说明 | 应用场景 |
|----|------|------|-----|-----|------|----------|
| 0000 | SND-NKE | M→S | 1 | 0 | 发送链路复位 | 初始化通信链路 |
| 0001 | SND-NR | M→S | 1 | 0 | 发送无响应 | 单向数据传输 |
| 0010 | SND-UD | M→S | 1 | 0 | 发送用户数据 | 配置参数，控制命令 |
| 0011 | REQ-UD1 | M→S | 1 | 0 | 请求用户数据1 | 请求当前测量值 |
| 0100 | REQ-UD2 | M→S | 1 | 0 | 请求用户数据2 | 请求历史数据 |
| 0101 | RSP-UD | S→M | 0 | 0 | 响应用户数据 | 响应主机请求 |
| 0110 | CNF-IR | S→M | 0 | 0 | 确认安装请求 | 设备安装确认 |
| 0111 | CNF-NR | S→M | 0 | 0 | 确认无响应 | 确认接收到数据 |
| 1000 | ACC-NR | S→M | 0 | 0 | 访问无响应 | 从机主动发送 |
| 1001 | ACC-DMD | S→M | 0 | 0 | 访问需求 | 请求主机关注 |
| 1010 | SND-IR | S→M | 0 | 0 | 发送安装请求 | 设备安装请求 |
| 1011 | ACC-IR | S→M | 0 | 0 | 访问安装请求 | 安装过程中的访问 |

#### C字段编码规则 (EN 13757)
根据标准，FCB、FCV和ACD、DFC位的编码应按照EN 60870-5-2规则使用。

#### C字段使用示例 (符合EN 13757标准)
```c
// 构建C字段 (RES位固定为0)
uint8_t build_c_field(uint8_t prm, uint8_t fcb_acd,
                      uint8_t fcv_dfc, uint8_t function_code) {
    return (0 << 7) |                    // RES = 0 (固定)
           (prm << 6) |                  // PRM位
           (fcb_acd << 5) |              // FCB/ACD位
           (fcv_dfc << 4) |              // FCV/DFC位
           (function_code & 0x0F);       // 功能码(4位)
}

// 主机请求数据 (PRM=1, FCB=0, FCV=1, REQ-UD1=3)
uint8_t c_req_ud1 = build_c_field(1, 0, 1, 0x03);  // 0x53

// 从机响应数据 (PRM=0, ACD=0, DFC=0, RSP-UD=5)
uint8_t c_rsp_ud = build_c_field(0, 0, 0, 0x05);   // 0x05

// T1模式从机发送 (PRM=0, ACD=0, DFC=0, ACC-NR=8)
uint8_t c_acc_nr = build_c_field(0, 0, 0, 0x08);   // 0x08

// 解析C字段
typedef struct {
    uint8_t res;           // 保留位 (应为0)
    uint8_t prm;           // 主/从标识
    uint8_t fcb_acd;       // FCB或ACD
    uint8_t fcv_dfc;       // FCV或DFC
    uint8_t function_code; // 功能码
} c_field_t;

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

### 地址字段

#### 地址结构 (8字节)
```
┌─────────────┬─────────────┬──────────┬──────────┐
│ MANF ID(2)  │ DEV ID(4)   │VER(1)    │TYPE(1)   │
└─────────────┴─────────────┴──────────┴──────────┘
```

#### 制造商ID编码
制造商ID使用3个字符编码到16位：
```
ID = (C1-64)*32² + (C2-64)*32 + (C3-64)
```
例如: "ABC" → ID = 0x4142

#### 设备类型
| 类型 | 设备 | 类型 | 设备 |
|------|------|------|------|
| 0x00 | 其他 | 0x07 | 水表 |
| 0x01 | 油表 | 0x08 | 热成本分配器 |
| 0x02 | 电表 | 0x09 | 压缩空气 |
| 0x03 | 气表 | 0x0A | 冷却表 |
| 0x04 | 热表 | 0x0B | 冷却表 |
| 0x05 | 蒸汽表 | 0x0C | 热表 |
| 0x06 | 温水表 | 0x0D | 热/冷表 |

### CI字段 (Control Information) 详解

CI字段位于地址字段之后，定义了后续数据的格式和处理方式。

#### CI字段分类

**基本CI字段 (0x00-0x7F)**
| CI值 | 名称 | 说明 | 数据格式 |
|------|------|------|----------|
| 0x51 | 应用层数据 | 标准应用层数据 | 直接数据记录 |
| 0x52 | 应用层数据 | 带时间戳的数据 | 时间戳+数据记录 |
| 0x53 | 应用层数据 | 带状态的数据 | 状态字+数据记录 |
| 0x5A | 应用层数据 | 短头部格式 | 简化头部+数据 |
| 0x5B | 应用层数据 | 长头部格式 | 完整头部+数据 |
| 0x6C | 应用层数据 | 12字节头部 | 扩展头部+数据 |
| 0x6D | 应用层数据 | 16字节头部 | 最大头部+数据 |
| 0x70 | 应用层错误 | 错误报告 | 错误码+描述 |
| 0x71 | 应用层错误 | 警告信息 | 警告码+描述 |

**传输层CI字段 (0x80-0xFF)**
| CI值 | 名称 | 说明 | 传输层格式 | 适用Frame Format |
|------|------|------|------------|------------------|
| 0x7A | 短传输层 | 4字节STL头部 | AN+ST+CW+数据 | Format A |
| 0x8A | 长传输层 | 8字节LTL头部 | 扩展传输层头部 | Format A/B |
| 0x8B | 长传输层 | 12字节LTL头部 | 完整传输层头部 | Format B |
| 0x8C | 长传输层 | 16字节LTL头部 | 最大传输层头部 | Format B |

#### CI字段与Frame Format的匹配关系

##### 1. 短传输层 (CI=0x7A) + Frame Format A
**适用场景**: 标准抄表数据，数据量小
```
Frame Format A第二块:
┌────┬─────────────────┬─────┐
│ 7A	│ AN ST CW [数据] │ CRC	│
│1字节	│    ≤14字节      │2字节	│
└────┴─────────────────┴─────┘
```

##### 2. 长传输层 (CI=0x8A/8B/8C) + Frame Format B
**适用场景**: 大量数据传输，需要多块传输
```
Frame Format B第二块:
┌────┬─────────────────────┬─────┐
│ 8A	│ LTL头部 + [数据]    │ CRC	│
│1字节	│     ≤114字节        │2字节	│
└────┴─────────────────────┴─────┘

Frame Format B可选块:
┌─────────────────────┬─────┐
│      [数据]         │ CRC	│
│     ≤127字节        │2字节	│
└─────────────────────┴─────┘
```

##### 3. 扩展链路层 (CI=0x8C-0x8F) + Frame Format选择
```c
// 根据扩展链路层类型选择Frame Format
switch (ci_field) {
    case 0x8D:  // 3字节扩展链路层
        if (total_payload < 250) {
            use_frame_format_A();
        } else {
            use_frame_format_B();
        }
        break;

    case 0x8E:  // 8字节扩展链路层
    case 0x8F:  // 10字节扩展链路层
        // 通常使用Format B以支持更大数据量
        use_frame_format_B();
        break;
}
```

#### 短传输层 (STL) - CI=0x7A

STL是最常用的传输层格式，头部结构如下：

```
字节偏移: 0    1    2    3    4...
         ┌────┬────┬────┬────┬─────────
         │ AN │ ST │ CW │ CW │ 应用数据
         └────┴────┴────┴────┴─────────
```

**字段说明:**
- **AN (Access Number)**: 访问号，用于检测重复和排序
- **ST (Status)**: 状态字节
- **CW (Configuration Word)**: 配置字 (16位，小端序)

#### 状态字节 (ST) 详解

```
Bit: 7	6	5	4	3	2	1	0
     │	│	│	│	│	│	│	│
     │	│	│	│	│	│	│	+-- 应用错误
     │	│	│	│	│	│	+------- 电源低电压
     │	│	│	│	│	+------------ 永久错误
     │	│	│	│	+----------------- 临时错误
     │	│	│	+---------------------- 特定应用错误
     │	│	+--------------------------- 加密模式指示
     │	+-------------------------------- 双向通信能力
     +-------------------------------------- 可访问性
```

**状态位含义:**
- **Bit 0**: 应用错误 (0=正常, 1=错误)
- **Bit 1**: 电源低电压 (0=正常, 1=低电压)
- **Bit 2**: 永久错误 (0=正常, 1=永久故障)
- **Bit 3**: 临时错误 (0=正常, 1=临时故障)
- **Bit 4**: 特定应用错误 (厂商定义)
- **Bit 5**: 加密模式指示 (0=无加密, 1=加密)
- **Bit 6**: 双向通信能力 (0=单向, 1=双向)
- **Bit 7**: 可访问性 (0=不可访问, 1=可访问)

#### 配置字 (CW) 详解

配置字是16位字段，定义加密和通信参数：

```
Bit: 15 14 13 12 11 10  9  8  7  6  5  4  3  2  1  0
     │  │  │  │  │  │  │  │  │  │  │  │  │  │  │  │
     │  │  │  │  │  │  │  │  │  │  │  │  +--+--+--+-- 加密模式(4位)
     │  │  │  │  │  │  │  │  │  │  +--+-------------- 跳频模式(2位)
     │  │  │  │  │  │  │  │  +--+------------------- 同步模式(2位)
     │  │  │  │  │  │  +--+------------------------- 可访问性(2位)
     │  │  │  │  +--+------------------------------- 双向能力(2位)
     │  │  +--+-------------------------------------- 保留(2位)
     +--+---------------------------------------------- 厂商特定(2位)
```

**加密模式 (Bit 3-0):**
- **0000**: 无加密
- **0001**: DES加密 (已弃用)
- **0101**: AES-128 (模式5)
- **0111**: AES-256 (模式7)
- **其他**: 保留或厂商特定

#### CI字段使用示例

```c
// 解析CI字段
void parse_ci_field(uint8_t ci, const uint8_t* data, uint32_t data_len) {
    if (ci == 0x7A) {
        // 短传输层
        if (data_len >= 4) {
            uint8_t access_number = data[0];
            uint8_t status = data[1];
            uint16_t config_word = *(uint16_t*)&data[2];

            printf("STL - AN:%d, ST:0x%02X, CW:0x%04X\n",
                   access_number, status, config_word);

            // 检查加密模式
            uint8_t encryption_mode = config_word & 0x0F;
            if (encryption_mode == 5) {
                printf("使用AES-128加密\n");
            }

            // 解析应用数据
            parse_application_data(&data[4], data_len - 4);
        }
    } else if (ci >= 0x51 && ci <= 0x6D) {
        // 直接应用层数据
        parse_application_data(data, data_len);
    }
}

// 构建STL头部
uint32_t build_stl_header(uint8_t* buffer, uint8_t access_number,
                         uint8_t status, uint16_t config_word) {
    buffer[0] = access_number;
    buffer[1] = status;
    *(uint16_t*)&buffer[2] = config_word;  // 小端序
    return 4;
}
```

#### 长传输层 (LTL) 简介

LTL用于需要更多控制信息的场景，包含：
- 扩展的访问号 (可能是32位)
- 详细的状态信息
- 扩展的配置参数
- 时间戳信息
- 安全参数

### 认证和分片层 (AFL) - OMS规范

#### AFL概述 (基于OMS-Spec Vol.2 §6)
认证和分片层(Authentication and Fragmentation Layer)是OMS规范中的重要层次，位于传输层和扩展链路层之间，提供消息认证、分片处理和消息计数器管理功能。

#### AFL结构
```c
// AFL字段结构 (基于OMS规范6.2)
typedef struct {
    uint8_t afl_li;           // AFL长度指示符 (可选)
    uint8_t afl_ki;           // AFL密钥信息 (可选)
    uint32_t afl_mcr;         // AFL消息计数器 (可选)
    uint8_t afl_mac[8];       // AFL消息认证码 (可选)
    uint16_t afl_ml;          // AFL消息长度 (可选，分片时)
} afl_header_t;

// AFL应用条件 (OMS规范6.3)
typedef enum {
    AFL_CONDITION_FRAGMENTED,     // 分片消息
    AFL_CONDITION_SECURITY_B,     // 安全配置文件B
    AFL_CONDITION_KI_FLAG,        // KI标志设置
    AFL_CONDITION_SECURITY_C      // 安全配置文件C (选择性)
} afl_condition_t;

// AFL处理函数
bool afl_is_required(uint8_t c_field, oms_security_profile_t profile,
                    bool ki_flag_set, bool is_fragmented) {
    // 分片消息必须使用AFL
    if (is_fragmented) return true;

    // 安全配置文件B的应用数据消息
    if (profile == OMS_SECURITY_PROFILE_B &&
        (c_field == 0x44 || c_field == 0x53 || c_field == 0x46 || c_field == 0x08)) {
        return true;
    }

    // KI标志设置的RSP-UD消息
    if (ki_flag_set && c_field == 0x08) return true;

    // 安全配置文件C的选择性消息
    if (profile == OMS_SECURITY_PROFILE_C) {
        // 根据具体消息类型决定
        return false; // 详见Annex F
    }

    return false;
}
```

#### 消息计数器机制 (基于OMS规范9.3.2)
```c
// 消息计数器定义
typedef struct {
    uint32_t cm;              // OMS设备计数器
    uint32_t cgw;             // 网关计数器
    uint32_t cm0;             // 初始消息计数器 (Security Profile C)
} message_counter_t;

// 消息计数器处理规则
void handle_message_counter(message_counter_t* counters,
                           bool is_transmit, bool is_authenticated) {
    if (is_transmit) {
        // 发送前递增计数器
        if (counters->cm < 0xFFFFFFFF) {
            counters->cm++;
        } else {
            // 计数器溢出，停止传输
            // 需要更换主密钥
        }
    } else if (is_authenticated) {
        // 接收到认证消息，更新计数器
        // 详细逻辑见OMS规范Figure 26
    }
}
```

### 扩展链路层 (Extended Link Layer) 详解

扩展链路层为无线通信提供额外的控制字段，特别适用于需要高级功能的应用场景。

#### 扩展链路层概述

扩展链路层在标准数据链路层之后提供额外的控制字段，用于：
- **通信控制**: 同步、流控制
- **安全功能**: 加密、认证
- **网络功能**: 目标地址、路由
- **服务质量**: 优先级、可靠性

#### CI字段用于扩展链路层

根据EN 13757标准，特定的CI值用于标识扩展链路层：

#### 扩展链路层CI字段 (基于EN 13757-4 §12.2和Annex G)

| CI值 | 长度 | 结构 | 服务功能 | 标准定义 |
|------|------|------|----------|----------|
| **8Ch** | 3字节 | CC, ACC | 通信控制，同步 | 无加密，无目标地址 |
| **8Dh** | 9字节 | CC, ACC, SN, PayloadCRC | 通信控制，同步，加密 | 有加密，无目标地址 |
| **8Eh** | 10字节 | CC, ACC, M2, A2 | 通信控制，同步，目标地址 | 无加密，有目标地址 |
| **8Fh** | 16字节 | CC, ACC, M2, A2, SN, PayloadCRC | 完整功能 | 有加密，有目标地址 |

#### 扩展链路层结构详解 (基于Annex G)

**CI = 8Ch (基本扩展，无加密)**
```c
typedef struct {
    uint8_t ci;           // 8Ch
    uint8_t cc;           // 通信控制字段
    uint8_t acc;          // 访问号字段
} extended_link_layer_8c_t;
```

**CI = 8Dh (带加密)**
```c
typedef struct {
    uint8_t ci;           // 8Dh
    uint8_t cc;           // 通信控制字段
    uint8_t acc;          // 访问号字段
    uint32_t sn;          // 会话号字段 (4字节)
    uint16_t payload_crc; // 载荷CRC (2字节)
} extended_link_layer_8d_t;
```

**CI = 8Eh (带目标地址)**
```c
typedef struct {
    uint8_t ci;           // 8Eh
    uint8_t cc;           // 通信控制字段
    uint8_t acc;          // 访问号字段
    uint16_t m2;          // 目标制造商ID (2字节)
    uint8_t a2[6];        // 目标地址字段 (6字节)
} extended_link_layer_8e_t;
```

**CI = 8Fh (完整功能)**
```c
typedef struct {
    uint8_t ci;           // 8Fh
    uint8_t cc;           // 通信控制字段
    uint8_t acc;          // 访问号字段
    uint16_t m2;          // 目标制造商ID (2字节)
    uint8_t a2[6];        // 目标地址字段 (6字节)
    uint32_t sn;          // 会话号字段 (4字节)
    uint16_t payload_crc; // 载荷CRC (2字节)
} extended_link_layer_8f_t;
```

#### 扩展链路层字段详解

##### 1. CC字段 (Communication Control) - 通信控制
```
Bit: 7	6	5	4	3	2	1	0
     │	│	│	│	│	│	│	│
     │	│	│	│	│	│	│	+-- 同步模式
     │	│	│	│	│	│	+------- 重传请求
     │	│	│	│	│	+------------ 优先级
     │	│	│	│	+----------------- 确认请求
     │	│	│	+---------------------- 流控制
     │	│	+--------------------------- 路由控制
     │	+-------------------------------- 安全级别
     +-------------------------------------- 扩展标志
```

**CC字段功能:**
- **Bit 0**: 同步模式 (0=异步, 1=同步)
- **Bit 1**: 重传请求 (0=无需重传, 1=请求重传)
- **Bit 2**: 优先级 (0=普通, 1=高优先级)
- **Bit 3**: 确认请求 (0=无需确认, 1=需要确认)
- **Bit 4**: 流控制 (0=正常, 1=暂停)
- **Bit 5**: 路由控制 (0=直接, 1=路由)
- **Bit 6**: 安全级别 (0=标准, 1=高安全)
- **Bit 7**: 扩展标志 (0=标准, 1=扩展)

##### 2. ACC字段 (Access Control) - 访问控制
```
Bit: 7	6	5	4	3	2	1	0
     │	│	│	│	│	│	│	│
     │	│	│	│	│	│	│	+-- 访问权限
     │	│	│	│	│	│	+------- 认证状态
     │	│	│	│	│	+------------ 加密状态
     │	│	│	│	+----------------- 会话状态
     │	│	│	+---------------------- 设备状态
     │	│	+--------------------------- 网络状态
     │	+-------------------------------- 服务状态
     +-------------------------------------- 保留位
```

**ACC字段功能:**
- **Bit 0**: 访问权限 (0=只读, 1=读写)
- **Bit 1**: 认证状态 (0=未认证, 1=已认证)
- **Bit 2**: 加密状态 (0=明文, 1=加密)
- **Bit 3**: 会话状态 (0=无会话, 1=会话中)
- **Bit 4**: 设备状态 (0=正常, 1=维护)
- **Bit 5**: 网络状态 (0=本地, 1=网络)
- **Bit 6**: 服务状态 (0=基本, 1=扩展)
- **Bit 7**: 保留位

#### 扩展链路层使用示例

##### CI=8Ch - 可选择服务
```c
typedef struct {
    uint8_t ci_field;       // 0x8C
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
    uint8_t variable_fields[17]; // 可变字段 (最多17字节)
} __attribute__((packed)) extended_link_8c_t;

// 构建8Ch扩展链路层
uint32_t build_extended_link_8c(uint8_t* buffer, uint8_t cc, uint8_t acc,
                               const uint8_t* var_fields, uint8_t var_len) {
    extended_link_8c_t* ext = (extended_link_8c_t*)buffer;
    ext->ci_field = 0x8C;
    ext->cc_field = cc;
    ext->acc_field = acc;

    if (var_fields && var_len <= 17) {
        memcpy(ext->variable_fields, var_fields, var_len);
        return 3 + var_len;
    }
    return 3;
}
```

##### CI=8Dh - 基本通信控制
```c
typedef struct {
    uint8_t ci_field;       // 0x8D
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
} __attribute__((packed)) extended_link_8d_t;

// 解析8Dh扩展链路层
void parse_extended_link_8d(const uint8_t* data) {
    const extended_link_8d_t* ext = (const extended_link_8d_t*)data;

    printf("扩展链路层 8Dh:\n");
    printf("  通信控制: 0x%02X\n", ext->cc_field);
    printf("  访问控制: 0x%02X\n", ext->acc_field);

    // 解析CC字段
    printf("  同步模式: %s\n", (ext->cc_field & 0x01) ? "同步" : "异步");
    printf("  优先级: %s\n", (ext->cc_field & 0x04) ? "高" : "普通");
    printf("  确认请求: %s\n", (ext->cc_field & 0x08) ? "需要" : "不需要");

    // 解析ACC字段
    printf("  访问权限: %s\n", (ext->acc_field & 0x01) ? "读写" : "只读");
    printf("  认证状态: %s\n", (ext->acc_field & 0x02) ? "已认证" : "未认证");
    printf("  加密状态: %s\n", (ext->acc_field & 0x04) ? "加密" : "明文");
}
```

##### CI=8Eh - 带加密的扩展链路层
```c
typedef struct {
    uint8_t ci_field;       // 0x8E
    uint8_t cc_field;       // 通信控制
    uint8_t acc_field;      // 访问控制
    uint32_t sequence_number; // 序列号 (4字节)
    uint16_t payload_crc;   // 载荷CRC (2字节)
} __attribute__((packed)) extended_link_8e_t;

// 构建8Eh扩展链路层
void build_extended_link_8e(extended_link_8e_t* ext, uint8_t cc, uint8_t acc,
                           uint32_t seq_num, uint16_t crc) {
    ext->ci_field = 0x8E;
    ext->cc_field = cc;
    ext->acc_field = acc;
    ext->sequence_number = seq_num;
    ext->payload_crc = crc;
}
```

#### 扩展链路层应用场景

##### 1. 高安全应用
```c
// 构建高安全扩展链路层
uint8_t cc_secure = 0x80 | 0x08;  // 高安全 + 确认请求
uint8_t acc_secure = 0x06;        // 已认证 + 加密
build_extended_link_8e(&ext_layer, cc_secure, acc_secure, seq_num, crc);
```

##### 2. 网络路由应用
```c
// 构建路由扩展链路层
uint8_t cc_route = 0x20 | 0x04;   // 路由控制 + 高优先级
uint8_t acc_route = 0x20;         // 网络状态
```

##### 3. 同步通信应用
```c
// 构建同步扩展链路层
uint8_t cc_sync = 0x01 | 0x08;    // 同步模式 + 确认请求
uint8_t acc_sync = 0x08;          // 会话状态
```

---

## 传输模式详解

### T模式 (Frequent Transmit)

#### 技术规格
- **频率**: 868.95 MHz
- **Chip码率**: 100 kbps
- **数据速率**: 66.67 kbps (DRate:Chip×2/3)
- **码率容差**: ±1% (高精度要求)
- **接收灵敏度**: -80dBm (低) / -90dBm (中) / -105dBm (高)
- **数据位置大容差**: ±3μS
- **前导码长度**: 48chips
- **定号码长度**: 2×8 chips

#### T1模式 - 单向传输
- **方向**: 从机 → 主机
- **特点**: 从机定期发送，主机仅接收
- **应用**: 定期抄表，低功耗场景
- **发送间隔**: 可配置 (通常几分钟到几小时)
- **码率**: T: 100 kbps (DRate:Chip×2/3)

##### T1模式法规参数
- **占空比**: ≤ 0.1% (在任意时间段)
- **最大突发时间**: 3-8ms (短数据突发)
- **发射功率**: ≤ 25mW (14dBm)
- **频段**: 868.95 MHz ± 25 kHz
- **带宽**: 50 kHz
- **数据编码**: 3 to 6 and short header

##### T1模式实际应用参数
```c
// T1模式典型配置
typedef struct {
    uint32_t transmission_interval_ms;  // 发送间隔
    uint32_t max_burst_time_ms;        // 最大突发时间
    float max_duty_cycle_1h;           // 1小时内最大占空比
    uint8_t max_power_dbm;             // 最大发射功率
} t1_mode_config_t;

// 配置示例 (基于EN 13757-4标准)
t1_mode_config_t t1_config = {
    .transmission_interval_ms = 3600000,  // 1小时间隔
    .max_burst_time_ms = 8,              // 8ms突发时间
    .max_duty_cycle_1h = 0.1,            // 0.1%占空比
    .max_power_dbm = 14                  // 14dBm功率
};
```

```
从机: [数据] ──────→ 主机
      定期发送      仅接收
```

#### T2模式 - 双向传输
- **方向**: 双向
- **特点**: 支持主机向从机发送命令
- **码率**: M-2-O: 100 kbps, O-2-M: 32.768 kbps
- **容差**: T: ±1%, R: ±1.5%
- **应用**: 远程配置，实时控制
- **时序**: 从机发送后短时间内监听响应

##### T2模式法规参数
- **占空比**: ≤ 0.1% (在任意时间段)
- **最大突发时间**: ≤ 100ms
- **监听窗口**: 发送后监听≤500ms
- **发射功率**: ≤ 25mW (14dBm)
- **接收灵敏度**: ≥ -105dBm

##### T2模式双向通信参数
```c
// T2模式配置
typedef struct {
    uint32_t transmission_interval_ms;  // 发送间隔
    uint32_t max_burst_time_ms;        // 最大突发时间 (≤100ms)
    uint32_t listen_window_ms;         // 监听窗口 (≤500ms)
    float max_duty_cycle_1h;           // 1小时内最大占空比 (≤10%)
    uint8_t max_power_dbm;             // 最大发射功率
} t2_mode_config_t;

// 配置示例 (根据EN 13757-4标准参数)
t2_mode_config_t t2_config = {
    .transmission_interval_ms = 900000,   // 15分钟间隔
    .max_burst_time_ms = 80,             // 80ms突发时间
    .listen_window_ms = 300,             // 300ms监听窗口
    .max_duty_cycle_1h = 0.1,            // 0.1%占空比
    .max_power_dbm = 14                  // 14dBm功率
};
```

```
从机: [数据] ──────→ 主机
      ↑              ↓
      [响应] ←────── [命令]
```

### C模式 (Frequent Receive)

#### 技术规格
- **频率**: 868.95 MHz
- **Chip码率**: 100 kbps
- **数据速率**: 100 kbps (DRate:Chip)
- **码率容差**: ±100ppm (超高精度，0.01%)
- **接收灵敏度**: -80dBm (低) / -90dBm (中) / -105dBm (高)
- **前导码长度**: 32chips (标准) 或 512位 (特殊格式)
- **定号码长度**: 未指定

#### 子模式详细参数

##### C1模式 - 频繁发送仅读表
- **传输方向**: 仅发送 (Transmit only)
- **数据速率**: 100 kbps
- **应用**: 移动或固定读表
- **占空比**: ≤ 0.1% (在任意时间段)
- **最大突发时间**: < 22ms
- **发射功率**: ≤ 25mW (14dBm)
- **频段**: NRZ and short header
- **典型应用**: 定期抄表，短数据突发

```c
// C1模式配置
typedef struct {
    uint32_t transmission_interval_ms;  // 发送间隔
    uint32_t max_burst_time_ms;        // 最大突发时间 (<22ms)
    float max_duty_cycle;              // 最大占空比 (≤0.1%)
    uint8_t max_power_dbm;             // 最大发射功率
} c1_mode_config_t;

// C1模式配置示例 (基于EN 13757-4标准)
c1_mode_config_t c1_config = {
    .transmission_interval_ms = 600000,  // 10分钟间隔
    .max_burst_time_ms = 20,            // 20ms突发时间
    .max_duty_cycle = 0.1,              // 0.1%占空比
    .max_power_dbm = 14                 // 14dBm功率
};

// C1模式合规性检查
bool validate_c1_compliance(uint32_t burst_time_ms, uint32_t interval_ms) {
    float duty_cycle = ((float)burst_time_ms / interval_ms) * 100.0f;
    return (duty_cycle <= 0.1f) && (burst_time_ms < 22);
}
```

##### C2模式 - 频繁接收双向通信
- **传输方向**: 双向 (Bidirectional)
- **发送速率**: 100 kbps
- **接收速率**: 50 kbps
- **应用**: 实时控制和监控
- **占空比**: M-2-O: ≤ 0.1%, O-2-M: ≤ 10% (发送/接收不同限制)
- **发送突发时间**: ≤ 100ms
- **接收窗口**: 可连续开启
- **发射功率**: ≤ 25mW (14dBm)

```c
// C2模式配置
typedef struct {
    uint32_t tx_burst_time_ms;         // 发送突发时间 (≤100ms)
    uint32_t rx_window_time_ms;        // 接收窗口时间
    bool continuous_rx_mode;           // 连续接收模式
    uint8_t tx_power_dbm;              // 发送功率
    int8_t rx_sensitivity_dbm;         // 接收灵敏度
} c2_mode_config_t;

// C2模式配置示例 (基于EN 13757-4标准)
c2_mode_config_t c2_config = {
    .tx_burst_time_ms = 80,            // 80ms发送突发
    .rx_window_time_ms = 0,            // 连续接收
    .continuous_rx_mode = true,        // 启用连续接收
    .tx_power_dbm = 14,                // 14dBm发送功率
    .rx_sensitivity_dbm = -105         // -105dBm接收灵敏度
};
```

#### 特点
- **双向通信**: 支持完整的双向数据交换
- **高可用性**: 从机大部分时间处于接收状态
- **实时性**: 支持实时命令和响应
- **功耗**: 相对较高
- **超高精度**: ±100ppm码率容差要求

#### C模式前导码详解 (基于EN 13757-4 §9.4.2)

**前导码模式**:
- **模式a**: 16 × (01) 01010100011101 01010100 11001101₂
- **模式b**: 16 × (01) 01010100011101 01010100 00111101₂

**关键特性**:
- **重复次数**: n = 16 (固定)
- **模式长度**: 每个重复32位
- **总前导码长度**: 16 × 32 = 512位
- **同步检测**: 接收器通过检测"01010101"模式实现帧同步
- **模式选择**: 两种模式支持不同的解码器实现

**前导码功能**:
1. **帧同步**: 使接收器能够检测到新帧的开始
2. **时钟恢复**: 提供足够的边沿用于时钟同步
3. **增益控制**: 允许接收器调整信号强度
4. **多用户支持**: 在多用户环境中提高通信容量

**实现注意事项**:
- 发送器必须在每次传输前发送完整的前导码
- 接收器应能够检测到"01010101"模式并停止前导码检测
- 前导码检测失败时应启动新的帧检测

#### 帧类型
- **Frame A**: 标准帧格式
- **Frame B**: 扩展帧格式

```
主机: [命令] ──────→ 从机
      ↑              ↓
      [响应] ←────── [处理]
```

### S模式 (Stationary)

#### 子模式详细参数

##### S1模式 - 标准固定模式
- **前导码长度**: 576chips (最长前导码)
- **应用**: 长距离固定通信
- **占空比**: ≤ 0.02% (在1小时内)
- **最大突发时间**: ≤ 100ms
- **发射功率**: ≤ 25mW (14dBm)
- **传输距离**: 最远 (高前导码增益)

```c
// S1模式配置
typedef struct {
    uint32_t preamble_chips;           // 前导码长度 (576chips)
    uint32_t max_burst_time_ms;        // 最大突发时间 (≤100ms)
    float max_duty_cycle_1h;           // 1小时内最大占空比 (≤1%)
    uint8_t max_power_dbm;             // 最大发射功率
    uint32_t transmission_interval_ms;  // 发送间隔
} s1_mode_config_t;

// S1模式配置示例 (基于EN 13757-4标准)
s1_mode_config_t s1_config = {
    .preamble_chips = 576,             // 576chips前导码
    .max_burst_time_ms = 90,           // 90ms突发时间
    .max_duty_cycle_1h = 0.02,         // 0.02%占空比
    .max_power_dbm = 14,               // 14dBm功率
    .transmission_interval_ms = 1800000 // 30分钟间隔
};
```

##### S1-m模式 - 移动模式
- **前导码长度**: 48chips (短前导码)
- **应用**: 移动设备，快速同步
- **占空比**: ≤ 0.02% (在1小时内)
- **最大突发时间**: ≤ 100ms
- **发射功率**: ≤ 25mW (14dBm)
- **移动性**: 支持移动中通信

```c
// S1-m模式配置
s1_mode_config_t s1m_config = {
    .preamble_chips = 48,              // 48chips前导码
    .max_burst_time_ms = 60,           // 60ms突发时间
    .max_duty_cycle_1h = 0.02,         // 0.02%占空比
    .max_power_dbm = 14,               // 14dBm功率
    .transmission_interval_ms = 300000  // 5分钟间隔 (移动设备更频繁)
};
```

##### S2模式 - 增强模式
- **前导码长度**: 48chips
- **码率容差**: ±1.5% (标准) 或 ±7% (可选)
- **应用**: 特殊应用，宽容差要求
- **占空比**: ≤ 1% (在1小时内)
- **最大突发时间**: ≤ 100ms
- **发射功率**: ≤ 25mW (14dBm)

```c
// S2模式配置
typedef struct {
    uint32_t preamble_chips;           // 前导码长度 (48chips)
    float clock_tolerance_percent;     // 时钟容差 (±1.5% 或 ±7%)
    uint32_t max_burst_time_ms;        // 最大突发时间
    float max_duty_cycle_1h;           // 1小时内最大占空比
    uint8_t max_power_dbm;             // 最大发射功率
} s2_mode_config_t;

// S2模式配置示例 (基于EN 13757-4标准)
s2_mode_config_t s2_config = {
    .preamble_chips = 48,              // 48chips前导码
    .clock_tolerance_percent = 1.5,    // ±1.5%容差 (可选7%)
    .max_burst_time_ms = 70,           // 70ms突发时间
    .max_duty_cycle_1h = 1.0,          // 1%占空比
    .max_power_dbm = 14                // 14dBm功率
};
```

#### 技术规格
- **频率**: 868.3 MHz
- **Chip码率**: 32.768 kbps
- **数据速率**: 16.384 kbps (DRate:Chip/2)
- **码率容差**: ±1.5%
- **接收灵敏度**: -80dBm (低) / -90dBm (中) / -105dBm (高)
- **数据位置大容差**: ±3μS
- **定号码长度**: 2×8 chips

#### 特点
- **固定安装**: 适用于固定位置设备
- **中等功耗**: 平衡功耗和性能
- **同步通信**: 支持同步数据传输
- **网络拓扑**: 支持星型和网状网络
- **可变前导码**: 根据应用场景选择不同长度的前导码

### N模式 (Narrowband)

#### 技术规格
- **频率**: 169.4-169.475 MHz
- **调制**: 4-GFSK
- **码率容差**: ±100ppm (超高精度，0.01%)
- **前导码长度**: 10bits (最短)
- **定号码长度**: 0 (不使用定号码)
- **接收灵敏度**: -90dBm (低) / -100dBm (中) / 高等级可变

#### 子模式详细参数

##### N1模式 (N1a-f) - 超长距离低速率
- **数据速率**: 2.4 kbps
- **接收灵敏度**: -123dBm @ 2.4kbps (超高灵敏度)
- **占空比**: ≤ 10% or less (在1小时内)
- **最大突发时间**: ≤ 1000ms (1秒)
- **发射功率**: ≤ 500mW (27dBm)
- **传输距离**: 最远 (数公里)
- **应用**: 农村、偏远地区

```c
// N1模式配置
typedef struct {
    uint32_t data_rate_bps;            // 数据速率 (2400bps)
    int8_t rx_sensitivity_dbm;         // 接收灵敏度 (-123dBm)
    uint32_t max_burst_time_ms;        // 最大突发时间 (≤1000ms)
    float max_duty_cycle_1h;           // 1小时内最大占空比 (≤0.1%)
    uint8_t max_power_dbm;             // 最大发射功率 (27dBm)
    uint32_t frequency_hz;             // 工作频率
} n1_mode_config_t;

// N1模式配置示例 (基于EN 13757-4标准)
n1_mode_config_t n1_config = {
    .data_rate_bps = 2400,             // 2.4kbps
    .rx_sensitivity_dbm = -123,        // -123dBm超高灵敏度
    .max_burst_time_ms = 800,          // 800ms突发时间
    .max_duty_cycle_1h = 10.0,         // 10%占空比
    .max_power_dbm = 27,               // 27dBm功率
    .frequency_hz = 169400000          // 169.4MHz
};
```

##### N2模式 (N2a-f) - 中距离中速率
- **数据速率**: 4.8 kbps
- **接收灵敏度**: -120dBm @ 4.8kbps (高灵敏度)
- **占空比**: ≤ 10% or less (在1小时内)
- **最大突发时间**: ≤ 500ms
- **发射功率**: ≤ 500mW (27dBm)
- **传输距离**: 中等距离
- **应用**: 城郊、中等密度区域

```c
// N2模式配置
n1_mode_config_t n2_config = {
    .data_rate_bps = 4800,             // 4.8kbps
    .rx_sensitivity_dbm = -120,        // -120dBm高灵敏度
    .max_burst_time_ms = 400,          // 400ms突发时间
    .max_duty_cycle_1h = 10.0,         // 10%占空比
    .max_power_dbm = 27,               // 27dBm功率
    .frequency_hz = 169412500          // 169.4125MHz
};
```

##### N2g模式 - 高速率模式
- **数据速率**: 19.2 kbps (9.6 kbps可选)
- **接收灵敏度**: -107dBm @ 19.2kbps
- **占空比**: ≤ 10% (在1小时内)
- **最大突发时间**: ≤ 100ms
- **发射功率**: ≤ 500mW (27dBm)
- **传输距离**: 较短距离，高数据量
- **应用**: 城市密集区域，高数据需求

```c
// N2g模式配置
n1_mode_config_t n2g_config = {
    .data_rate_bps = 19200,            // 19.2kbps
    .rx_sensitivity_dbm = -107,        // -107dBm
    .max_burst_time_ms = 80,           // 80ms突发时间
    .max_duty_cycle_1h = 10.0,         // 10%占空比
    .max_power_dbm = 27,               // 27dBm功率
    .frequency_hz = 169437500          // 169.4375MHz
};
```

#### 信道分配
- **N1**: 2.4 kbps (信道2a, 2b)
- **N2**: 4.8 kbps (信道1a, 1b, 3a, 3b)
- **N3**: 19.2 kbps (信道0)

#### 特点
- **长距离**: 传输距离可达数公里
- **高功率**: 最大500mW
- **窄带**: 减少干扰
- **适用**: 农村、偏远地区

### R模式 (Frequent Receive, Long Range)

#### 技术规格
- **频率**: 868.33 MHz
- **Chip码率**: 4.8 kbps
- **数据速率**: 2.4 kbps (DRate:Chip/2)
- **码率容差**: ±1.5%
- **接收灵敏度**: -80dBm (低) / -90dBm (中) / -110dBm (高)
- **数据位置大容差**: ±15μS (较大容差)
- **前导码长度**: 90chips (较长前导码)
- **定号码长度**: 2×8 chips

#### R模式法规和应用参数

##### R2模式 - 长距离双向通信
- **传输方向**: 双向 (Bidirectional)
- **发送速率**: 4.8 kbps Chip码率，2.4 kbps数据速率
- **接收速率**: 4.8 kbps Chip码率，2.4 kbps数据速率
- **占空比**: ≤ 1% (在1小时内)
- **最大突发时间**: ≤ 500ms
- **发射功率**: ≤ 25mW (14dBm)
- **接收灵敏度**: -110dBm (超高灵敏度)
- **前导码长度**: 90chips (较长前导码)

```c
// R2模式配置
typedef struct {
    uint32_t chip_rate_bps;            // Chip码率 (4800bps)
    uint32_t data_rate_bps;            // 数据速率 (2400bps)
    int8_t rx_sensitivity_dbm;         // 接收灵敏度 (-110dBm)
    uint32_t max_burst_time_ms;        // 最大突发时间 (≤500ms)
    float max_duty_cycle_1h;           // 1小时内最大占空比 (≤10%)
    uint8_t max_power_dbm;             // 最大发射功率 (14dBm)
    uint32_t preamble_chips;           // 前导码长度 (90chips)
    float data_position_tolerance_us;  // 数据位置容差 (±15μS)
} r2_mode_config_t;

// R2模式配置示例 (基于EN 13757-4标准)
r2_mode_config_t r2_config = {
    .chip_rate_bps = 4800,             // 4.8kbps Chip码率
    .data_rate_bps = 2400,             // 2.4kbps数据速率
    .rx_sensitivity_dbm = -110,        // -110dBm超高灵敏度
    .max_burst_time_ms = 400,          // 400ms突发时间
    .max_duty_cycle_1h = 1.0,          // 1%占空比
    .max_power_dbm = 14,               // 14dBm功率
    .preamble_chips = 90,              // 90chips前导码
    .data_position_tolerance_us = 15.0 // ±15μS容差
};
```

#### 特点
- **长距离**: 传输距离较远，高接收灵敏度(-110dBm)
- **低速率**: 4.8 kbps Chip码率，2.4 kbps数据速率
- **高灵敏度**: 接收灵敏度高，适合恶劣环境
- **较大容差**: ±15μS数据位置容差，适应时钟偏差
- **双向通信**: 支持完整的双向数据交换
- **适用**: 地下室、管道井等远距离或遮挡环境

### F模式 (Frequent Transmit, ISM)

#### 技术规格
- **频率**: 433.82 MHz (全球ISM频段)
- **Chip码率**: 2.4 kbps
- **码率容差**: ±100ppm (超高精度，0.01%)
- **接收灵敏度**: -105dBm (低) / -110dBm (中) / -117dBm (高)
- **前导码长度**: 39bits
- **定号码长度**: 未指定

#### 子模式
- **F2-m**: 移动模式，2.4 kbps
- **F2**: 标准模式，2.4 kbps

#### F模式法规和应用参数

##### F2模式 - 全球ISM频段
- **频率**: 433.82 MHz (全球ISM频段)
- **数据速率**: 2.4 kbps
- **占空比**: ≤ 10% (在1小时内)
- **最大突发时间**: ≤ 100ms
- **发射功率**: ≤ 10mW (10dBm)
- **接收灵敏度**: -117dBm (最高灵敏度)
- **前导码长度**: 39bits
- **码率容差**: ±100ppm (超高精度)

```c
// F2模式配置
typedef struct {
    uint32_t frequency_hz;             // 工作频率 (433.82MHz)
    uint32_t data_rate_bps;            // 数据速率 (2400bps)
    int8_t rx_sensitivity_dbm;         // 接收灵敏度 (-117dBm)
    uint32_t max_burst_time_ms;        // 最大突发时间 (≤100ms)
    float max_duty_cycle_1h;           // 1小时内最大占空比 (≤10%)
    uint8_t max_power_dbm;             // 最大发射功率 (10dBm)
    uint32_t preamble_bits;            // 前导码长度 (39bits)
    float clock_tolerance_ppm;         // 时钟容差 (±100ppm)
} f2_mode_config_t;

// F2模式配置示例 (基于EN 13757-4标准)
f2_mode_config_t f2_config = {
    .frequency_hz = 433820000,         // 433.82MHz
    .data_rate_bps = 2400,             // 2.4kbps
    .rx_sensitivity_dbm = -117,        // -117dBm最高灵敏度
    .max_burst_time_ms = 80,           // 80ms突发时间
    .max_duty_cycle_1h = 10.0,         // 10%占空比
    .max_power_dbm = 10,               // 10dBm功率
    .preamble_bits = 39,               // 39bits前导码
    .clock_tolerance_ppm = 100.0       // ±100ppm容差
};
```

##### F2-m模式 - 移动模式
- **应用**: 便携式移动设备
- **移动性**: 支持移动中通信
- **低功耗模式**: 特别的低功耗模式
- **快速同步**: 同步算法
- **全球兼容**: 433MHz全球ISM频段

```c
// F2-m模式配置 (移动模式)
f2_mode_config_t f2m_config = {
    .frequency_hz = 433820000,         // 433.82MHz
    .data_rate_bps = 2400,             // 2.4kbps
    .rx_sensitivity_dbm = -115,        // -115dBm (移动模式稍低)
    .max_burst_time_ms = 60,           // 60ms突发时间 (移动模式)
    .max_duty_cycle_1h = 5.0,          // 5%占空比 (移动节能)
    .max_power_dbm = 8,                // 8dBm功率 (移动节能)
    .preamble_bits = 39,               // 39bits前导码
    .clock_tolerance_ppm = 100.0       // ±100ppm容差
};
```

#### 特点
- **全球频段**: 433 MHz ISM频段，全球通用
- **低功耗**: 适合电池供电，超高精度要求
- **低速率**: 2.4 kbps，低功耗
- **高灵敏度**: 最高可达-117dBm接收灵敏度
- **移动支持**: F2-m模式支持移动应用
- **适用**: 便携式设备、全球部署应用

---

## P2 Companion Standard 规范

### P2 Companion Standard 概述
P2 Companion Standard 是对EN 13757-4标准的重要补充规范，主要由荷兰Enexis等公司推动制定，旨在增强wM-Bus协议的互操作性和可靠性。

#### P2标准的核心贡献
- **FT3帧格式**: 定义了基于EN 60870-5-1的帧格式
- **双重校验**: 使用多个Checksum提高传输可靠性
- **可变长度**: 支持可变长度的链路用户数据块
- **标准化**: 基于成熟的EN 60870-5-1标准

### P2帧格式详解 (FT3 - General Format A)

#### 完整P2 FT3帧结构
```
┌────┬────┬────┬────┬────┬──────────┬─────────────────┬──────────┬─────┐
│ PL │ L  │ C  │ M  │ A  │ Checksum │ Link user data  │ Checksum │ ... │
│    │    │    │    │    │          │ (Variable len)  │          │     │
└────┴────┴────┴────┴────┴──────────┴─────────────────┴──────────┴─────┘
```

#### P2 FT3字段详细说明
- **PL**: Preamble (前导码)
- **L**: Length (长度字段)
- **C**: Control field (控制字段)
- **M**: Manufacturer ID (制造商ID)
- **A**: Address field of the sending Meter (发送设备地址字段)
- **Checksum**: 按照EN 60870-5-1规范的校验和
- **Link user data**: 可变长度数据块
- **Checksum**: 数据块的校验和

#### P2 FT3字段详细解析

##### Preamble (PL)
- **功能**: 帧前导码，用于接收器同步
- **格式**: 按照EN 60870-5-1规范定义
- **作用**: 确保接收器能够正确检测帧的开始

##### Length (L)
- **功能**: 指示后续数据的长度
- **格式**: 单字节长度字段
- **计算**: 从Control field开始到最后一个Checksum的总长度

##### Control field (C)
- **功能**: 控制字段，定义帧的类型和功能
- **格式**: 按照EN 60870-5-1规范
- **内容**: 包含帧功能码和控制位

##### Manufacturer ID (M)
- **功能**: 制造商标识
- **格式**: 制造商特定格式
- **作用**: 标识设备制造商

##### Address field (A)
- **功能**: 发送设备的地址字段
- **格式**: 设备地址信息
- **作用**: 标识发送数据的具体设备

##### Checksum (多个)
- **功能**: 数据完整性校验
- **格式**: 按照EN 60870-5-1规范
- **位置**: 在关键数据段后插入
- **作用**: 确保数据传输的可靠性

#### P2 FT3帧处理机制

##### FT3帧接收状态机
```c
// P2 FT3帧接收状态机
typedef enum {
    FT3_IDLE,
    FT3_PREAMBLE,
    FT3_LENGTH,
    FT3_CONTROL,
    FT3_MANUFACTURER,
    FT3_ADDRESS,
    FT3_CHECKSUM1,
    FT3_USER_DATA,
    FT3_CHECKSUM2,
    FT3_COMPLETE,
    FT3_ERROR
} p2_ft3_state_t;

p2_ft3_state_t process_p2_ft3_byte(uint8_t byte) {
    static p2_ft3_state_t state = FT3_IDLE;
    static uint8_t expected_length = 0;
    static uint8_t received_bytes = 0;
    static uint8_t user_data_length = 0;

    switch (state) {
        case FT3_IDLE:
            // 检测前导码
            if (is_valid_preamble(byte)) {
                state = FT3_PREAMBLE;
                received_bytes = 0;
            }
            break;

        case FT3_PREAMBLE:
            // 接收长度字段
            expected_length = byte;
            state = FT3_LENGTH;
            break;

        case FT3_LENGTH:
            // 接收控制字段
            state = FT3_CONTROL;
            break;

        case FT3_CONTROL:
            // 接收制造商ID
            state = FT3_MANUFACTURER;
            break;

        case FT3_MANUFACTURER:
            // 接收地址字段
            state = FT3_ADDRESS;
            break;

        case FT3_ADDRESS:
            // 接收第一个校验和
            state = FT3_CHECKSUM1;
            break;

        case FT3_CHECKSUM1:
            // 开始接收用户数据
            user_data_length = calculate_user_data_length(expected_length);
            if (user_data_length > 0) {
                state = FT3_USER_DATA;
                received_bytes = 0;
            } else {
                state = FT3_COMPLETE;
            }
            break;

        case FT3_USER_DATA:
            received_bytes++;
            if (received_bytes >= user_data_length) {
                state = FT3_CHECKSUM2;
            }
            break;

        case FT3_CHECKSUM2:
            // 帧接收完成
            state = FT3_COMPLETE;
            break;

        case FT3_COMPLETE:
            // 重置状态机
            state = FT3_IDLE;
            break;

        case FT3_ERROR:
            // 错误恢复
            state = FT3_IDLE;
            break;
    }

    return state;
}
```

### P2 FT3错误检测和恢复

#### 多层错误检测
1. **前导码检测**: Preamble格式正确性验证
2. **长度校验**: Length字段与实际数据长度一致性
3. **Checksum校验**: 按照EN 60870-5-1规范的多重校验
4. **帧完整性**: 整个FT3帧结构的完整性验证

#### 错误恢复策略
```c
// P2 FT3错误恢复机制
typedef enum {
    P2_FT3_ERROR_NONE,
    P2_FT3_ERROR_PREAMBLE_INVALID,
    P2_FT3_ERROR_LENGTH_INVALID,
    P2_FT3_ERROR_CHECKSUM_FAILED,
    P2_FT3_ERROR_FRAME_INCOMPLETE
} p2_ft3_error_type_t;

p2_ft3_error_type_t validate_p2_ft3_frame(const uint8_t* frame, uint32_t length) {
    uint32_t offset = 0;

    // 检查前导码
    if (!is_valid_preamble(frame[offset])) {
        return P2_FT3_ERROR_PREAMBLE_INVALID;
    }
    offset++;

    // 检查长度字段
    uint8_t declared_length = frame[offset++];
    if (declared_length + 2 != length) {  // +2 for PL and L fields
        return P2_FT3_ERROR_LENGTH_INVALID;
    }

    // 跳过C, M, A字段
    offset += 3;  // Control, Manufacturer, Address

    // 验证第一个Checksum
    uint8_t checksum1 = frame[offset++];
    uint8_t calculated_checksum1 = calculate_en60870_checksum(&frame[1], offset - 2);
    if (checksum1 != calculated_checksum1) {
        return P2_FT3_ERROR_CHECKSUM_FAILED;
    }

    // 如果有用户数据，验证第二个Checksum
    if (offset < length) {
        uint32_t user_data_start = offset;
        uint32_t user_data_length = length - offset - 1;  // -1 for final checksum

        uint8_t checksum2 = frame[length - 1];
        uint8_t calculated_checksum2 = calculate_en60870_checksum(&frame[user_data_start], user_data_length);
        if (checksum2 != calculated_checksum2) {
            return P2_FT3_ERROR_CHECKSUM_FAILED;
        }
    }

    return P2_FT3_ERROR_NONE;
}
```

### P2与EN 13757-4的兼容性

#### 向后兼容策略
- **核心保持**: EN 13757-4的核心字段完全保留
- **扩展增强**: P2仅在外层添加同步和边界标识
- **可选实现**: 设备可选择支持P2扩展

#### 实现选择指南
```c
// 帧格式选择策略
typedef enum {
    FRAME_FORMAT_EN13757_ONLY,    // 纯EN 13757-4格式
    FRAME_FORMAT_P2_ENHANCED,     // P2增强格式
    FRAME_FORMAT_AUTO_DETECT      // 自动检测
} frame_format_mode_t;

frame_format_mode_t select_frame_format(device_capability_t capability,
                                       network_environment_t environment) {
    if (environment == NETWORK_MIXED_VENDORS) {
        return FRAME_FORMAT_P2_ENHANCED;  // 多厂商环境使用P2
    } else if (capability == DEVICE_BASIC) {
        return FRAME_FORMAT_EN13757_ONLY; // 基础设备使用标准格式
    } else {
        return FRAME_FORMAT_AUTO_DETECT;  // 智能设备自动适应
    }
}
```

### P2实际应用示例

#### 智能水表P2帧示例
```
实际P2帧数据:
68 1E 1E 68 44 B8 06 12 34 56 78 01 07 7A 25 00 00 00 04 13 15 31 00 00 42 6C 00 00 A2 01 16

解析结果:
68          - 起始标识
1E 1E       - 长度字段 (30字节) 重复
68          - 同步确认
44          - 控制字段 (REQ-UD2)
B8 06       - 制造商ID (0x06B8)
12 34 56 78 - 设备地址 (78563412)
01          - 版本
07          - 设备类型 (水表)
7A          - CI字段 (STL)
25 00 00 00 - STL头部 (AN=0x25, ST=0x00, CW=0x0000)
04 13 15 31 00 00 - 数据记录 (水量: 31.15立方米)
42 6C 00 00 - 数据记录 (日期)
A2 01       - CRC校验
16          - 结束标识
```

#### P2帧构建完整示例
```c
// P2帧构建器
typedef struct {
    uint8_t start1;           // 0x68
    uint8_t length1;          // 数据长度
    uint8_t length2;          // 长度重复
    uint8_t start2;           // 0x68
    uint8_t control;          // C字段
    uint16_t manufacturer;    // 制造商ID
    uint32_t address;         // 设备地址
    uint8_t version;          // 版本
    uint8_t device_type;      // 设备类型
    uint8_t ci_field;         // CI字段
    uint8_t payload[];        // 可变长度载荷
    // uint16_t crc;          // CRC (在载荷后)
    // uint8_t end;           // 0x16 (在CRC后)
} __attribute__((packed)) p2_frame_header_t;

// 构建P2帧
uint32_t build_p2_frame(uint8_t* buffer, uint32_t buffer_size,
                       uint16_t manufacturer_id, uint32_t device_address,
                       uint8_t device_version, uint8_t device_type,
                       uint8_t ci_field, const uint8_t* payload, uint16_t payload_len) {

    if (buffer_size < (10 + payload_len + 3)) {  // 头部+载荷+CRC+结束
        return 0;  // 缓冲区太小
    }

    uint32_t offset = 0;
    uint8_t data_length = 6 + 1 + payload_len;  // 地址+CI+载荷长度

    // P2帧头部
    buffer[offset++] = 0x68;                    // 起始标识
    buffer[offset++] = data_length;             // 长度字段
    buffer[offset++] = data_length;             // 长度重复
    buffer[offset++] = 0x68;                    // 同步确认
    buffer[offset++] = 0x44;                    // 控制字段 (示例)

    // 设备地址字段
    *(uint16_t*)&buffer[offset] = manufacturer_id;  offset += 2;
    *(uint32_t*)&buffer[offset] = device_address;   offset += 4;
    buffer[offset++] = device_version;
    buffer[offset++] = device_type;

    // CI字段和载荷
    buffer[offset++] = ci_field;
    memcpy(&buffer[offset], payload, payload_len);
    offset += payload_len;

    // CRC计算 (从C字段开始到载荷结束)
    uint16_t crc = calculate_crc(&buffer[4], data_length);
    *(uint16_t*)&buffer[offset] = crc;
    offset += 2;

    // 结束标识
    buffer[offset++] = 0x16;

    return offset;
}
```

### P2标准的优势

#### 技术优势
1. **同步可靠性**: 双68字符提供强同步能力
2. **错误检测**: 多层校验机制
3. **边界明确**: 明确的帧起始和结束标识
4. **向后兼容**: 完全兼容EN 13757-4核心

#### 商业优势
1. **互操作性**: 不同厂商设备更好兼容
2. **部署简化**: 减少现场调试时间
3. **维护便利**: 更容易的故障诊断
4. **标准化**: 行业广泛采用的事实标准

---

## OMS规范

### 安全信息传输协议 (SITP) - OMS规范

#### SITP概述 (基于OMS-Spec Vol.2 §9.4)
安全信息传输协议(Security Information Transfer Protocol)是OMS规范中的应用层安全协议，用于保护关键应用命令和响应，防止未授权访问。

#### SITP应用安全配置文件
```c
// OMS应用安全配置文件 (Table 47)
typedef enum {
    ASP_NONE = 0,             // 无应用安全
    ASP01_KEY_EXCHANGE,       // 对称密钥交换
    ASP02_SECURITY_INFO_C,    // Security Profile C安全信息交换
    ASP03_FIRMWARE_AUTH,      // 固件镜像认证
    ASP10_AUTH_DATA,          // 端到端认证应用数据
    ASP20_SECURED_DATA        // 端到端加密应用数据
} oms_asp_profile_t;

// SITP配置结构
typedef struct {
    oms_asp_profile_t profile;
    char* cipher_method;
    uint16_t key_length_bits;
    uint8_t key_id;
    char* standard_reference;
} sitp_config_t;

// SITP配置表
static const sitp_config_t sitp_configs[] = {
    {
        .profile = ASP01_KEY_EXCHANGE,
        .cipher_method = "AES128 Key Wrap",
        .key_length_bits = 128,
        .key_id = 0x01,  // KeyID for key exchange
        .standard_reference = "prEN 13757-7:2023, Annex A, A.8"
    },
    {
        .profile = ASP03_FIRMWARE_AUTH,
        .cipher_method = "AES128 + ASN.1 DER",
        .key_length_bits = 128,
        .key_id = 0x03,  // KeyID for firmware authentication
        .standard_reference = "Annex F, appendix F.A"
    },
    {
        .profile = ASP10_AUTH_DATA,
        .cipher_method = "CMAC-16 (AES128)",
        .key_length_bits = 128,
        .key_id = 0x10,  // KeyID for authenticated data
        .standard_reference = "prEN 13757-3:2023, Annex I, I.2.4.6"
    },
    {
        .profile = ASP20_SECURED_DATA,
        .cipher_method = "AES128-CCM (8 Byte)",
        .key_length_bits = 128,
        .key_id = 0x20,  // KeyID for secured data
        .standard_reference = "prEN 13757-7:2023, Annex A, A.9.7"
    }
};
```

#### SITP使用场景
```c
// SITP必需使用的场景
typedef enum {
    SITP_USE_CASE_KEY_MGMT,       // 密钥管理 (OMS-UC-08)
    SITP_USE_CASE_FIRMWARE_UPDATE, // 固件更新 (OMS-UC-05)
    SITP_USE_CASE_SECURITY_C,     // Security Profile C密钥交换
    SITP_USE_CASE_CRITICAL_CMD    // 关键应用命令
} sitp_use_case_t;

// SITP消息处理
bool process_sitp_message(const uint8_t* message, size_t length,
                         oms_asp_profile_t profile, uint8_t key_id) {
    // 1. 验证SITP容器完整性
    if (!verify_sitp_container(message, length)) {
        return false;
    }

    // 2. 根据ASP配置文件解密/验证
    switch (profile) {
        case ASP10_AUTH_DATA:
            return verify_cmac_16(message, length, key_id);
        case ASP20_SECURED_DATA:
            return decrypt_aes_ccm(message, length, key_id);
        default:
            return false;
    }
}
```

### OMS概述
OMS (Open Metering System) 是基于EN 13757的开放计量系统规范，定义了设备互操作性标准。OMS规范采用并推广了P2 Companion Standard的帧格式增强。

### DIF (Data Information Field)

#### DIF结构
```
Bit:  7    6    5    4    3    2    1    0
      |    |              +---------------数据字段编码 (4位)
      |    +------------------------------存储编号 (3位)
      +-----------------------------------扩展位 (1位) 


Bit:      7    6    5    4    3    2    1    0
     ┌────┬────┬────┬────┬────┬────┬────┬────┐
     │EXT │Storage Number│ Data Field Coding │
     └────┴────┴────┴────┴────┴────┴────┴────┘
      │   │              │
      │   │              └──────────── 数据字段编码 (4位)
      │   │                            0000: 无数据
      │   │                            0001: 8位整数
      │   │                            0010: 16位整数
      │   └──存储编号 (3位)              0011: 24位整数
      │      000: 当前值 (瞬时值)        0100: 32位整数
      │      001: 最小值                0101: 32位实数
      │      010: 最大值                0110: 48位整数
      │      011: 错误值                0111: 64位整数
      │      100-111: 历史值 (按时间顺序) 1000: 选择编码                      
      │                                1001: 2位BCD
      │                                1010: 4位BCD
      │                                1011: 6位BCD
      └───  扩展位 (1位)                1100: 8位BCD
            0: 无扩展DIF                1101: 变长
            1: 后续有扩展DIF             1110: 12位BCD
                                       1111: 特殊功能
```

#### 数据字段编码 (4位)
| 编码 | 长度 | 类型 | 说明 |
|------|------|------|------|
| 0000 | 0 | 无数据 | 特殊功能 |
| 0001 | 1 | 8位整数 | |
| 0010 | 2 | 16位整数 | |
| 0011 | 3 | 24位整数 | |
| 0100 | 4 | 32位整数 | |
| 0101 | 4 | 32位实数 | IEEE 754 |
| 0110 | 6 | 48位整数 | |
| 0111 | 8 | 64位整数 | |
| 1001 | 2 | 2位BCD | |
| 1010 | 4 | 4位BCD | |
| 1011 | 6 | 6位BCD | |
| 1100 | 8 | 8位BCD | |

#### 存储编号 (3位) 用于区分同一类型的多个数据记录
| 编码  | 说明 |
|------|------|
| 000 | 当前值 (瞬时值) |
| 001 | 最小值 |
| 010 | 最大值 |
| 011 | 错误值 |
| 100-111 | 历史值 (按时间顺序) |

#### 扩展位 (1位) 用于后续扩展DIF
| 编码 | 说明 |
|------|------|
| 0 | 无扩展DIF |
| 1 | 后续有扩展DIF |

### VIF (Value Information Field)

#### 体积 (Volume)
| VIF | 单位 | 倍数 |
|-----|------|------|
| 0x10 | m³ | 10⁻⁶ |
| 0x11 | m³ | 10⁻⁵ |
| 0x12 | m³ | 10⁻⁴ |
| 0x13 | m³ | 10⁻³ (升) |
| 0x14 | m³ | 10⁻² |
| 0x15 | m³ | 10⁻¹ |
| 0x16 | m³ | 10⁰ |
| 0x17 | m³ | 10¹ |

#### 流量 (Flow)
| VIF | 单位 | 倍数 |
|-----|------|------|
| 0x38 | m³/h | 10⁻⁶ |
| 0x39 | m³/h | 10⁻⁵ |
| 0x3A | m³/h | 10⁻⁴ |
| 0x3B | m³/h | 10⁻³ (L/h) |
| 0x3C | m³/h | 10⁻² |
| 0x3D | m³/h | 10⁻¹ |
| 0x3E | m³/h | 10⁰ |
| 0x3F | m³/h | 10¹ |

#### 能量 (Energy)
| VIF | 单位 | 倍数 |
|-----|------|------|
| 0x00 | Wh | 10⁻³ |
| 0x01 | Wh | 10⁻² |
| 0x02 | Wh | 10⁻¹ |
| 0x03 | Wh | 10⁰ |
| 0x04 | Wh | 10¹ |
| 0x05 | Wh | 10² |
| 0x06 | Wh | 10³ (kWh) |
| 0x07 | Wh | 10⁴ |

#### 时间和日期
| VIF | 含义 |
|-----|------|
| 0x6C | 时间点(秒) |
| 0x6D | 时间点(分钟) |
| 0x6E | 时间点(小时) |
| 0x6F | 时间点(天) |

### 加密和安全

#### 加密模式
- **Mode 0**: 无加密
- **Mode 1**: DES加密 (已弃用)
- **Mode 5**: AES-128加密
- **Mode 7**: AES-256加密

#### AES加密流程
1. **密钥管理**: 16字节AES密钥
2. **初始向量**: 设备地址+访问号
3. **加密范围**: CI字段之后的所有数据
4. **认证**: 可选的消息认证码

---

## 设备安装过程

### 安装模式流程

#### 1. 设备上电
```
设备 → 自检 → 初始化射频 → 进入安装模式
```

#### 2. 安装请求 (Installation Request)
```
从机: [SND-IR] ──────→ 主机
      设备信息         接收并记录
```

#### 3. 安装确认 (Installation Confirmation)
```
主机: [CNF-IR] ──────→ 从机
      确认信息         接收确认
```

#### 4. 参数配置
```
主机: [配置命令] ────→ 从机
      参数设置        应用配置
```

### 安装数据内容

#### 设备识别信息
- 制造商ID
- 设备序列号
- 设备类型
- 软件版本
- 硬件版本

#### 配置参数
- 发送间隔
- 传输功率
- 加密密钥
- 工作模式

### 安装状态机
```
┌──────────────┐   POWER ON   ┌──────────────┐
│     IDLE     │  ──────────→ │ Install Mode │
└──────────────┘              └──────────────┘
       ↑                              │
       │                              │ 发送安装请求
       │                              ↓
┌──────────────┐              ┌──────────────┐
│ Normal Work  │ ←─────────── │   Wait ACK   │
└──────────────┘   RECV ACK   └──────────────┘
```

---

## 数据交换过程

### T1模式数据交换

#### 定期发送流程
```
1. 从机定时器触发
2. 准备数据包
3. 射频发送
4. 返回休眠
```

#### 数据包结构
```
┌────┬────┬────┬────┬────┬──────┬────┬───────────┬────┐
│ 68 │ LL │ LL │ 68 │ C  │ ADDR │ CI │ App DATA  │ 16 │
└────┴────┴────┴────┴────┴──────┴────┴───────────┴────┘
```

### C模式数据交换

#### 命令-响应流程
```
1. 主机发送命令
2. 从机接收处理
3. 从机发送响应
4. 主机接收确认
```

#### 时序图
```
主机    从机
 │       │
 │ REQ   │
 ├──────→│
 │       │ 处理
 │ RSP   │
 │←──────┤
 │       │
```

### 数据记录格式 (基于EN 13757-3:2018完整规范)

#### DIF/VIF编码完整规范

##### 数据信息字段 (DIF) 详细编码 (基于EN 13757-3:2018 Table 6-9)
```c
// DIF字段结构
typedef union {
    uint8_t raw;
    struct {
        uint8_t data_field_length : 4;   // 数据长度编码 (0000-1111)
        uint8_t function_field : 2;      // 功能字段 (00-11)
        uint8_t data_field_coding : 1;   // 数据编码类型 (0-1)
        uint8_t extension_bit : 1;       // 扩展位 (0-1)
    } __attribute__((packed)) fields;
} dif_t;

// DIF数据长度编码 (Table 7)
typedef enum {
    DIF_LENGTH_NONE = 0x0,           // 无数据
    DIF_LENGTH_8BIT = 0x1,           // 8位整数/8位BCD
    DIF_LENGTH_16BIT = 0x2,          // 16位整数/4位BCD
    DIF_LENGTH_24BIT = 0x3,          // 24位整数/6位BCD
    DIF_LENGTH_32BIT = 0x4,          // 32位整数/8位BCD
    DIF_LENGTH_32BIT_REAL = 0x5,     // 32位IEEE 754浮点数
    DIF_LENGTH_48BIT = 0x6,          // 48位整数/12位BCD
    DIF_LENGTH_64BIT = 0x7,          // 64位整数/16位BCD
    DIF_LENGTH_SELECTION = 0x8,      // 选择用于读出
    DIF_LENGTH_2BCD = 0x9,           // 2位BCD
    DIF_LENGTH_4BCD = 0xA,           // 4位BCD
    DIF_LENGTH_6BCD = 0xB,           // 6位BCD
    DIF_LENGTH_8BCD = 0xC,           // 8位BCD
    DIF_LENGTH_VARIABLE = 0xD,       // 可变长度
    DIF_LENGTH_12BCD = 0xE,          // 12位BCD
    DIF_LENGTH_SPECIAL = 0xF         // 特殊功能
} dif_length_t;

// DIF功能字段 (Table 8)
typedef enum {
    DIF_FUNC_INSTANTANEOUS = 0x0,    // 瞬时值
    DIF_FUNC_MAXIMUM = 0x1,          // 最大值
    DIF_FUNC_MINIMUM = 0x2,          // 最小值
    DIF_FUNC_ERROR = 0x3             // 错误值
} dif_function_t;

// DIFE扩展字段 (Table 9)
typedef union {
    uint8_t raw;
    struct {
        uint8_t storage_number : 4;      // 存储编号 (0-15)
        uint8_t tariff : 2;              // 费率信息 (0-3)
        uint8_t subunit : 1;             // 子单元 (0-1)
        uint8_t extension_bit : 1;       // 扩展位 (0-1)
    } __attribute__((packed)) fields;
} dife_t;
```

##### 值信息字段 (VIF) 详细编码 (基于EN 13757-3:2018 Table 11-16)
```c
// VIF主表编码 (Table 11) - 重要编码
typedef enum {
    // 能量类 (E000 0nnn) - Wh
    VIF_ENERGY_WH_E3 = 0x00,        // 能量 0.001 Wh
    VIF_ENERGY_WH_E2 = 0x01,        // 能量 0.01 Wh
    VIF_ENERGY_WH_E1 = 0x02,        // 能量 0.1 Wh
    VIF_ENERGY_WH_E0 = 0x03,        // 能量 1 Wh
    VIF_ENERGY_WH_E1_POS = 0x04,    // 能量 10 Wh
    VIF_ENERGY_WH_E2_POS = 0x05,    // 能量 100 Wh
    VIF_ENERGY_WH_E3_POS = 0x06,    // 能量 1000 Wh
    VIF_ENERGY_WH_E4_POS = 0x07,    // 能量 10000 Wh

    // 体积类 (E001 0nnn) - m³
    VIF_VOLUME_M3_E6 = 0x10,        // 体积 0.000001 m³
    VIF_VOLUME_M3_E5 = 0x11,        // 体积 0.00001 m³
    VIF_VOLUME_M3_E4 = 0x12,        // 体积 0.0001 m³
    VIF_VOLUME_M3_E3 = 0x13,        // 体积 0.001 m³
    VIF_VOLUME_M3_E2 = 0x14,        // 体积 0.01 m³
    VIF_VOLUME_M3_E1 = 0x15,        // 体积 0.1 m³
    VIF_VOLUME_M3_E0 = 0x16,        // 体积 1 m³
    VIF_VOLUME_M3_E1_POS = 0x17,    // 体积 10 m³

    // 功率类 (E010 1nnn) - W
    VIF_POWER_W_E3 = 0x28,          // 功率 0.001 W
    VIF_POWER_W_E2 = 0x29,          // 功率 0.01 W
    VIF_POWER_W_E1 = 0x2A,          // 功率 0.1 W
    VIF_POWER_W_E0 = 0x2B,          // 功率 1 W
    VIF_POWER_W_E1_POS = 0x2C,      // 功率 10 W
    VIF_POWER_W_E2_POS = 0x2D,      // 功率 100 W
    VIF_POWER_W_E3_POS = 0x2E,      // 功率 1000 W
    VIF_POWER_W_E4_POS = 0x2F,      // 功率 10000 W

    // 体积流量类 (E011 1nnn) - m³/h
    VIF_VOLUME_FLOW_M3H_E6 = 0x38,  // 体积流量 0.000001 m³/h
    VIF_VOLUME_FLOW_M3H_E5 = 0x39,  // 体积流量 0.00001 m³/h
    VIF_VOLUME_FLOW_M3H_E4 = 0x3A,  // 体积流量 0.0001 m³/h
    VIF_VOLUME_FLOW_M3H_E3 = 0x3B,  // 体积流量 0.001 m³/h
    VIF_VOLUME_FLOW_M3H_E2 = 0x3C,  // 体积流量 0.01 m³/h
    VIF_VOLUME_FLOW_M3H_E1 = 0x3D,  // 体积流量 0.1 m³/h
    VIF_VOLUME_FLOW_M3H_E0 = 0x3E,  // 体积流量 1 m³/h
    VIF_VOLUME_FLOW_M3H_E1_POS = 0x3F, // 体积流量 10 m³/h

    // 特殊VIF
    VIF_EXTENSION_FD = 0xFD,         // VIF扩展 (下一字节为真正VIF)
    VIF_EXTENSION_FB = 0xFB,         // VIF扩展 (下一字节为真正VIF)
    VIF_ANY_VIF = 0x7C,              // 任意VIF (用于读出选择)
    VIF_MANUFACTURER_SPECIFIC = 0x7F  // 制造商特定编码
} vif_primary_t;

// VIF扩展表 (VIF=FDh) (基于EN 13757-3:2018 Table 12)
typedef enum {
    VIFE_CREDIT_CURRENCY = 0x01,     // 信贷 (货币单位)
    VIFE_DEBIT_CURRENCY = 0x02,      // 借记 (货币单位)
    VIFE_ACCESS_NUMBER = 0x03,       // 访问编号
    VIFE_MEDIUM = 0x04,              // 介质 (如水、气体等)
    VIFE_MANUFACTURER = 0x05,        // 制造商
    VIFE_PARAMETER_SET_ID = 0x06,    // 参数集标识
    VIFE_MODEL_VERSION = 0x07,       // 模型/版本
    VIFE_HARDWARE_VERSION = 0x08,    // 硬件版本
    VIFE_FIRMWARE_VERSION = 0x09,    // 固件版本
    VIFE_SOFTWARE_VERSION = 0x0A,    // 软件版本
    VIFE_CUSTOMER_LOCATION = 0x0B,   // 客户位置
    VIFE_CUSTOMER = 0x0C,            // 客户
    VIFE_ACCESS_CODE_USER = 0x0D,    // 访问代码用户
    VIFE_ACCESS_CODE_OPERATOR = 0x0E, // 访问代码操作员
    VIFE_ACCESS_CODE_SYSTEM = 0x0F,  // 访问代码系统
    VIFE_DEVICE_TYPE = 0x10,         // 设备类型
    VIFE_MANUFACTURER_ID = 0x11,     // 制造商标识
    VIFE_PARAMETER_SET_VERSION = 0x12, // 参数集版本
    VIFE_OTHER_SOFTWARE_VERSION = 0x13, // 其他软件版本
    VIFE_BATTERY_LIFE_TIME = 0x14,   // 电池寿命时间
    VIFE_BATTERY_LIFE_LEFT = 0x15,   // 电池剩余寿命
    VIFE_DATE = 0x16,                // 日期
    VIFE_TIME = 0x17,                // 时间
    VIFE_DATE_TIME = 0x18,           // 日期时间
    VIFE_UNIT_1 = 0x19,              // 单位1
    VIFE_UNIT_2 = 0x1A,              // 单位2
    VIFE_UNIT_3 = 0x1B,              // 单位3
    VIFE_AVERAGING_DURATION = 0x1C,  // 平均持续时间
    VIFE_ACTUALITY_DURATION = 0x1D,  // 实际持续时间
    VIFE_FABRICATION_NO = 0x1E,      // 制造编号
    VIFE_ENHANCED_ID = 0x1F,         // 增强标识
    VIFE_BUS_ADDRESS = 0x20          // 总线地址
} vife_fd_t;
```

#### 基本记录
```
┌─────┬─────┬──────────┐
│ DIF │ VIF │   DATA   │
└─────┴─────┴──────────┘
```

#### 扩展记录
```
┌─────┬──────┬─────┬──────────┐
│ DIF │ DIFE │ VIF │   DATA   │
└─────┴──────┴─────┴──────────┘
```

### 错误处理

#### 错误类型
- **CRC错误**: 数据传输错误
- **长度错误**: 帧长度不匹配
- **地址错误**: 设备地址不匹配
- **命令错误**: 不支持的命令

#### 重传机制
- **自动重传**: 检测到错误自动重传
- **重传次数**: 通常3次
- **退避算法**: 指数退避

### OMS对wM-Bus的主要扩展

#### 概述
OMS (Open Metering System) 不仅仅是对VIF/DIF字段的扩展，而是对整个wM-Bus生态系统的全面标准化和增强。以下是OMS的主要扩展领域：

#### 1. VIF/DIF标准化扩展 ⭐⭐⭐⭐⭐

##### VIF扩展
```c
// OMS VIF标准化
typedef struct {
    uint8_t vif_code;             // 标准VIF代码
    char description[64];         // 标准化描述
    char unit[16];               // 标准化单位
    float multiplier;            // 标准化乘数
    uint16_t obis_code;          // 对应OBIS代码
} oms_vif_definition_t;

// OMS标准VIF表 (部分示例)
static const oms_vif_definition_t oms_vif_table[] = {
    {0x13, "Volume", "m³", 0.001, 0x0100},           // 体积 (升)
    {0x14, "Volume", "m³", 0.01, 0x0100},            // 体积 (10升)
    {0x15, "Volume", "m³", 0.1, 0x0100},             // 体积 (100升)
    {0x16, "Volume", "m³", 1.0, 0x0100},             // 体积 (立方米)
    {0x2B, "Power", "W", 1.0, 0x0F00},               // 功率 (瓦特)
    {0x2C, "Power", "W", 10.0, 0x0F00},              // 功率 (10瓦特)
    {0x3B, "Volume Flow", "m³/h", 0.001, 0x0900},    // 体积流量
    // ... 更多标准化VIF定义
};
```

##### DIF扩展
```c
// OMS DIF标准化处理
typedef struct {
    uint8_t data_field_coding;    // 数据字段编码 (4位)
    uint8_t storage_number;       // 存储编号 (3位)
    bool extension_bit;           // 扩展位 (1位)
    uint8_t data_length;          // 标准化数据长度
    data_type_t data_type;        // 标准化数据类型
} oms_dif_info_t;

// OMS数据类型标准化
typedef enum {
    OMS_DATA_TYPE_INTEGER,        // 整数类型
    OMS_DATA_TYPE_BCD,           // BCD编码
    OMS_DATA_TYPE_REAL,          // 实数类型
    OMS_DATA_TYPE_STRING,        // 字符串类型
    OMS_DATA_TYPE_DATE,          // 日期类型
    OMS_DATA_TYPE_TIME,          // 时间类型
    OMS_DATA_TYPE_DATETIME       // 日期时间类型
} oms_data_type_t;
```

#### 2. 安全配置文件扩展 ⭐⭐⭐⭐⭐

```c
// OMS安全配置文件
typedef enum {
    OMS_SECURITY_PROFILE_A,      // AES-128 CBC + CMAC
    OMS_SECURITY_PROFILE_B,      // AES-128 CBC + CMAC (增强)
    OMS_SECURITY_PROFILE_C,      // TLS 1.2 (最高级)
    OMS_SECURITY_PROFILE_D       // 动态密钥管理
} oms_security_profile_t;

// 安全配置文件A实现
typedef struct {
    uint8_t aes_key[16];         // AES-128密钥
    uint32_t frame_counter;      // 帧计数器
    uint8_t cmac_length;         // CMAC长度 (4或8字节)
    bool encryption_enabled;     // 加密使能
    bool authentication_enabled; // 认证使能
} oms_security_profile_a_t;

// 安全配置文件C (TLS)实现
typedef struct {
    tls_version_t tls_version;   // TLS版本 (1.2)
    cipher_suite_t cipher_suite; // 加密套件
    certificate_t device_cert;   // 设备证书
    private_key_t device_key;    // 设备私钥
    ca_certificate_t ca_cert;    // CA证书
} oms_security_profile_c_t;
```

#### 3. 设备类型标准化 ⭐⭐⭐⭐

```c
// OMS标准设备类型
typedef enum {
    OMS_DEVICE_OTHER = 0x00,                    // 其他设备
    OMS_DEVICE_OIL_METER = 0x01,               // 油表
    OMS_DEVICE_ELECTRICITY_METER = 0x02,        // 电表
    OMS_DEVICE_GAS_METER = 0x03,               // 气表
    OMS_DEVICE_HEAT_METER = 0x04,              // 热表
    OMS_DEVICE_STEAM_METER = 0x05,             // 蒸汽表
    OMS_DEVICE_WARM_WATER_METER = 0x06,        // 温水表
    OMS_DEVICE_WATER_METER = 0x07,             // 水表
    OMS_DEVICE_HEAT_COST_ALLOCATOR = 0x08,     // 热量分配器
    OMS_DEVICE_COMPRESSED_AIR = 0x09,          // 压缩空气表
    OMS_DEVICE_COOLING_METER = 0x0A,           // 冷量表
    OMS_DEVICE_HEAT_METER_INLET = 0x0B,        // 热表(进水)
    OMS_DEVICE_HEAT_METER_OUTLET = 0x0C,       // 热表(出水)
    OMS_DEVICE_HEAT_METER_INLET_OUTLET = 0x0D, // 热表(进出水)
    OMS_DEVICE_BUS_SYSTEM_COMPONENT = 0x0E,    // 总线系统组件
    OMS_DEVICE_UNKNOWN = 0x0F,                 // 未知设备
    // 扩展设备类型 (0x10-0xFF)
    OMS_DEVICE_SMOKE_DETECTOR = 0x1A,          // 烟雾探测器
    OMS_DEVICE_ROOM_SENSOR = 0x1B,             // 房间传感器
    OMS_DEVICE_GAS_DETECTOR = 0x1C,            // 气体探测器
    OMS_DEVICE_PRESSURE_SENSOR = 0x20,         // 压力传感器
    OMS_DEVICE_AD_CONVERTER = 0x21,            // A/D转换器
} oms_device_type_t;

// 设备能力描述
typedef struct {
    oms_device_type_t device_type;      // 设备类型
    char device_name[32];               // 设备名称
    uint32_t supported_vif_mask;        // 支持的VIF掩码
    bool bidirectional_capable;         // 双向通信能力
    oms_security_profile_t security_profiles[4]; // 支持的安全配置文件
    uint8_t max_data_records;           // 最大数据记录数
} oms_device_capability_t;
```

#### 4. 应用层协议扩展 ⭐⭐⭐

##### CI字段扩展
```c
// OMS扩展的CI字段定义
#define OMS_CI_SHORT_TRANSPORT_LAYER    0x7A    // 短传输层
#define OMS_CI_LONG_TRANSPORT_LAYER     0x8A    // 长传输层
#define OMS_CI_EXTENDED_LINK_LAYER      0x8C    // 扩展链路层
#define OMS_CI_AUTHENTICATION_ERROR     0xAF    // 认证错误
#define OMS_CI_APPLICATION_ERROR        0xA0    // 应用错误
#define OMS_CI_ALARM_STATUS            0xA1    // 告警状态
#define OMS_CI_APPLICATION_RESET       0xA2    // 应用重置
#define OMS_CI_USER_DATA_CONTAINER     0xA3    // 用户数据容器

// 传输层增强
typedef struct {
    uint8_t access_number;       // 访问编号
    uint8_t status;             // 状态字段
    uint16_t configuration_word; // 配置字
    uint32_t message_counter;    // 消息计数器 (防重放)
} oms_transport_layer_t;
```

##### 消息计数器机制
```c
// OMS消息计数器 (防重放攻击)
typedef struct {
    uint32_t tx_counter;         // 发送计数器
    uint32_t rx_counter;         // 接收计数器
    uint32_t last_valid_counter; // 最后有效计数器
    bool counter_sync_required;  // 需要计数器同步
} oms_message_counter_t;

// 计数器验证
bool oms_validate_message_counter(oms_message_counter_t* counter,
                                 uint32_t received_counter) {
    // 防重放检查
    if (received_counter <= counter->last_valid_counter) {
        return false; // 重放攻击
    }

    // 计数器跳跃检查
    if (received_counter > counter->last_valid_counter + 1000) {
        counter->counter_sync_required = true;
        return false; // 计数器跳跃过大
    }

    counter->last_valid_counter = received_counter;
    return true;
}
```

#### 5. 互操作性规范 ⭐⭐⭐

##### 制造商ID标准化
```c
// OMS制造商信息管理
typedef struct {
    uint16_t manufacturer_id;     // 3字节制造商代码
    char manufacturer_name[32];   // 制造商名称
    char country_code[4];        // 国家代码
    bool oms_certified;          // OMS认证状态
    uint32_t certification_date; // 认证日期
    oms_device_type_t supported_devices[16]; // 支持的设备类型
} oms_manufacturer_info_t;

// OMS认证设备数据库
static const oms_manufacturer_info_t oms_certified_manufacturers[] = {
    {0x4024, "Kamstrup", "DK", true, 20230101, {OMS_DEVICE_HEAT_METER, OMS_DEVICE_WATER_METER}},
    {0x4D2C, "Sensus", "DE", true, 20230115, {OMS_DEVICE_WATER_METER, OMS_DEVICE_GAS_METER}},
    {0x5068, "Itron", "FR", true, 20230201, {OMS_DEVICE_GAS_METER, OMS_DEVICE_ELECTRICITY_METER}},
    // ... 更多认证制造商
};
```

##### 兼容性矩阵
```c
// 设备兼容性检查
typedef struct {
    oms_device_type_t device_a;
    oms_device_type_t device_b;
    bool compatible;
    char compatibility_notes[128];
} oms_compatibility_entry_t;

// 兼容性验证
bool oms_check_device_compatibility(oms_device_type_t device1,
                                   oms_device_type_t device2) {
    // 检查兼容性矩阵
    for (int i = 0; i < sizeof(compatibility_matrix)/sizeof(oms_compatibility_entry_t); i++) {
        if ((compatibility_matrix[i].device_a == device1 &&
             compatibility_matrix[i].device_b == device2) ||
            (compatibility_matrix[i].device_a == device2 &&
             compatibility_matrix[i].device_b == device1)) {
            return compatibility_matrix[i].compatible;
        }
    }
    return false; // 默认不兼容
}
```

#### 6. 数据记录格式扩展 ⭐⭐⭐

##### 标准化数据记录
```c
// OMS标准数据记录格式
typedef struct {
    uint8_t dif;              // 数据信息字段
    uint8_t dife[4];          // 扩展DIF (可选)
    uint8_t vif;              // 值信息字段
    uint8_t vife[4];          // 扩展VIF (可选)
    uint8_t data[];           // 标准化数据
} oms_data_record_t;

// 时间戳标准化
typedef struct {
    uint8_t type;             // 时间戳类型
    union {
        struct {              // Type A: 日期
            uint8_t day;      // 日 (1-31)
            uint8_t month;    // 月 (1-12)
            uint8_t year;     // 年 (0-99, 相对2000年)
        } date;
        struct {              // Type B: 日期时间
            uint8_t minute;   // 分钟 (0-59)
            uint8_t hour;     // 小时 (0-23)
            uint8_t day;      // 日 (1-31)
            uint8_t month;    // 月 (1-12)
            uint8_t year;     // 年 (0-99, 相对2000年)
        } datetime;
        uint32_t unix_time;   // Type C: Unix时间戳
    };
} oms_timestamp_t;
```

##### 数据质量标识
```c
// OMS数据质量标识
typedef enum {
    OMS_DATA_QUALITY_GOOD = 0x00,        // 数据良好
    OMS_DATA_QUALITY_UNCERTAIN = 0x01,   // 数据不确定
    OMS_DATA_QUALITY_SUBSTITUTE = 0x02,  // 替代数据
    OMS_DATA_QUALITY_ESTIMATED = 0x03,   // 估算数据
    OMS_DATA_QUALITY_INVALID = 0x04      // 无效数据
} oms_data_quality_t;

// 带质量标识的数据记录
typedef struct {
    oms_data_record_t record;     // 标准数据记录
    oms_data_quality_t quality;   // 数据质量
    oms_timestamp_t timestamp;    // 时间戳
    uint8_t source_id;           // 数据源ID
} oms_quality_data_record_t;
```

#### 7. 错误处理和状态报告扩展 ⭐⭐

##### 标准化状态字段
```c
// OMS设备状态扩展
typedef struct {
    uint8_t device_status;        // 设备状态
    uint8_t application_status;   // 应用状态
    uint8_t error_flags;         // 错误标志
    uint8_t alarm_flags;         // 告警标志
    uint8_t warning_flags;       // 警告标志
    uint32_t error_counter;      // 错误计数器
} oms_status_info_t;

// 设备状态位定义
#define OMS_DEVICE_STATUS_POWER_LOW         0x01  // 电量低
#define OMS_DEVICE_STATUS_PERMANENT_ERROR   0x02  // 永久错误
#define OMS_DEVICE_STATUS_TEMPORARY_ERROR   0x04  // 临时错误
#define OMS_DEVICE_STATUS_SPECIFIC_ERROR    0x08  // 特定错误
#define OMS_DEVICE_STATUS_BATTERY_LOW       0x10  // 电池电量低
#define OMS_DEVICE_STATUS_REVERSE_FLOW      0x20  // 反向流量
#define OMS_DEVICE_STATUS_LEAK_DETECTED     0x40  // 检测到泄漏
#define OMS_DEVICE_STATUS_BURST_DETECTED    0x80  // 检测到爆管

// 应用状态位定义
#define OMS_APP_STATUS_CLOCK_SYNC_ERROR     0x01  // 时钟同步错误
#define OMS_APP_STATUS_CONFIG_ERROR         0x02  // 配置错误
#define OMS_APP_STATUS_SECURITY_ERROR       0x04  // 安全错误
#define OMS_APP_STATUS_CALIBRATION_ERROR    0x08  // 校准错误
```

##### 错误报告机制
```c
// OMS错误报告
typedef struct {
    uint16_t error_code;         // 错误代码
    uint32_t timestamp;          // 错误时间戳
    uint8_t severity;           // 严重程度 (1-5)
    char description[64];        // 错误描述
    uint8_t recovery_action;     // 恢复动作
} oms_error_report_t;

// 错误处理函数
void oms_handle_error(oms_error_report_t* error) {
    // 记录错误
    log_error(error);

    // 根据严重程度处理
    switch (error->severity) {
        case 1: // 信息
            break;
        case 2: // 警告
            send_warning_notification(error);
            break;
        case 3: // 错误
            send_error_notification(error);
            break;
        case 4: // 严重错误
            send_critical_notification(error);
            initiate_recovery_action(error->recovery_action);
            break;
        case 5: // 致命错误
            send_fatal_notification(error);
            shutdown_device();
            break;
    }
}
```

#### 8. 网络层扩展 ⭐⭐

##### 路由和中继
```c
// OMS网络路由
typedef struct {
    uint32_t source_address;     // 源地址
    uint32_t destination_address; // 目标地址
    uint8_t hop_count;          // 跳数
    uint8_t max_hops;           // 最大跳数
    uint32_t route_path[8];     // 路由路径
    uint8_t signal_strength;    // 信号强度
} oms_route_info_t;

// 网络拓扑管理
typedef struct {
    uint32_t network_id;        // 网络ID
    uint8_t node_count;         // 节点数量
    uint32_t node_list[256];    // 节点列表
    oms_route_info_t routes[256]; // 路由表
    uint32_t last_update;       // 最后更新时间
} oms_network_topology_t;

// 中继功能
bool oms_relay_message(const uint8_t* message, size_t length,
                      uint32_t destination_address) {
    // 查找路由
    oms_route_info_t* route = find_route(destination_address);
    if (!route) {
        return false; // 无路由
    }

    // 检查跳数限制
    if (route->hop_count >= route->max_hops) {
        return false; // 超过最大跳数
    }

    // 转发消息
    route->hop_count++;
    return forward_message(message, length, route);
}
```

#### 9. 配置和管理扩展 ⭐⭐

##### 远程配置
```c
// OMS远程配置类型
typedef enum {
    OMS_CONFIG_READING_INTERVAL,     // 读取间隔
    OMS_CONFIG_TRANSMISSION_POWER,   // 传输功率
    OMS_CONFIG_SECURITY_KEYS,        // 安全密钥
    OMS_CONFIG_DEVICE_PARAMETERS,    // 设备参数
    OMS_CONFIG_ALARM_THRESHOLDS,     // 告警阈值
    OMS_CONFIG_NETWORK_SETTINGS,     // 网络设置
    OMS_CONFIG_TIME_SYNC,           // 时间同步
    OMS_CONFIG_FIRMWARE_UPDATE      // 固件更新
} oms_config_type_t;

// 配置参数结构
typedef struct {
    oms_config_type_t type;      // 配置类型
    uint16_t parameter_id;       // 参数ID
    uint8_t data_length;         // 数据长度
    uint8_t data[64];           // 配置数据
    uint32_t timestamp;          // 配置时间戳
    bool persistent;             // 是否持久化
} oms_config_parameter_t;

// 远程配置处理
bool oms_process_remote_config(const oms_config_parameter_t* config) {
    // 验证配置权限
    if (!verify_config_permission(config->type)) {
        return false;
    }

    // 应用配置
    switch (config->type) {
        case OMS_CONFIG_READING_INTERVAL:
            set_reading_interval(*(uint32_t*)config->data);
            break;
        case OMS_CONFIG_TRANSMISSION_POWER:
            set_transmission_power(config->data[0]);
            break;
        case OMS_CONFIG_SECURITY_KEYS:
            update_security_keys(config->data, config->data_length);
            break;
        // ... 其他配置类型
    }

    // 持久化配置
    if (config->persistent) {
        save_config_to_flash(config);
    }

    return true;
}
```

### OMS扩展重要性总结

#### 重要性排序 (⭐⭐⭐⭐⭐ = 最重要)

1. **🥇 VIF/DIF标准化** ⭐⭐⭐⭐⭐
   - 数据互操作性的核心基础
   - 确保不同厂商设备数据格式一致

2. **🥈 安全配置文件** ⭐⭐⭐⭐⭐
   - 信息安全的重要保障
   - 提供多层次安全解决方案

3. **🥉 设备类型标准化** ⭐⭐⭐⭐
   - 设备识别和管理的基础
   - 简化设备集成和维护

4. **应用层协议扩展** ⭐⭐⭐
   - 功能增强和可靠性提升
   - 防重放攻击等安全机制

5. **互操作性规范** ⭐⭐⭐
   - 厂商兼容性保证
   - 降低系统集成风险

6. **数据记录格式** ⭐⭐⭐
   - 数据一致性和质量保证
   - 时间戳和质量标识标准化

7. **错误处理和状态报告** ⭐⭐
   - 系统可靠性和可维护性
   - 故障诊断和预防

8. **网络层扩展** ⭐⭐
   - 网络功能增强
   - 支持复杂网络拓扑

9. **配置和管理** ⭐⭐
   - 运维便利性
   - 远程管理能力

#### 核心价值

OMS的核心价值不仅仅是技术扩展，更是：

1. **🎯 标准化整个生态系统** - 从数据格式到安全机制的全面标准化
2. **🔗 确保互操作性** - 不同厂商设备能够无缝协作
3. **🔒 增强安全性** - 提供完整的端到端安全解决方案
4. **⚡ 简化部署** - 标准化大幅减少系统集成复杂度
5. **🛠️ 提升可维护性** - 统一的错误处理和状态报告机制

---

## wM-Bus网关应用

### 概述

wM-Bus网关是连接表计设备与数据中心的关键基础设施，负责数据收集、协议转换、远程控制等核心功能。基于EN 13757-4标准，wM-Bus模块可以配置为"Other Device"角色，实现完整的网关功能。

### 网关核心功能

#### 1. 数据收集与转发

```c
// wM-Bus网关数据收集功能
typedef struct {
    // 表计管理
    uint32_t max_meters;              // 最大支持表计数量
    uint32_t active_meters;           // 当前活跃表计数
    meter_info_t *meter_database;     // 表计数据库

    // 数据收集
    bool auto_discovery;              // 自动发现新表计
    uint32_t collection_interval_s;   // 数据收集间隔(秒)
    uint32_t max_retry_count;         // 最大重试次数

    // 数据缓存
    data_buffer_t local_storage;      // 本地数据缓存
    uint32_t max_cache_size_mb;       // 最大缓存大小(MB)
    uint32_t cache_retention_hours;   // 缓存保留时间(小时)
} wmbus_gateway_collector_t;
```

#### 2. 双向通信能力

网关支持以下双向通信模式：

| 模式 | 上行频率 | 下行频率 | 上行速率 | 下行速率 | 应用场景 |
|------|----------|----------|----------|----------|----------|
| **T2** | 868.95 MHz | 868.3 MHz | 66.67 kbps | 16.384 kbps | 标准双向通信 |
| **C2** | 868.95 MHz | 869.525 MHz | 100 kbps | 50 kbps | 频繁接收控制 |
| **R2** | 868.33 MHz | 868.33 MHz | 2.4 kbps | 2.4 kbps | 长距离双向 |

```c
// 双向通信配置
typedef struct {
    // 通信模式选择
    enum {
        WMBUS_GATEWAY_MODE_T2,        // T2模式双向通信
        WMBUS_GATEWAY_MODE_C2,        // C2模式频繁接收
        WMBUS_GATEWAY_MODE_R2         // R2模式长距离
    } communication_mode;

    // 远程控制功能
    bool remote_configuration;        // 远程配置表计
    bool remote_firmware_update;      // 远程固件升级
    bool remote_parameter_setting;    // 远程参数设置
    bool remote_valve_control;        // 远程阀门控制

    // 响应时间配置
    uint32_t response_timeout_ms;     // 响应超时时间
    uint32_t command_retry_count;     // 命令重试次数
    uint32_t fac_timeout_s;          // FAC超时时间
} wmbus_gateway_bidirectional_t;
```

#### 3. 协议转换与接口

```c
// 网关协议转换功能
typedef struct {
    // wM-Bus接口 (下行)
    struct {
        wmbus_mode_t active_modes[6];     // 支持的wM-Bus模式
        uint32_t listen_frequencies[4];   // 监听频率列表
        int8_t rx_sensitivity_dbm;        // 接收灵敏度
        uint8_t tx_power_dbm;             // 发射功率
    } wmbus_interface;

    // 上行通信接口 (到数据中心)
    union {
        struct {
            uint32_t ip_address;          // IP地址
            uint16_t port;                // 端口号
            bool dhcp_enabled;            // DHCP使能
        } ethernet;

        struct {
            char apn[32];                 // APN设置
            char operator_name[16];       // 运营商名称
            uint8_t signal_strength;      // 信号强度
        } cellular_4g;

        struct {
            char ssid[32];                // WiFi SSID
            char password[64];            // WiFi密码
            uint8_t channel;              // WiFi信道
        } wifi;
    } uplink_interface;

    // 协议转换
    protocol_converter_t converter;       // 协议转换器
    data_format_t output_format;          // 输出数据格式
} wmbus_gateway_interfaces_t;
```

### 网关部署架构

#### 1. 集中式网关架构

```
┌─────────────────────────────────────────────────────────────┐
│                    数据中心/云平台                          │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │ 数据存储    │  │ 业务逻辑    │  │ 用户界面    │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
└─────────────────────┬───────────────────────────────────────┘
                      │ TCP/IP (以太网/4G/WiFi)
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                 wM-Bus网关                                  │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │ 协议转换    │  │ 数据缓存    │  │ 设备管理    │        │
│  │ 安全加密    │  │ 本地存储    │  │ 远程控制    │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
└─────────────────────┬───────────────────────────────────────┘
                      │ wM-Bus (868MHz/169MHz)
                      │
┌─────────────────────▼───────────────────────────────────────┐
│                   表计设备层                                │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────┐        │
│  │   水表      │  │   电表      │  │   气表      │        │
│  │ (T1/T2)     │  │ (C1/C2)     │  │ (S1/S2)     │        │
│  └─────────────┘  └─────────────┘  └─────────────┘        │
└─────────────────────────────────────────────────────────────┘
```

#### 2. 分布式网关架构

```
数据中心 ←─── 主网关 ←─── 子网关1 ←─── 表计群1
    │           │           │
    │           │           └─── 表计群2
    │           │
    │           └─── 子网关2 ←─── 表计群3
    │                       │
    │                       └─── 表计群4
    │
    └─── 备用网关 ←─── 表计群5 (冗余)
```

### 网关技术实现

#### 1. 网关初始化配置

```c
// 网关初始化函数
wmbus_gateway_result_t wmbus_gateway_init(wmbus_gateway_config_t *config) {
    wmbus_gateway_result_t result = {0};

    // 1. 硬件初始化
    if (wmbus_radio_init(&config->radio_config) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_RADIO_INIT;
        return result;
    }

    // 2. 协议栈初始化
    if (wmbus_stack_init(&config->stack_config) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_STACK_INIT;
        return result;
    }

    // 3. 网络接口初始化
    if (network_interface_init(&config->network_config) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_NETWORK_INIT;
        return result;
    }

    // 4. 安全模块初始化
    if (security_module_init(&config->security_config) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_SECURITY_INIT;
        return result;
    }

    // 5. 数据库初始化
    if (meter_database_init(&config->database_config) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_DATABASE_INIT;
        return result;
    }

    result.success = true;
    return result;
}
```

#### 2. 表计自动发现

```c
// 表计自动发现功能
typedef struct {
    uint32_t discovery_interval_s;    // 发现间隔(秒)
    uint32_t discovery_timeout_s;     // 发现超时(秒)
    uint8_t discovery_channels[16];   // 发现信道列表
    bool continuous_discovery;        // 持续发现模式
} meter_discovery_config_t;

// 发现新表计
wmbus_gateway_result_t discover_new_meters(meter_discovery_config_t *config) {
    wmbus_gateway_result_t result = {0};

    for (int i = 0; i < sizeof(config->discovery_channels); i++) {
        uint8_t channel = config->discovery_channels[i];
        if (channel == 0) break;

        // 切换到发现信道
        wmbus_set_channel(channel);

        // 监听表计广播
        wmbus_frame_t frame;
        if (wmbus_receive_frame(&frame, config->discovery_timeout_s * 1000) == WMBUS_OK) {
            // 解析表计信息
            meter_info_t meter_info;
            if (parse_meter_info(&frame, &meter_info) == WMBUS_OK) {
                // 添加到数据库
                add_meter_to_database(&meter_info);
                result.meters_discovered++;
            }
        }
    }

    result.success = true;
    return result;
}
```

#### 3. 数据收集与转发

```c
// 数据收集任务
void gateway_data_collection_task(void *parameters) {
    wmbus_gateway_context_t *ctx = (wmbus_gateway_context_t *)parameters;

    while (ctx->running) {
        // 遍历所有注册的表计
        for (int i = 0; i < ctx->meter_count; i++) {
            meter_info_t *meter = &ctx->meters[i];

            // 检查是否需要读取数据
            if (is_data_collection_due(meter)) {
                // 发送读取命令 (双向模式)
                if (meter->supports_bidirectional) {
                    wmbus_command_t read_cmd = {
                        .command_type = WMBUS_CMD_READ_DATA,
                        .target_address = meter->address,
                        .security_mode = meter->security_mode
                    };

                    wmbus_response_t response;
                    if (wmbus_send_command(&read_cmd, &response, 5000) == WMBUS_OK) {
                        // 处理响应数据
                        process_meter_data(meter, &response.data);

                        // 转发到数据中心
                        forward_data_to_datacenter(meter, &response.data);
                    }
                } else {
                    // 单向模式：等待表计主动发送
                    wait_for_meter_transmission(meter);
                }
            }
        }

        // 等待下一个收集周期
        vTaskDelay(pdMS_TO_TICKS(ctx->collection_interval_ms));
    }
}
```

#### 4. 远程控制功能

```c
// 远程控制命令处理
typedef enum {
    REMOTE_CMD_READ_DATA,           // 读取数据
    REMOTE_CMD_SET_PARAMETER,       // 设置参数
    REMOTE_CMD_FIRMWARE_UPDATE,     // 固件升级
    REMOTE_CMD_VALVE_CONTROL,       // 阀门控制
    REMOTE_CMD_RESET_DEVICE,        // 设备重置
    REMOTE_CMD_SYNC_TIME           // 时间同步
} remote_command_type_t;

// 处理远程控制命令
wmbus_gateway_result_t handle_remote_command(remote_command_t *cmd) {
    wmbus_gateway_result_t result = {0};

    // 查找目标表计
    meter_info_t *target_meter = find_meter_by_address(cmd->target_address);
    if (!target_meter) {
        result.error_code = GATEWAY_ERROR_METER_NOT_FOUND;
        return result;
    }

    // 检查表计是否支持双向通信
    if (!target_meter->supports_bidirectional) {
        result.error_code = GATEWAY_ERROR_UNIDIRECTIONAL_METER;
        return result;
    }

    switch (cmd->command_type) {
        case REMOTE_CMD_VALVE_CONTROL:
            result = execute_valve_control(target_meter, cmd->parameters);
            break;

        case REMOTE_CMD_SET_PARAMETER:
            result = execute_parameter_setting(target_meter, cmd->parameters);
            break;

        case REMOTE_CMD_FIRMWARE_UPDATE:
            result = execute_firmware_update(target_meter, cmd->parameters);
            break;

        default:
            result.error_code = GATEWAY_ERROR_UNSUPPORTED_COMMAND;
            break;
    }

    // 记录命令执行结果
    log_command_execution(cmd, &result);

    return result;
}
```

### 网关安全机制

#### 1. 端到端加密

```c
// 网关安全配置
typedef struct {
    // wM-Bus层安全
    struct {
        uint8_t aes_key[16];              // AES-128密钥
        uint32_t message_counter;         // 消息计数器
        security_profile_t profile;       // 安全配置文件
        bool key_rotation_enabled;        // 密钥轮换
    } wmbus_security;

    // 网络层安全
    struct {
        bool tls_enabled;                 // TLS加密
        char certificate_path[256];       // 证书路径
        char private_key_path[256];       // 私钥路径
        uint16_t tls_version;            // TLS版本
    } network_security;

    // 访问控制
    struct {
        bool authentication_required;     // 需要身份认证
        uint32_t session_timeout_s;      // 会话超时
        uint8_t max_failed_attempts;     // 最大失败次数
    } access_control;
} gateway_security_config_t;
```

#### 2. 密钥管理

```c
// 密钥管理系统
typedef struct {
    uint8_t master_key[16];           // 主密钥
    uint8_t transport_key[16];        // 传输密钥
    uint32_t key_version;             // 密钥版本
    uint64_t key_expiry_time;         // 密钥过期时间
    bool key_update_pending;          // 密钥更新待处理
} meter_key_info_t;

// 密钥轮换流程
wmbus_gateway_result_t rotate_meter_keys(uint32_t meter_address) {
    wmbus_gateway_result_t result = {0};

    // 1. 生成新密钥
    uint8_t new_key[16];
    if (generate_random_key(new_key, sizeof(new_key)) != WMBUS_OK) {
        result.error_code = GATEWAY_ERROR_KEY_GENERATION;
        return result;
    }

    // 2. 通过安全信道发送新密钥
    wmbus_command_t key_update_cmd = {
        .command_type = WMBUS_CMD_KEY_UPDATE,
        .target_address = meter_address,
        .data = new_key,
        .data_length = sizeof(new_key),
        .security_mode = SECURITY_MODE_7  // 使用当前密钥加密
    };

    wmbus_response_t response;
    if (wmbus_send_command(&key_update_cmd, &response, 10000) == WMBUS_OK) {
        // 3. 更新本地密钥存储
        update_meter_key(meter_address, new_key);
        result.success = true;
    } else {
        result.error_code = GATEWAY_ERROR_KEY_UPDATE_FAILED;
    }

    return result;
}
```

### 网关性能优化

#### 1. 多信道并发处理

```c
// 多信道管理
typedef struct {
    uint8_t channel_count;            // 信道数量
    wmbus_channel_t channels[8];      // 信道配置
    bool channel_active[8];           // 信道活跃状态
    uint32_t channel_load[8];         // 信道负载
} multi_channel_manager_t;

// 信道负载均衡
uint8_t select_optimal_channel(multi_channel_manager_t *manager) {
    uint8_t best_channel = 0;
    uint32_t min_load = UINT32_MAX;

    for (int i = 0; i < manager->channel_count; i++) {
        if (manager->channel_active[i] && manager->channel_load[i] < min_load) {
            min_load = manager->channel_load[i];
            best_channel = i;
        }
    }

    return best_channel;
}
```

#### 2. 数据缓存与同步

```c
// 数据缓存管理
typedef struct {
    data_record_t *records;           // 数据记录数组
    uint32_t record_count;            // 记录数量
    uint32_t max_records;             // 最大记录数
    uint64_t oldest_timestamp;        // 最旧记录时间戳
    uint64_t newest_timestamp;        // 最新记录时间戳
    bool sync_pending;                // 同步待处理
} data_cache_t;

// 数据同步任务
void gateway_data_sync_task(void *parameters) {
    wmbus_gateway_context_t *ctx = (wmbus_gateway_context_t *)parameters;

    while (ctx->running) {
        if (ctx->cache.sync_pending || is_sync_interval_due()) {
            // 同步缓存数据到数据中心
            sync_result_t result = sync_data_to_datacenter(&ctx->cache);

            if (result.success) {
                // 清理已同步的数据
                cleanup_synced_data(&ctx->cache, result.synced_count);
                ctx->cache.sync_pending = false;
            } else {
                // 同步失败，等待重试
                vTaskDelay(pdMS_TO_TICKS(ctx->sync_retry_interval_ms));
            }
        }

        vTaskDelay(pdMS_TO_TICKS(ctx->sync_check_interval_ms));
    }
}
```

### 网关部署指南

#### 1. 硬件要求

| 组件 | 最低要求 | 推荐配置 | 说明 |
|------|----------|----------|------|
| **CPU** | ARM Cortex-M4 | ARM Cortex-A7 | 协议处理和数据转换 |
| **内存** | 512KB RAM | 2MB RAM | 数据缓存和协议栈 |
| **存储** | 2MB Flash | 8MB Flash | 固件和配置存储 |
| **射频** | 868MHz收发器 | 双频段收发器 | wM-Bus通信 |
| **网络** | 以太网/4G | 以太网+4G+WiFi | 上行通信冗余 |

#### 2. 软件架构

```c
// 网关软件架构
typedef struct {
    // 系统层
    rtos_kernel_t *kernel;            // 实时操作系统
    task_scheduler_t *scheduler;      // 任务调度器
    memory_manager_t *memory_mgr;     // 内存管理器

    // 协议层
    wmbus_stack_t *wmbus_stack;       // wM-Bus协议栈
    tcp_ip_stack_t *tcpip_stack;      // TCP/IP协议栈
    security_engine_t *security;      // 安全引擎

    // 应用层
    data_collector_t *collector;      // 数据收集器
    command_processor_t *cmd_proc;    // 命令处理器
    protocol_converter_t *converter;  // 协议转换器

    // 管理层
    device_manager_t *dev_mgr;        // 设备管理器
    config_manager_t *config_mgr;     // 配置管理器
    log_manager_t *log_mgr;          // 日志管理器
} gateway_software_architecture_t;
```

#### 3. 部署检查清单

- [ ] **硬件连接**: 天线、电源、网络接口
- [ ] **射频测试**: 发射功率、接收灵敏度、频率准确度
- [ ] **网络配置**: IP地址、路由、防火墙设置
- [ ] **安全配置**: 证书安装、密钥配置、访问控制
- [ ] **表计注册**: 设备发现、地址分配、密钥分发
- [ ] **数据验证**: 数据收集、格式转换、上传测试
- [ ] **远程控制**: 命令下发、响应验证、错误处理
- [ ] **监控告警**: 状态监控、故障告警、性能统计

---

## 实际应用示例 (基于EN 13757-4真实标准)

### 标准帧示例 (基于EN 13757-4 Annex C)

#### Mode T1帧示例 (基于标准C.2节)
```
条件:
- 制造商: "CEN" (编码为0CAEh)
- 设备ID: 12345678h, 版本: 01h, 类型: 07h (水表)
- 数据: 876543升水量
- C字段: 44h (SND/NO REPLY, 表计主动发送)

帧结构 (Frame Format A):
第一块: 0F 44 AE 0C 78 56 34 12 01 07 [CRC1] [CRC2]
第二块: 78 0B 13 43 65 87 [CRC1] [CRC2]

字段解析:
- L字段: 0Fh (15字节数据长度)
- C字段: 44h (SND-NR, 表计主动发送)
- M字段: 0CAEh ("CEN"制造商编码)
- A字段: 12345678h 01h 07h (ID+版本+设备类型)
- CI字段: 78h (应用数据，无头部)
- DIF: 0Bh (6位BCD瞬时值)
- VIF: 13h (体积，升)
- 数据: 876543h (BCD编码的876543升)
```

#### Mode C1帧示例 (基于标准C.3节)
```
条件:
- 使用Frame Format B
- 扩展链路层 CI=8Ch
- 同步帧 CC=20h, ACC=27h

帧结构 (Frame Format B, 单块):
14 44 AE 0C 78 56 34 12 01 07 8C 20 27 78 0B 13 43 65 87 [CRC1] [CRC2]

字段解析:
- L字段: 14h (20字节总长度)
- C字段: 44h (SND-NR)
- M字段: 0CAEh ("CEN")
- A字段: 12345678h 01h 07h
- CI字段: 8Ch (扩展链路层)
- CC字段: 20h (同步帧标识)
- ACC字段: 27h (访问号)
- CI字段: 78h (应用数据)
- 数据: 0B 13 43 65 87 (同上)
- CRC: 7AC5h (计算得出)
```

### P2通信流程示例 (基于P2标准§4.3)

#### P2安装流程 (P2标准§4.3.1和§4.3.9)
```
1. M-Bus设备发送安装请求 (SND_IR):
   46h [M-field] [A-field] 8Ch 80h [ACC] 7Ah [AC] [ST] 00h [数据] [CRC]

2. E-meter确认安装 (CNF_IR):
   06h [M-field] [A-field] 80h [目标ID] [目标MAN] [目标VER] [目标DEV] [AC] [ST] 00h [CRC]

3. M-Bus设备设置绑定标志，启用频繁访问周期
```

#### P2小时数据消息 (P2标准§4.3.2)
```
正常小时消息 (除00:00外每小时):
44h [M-field] [A-field] 8Ch 24h [ACC] 7Ah [AC] [ST] 01h 29h [LE] 00h [CTR] [加密数据] [GCM标签] [CRC]

每日消息 (00:00 UTC):
44h [M-field] [A-field] 8Ch 24h [ACC] 7Ah [AC] [ST] 01h 29h [LE] 00h [CTR] [扩展加密数据] [GCM标签] [CRC]
```

#### P2五分钟数据消息 (P2标准§4.3.3)
```
Mode C1传输 (Frame Format B, 52字节+前导码):
44h [M-field] [A-field] 01h 29h [LE] 00h [CTR] [体积索引+时间戳] [GCM标签]

特点:
- 使用Mode C1传输
- 安全模式9加密和认证
- 总包长度60字节 (480位)
- 仅传输体积索引和时间戳
```

#### P2控制消息流程 (P2标准§4.3.4-4.3.5)
```
1. E-meter发送控制命令 (SND_UD):
   53h [E-meter地址] 8Ah [目标设备] [AC] [ST] 01h 29h [LE] 00h [CTR] [加密命令] [GCM标签] [CRC]

2. M-Bus设备确认 (ACK):
   00h [M-Bus地址] 8Ch 80h [ACC] 8Ah [AC] [ST] 01h 29h 00h 00h [CTR] [GCM标签] [CRC]

3. E-meter请求数据 (SND_UD2):
   43h [E-meter地址] 5Bh [目标设备] [AC] [ST] 01h 29h [LE] 00h [CTR] [加密请求] [GCM标签] [CRC]

4. M-Bus设备响应 (RSP_UD):
   08h [M-Bus地址] 8Ch 80h [ACC] 7Ah [AC] [ST] 01h 29h [LE] 00h [CTR] [加密数据] [GCM标签] [CRC]
```

#### P2时钟同步 (P2标准§4.3.6)
```
用户密钥未设置时 (Security Mode 0):
53h [E-meter地址] 64h [目标设备] [AC] [ST] 00h [时钟数据] [CRC]

用户密钥已设置时 (Security Mode 9):
53h [E-meter地址] 64h [目标设备] [AC] [ST] 01h 29h [LE] 00h [CTR] [加密时钟数据] [GCM标签] [CRC]
```

### 水表数据示例

#### 原始数据帧
```
68 2E 44 B8 06 12 34 56 78 01 07 7A 2A 00 00 00 
2F 2F 0C 14 27 04 85 02 04 6D 32 37 1F 15 02 FD 
17 00 00 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 2F 16
```

#### 解析结果
- **帧头**: 68 (起始)
- **长度**: 2E (46字节)
- **控制**: 06 (CNF-IR)
- **制造商ID**: 1234 (0x3412)
- **设备ID**: 78563412 (BCD格式)
- **版本**: 01
- **设备类型**: 07 (水表)
- **CI字段**: 7A (短传输层)
- **数据记录1**: 体积测量
- **数据记录2**: 时间戳
- **帧尾**: 16 (结束)

### 配置示例

#### 设备配置参数
```c
// T1模式配置
wmbus_config_t config = {
    .mode = WMBUS_MODE_T1,
    .frequency = 868950000,  // 868.95 MHz
    .power = 14,             // 14 dBm
    .interval = 3600,        // 1小时发送间隔
    .encryption = MODE_5,    // AES-128加密
};
```

#### 数据记录配置
```c
// 体积数据记录
data_record_t volume_record = {
    .dif = 0x0C,            // 8位BCD
    .vif = 0x13,            // 体积(升)
    .storage_number = 0,     // 当前值
    .data_length = 4,
};
```

---

## 总结

wM-Bus协议是一个完整的无线计量通信解决方案，具有以下优势：

### 技术优势
- **多模式支持**: 适应不同应用场景
- **标准化**: 确保设备互操作性
- **低功耗**: 支持长期电池供电
- **安全性**: 提供加密和认证机制

### 应用价值
- **降低成本**: 减少人工抄表成本
- **提高效率**: 实现远程自动抄表
- **数据准确**: 避免人工读数错误
- **系统集成**: 便于集成到智能系统

### 发展趋势
- **IoT集成**: 与物联网平台集成
- **云端服务**: 数据云端处理和分析
- **AI应用**: 智能数据分析和预测
- **标准演进**: 持续的标准更新和完善

---

## 附录A: 技术参数对照表

### 详细模式参数表 (基于EN 13757-4标准)

| 模式 | 子模式 | 接收灵敏度<br/>(低速率) | 接收灵敏度<br/>(中等级别) | 接收灵敏度<br/>(高等级别) | Chip码率(kbps) | Chip码率容差 | 数据位置大容差 | 前导码长度 | 定号码长度 |
|------|--------|------------|------------|------------|------------|------------|------------|------------|------------|
| **S** | S1 | -80dBm | -90dBm | -105dBm | 32.768<br/>DRate:Chip/2 | +-1.5% | +-3uS | 576chips | 2*8 chips |
| **S** | S1-m | -80dBm | -90dBm | -105dBm | 32.768<br/>DRate:Chip/2 | +-1.5% | +-3uS | 48chips | 2*8 chips |
| **S** | S2 | -80dBm | -90dBm | -105dBm | 32.768<br/>DRate:Chip/2 | +-1.5% | +-3uS | 48chips(可选择7%) | 2*8 chips |
| **T** | T1 | -80dBm | -90dBm | -105dBm | 100<br/>DRate:Chip*2/3 | T: +-1% | +-3uS | 48chips | 2*8 chips |
| **T** | T2 | -80dBm | -90dBm | -105dBm | T:100 DRate:Chip*2/3<br/>R:32.768 DRate:Chip/2 | T: +-1%<br/>R: +-1.5% | +-3uS | 48chips | 2*8 chips |
| **R** | R2 | -80dBm | -90dBm | -110dBm | 4.8 DRate:Chip/2 | +-1.5% | +-15uS | 90chips | 2*8 chips |
| **C** | C1 | -80dBm | -90dBm | -105dBm | 100 DRate:Chip | +-100ppm | -- | 32chips | -- |
| **C** | C2 | -80dBm | -90dBm | -105dBm | T:100 R:50<br/>DRate:Chip | +-100ppm | -- | 32chips | -- |
| **N** | N1a-f | -90dBm | -100dBm | -123dBm @ 2.4kbps<br/>-120dBm @ 4.8kbps | 2.4 or 4.8 | +-100ppm | -- | 10bits | 0 |
| **N** | N2a-f | -90dBm | -100dBm | -123dBm @ 2.4kbps<br/>-120dBm @ 4.8kbps | 2.4 or 4.8 | +-100ppm | -- | 10bits | 0 |
| **N** | N2g | -90dBm | -100dBm | -107dBm @ 19.2kbps | 9.6(19.2) | +-100ppm | -- | 10bits | 0 |
| **F** | F2-m | -105dBm | -110dBm | -117dBm | 2.4 | +-100ppm | -- | 39bits | -- |
| **F** | F2 | -105dBm | -110dBm | -117dBm | 2.4 | +-100ppm | -- | 39bits | -- |

### 详细模式对比表

#### 基本参数对比
| 模式 | 子模式 | 频率(MHz) | 数据速率(kbps) | 功率(mW) | 传输方向 | 应用场景 |
|------|--------|-----------|----------------|----------|----------|----------|
| **T** | T1 | 868.95 | 66.67 | ≤25 | 单向 | 定期抄表 |
| **T** | T2 | 868.95 (M→O), 868.3 (O→M) | 66.67/16.384 | ≤25 | 双向 | 远程控制 |
| **C** | C1 | 868.95 | 100 | ≤25 | 单向 | 频繁读表 |
| **C** | C2 | 868.95 | 100/50 | ≤25 | 双向 | 实时控制 |
| **S** | S1 | 868.3 | 16.384 | ≤25 | 双向 | 长距离固定 |
| **S** | S1-m | 868.3 | 16.384 | ≤25 | 双向 | 移动设备 |
| **S** | S2 | 868.3 | 16.384 | ≤25 | 双向 | 特殊应用 |
| **N** | N1a-f | 169.4-169.475 | 2.4 | ≤500 | 双向 | 超长距离 |
| **N** | N2a-f | 169.4-169.475 | 4.8 | ≤500 | 双向 | 中长距离 |
| **N** | N2g | 169.4-169.475 | 19.2 | ≤500 | 双向 | 高速率 |
| **R** | R2 | 868.33 | 2.4 | ≤25 | 双向 | 地下/遮挡 |
| **F** | F2 | 433.82 | 2.4 | ≤10 | 双向 | 全球ISM |
| **F** | F2-m | 433.82 | 2.4 | ≤10 | 双向 | 移动全球 |

#### 法规参数对比
| 模式 | 子模式 | 占空比限制 | 最大突发时间 | 接收灵敏度(dBm) | 前导码长度 | 特殊要求 |
|------|--------|------------|--------------|-----------------|------------|----------|
| **T** | T1 | ≤1% (1h) | ~100ms | -105 | 48chips | 定期发送 |
| **T** | T2 | ≤10% (1h) | ≤100ms | -105 | 48chips | 监听窗口≤500ms |
| **C** | C1 | ≤0.1% | <22ms | -105 | 32chips | 短数据突发 |
| **C** | C2 | ≤100% | ≤100ms | -105 | 32chips | 连续接收 |
| **S** | S1 | ≤1% (1h) | ≤100ms | -105 | 576chips | 最长前导码 |
| **S** | S1-m | ≤1% (1h) | ≤100ms | -105 | 48chips | 移动模式 |
| **S** | S2 | ≤1% (1h) | ≤100ms | -105 | 48chips | 宽容差±7% |
| **N** | N1a-f | ≤0.1% (1h) | ≤1000ms | -123 | 10bits | 超高灵敏度 |
| **N** | N2a-f | ≤1% (1h) | ≤500ms | -120 | 10bits | 高灵敏度 |
| **N** | N2g | ≤10% (1h) | ≤100ms | -107 | 10bits | 高速率 |
| **R** | R2 | ≤10% (1h) | ≤500ms | -110 | 90chips | 大容差±15μS |
| **F** | F2 | ≤10% (1h) | ≤100ms | -117 | 39bits | 全球ISM |
| **F** | F2-m | ≤5% (1h) | ≤60ms | -115 | 39bits | 移动节能 |

#### 应用场景参考
| 应用场景 | 适用模式 | 理由 |
|----------|----------|------|
| **住宅水表** | T1, C1 | 低功耗，定期抄表 |
| **商业电表** | T2, C2 | 双向控制，实时监控 |
| **地下管网** | R2 | 高灵敏度，穿透能力强 |
| **农村地区** | N1a-f | 超长距离，高功率 |
| **移动设备** | S1-m, F2-m | 移动模式，快速同步 |
| **全球部署** | F2, F2-m | 433MHz全球ISM频段 |
| **高密度城区** | N2g, C2 | 高数据率，实时性 |
| **电池供电** | T1, C1, F2 | 超低占空比，长电池寿命 |

### 模式选择决策流程

#### 决策树
```
开始选择wM-Bus模式
│
├─ 是否需要全球部署？
│  ├─ 是 → F2/F2-m (433MHz ISM)
│  └─ 否 → 继续
│
├─ 传输距离要求？
│  ├─ 超长距离(>5km) → N1a-f (169MHz, 高功率)
│  ├─ 长距离(1-5km) → N2a-f, R2
│  └─ 中短距离(<1km) → T, C, S模式
│
├─ 功耗要求？
│  ├─ 超低功耗 → C1 (0.1%占空比)
│  ├─ 低功耗 → T1, F2 (1%占空比)
│  └─ 中等功耗 → T2, C2, S, R2 (10%占空比)
│
├─ 通信方向？
│  ├─ 仅发送 → T1, C1
│  └─ 双向 → T2, C2, S, N, R2, F2
│
├─ 数据速率要求？
│  ├─ 高速率(>10kbps) → C1/C2 (100kbps), N2g (19.2kbps)
│  ├─ 中速率(10-50kbps) → T1/T2 (66.67kbps), S (16.384kbps)
│  └─ 低速率(<10kbps) → N1/N2 (2.4/4.8kbps), R2/F2 (2.4kbps)
│
└─ 特殊环境？
   ├─ 地下/遮挡 → R2 (高灵敏度-110dBm)
   ├─ 移动应用 → S1-m, F2-m
   └─ 固定应用 → S1 (长前导码)
```

#### 实用选择代码
```c
// 模式选择辅助函数
typedef enum {
    WMBUS_MODE_T1, WMBUS_MODE_T2,
    WMBUS_MODE_C1, WMBUS_MODE_C2,
    WMBUS_MODE_S1, WMBUS_MODE_S1M, WMBUS_MODE_S2,
    WMBUS_MODE_N1, WMBUS_MODE_N2, WMBUS_MODE_N2G,
    WMBUS_MODE_R2,
    WMBUS_MODE_F2, WMBUS_MODE_F2M
} wmbus_mode_t;

typedef struct {
    bool global_deployment;        // 全球部署需求
    uint32_t max_distance_m;      // 最大传输距离(米)
    float max_power_consumption;  // 最大功耗要求
    bool bidirectional_required;  // 是否需要双向通信
    uint32_t min_data_rate_bps;   // 最小数据速率要求
    bool underground_deployment;  // 地下部署
    bool mobile_device;           // 移动设备
} application_requirements_t;

wmbus_mode_t select_optimal_mode(application_requirements_t* req) {
    // 全球部署优先选择F模式
    if (req->global_deployment) {
        return req->mobile_device ? WMBUS_MODE_F2M : WMBUS_MODE_F2;
    }

    // 超长距离选择N模式
    if (req->max_distance_m > 5000) {
        if (req->min_data_rate_bps > 10000) {
            return WMBUS_MODE_N2G;
        } else if (req->min_data_rate_bps > 3000) {
            return WMBUS_MODE_N2;
        } else {
            return WMBUS_MODE_N1;
        }
    }

    // 地下/遮挡环境选择R模式
    if (req->underground_deployment) {
        return WMBUS_MODE_R2;
    }

    // 超低功耗要求
    if (req->max_power_consumption < 0.001) {  // <0.1%占空比
        return WMBUS_MODE_C1;
    }

    // 高数据速率要求
    if (req->min_data_rate_bps > 50000) {
        return req->bidirectional_required ? WMBUS_MODE_C2 : WMBUS_MODE_C1;
    }

    // 移动设备
    if (req->mobile_device) {
        return WMBUS_MODE_S1M;
    }

    // 默认选择T模式
    return req->bidirectional_required ? WMBUS_MODE_T2 : WMBUS_MODE_T1;
}
```
| 主要应用 | 定期抄表 | 实时控制 | 固定设备 | 农村地区 | 地下设备 | 便携设备 |

### 技术参数详解

#### 接收灵敏度等级说明
- **低速率级别**: 基本接收要求，适用于近距离通信
- **中等级别**: 标准接收要求，适用于一般应用场景
- **高等级别**: 高灵敏度要求，适用于远距离或恶劣环境

#### Chip码率与数据速率关系
- **DRate:Chip/2**: 数据速率 = Chip码率 ÷ 2 (如S模式: 32.768kbps ÷ 2 = 16.384kbps数据速率)
- **DRate:Chip*2/3**: 数据速率 = Chip码率 × 2/3 (如T模式: 100kbps × 2/3 = 66.67kbps数据速率)
- **DRate:Chip**: 数据速率 = Chip码率 (如C模式: 100kbps = 100kbps数据速率)

#### 码率容差说明
- **±1%**: 高精度要求，适用于T模式
- **±1.5%**: 标准精度要求，适用于S/R模式
- **±100ppm**: 超高精度要求，适用于C/N/F模式 (0.01%)

#### 前导码长度对比
- **10bits**: N模式，最短前导码
- **32chips**: C模式，中等长度
- **39bits**: F模式，中等长度
- **48chips**: T/S/R模式，标准长度
- **90chips**: R2模式，较长前导码
- **576chips**: S1模式，最长前导码

#### 定号码长度
- **2*8 chips**: 大多数模式使用16chips定号码
- **0**: N模式不使用定号码
- **--**: C/F模式定号码长度未指定

### 前导码对比表 (基于EN 13757-4)

| 模式 | 前导码要求 | 具体格式 | 总长度 | 特殊说明 |
|------|------------|----------|--------|----------|
| **T模式** | ≥19chips | n×(01) 0000111101 | ≥19chips | 标准要求 |
| **C模式** | 64chips | 16×(01) 01010100011101 01010100 11001101₂<br/>或 16×(01) 01010100011101 01010100 00111101₂ | 64chips | 32前导+32同步 |
| **S模式** | 48-576chips | 标准帧头+同步字 | 可变长度 | S1=576chips, S1-m/S2=48chips |
| **N模式** | 10bits | 标准帧头+同步字 | 10bits | 最短前导码 |
| **R模式** | ≥39chips | n×(01) 000111011010010110 | ≥39chips | Manchester编码 |
| **F模式** | 39bits | 标准帧头+同步字 | 39bits | 标准要求 |

#### 前导码功能说明
- **标准前导码**: 用于基本的帧同步和时钟恢复
- **C模式前导码(64chips)**:
  - 32 chips前导码用于时钟恢复
  - 32 chips同步字用于帧检测
  - 支持T模式兼容性检测
  - 两种帧格式(A/B)选择机制
- **可变长度前导码**: S模式根据子模式有不同的前导码长度要求

### 数据速率与距离关系

```
传输距离 (m)
    ↑
5000│     N模式
    │    ╱
4000│   ╱
    │  ╱
3000│ ╱
    │╱
2000├─────────── R模式
    │
1000├─── T/C/S/F模式
    │
   0└─────────────────→ 数据速率 (kbps)
    0   10   50  100
```

---

## 附录B: 错误码和故障排除

### 常见错误码

| 错误码 | 名称 | 说明 | 解决方案 |
|--------|------|------|----------|
| 0x01 | CRC_ERROR | CRC校验失败 | 检查射频质量，重传 |
| 0x02 | LENGTH_ERROR | 长度字段错误 | 检查帧格式 |
| 0x03 | ADDRESS_ERROR | 地址不匹配 | 验证设备地址配置 |
| 0x04 | COMMAND_ERROR | 不支持的命令 | 检查命令兼容性 |
| 0x05 | TIMEOUT_ERROR | 响应超时 | 检查设备状态和距离 |
| 0x06 | ENCRYPTION_ERROR | 加密错误 | 验证密钥配置 |

### 故障排除流程

#### 通信故障
```
通信失败
    ↓
检查射频参数
    ↓
检查设备地址
    ↓
检查加密配置
    ↓
检查网络拓扑
    ↓
分析错误日志
```



---

## 附录C: 开发指南

### 硬件设计要点

#### 射频前端设计
```
MCU ←→ RF芯片 ←→ 匹配网络 ←→ 天线
```

#### 关键参数
- **灵敏度**: ≥-110 dBm
- **发射功率**: 可调节0-25mW
- **频率稳定性**: ±10 ppm
- **谐波抑制**: ≥40 dB

#### 天线设计
- **类型**: 单极子天线、PCB天线
- **增益**: 0-2 dBi
- **阻抗**: 50Ω
- **带宽**: 覆盖工作频段

### 软件架构

#### 协议栈结构
```c
// 应用层
typedef struct {
    uint8_t device_type;
    uint32_t device_id;
    data_record_t records[MAX_RECORDS];
} application_data_t;

// 传输层
typedef struct {
    uint8_t ci_field;
    uint8_t access_number;
    uint8_t status;
    uint16_t config_word;
} transport_header_t;

// 数据链路层
typedef struct {
    uint8_t start_char;
    uint8_t length;
    uint8_t control;
    address_t address;
    uint8_t* payload;
    uint8_t end_char;
} dll_frame_t;
```

#### 状态机实现
```c
typedef enum {
    STATE_IDLE,
    STATE_TX_PREPARE,
    STATE_TX_SENDING,
    STATE_RX_LISTENING,
    STATE_RX_PROCESSING,
    STATE_ERROR
} wmbus_state_t;

void wmbus_state_machine(wmbus_context_t* ctx) {
    switch(ctx->state) {
        case STATE_IDLE:
            handle_idle_state(ctx);
            break;
        case STATE_TX_PREPARE:
            handle_tx_prepare(ctx);
            break;
        // ... 其他状态处理
    }
}
```

### 测试和验证

#### 单元测试
```c
// DIF/VIF解析测试
void test_dif_vif_parsing() {
    uint8_t test_data[] = {0x0C, 0x13, 0x39, 0x30, 0x00, 0x00};
    data_record_t record = parse_data_record(test_data, sizeof(test_data));

    assert(record.dif_info.data_length == 4);
    assert(record.vif_info.multiplier == 0.001);
    assert(record.scaled_value == 12.345);
}
```

#### 集成测试
- **射频测试**: 功率、频率、调制质量
- **协议测试**: 帧格式、时序、错误处理
- **互操作测试**: 与其他厂商设备通信
- **性能测试**: 距离、功耗、可靠性

#### 认证测试
- **CE认证**: 欧洲市场准入
- **FCC认证**: 美国市场准入
- **OMS认证**: 互操作性认证
- **能效认证**: 能源效率标准

---

## 附录D: 实际部署案例

### 案例1: 住宅小区智能水表系统

#### 系统架构
```
水表(T1模式) ──→ 集中器 ──→ 数据中心
     │            │          │
   定期发送      数据汇聚    数据处理
```

#### 技术参数
- **设备数量**: 500个水表
- **通信模式**: T1模式
- **发送间隔**: 4小时
- **电池寿命**: 10年
- **通信距离**: 100-500米

#### 部署经验
1. **网络规划**: 合理布置集中器位置
2. **功率调整**: 根据距离调整发射功率
3. **时间同步**: 避免同时发送造成冲突
4. **维护策略**: 定期检查电池状态

### 案例2: 工业园区多表集抄系统

#### 系统特点
- **多种表计**: 水表、电表、气表、热表
- **通信模式**: C模式双向通信
- **实时监控**: 支持远程控制和配置
- **数据加密**: 使用AES-128加密

#### 关键技术
```c
// 多表类型支持
typedef enum {
    METER_TYPE_WATER = 0x07,
    METER_TYPE_GAS = 0x03,
    METER_TYPE_ELECTRIC = 0x02,
    METER_TYPE_HEAT = 0x04
} meter_type_t;

// 统一数据格式
typedef struct {
    meter_type_t type;
    uint32_t meter_id;
    float current_value;
    float flow_rate;
    timestamp_t last_reading;
} meter_data_t;
```

### 案例3: 农村地区远程抄表

#### 挑战和解决方案
- **距离远**: 使用N模式长距离传输
- **地形复杂**: 部署中继器扩展覆盖
- **维护困难**: 提高设备可靠性
- **成本控制**: 系统架构考虑

#### 网络拓扑
```
表计 ──→ 中继器 ──→ 集中器 ──→ 数据中心
  │       │         │         │
N模式   信号放大   数据汇聚   远程监控
```

---

## 附录E: P2 Companion Standard合规性检查清单

### P2设备合规性要求 (基于P2标准v5.0 revision 5.9)

#### P2强制性要求检查
```c
// P2合规性检查项目
typedef struct {
    char* category;
    char* requirement;
    bool mandatory;
    char* p2_reference;
    bool (*check_function)(void);
} p2_compliance_check_t;

static const p2_compliance_check_t p2_checks[] = {
    // 帧格式要求
    {
        .category = "Frame Format",
        .requirement = "使用FT3帧格式 (基于EN 60870-5-1)",
        .mandatory = true,
        .p2_reference = "P2标准§3.1",
        .check_function = check_ft3_frame_format
    },
    {
        .category = "Frame Format",
        .requirement = "支持允许的C字段值，禁止REQ_UD1/ACC_NR/ACC_DMD",
        .mandatory = true,
        .p2_reference = "P2标准§3.1.2",
        .check_function = check_c_field_restrictions
    },
    {
        .category = "Frame Format",
        .requirement = "最大报文长度255字节",
        .mandatory = true,
        .p2_reference = "P2标准§3.1.1",
        .check_function = check_max_frame_length
    },

    // 安全要求
    {
        .category = "Security",
        .requirement = "强制使用安全模式9 (AES-128 GCM)",
        .mandatory = true,
        .p2_reference = "P2标准§5",
        .check_function = check_security_mode_9
    },
    {
        .category = "Security",
        .requirement = "正确实现调用计数器验证 (最大差值100)",
        .mandatory = true,
        .p2_reference = "P2标准§5.4",
        .check_function = check_invocation_counter
    },
    {
        .category = "Security",
        .requirement = "12字节GCM认证标签",
        .mandatory = true,
        .p2_reference = "P2标准§5.2",
        .check_function = check_gcm_tag_length
    },
    {
        .category = "Security",
        .requirement = "配置字固定值2901h",
        .mandatory = true,
        .p2_reference = "P2标准§5.1",
        .check_function = check_config_word_value
    },

    // 安装和绑定
    {
        .category = "Installation",
        .requirement = "支持安装模式 (SND_IR/CNF_IR)",
        .mandatory = true,
        .p2_reference = "P2标准§3.1.5.1",
        .check_function = check_installation_mode
    },
    {
        .category = "Installation",
        .requirement = "按钮激活安装模式 (3次/天限制)",
        .mandatory = true,
        .p2_reference = "P2标准§3.1.5.1",
        .check_function = check_button_activation
    },
    {
        .category = "Installation",
        .requirement = "绑定后启用频繁访问周期",
        .mandatory = true,
        .p2_reference = "P2标准§3.1.5.1",
        .check_function = check_fac_after_binding
    },

    // 数据传输
    {
        .category = "Data Transmission",
        .requirement = "小时数据消息 (SND_NR)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.2",
        .check_function = check_hourly_data_message
    },
    {
        .category = "Data Transmission",
        .requirement = "5分钟数据 (Mode C1, 52字节)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.3",
        .check_function = check_5min_data_message
    },
    {
        .category = "Data Transmission",
        .requirement = "UTC时间基准 (燃气表)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.2",
        .check_function = check_utc_time_base
    },

    // 控制和响应
    {
        .category = "Control",
        .requirement = "支持时钟同步 (加密/非加密)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.6",
        .check_function = check_clock_sync_support
    },
    {
        .category = "Control",
        .requirement = "支持按需数据请求 (REQ_UD2/RSP_UD)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.7",
        .check_function = check_on_demand_data
    },
    {
        .category = "Control",
        .requirement = "密钥更换支持 (安全模式0)",
        .mandatory = true,
        .p2_reference = "P2标准§4.3.8",
        .check_function = check_key_exchange
    }
};

// P2合规性检查执行函数
bool execute_p2_compliance_check(void) {
    bool all_passed = true;
    int total_checks = sizeof(p2_checks) / sizeof(p2_checks[0]);
    int passed_checks = 0;

    printf("=== P2 Companion Standard v5.0 合规性检查 ===\n\n");

    for (int i = 0; i < total_checks; i++) {
        bool result = p2_checks[i].check_function();

        printf("[%s] %s - %s\n",
               p2_checks[i].category,
               p2_checks[i].requirement,
               result ? "✅ 通过" : "❌ 失败");
        printf("参考: %s\n", p2_checks[i].p2_reference);

        if (result) {
            passed_checks++;
        } else if (p2_checks[i].mandatory) {
            all_passed = false;
            printf("⚠️  强制性要求失败！\n");
        }
        printf("\n");
    }

    printf("=== 检查结果汇总 ===\n");
    printf("总检查项目: %d\n", total_checks);
    printf("通过项目: %d\n", passed_checks);
    printf("通过率: %.1f%%\n", (float)passed_checks / total_checks * 100);
    printf("P2合规性: %s\n", all_passed ? "✅ 符合" : "❌ 不符合");

    return all_passed;
}
```

---

## 附录F: OMS合规性检查清单

### OMS设备合规性要求 (基于OMS-Spec Vol.2 v5.0.1)

#### 必需的安全配置文件支持
```c
// OMS设备必需支持的安全配置文件 (Table 44)
typedef struct {
    char* communication_type;
    oms_security_profile_t required_profiles[4];
    oms_security_profile_t optional_profiles[2];
} oms_compliance_requirement_t;

static const oms_compliance_requirement_t oms_requirements[] = {
    // 无线单向通信
    {
        .communication_type = "Wireless Unidirectional",
        .required_profiles = {OMS_SECURITY_PROFILE_A, OMS_SECURITY_PROFILE_B,
                             OMS_SECURITY_PROFILE_D, 0},
        .optional_profiles = {0, 0}
    },
    // 无线双向通信
    {
        .communication_type = "Wireless Bidirectional",
        .required_profiles = {OMS_SECURITY_PROFILE_A, OMS_SECURITY_PROFILE_B,
                             OMS_SECURITY_PROFILE_D, 0},
        .optional_profiles = {OMS_SECURITY_PROFILE_C, 0}
    },
    // 有线通信
    {
        .communication_type = "Wired Communication",
        .required_profiles = {OMS_SECURITY_PROFILE_NONE, OMS_SECURITY_PROFILE_A,
                             OMS_SECURITY_PROFILE_B, OMS_SECURITY_PROFILE_D},
        .optional_profiles = {OMS_SECURITY_PROFILE_C, 0}
    }
};
```

#### OMS合规性检查清单
```c
// OMS合规性检查项目
typedef struct {
    char* category;
    char* requirement;
    bool mandatory;
    char* reference;
    bool (*check_function)(void);
} oms_compliance_check_t;

static const oms_compliance_check_t oms_checks[] = {
    // 安全要求
    {
        .category = "Security",
        .requirement = "支持至少一种安全配置文件",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 Table 44",
        .check_function = check_security_profile_support
    },
    {
        .category = "Security",
        .requirement = "无线通信必须加密消费数据",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 §9.1",
        .check_function = check_wireless_encryption
    },
    {
        .category = "Security",
        .requirement = "消息计数器正确处理",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 §9.3.2",
        .check_function = check_message_counter
    },

    // 协议要求
    {
        .category = "Protocol",
        .requirement = "支持标准CI字段",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 Table 1",
        .check_function = check_ci_field_support
    },
    {
        .category = "Protocol",
        .requirement = "正确实现AFL层",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 §6",
        .check_function = check_afl_implementation
    },
    {
        .category = "Protocol",
        .requirement = "支持设备类型标识",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 Table 2/3",
        .check_function = check_device_type_support
    },

    // 互操作性要求
    {
        .category = "Interoperability",
        .requirement = "OMS数据点编码",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 Annex B",
        .check_function = check_data_point_encoding
    },
    {
        .category = "Interoperability",
        .requirement = "标准OBIS代码支持",
        .mandatory = true,
        .reference = "OMS-Spec Vol.2 Annex A",
        .check_function = check_obis_code_support
    }
};

// 合规性检查执行函数
bool execute_oms_compliance_check(void) {
    bool all_passed = true;

    for (int i = 0; i < sizeof(oms_checks)/sizeof(oms_checks[0]); i++) {
        bool result = oms_checks[i].check_function();

        printf("检查项目: %s - %s\n", oms_checks[i].category,
               oms_checks[i].requirement);
        printf("结果: %s\n", result ? "通过" : "失败");
        printf("参考: %s\n\n", oms_checks[i].reference);

        if (oms_checks[i].mandatory && !result) {
            all_passed = false;
        }
    }

    return all_passed;
}
```

---

## 附录G: EN 13757-4 测试和验证要求

### 射频测试要求 (EN 13757-4 §11)

#### 必需测试项目
```c
// EN 13757-4 测试规范
typedef struct {
    char test_name[64];
    char test_method[128];
    char acceptance_criteria[128];
    bool mandatory;
} en13757_test_spec_t;

// 射频测试项目
static const en13757_test_spec_t rf_test_specs[] = {
    {
        .test_name = "发射功率测试",
        .test_method = "在标称频率测量平均功率",
        .acceptance_criteria = "在规定功率范围内 ±2dB",
        .mandatory = true
    },
    {
        .test_name = "频率精度测试",
        .test_method = "测量载波频率偏差",
        .acceptance_criteria = "±10ppm (T模式), ±100ppm (N模式)",
        .mandatory = true
    },
    {
        .test_name = "调制精度测试",
        .test_method = "测量FSK频偏精度",
        .acceptance_criteria = "标称频偏 ±5%",
        .mandatory = true
    },
    {
        .test_name = "杂散发射测试",
        .test_method = "测量带外发射功率",
        .acceptance_criteria = "≤ -36dBm (T模式)",
        .mandatory = true
    },
    {
        .test_name = "接收灵敏度测试",
        .test_method = "测量最小可检测信号",
        .acceptance_criteria = "≥ -105dBm (T模式)",
        .mandatory = true
    }
};
```

#### 协议一致性测试
```c
// 协议测试用例
typedef struct {
    char test_case[64];
    char description[256];
    char expected_result[128];
} protocol_test_case_t;

static const protocol_test_case_t protocol_tests[] = {
    {
        .test_case = "帧格式验证",
        .description = "验证L-C-M-A-CI-数据-CRC帧结构",
        .expected_result = "正确解析所有字段"
    },
    {
        .test_case = "CRC校验测试",
        .description = "验证CRC计算和校验功能",
        .expected_result = "正确检测和拒绝错误帧"
    },
    {
        .test_case = "地址匹配测试",
        .description = "验证设备地址过滤功能",
        .expected_result = "只响应匹配地址的帧"
    },
    {
        .test_case = "模式兼容性测试",
        .description = "验证不同传输模式的互操作性",
        .expected_result = "正确处理各模式帧格式"
    }
};
```

### 互操作性测试 (EN 13757-4 §12)

#### 多厂商兼容性验证
- **设备发现**: 验证不同厂商设备的自动发现
- **数据交换**: 验证标准数据格式的正确解析
- **错误处理**: 验证异常情况的统一处理
- **安全功能**: 验证加密和认证的兼容性

#### 现场部署验证
```c
// 现场测试检查清单
typedef struct {
    char item[128];
    bool completed;
    char notes[256];
} field_test_item_t;

static field_test_item_t field_tests[] = {
    {
        .item = "射频覆盖范围测试",
        .completed = false,
        .notes = "在实际部署环境测试通信距离"
    },
    {
        .item = "干扰抗性测试",
        .completed = false,
        .notes = "验证在实际射频环境中的性能"
    },
    {
        .item = "长期稳定性测试",
        .completed = false,
        .notes = "验证长期运行的稳定性和可靠性"
    },
    {
        .item = "功耗验证测试",
        .completed = false,
        .notes = "验证实际功耗符合设计要求"
    }
};
```

---

## 附录H: 标准和规范参考

### 相关标准

#### EN 13757系列标准 (Communication systems for meters)
- **EN 13757-1:2014**: 通信系统总体要求 - 数据交换
- **EN 13757-2:2004**: 物理层和数据链路层 - 有线M-Bus
- **EN 13757-3:2018**: 专用应用层 - 应用协议
- **EN 13757-4:2013**: 无线计量仪表读取 - 射频通信 ⭐
- **EN 13757-5:2008**: 中继器 - 信号中继和放大
- **EN 13757-6:2015**: 本地总线 - 安装和维护

#### EN 13757-4:2013 详细内容
本文档主要基于 **BS EN 13757-4:2013** 标准，该标准定义了：

**第1部分: 总则 (§1-7)**
- 范围和应用领域
- 规范性引用文件
- 术语和定义
- 缩略语和符号
- 一般要求
- 系统概述
- 互操作性要求

**第2部分: 物理层 (§8-10)**
- 射频特性和调制参数
- 频段分配和功率限制
- 发射器和接收器性能要求
- 天线和传播特性
- 射频测试方法

**第3部分: 数据链路层 (§11-13)**
- 帧格式和字段定义
- 错误检测和纠正
- 流控制和确认机制
- 地址管理和过滤
- 协议状态机

**第4部分: 测试和验证 (§14-16)**
- 射频性能测试
- 协议一致性测试
- 互操作性验证
- 现场部署测试
- 认证要求

#### 相关支撑标准
- **EN 60870-5-1**: 传输规程 - 基本应用功能
- **EN 300 220**: 短距离设备射频要求
- **ETSI EN 300 113**: 陆地移动业务设备
- **IEC 62056**: 电力计量数据交换

### OMS规范 (Open Metering System)
- **OMS-Spec Vol.1 General**: OMS总体规范和设备要求
- **OMS-Spec Vol.2 Primary v5.0.1**: 主要通信规范 ⭐
  - M-Bus帧结构和层模型
  - 安全配置文件 (A/B/C/D)
  - 认证和分片层 (AFL)
  - 安全信息传输协议 (SITP)
  - 消息计数器机制
  - TLS支持 (Security Profile C)

#### OMS-Spec Vol.2 v5.0.1 主要章节
- **第2章**: M-Bus帧结构和OMS层模型
- **第3章**: 地址处理 (有线/无线M-Bus)
- **第4-5章**: 物理层和链路层规范
- **第6章**: 认证和分片层 (AFL)
- **第7章**: 传输层 (TPL)
- **第8章**: 应用协议
- **第9章**: 安全机制和密钥管理
- **附录F**: TLS与无线M-Bus (Security Profile C)
- **附录M**: 双向通信用例

### P2 Companion Standard (Enexis智能电表要求)
- **P2 Companion Standard v5.0 revision 5.9**: Enexis智能电表要求 ⭐
- **P2 Frame Format FT3**: 基于EN 60870-5-1的特定帧格式
- **P2 Security Mode 9**: 强制AES-128 GCM加密和认证
- **P2 Installation Mode**: 特定的设备安装和绑定流程
- **P2 Timing Requirements**: 5分钟和小时数据传输要求

#### P2 Companion Standard v5.0 revision 5.9 主要内容
- **第1章**: 总体要求和规范性引用
- **第2章**: 系统架构 (E-meter + M-Bus设备)
- **第3章**: 数据链路层 (FT3帧格式)
- **第4章**: 通信流程 (安装、数据传输、控制)
- **第5章**: 加密层 (Security Mode 9)
- **第6章**: 应用层数据格式
- **附录A**: 消息示例和测试用例
- **附录B**: 实际报文示例

### OMS规范版本
- **OMS Volume 1**: 总体架构和要求
- **OMS Volume 2**: 技术规范
- **OMS Volume 3**: 测试规范
- **OMS Volume 4**: 安全规范

### 认证机构
- **OMS Group**: OMS认证
- **DLMS UA**: 数据模型标准
- **CENELEC**: 欧洲电工标准化委员会
- **ETSI**: 欧洲电信标准协会

### 测试工具
- **wmbusmeters.org**: 在线数据解析工具
- **OMS Test Tool**: 官方测试工具
- **Protocol Analyzer**: 协议分析仪
- **RF Test Equipment**: 射频测试设备

### OBIS代码映射规范 (基于OMS Annex A和EN 13757-3:2018)

#### OBIS代码结构 (基于IEC 62056-61)
```c
// OBIS代码结构 A.B.C.D.E.F
typedef struct {
    uint8_t a;    // 介质 (0=抽象对象, 1=电, 6=热, 7=气体, 8=水)
    uint8_t b;    // 通道 (0=无通道, 1-64=通道号)
    uint8_t c;    // 物理量 (1=有功功率, 2=无功功率, 9=时间等)
    uint8_t d;    // 处理类型 (0=总计, 1-63=费率, 64-127=历史)
    uint8_t e;    // 进一步处理 (0=总计, 1-127=特定处理)
    uint8_t f;    // 存储 (0=当前, 1-99=历史值)
} obis_code_t;

// OMS标准OBIS代码 (基于OMS Annex A)
typedef enum {
    // 通用对象 (A=0)
    OBIS_DEVICE_ID_1        = 0x000060010000,  // 0.0.96.1.0.255 设备ID1
    OBIS_DEVICE_ID_2        = 0x000060020000,  // 0.0.96.2.0.255 设备ID2
    OBIS_MANUFACTURER_ID    = 0x000060030000,  // 0.0.96.3.0.255 制造商ID
    OBIS_PARAMETER_VERSION  = 0x000060040000,  // 0.0.96.4.0.255 参数版本
    OBIS_FIRMWARE_VERSION   = 0x000060050000,  // 0.0.96.5.0.255 固件版本
    OBIS_SOFTWARE_VERSION   = 0x000060060000,  // 0.0.96.6.0.255 软件版本
    OBIS_HARDWARE_VERSION   = 0x000060070000,  // 0.0.96.7.0.255 硬件版本
    OBIS_DEVICE_TYPE        = 0x000060080000,  // 0.0.96.8.0.255 设备类型
    OBIS_FABRICATION_NO     = 0x000060090000,  // 0.0.96.9.0.255 制造编号
    OBIS_ENHANCED_ID        = 0x0000600A0000,  // 0.0.96.10.0.255 增强ID

    // 电能 (A=1)
    OBIS_ACTIVE_ENERGY_TOTAL     = 0x010001080000, // 1.0.1.8.0.255 有功电能总计
    OBIS_ACTIVE_ENERGY_TARIFF1   = 0x010001080100, // 1.0.1.8.1.255 有功电能费率1
    OBIS_ACTIVE_ENERGY_TARIFF2   = 0x010001080200, // 1.0.1.8.2.255 有功电能费率2
    OBIS_REACTIVE_ENERGY_TOTAL   = 0x010002080000, // 1.0.2.8.0.255 无功电能总计
    OBIS_ACTIVE_POWER_TOTAL      = 0x010001070000, // 1.0.1.7.0.255 有功功率总计
    OBIS_REACTIVE_POWER_TOTAL    = 0x010002070000, // 1.0.2.7.0.255 无功功率总计
    OBIS_VOLTAGE_L1              = 0x010020070000, // 1.0.32.7.0.255 L1电压
    OBIS_VOLTAGE_L2              = 0x010034070000, // 1.0.52.7.0.255 L2电压
    OBIS_VOLTAGE_L3              = 0x010048070000, // 1.0.72.7.0.255 L3电压
    OBIS_CURRENT_L1              = 0x01001F070000, // 1.0.31.7.0.255 L1电流
    OBIS_CURRENT_L2              = 0x010033070000, // 1.0.51.7.0.255 L2电流
    OBIS_CURRENT_L3              = 0x010047070000, // 1.0.71.7.0.255 L3电流

    // 热能 (A=6)
    OBIS_HEAT_ENERGY_TOTAL       = 0x060001080000, // 6.0.1.8.0.255 热能总计
    OBIS_HEAT_POWER              = 0x060001070000, // 6.0.1.7.0.255 热功率
    OBIS_HEAT_VOLUME_TOTAL       = 0x060002080000, // 6.0.2.8.0.255 热水体积总计
    OBIS_HEAT_VOLUME_FLOW        = 0x060002070000, // 6.0.2.7.0.255 热水体积流量
    OBIS_HEAT_TEMP_FLOW          = 0x060003070000, // 6.0.3.7.0.255 进水温度
    OBIS_HEAT_TEMP_RETURN        = 0x060004070000, // 6.0.4.7.0.255 回水温度
    OBIS_HEAT_TEMP_DIFF          = 0x060005070000, // 6.0.5.7.0.255 温差

    // 燃气 (A=7)
    OBIS_GAS_VOLUME_TOTAL        = 0x070001080000, // 7.0.1.8.0.255 燃气体积总计
    OBIS_GAS_VOLUME_FLOW         = 0x070001070000, // 7.0.1.7.0.255 燃气体积流量
    OBIS_GAS_TEMPERATURE         = 0x070002070000, // 7.0.2.7.0.255 燃气温度
    OBIS_GAS_PRESSURE            = 0x070003070000, // 7.0.3.7.0.255 燃气压力
    OBIS_GAS_CONVERSION_FACTOR   = 0x070004070000, // 7.0.4.7.0.255 转换因子
    OBIS_GAS_CALORIFIC_VALUE     = 0x070005070000, // 7.0.5.7.0.255 热值

    // 水 (A=8)
    OBIS_WATER_VOLUME_TOTAL      = 0x080001080000, // 8.0.1.8.0.255 水体积总计
    OBIS_WATER_VOLUME_FLOW       = 0x080001070000, // 8.0.1.7.0.255 水体积流量
    OBIS_WATER_TEMPERATURE       = 0x080002070000, // 8.0.2.7.0.255 水温度
    OBIS_WATER_PRESSURE          = 0x080003070000, // 8.0.3.7.0.255 水压力

    // 时间和日期 (A=0, C=9)
    OBIS_CLOCK_TIME              = 0x000009060000, // 0.0.9.6.0.255 时钟时间
    OBIS_CLOCK_DATE              = 0x000009010000, // 0.0.9.1.0.255 时钟日期
    OBIS_CLOCK_DATE_TIME         = 0x000009000000, // 0.0.9.0.0.255 时钟日期时间

    // 状态和事件 (A=0, C=96)
    OBIS_STATUS_WORD             = 0x000060600000, // 0.0.96.96.0.255 状态字
    OBIS_ERROR_FLAGS             = 0x000060610000, // 0.0.96.97.0.255 错误标志
    OBIS_ALARM_FLAGS             = 0x000060620000, // 0.0.96.98.0.255 报警标志
    OBIS_EVENT_COUNTER           = 0x000060630000, // 0.0.96.99.0.255 事件计数器
} oms_obis_codes_t;

// VIF到OBIS映射表
typedef struct {
    uint8_t vif_code;
    uint64_t obis_code;
    char* description;
    char* unit;
    int exponent;
} vif_obis_mapping_t;

// VIF到OBIS映射表 (部分重要映射)
static const vif_obis_mapping_t vif_obis_map[] = {
    // 能量映射
    {0x00, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", -3},
    {0x01, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", -2},
    {0x02, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", -1},
    {0x03, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", 0},
    {0x04, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", 1},
    {0x05, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", 2},
    {0x06, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", 3},
    {0x07, OBIS_ACTIVE_ENERGY_TOTAL, "有功电能", "Wh", 4},

    // 体积映射 (水/气体)
    {0x10, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -6},
    {0x11, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -5},
    {0x12, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -4},
    {0x13, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -3},
    {0x14, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -2},
    {0x15, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", -1},
    {0x16, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", 0},
    {0x17, OBIS_WATER_VOLUME_TOTAL, "水体积", "m³", 1},

    // 功率映射
    {0x28, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", -3},
    {0x29, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", -2},
    {0x2A, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", -1},
    {0x2B, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", 0},
    {0x2C, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", 1},
    {0x2D, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", 2},
    {0x2E, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", 3},
    {0x2F, OBIS_ACTIVE_POWER_TOTAL, "有功功率", "W", 4},

    // 体积流量映射
    {0x38, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -6},
    {0x39, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -5},
    {0x3A, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -4},
    {0x3B, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -3},
    {0x3C, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -2},
    {0x3D, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", -1},
    {0x3E, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", 0},
    {0x3F, OBIS_WATER_VOLUME_FLOW, "水体积流量", "m³/h", 1}
};

// VIF到OBIS转换函数
uint64_t convert_vif_to_obis(uint8_t vif, uint8_t device_type, uint8_t storage_number) {
    // 根据设备类型调整OBIS代码的A字段
    uint8_t medium = 0;
    switch (device_type) {
        case 0x02: medium = 1; break;  // 电表 -> A=1
        case 0x03: medium = 7; break;  // 燃气表 -> A=7
        case 0x06: medium = 6; break;  // 热表 -> A=6
        case 0x07: medium = 8; break;  // 水表 -> A=8
        default: medium = 0; break;    // 其他 -> A=0
    }

    // 查找VIF映射
    for (int i = 0; i < sizeof(vif_obis_map)/sizeof(vif_obis_map[0]); i++) {
        if (vif_obis_map[i].vif_code == vif) {
            uint64_t obis = vif_obis_map[i].obis_code;
            // 修改A字段为设备类型对应的介质
            obis = (obis & 0x0000FFFFFFFFFFFF) | ((uint64_t)medium << 40);
            // 修改F字段为存储编号
            obis = (obis & 0xFFFFFFFFFFFF0000) | storage_number;
            return obis;
        }
    }

    return 0; // 未找到映射
}
```

---

## 密钥交换FAC过程实现

### 🔐 **密钥交换FAC概述**

频繁访问周期(FAC)中的密钥交换是wM-Bus网络安全管理的核心功能，用于定期更新设备密钥，确保长期通信安全。

#### 密钥交换FAC状态机
```c
// 密钥交换FAC状态机
typedef enum {
    FAC_KEY_EXCHANGE_IDLE,           // 空闲状态
    FAC_KEY_EXCHANGE_INITIATED,      // 密钥交换启动
    FAC_KEY_EXCHANGE_CHALLENGE_SENT, // 挑战已发送
    FAC_KEY_EXCHANGE_RESPONSE_WAIT,  // 等待响应
    FAC_KEY_EXCHANGE_VERIFY,         // 验证阶段
    FAC_KEY_EXCHANGE_CONFIRM,        // 确认阶段
    FAC_KEY_EXCHANGE_COMPLETE,       // 交换完成
    FAC_KEY_EXCHANGE_FAILED,         // 交换失败
    FAC_KEY_EXCHANGE_TIMEOUT         // 超时
} fac_key_exchange_state_t;

// 密钥交换类型
typedef enum {
    KEY_EXCHANGE_INITIAL,            // 初始密钥交换
    KEY_EXCHANGE_RENEWAL,            // 密钥更新
    KEY_EXCHANGE_EMERGENCY,          // 紧急密钥更换
    KEY_EXCHANGE_SCHEDULED           // 定期密钥轮换
} key_exchange_type_t;

// 密钥交换参数
typedef struct {
    uint32_t device_id;              // 设备ID
    uint8_t master_key[16];          // 主密钥 (AES-128)
    uint8_t session_key[16];         // 会话密钥
    uint8_t new_key[16];             // 新密钥
    uint32_t key_version;            // 密钥版本号
    uint32_t exchange_timestamp;     // 交换时间戳
    uint8_t challenge[8];            // 挑战值
    uint8_t response[8];             // 响应值
    uint8_t auth_tag[8];             // 认证标签
    fac_key_exchange_state_t state;  // 当前状态
    key_exchange_type_t type;        // 交换类型
    uint8_t retry_count;             // 重试次数
    uint32_t timeout_ms;             // 超时时间
} fac_key_exchange_context_t;
```

### 🔄 **密钥交换FAC协议流程**

#### 完整的密钥交换时序图
```
主机(Collector)                           从机(Meter)
      |                                        |
      |  1. 广播FAC窗口通知 (CI=90h)             |
      |=======================================>|
      |                                        |
      |  2. 密钥交换请求 (CI=90h)                |
      |<=======================================|
      |                                        |
      |  3. 密钥交换挑战 (CI=91h)                |
      |=======================================>|
      |                                        |
      |  4. 密钥交换响应 (CI=91h)                |
      |<=======================================|
      |                                        |
      |  5. 密钥交换确认 (CI=92h)                |
      |=======================================>|
      |                                        |
      |  6. 密钥激活 (定时)                      |
      |<-------------------------------------->|
```

#### 主机端实现
```c
// 主机端密钥交换FAC管理器
typedef struct {
    fac_key_exchange_context_t active_exchanges[50]; // 活跃的密钥交换
    uint16_t active_exchange_count;
    uint32_t fac_window_start_time;
    uint32_t fac_window_duration_ms;
    bool fac_window_active;
    uint8_t master_key_database[1000][16]; // 主密钥数据库
    uint32_t key_exchange_sequence;
} fac_key_exchange_manager_t;

// 启动密钥交换FAC窗口
bool start_key_exchange_fac_window(fac_key_exchange_manager_t* manager,
                                  uint32_t duration_ms) {
    printf("🔐 启动密钥交换FAC窗口 (持续时间: %d ms)\n", duration_ms);

    manager->fac_window_start_time = get_current_time();
    manager->fac_window_duration_ms = duration_ms;
    manager->fac_window_active = true;
    manager->active_exchange_count = 0;

    // 发送FAC窗口通知广播
    wmbus_frame_t fac_announce = {
        .start_byte = 0x68,
        .l_field = 0x0A,
        .c_field = 0x53,              // SND-UD
        .m_field = 0xFFFF,            // 广播制造商
        .a_field = 0xFFFFFFFF,        // 广播地址
        .ci_field = 0x90,             // 密钥交换通知
        .data = {
            0x01,                     // FAC窗口开启
            (uint8_t)(duration_ms & 0xFF),
            (uint8_t)((duration_ms >> 8) & 0xFF),
            (uint8_t)((duration_ms >> 16) & 0xFF),
            (uint8_t)((duration_ms >> 24) & 0xFF),
            0x00, 0x00               // 保留字段
        },
        .end_byte = 0x16
    };

    // 计算并添加CRC
    calculate_and_add_crc(&fac_announce);

    // 广播FAC窗口通知
    bool success = transmit_wmbus_frame(&fac_announce);

    if (success) {
        printf("✅ FAC窗口通知已广播\n");
        start_fac_window_listener(manager);
        return true;
    } else {
        printf("❌ FAC窗口通知广播失败\n");
        manager->fac_window_active = false;
        return false;
    }
}

// 处理密钥交换请求
bool process_key_exchange_request(fac_key_exchange_manager_t* manager,
                                 wmbus_frame_t* request_frame) {
    if (!manager->fac_window_active) {
        printf("⚠️ FAC窗口未激活，拒绝密钥交换请求\n");
        return false;
    }

    if (manager->active_exchange_count >= 50) {
        printf("⚠️ 活跃交换数量已达上限，拒绝新请求\n");
        return false;
    }

    // 解析请求
    uint32_t device_id = request_frame->a_field;
    uint8_t* request_data = request_frame->data;

    printf("🔑 收到设备 %08X 的密钥交换请求\n", device_id);

    // 创建新的密钥交换上下文
    fac_key_exchange_context_t* context =
        &manager->active_exchanges[manager->active_exchange_count];

    context->device_id = device_id;
    context->state = FAC_KEY_EXCHANGE_INITIATED;
    context->type = (key_exchange_type_t)request_data[0];
    context->key_version = get_current_key_version(device_id);
    context->exchange_timestamp = get_current_time();
    context->retry_count = 0;
    context->timeout_ms = 30000; // 30秒超时

    // 查找设备的主密钥
    if (!get_device_master_key(device_id, context->master_key)) {
        printf("❌ 未找到设备主密钥\n");
        return false;
    }

    // 生成挑战值
    generate_random_bytes(context->challenge, 8);

    // 生成新的会话密钥
    generate_session_key(context->session_key, context->master_key,
                        context->challenge, context->exchange_timestamp);

    // 发送密钥交换挑战
    bool success = send_key_exchange_challenge(context);

    if (success) {
        context->state = FAC_KEY_EXCHANGE_CHALLENGE_SENT;
        manager->active_exchange_count++;
        printf("✅ 密钥交换挑战已发送给设备 %08X\n", device_id);
        return true;
    } else {
        printf("❌ 发送密钥交换挑战失败\n");
        return false;
    }
}

// 发送密钥交换挑战
bool send_key_exchange_challenge(fac_key_exchange_context_t* context) {
    wmbus_frame_t challenge_frame = {
        .start_byte = 0x68,
        .l_field = 0x18,              // 24字节数据
        .c_field = 0x53,              // SND-UD
        .m_field = 0x4D44,            // 主机制造商ID
        .a_field = context->device_id,
        .ci_field = 0x91,             // 密钥交换挑战
        .data = {0}
    };

    // 构建挑战数据
    uint8_t* data = challenge_frame.data;
    data[0] = (uint8_t)context->type;                    // 交换类型
    data[1] = (uint8_t)(context->key_version & 0xFF);    // 密钥版本
    data[2] = (uint8_t)((context->key_version >> 8) & 0xFF);
    data[3] = (uint8_t)((context->key_version >> 16) & 0xFF);
    data[4] = (uint8_t)((context->key_version >> 24) & 0xFF);

    // 复制挑战值
    memcpy(&data[5], context->challenge, 8);

    // 时间戳
    uint32_t timestamp = context->exchange_timestamp;
    data[13] = (uint8_t)(timestamp & 0xFF);
    data[14] = (uint8_t)((timestamp >> 8) & 0xFF);
    data[15] = (uint8_t)((timestamp >> 16) & 0xFF);
    data[16] = (uint8_t)((timestamp >> 24) & 0xFF);

    // 计算认证标签 (使用主密钥)
    calculate_cmac_auth_tag(context->master_key, data, 17, &data[17]);

    challenge_frame.end_byte = 0x16;
    calculate_and_add_crc(&challenge_frame);

    return transmit_wmbus_frame(&challenge_frame);
}

// 发送密钥交换确认
bool send_key_exchange_confirmation(fac_key_exchange_context_t* context) {
    wmbus_frame_t confirm_frame = {
        .start_byte = 0x68,
        .l_field = 0x10,              // 16字节数据
        .c_field = 0x53,              // SND-UD
        .m_field = 0x4D44,            // 主机制造商ID
        .a_field = context->device_id,
        .ci_field = 0x92,             // 密钥交换确认
        .data = {0}
    };

    uint8_t* data = confirm_frame.data;

    // 确认状态
    data[0] = 0x01;                   // 成功

    // 新密钥版本
    uint32_t new_version = context->key_version + 1;
    data[1] = (uint8_t)(new_version & 0xFF);
    data[2] = (uint8_t)((new_version >> 8) & 0xFF);
    data[3] = (uint8_t)((new_version >> 16) & 0xFF);
    data[4] = (uint8_t)((new_version >> 24) & 0xFF);

    // 激活时间戳
    uint32_t activation_time = get_current_time() + 60000; // 1分钟后激活
    data[5] = (uint8_t)(activation_time & 0xFF);
    data[6] = (uint8_t)((activation_time >> 8) & 0xFF);
    data[7] = (uint8_t)((activation_time >> 16) & 0xFF);
    data[8] = (uint8_t)((activation_time >> 24) & 0xFF);

    // 计算确认认证标签
    calculate_cmac_auth_tag(context->new_key, data, 9, &data[9]);

    confirm_frame.end_byte = 0x16;
    calculate_and_add_crc(&confirm_frame);

    bool success = transmit_wmbus_frame(&confirm_frame);

    if (success) {
        context->state = FAC_KEY_EXCHANGE_CONFIRM;

        // 更新密钥数据库
        update_device_key(context->device_id, context->new_key, new_version);

        printf("✅ 密钥交换确认已发送，新密钥将在1分钟后激活\n");

        // 设置密钥激活定时器
        schedule_key_activation(context->device_id, activation_time);

        return true;
    } else {
        printf("❌ 发送密钥交换确认失败\n");
        context->state = FAC_KEY_EXCHANGE_FAILED;
        return false;
    }
}
```

#### 从机端实现
```c
// 从机端密钥交换处理器
typedef struct {
    uint32_t device_id;
    uint8_t current_master_key[16];
    uint8_t pending_new_key[16];
    uint32_t current_key_version;
    fac_key_exchange_state_t state;
    uint32_t last_exchange_time;
    bool fac_window_detected;
    uint32_t fac_window_end_time;
} meter_key_exchange_t;

// 检测FAC窗口通知
bool detect_fac_window_announcement(meter_key_exchange_t* meter,
                                   wmbus_frame_t* announce_frame) {
    if (announce_frame->ci_field != 0x90) {
        return false; // 不是FAC窗口通知
    }

    uint8_t* data = announce_frame->data;
    if (data[0] != 0x01) {
        return false; // 不是窗口开启通知
    }

    // 提取窗口持续时间
    uint32_t duration_ms = data[1] | (data[2] << 8) | (data[3] << 16) | (data[4] << 24);

    meter->fac_window_detected = true;
    meter->fac_window_end_time = get_current_time() + duration_ms;

    printf("🔐 检测到FAC窗口通知，持续时间: %d ms\n", duration_ms);

    // 检查是否需要密钥交换
    if (should_request_key_exchange(meter)) {
        return request_key_exchange(meter);
    }

    return true;
}

// 判断是否需要密钥交换
bool should_request_key_exchange(meter_key_exchange_t* meter) {
    uint32_t current_time = get_current_time();

    // 检查密钥年龄 (超过1年需要更新)
    if ((current_time - meter->last_exchange_time) > (365 * 24 * 3600 * 1000)) {
        printf("🔑 密钥已使用超过1年，需要更新\n");
        return true;
    }

    // 检查密钥版本 (如果版本过低)
    if (meter->current_key_version < get_minimum_required_key_version()) {
        printf("🔑 密钥版本过低，需要更新\n");
        return true;
    }

    // 检查是否有安全事件要求密钥更换
    if (has_security_event_requiring_key_change()) {
        printf("🔑 检测到安全事件，需要紧急更换密钥\n");
        return true;
    }

    return false;
}

// 请求密钥交换
bool request_key_exchange(meter_key_exchange_t* meter) {
    if (!meter->fac_window_detected) {
        printf("⚠️ 未检测到FAC窗口，无法请求密钥交换\n");
        return false;
    }

    printf("🔑 发送密钥交换请求\n");

    wmbus_frame_t request_frame = {
        .start_byte = 0x68,
        .l_field = 0x0C,              // 12字节数据
        .c_field = 0x53,              // SND-UD
        .m_field = 0x1234,            // 设备制造商ID
        .a_field = meter->device_id,
        .ci_field = 0x90,             // 密钥交换请求
        .data = {0}
    };

    uint8_t* data = request_frame.data;

    // 交换类型
    data[0] = KEY_EXCHANGE_RENEWAL;

    // 当前密钥版本
    data[1] = (uint8_t)(meter->current_key_version & 0xFF);
    data[2] = (uint8_t)((meter->current_key_version >> 8) & 0xFF);
    data[3] = (uint8_t)((meter->current_key_version >> 16) & 0xFF);
    data[4] = (uint8_t)((meter->current_key_version >> 24) & 0xFF);

    // 设备能力标志
    data[5] = 0x01;                   // 支持AES-128
    data[6] = 0x02;                   // 支持CMAC认证

    // 计算请求认证标签
    calculate_cmac_auth_tag(meter->current_master_key, data, 7, &data[7]);

    request_frame.end_byte = 0x16;
    calculate_and_add_crc(&request_frame);

    bool success = transmit_wmbus_frame(&request_frame);

    if (success) {
        meter->state = FAC_KEY_EXCHANGE_INITIATED;
        printf("✅ 密钥交换请求已发送\n");
        return true;
    } else {
        printf("❌ 发送密钥交换请求失败\n");
        return false;
    }
}
```

### 🔐 **密钥管理和安全功能**

#### 密钥生成和派生算法
```c
// 会话密钥生成 (基于HKDF)
void generate_session_key(uint8_t* session_key,
                         uint8_t* master_key,
                         uint8_t* challenge,
                         uint32_t timestamp) {
    // 使用HKDF (HMAC-based Key Derivation Function)
    uint8_t salt[16];
    memcpy(salt, challenge, 8);
    memcpy(&salt[8], &timestamp, 4);
    memset(&salt[12], 0, 4);

    uint8_t info[] = "wM-Bus Session Key";

    hkdf_extract_and_expand(master_key, 16, salt, 16,
                           info, strlen((char*)info),
                           session_key, 16);

    printf("🔑 会话密钥已生成\n");
}

// 挑战响应计算
void calculate_challenge_response(uint8_t* master_key,
                                 uint8_t* challenge,
                                 uint32_t timestamp,
                                 uint8_t* response) {
    uint8_t input[12];
    memcpy(input, challenge, 8);
    memcpy(&input[8], &timestamp, 4);

    // 使用HMAC-SHA256计算响应，取前8字节
    uint8_t hmac_output[32];
    hmac_sha256(master_key, 16, input, 12, hmac_output);
    memcpy(response, hmac_output, 8);
}

// 密钥激活处理
void activate_new_key(meter_key_exchange_t* meter, uint32_t new_version) {
    printf("🔑 激活新密钥，版本: %d\n", new_version);

    // 备份当前密钥
    uint8_t old_key[16];
    memcpy(old_key, meter->current_master_key, 16);
    uint32_t old_version = meter->current_key_version;

    // 激活新密钥
    memcpy(meter->current_master_key, meter->pending_new_key, 16);
    meter->current_key_version = new_version;

    // 清除待激活密钥
    memset(meter->pending_new_key, 0, 16);

    // 保存到非易失性存储
    save_key_to_nvs(meter->device_id, meter->current_master_key, new_version);

    // 记录密钥更换事件
    log_key_change_event(meter->device_id, old_version, new_version);

    printf("✅ 密钥激活完成\n");
}

// 密钥版本管理
typedef struct {
    uint32_t device_id;
    uint32_t key_version;
    uint32_t creation_time;
    uint32_t activation_time;
    uint32_t expiration_time;
    uint8_t key_hash[8];             // 密钥哈希值 (用于验证)
    bool is_active;
    bool is_compromised;
} key_version_record_t;

// 密钥数据库管理
typedef struct {
    key_version_record_t records[10000];
    uint16_t record_count;
    uint32_t next_version_number;
    uint8_t master_key_encryption_key[16]; // 用于加密存储的主密钥
} key_database_t;

// 密钥轮换策略
typedef struct {
    uint32_t rotation_interval_days;    // 轮换间隔 (天)
    uint32_t warning_period_days;       // 预警期 (天)
    uint32_t grace_period_days;         // 宽限期 (天)
    bool auto_rotation_enabled;         // 自动轮换启用
    uint8_t max_concurrent_exchanges;   // 最大并发交换数
    uint32_t fac_window_duration_ms;    // FAC窗口持续时间
} key_rotation_policy_t;

// 默认密钥轮换策略
static const key_rotation_policy_t default_rotation_policy = {
    .rotation_interval_days = 365,      // 每年轮换一次
    .warning_period_days = 30,          // 提前30天预警
    .grace_period_days = 7,             // 7天宽限期
    .auto_rotation_enabled = true,
    .max_concurrent_exchanges = 50,
    .fac_window_duration_ms = 300000    // 5分钟FAC窗口
};

// 检查密钥是否需要轮换
bool check_key_rotation_needed(key_database_t* db, uint32_t device_id) {
    key_version_record_t* record = find_key_record(db, device_id);
    if (!record) {
        printf("⚠️ 未找到设备 %08X 的密钥记录\n", device_id);
        return false;
    }

    uint32_t current_time = get_current_time();
    uint32_t key_age_days = (current_time - record->activation_time) / (24 * 3600 * 1000);

    // 检查是否超过轮换间隔
    if (key_age_days >= default_rotation_policy.rotation_interval_days) {
        printf("🔑 设备 %08X 密钥已使用 %d 天，需要轮换\n", device_id, key_age_days);
        return true;
    }

    // 检查是否进入预警期
    uint32_t days_until_rotation = default_rotation_policy.rotation_interval_days - key_age_days;
    if (days_until_rotation <= default_rotation_policy.warning_period_days) {
        printf("⚠️ 设备 %08X 密钥将在 %d 天后到期\n", device_id, days_until_rotation);
    }

    // 检查密钥是否被标记为泄露
    if (record->is_compromised) {
        printf("🚨 设备 %08X 密钥已被标记为泄露，需要紧急轮换\n", device_id);
        return true;
    }

    return false;
}

// 批量密钥轮换管理
void schedule_batch_key_rotation(key_database_t* db,
                                fac_key_exchange_manager_t* manager) {
    printf("🔄 开始批量密钥轮换检查\n");

    uint16_t devices_needing_rotation = 0;
    uint32_t devices_to_rotate[1000];

    // 扫描所有设备，找出需要轮换的设备
    for (int i = 0; i < db->record_count; i++) {
        key_version_record_t* record = &db->records[i];

        if (record->is_active && check_key_rotation_needed(db, record->device_id)) {
            if (devices_needing_rotation < 1000) {
                devices_to_rotate[devices_needing_rotation] = record->device_id;
                devices_needing_rotation++;
            }
        }
    }

    printf("📊 发现 %d 个设备需要密钥轮换\n", devices_needing_rotation);

    if (devices_needing_rotation == 0) {
        printf("✅ 所有设备密钥状态正常\n");
        return;
    }

    // 分批处理密钥轮换
    uint16_t batch_size = default_rotation_policy.max_concurrent_exchanges;
    uint16_t batches = (devices_needing_rotation + batch_size - 1) / batch_size;

    for (uint16_t batch = 0; batch < batches; batch++) {
        uint16_t batch_start = batch * batch_size;
        uint16_t batch_end = MIN(batch_start + batch_size, devices_needing_rotation);

        printf("🔄 处理第 %d/%d 批密钥轮换 (%d-%d)\n",
               batch + 1, batches, batch_start + 1, batch_end);

        // 启动FAC窗口
        if (!start_key_exchange_fac_window(manager,
                                          default_rotation_policy.fac_window_duration_ms)) {
            printf("❌ 启动FAC窗口失败，跳过此批次\n");
            continue;
        }

        // 等待FAC窗口完成
        wait_for_fac_window_completion(manager);

        // 检查轮换结果
        uint16_t successful_rotations = 0;
        for (uint16_t i = batch_start; i < batch_end; i++) {
            if (is_key_rotation_successful(devices_to_rotate[i])) {
                successful_rotations++;
            }
        }

        printf("📊 批次 %d 完成: %d/%d 设备成功轮换密钥\n",
               batch + 1, successful_rotations, batch_end - batch_start);

        // 批次间延迟
        if (batch < batches - 1) {
            printf("⏳ 等待 30 秒后处理下一批次...\n");
            delay_ms(30000);
        }
    }

    printf("✅ 批量密钥轮换完成\n");
}
```

### 📊 **密钥交换FAC性能监控**

#### 性能指标和监控
```c
// 密钥交换性能统计
typedef struct {
    uint32_t total_exchanges_attempted;  // 尝试的交换总数
    uint32_t successful_exchanges;       // 成功的交换数
    uint32_t failed_exchanges;           // 失败的交换数
    uint32_t timeout_exchanges;          // 超时的交换数
    uint32_t avg_exchange_duration_ms;   // 平均交换持续时间
    uint32_t max_exchange_duration_ms;   // 最大交换持续时间
    uint32_t min_exchange_duration_ms;   // 最小交换持续时间
    float success_rate;                  // 成功率
    uint32_t last_update_time;           // 最后更新时间
} key_exchange_statistics_t;

// 更新密钥交换统计
void update_key_exchange_statistics(key_exchange_statistics_t* stats,
                                   fac_key_exchange_context_t* context) {
    stats->total_exchanges_attempted++;

    uint32_t exchange_duration = get_current_time() - context->exchange_timestamp;

    switch (context->state) {
        case FAC_KEY_EXCHANGE_COMPLETE:
            stats->successful_exchanges++;

            // 更新持续时间统计
            if (stats->total_exchanges_attempted == 1) {
                stats->avg_exchange_duration_ms = exchange_duration;
                stats->max_exchange_duration_ms = exchange_duration;
                stats->min_exchange_duration_ms = exchange_duration;
            } else {
                stats->avg_exchange_duration_ms =
                    (stats->avg_exchange_duration_ms * (stats->successful_exchanges - 1) +
                     exchange_duration) / stats->successful_exchanges;

                if (exchange_duration > stats->max_exchange_duration_ms) {
                    stats->max_exchange_duration_ms = exchange_duration;
                }

                if (exchange_duration < stats->min_exchange_duration_ms) {
                    stats->min_exchange_duration_ms = exchange_duration;
                }
            }
            break;

        case FAC_KEY_EXCHANGE_FAILED:
            stats->failed_exchanges++;
            break;

        case FAC_KEY_EXCHANGE_TIMEOUT:
            stats->timeout_exchanges++;
            break;

        default:
            // 交换仍在进行中
            return;
    }

    // 计算成功率
    stats->success_rate = (float)stats->successful_exchanges /
                         stats->total_exchanges_attempted * 100.0f;

    stats->last_update_time = get_current_time();

    printf("📊 密钥交换统计更新: 成功率 %.1f%% (%d/%d)\n",
           stats->success_rate, stats->successful_exchanges,
           stats->total_exchanges_attempted);
}

// 显示密钥交换性能报告
void display_key_exchange_performance_report(key_exchange_statistics_t* stats) {
    printf("=== 密钥交换FAC性能报告 ===\n\n");

    printf("📊 总体统计:\n");
    printf("  尝试交换总数: %d\n", stats->total_exchanges_attempted);
    printf("  成功交换数: %d\n", stats->successful_exchanges);
    printf("  失败交换数: %d\n", stats->failed_exchanges);
    printf("  超时交换数: %d\n", stats->timeout_exchanges);
    printf("  成功率: %.2f%%\n", stats->success_rate);

    printf("\n⏱️ 性能指标:\n");
    printf("  平均交换时间: %d ms\n", stats->avg_exchange_duration_ms);
    printf("  最大交换时间: %d ms\n", stats->max_exchange_duration_ms);
    printf("  最小交换时间: %d ms\n", stats->min_exchange_duration_ms);

    printf("\n📈 性能评估:\n");
    if (stats->success_rate >= 95.0f) {
        printf("  ✅ 优秀 - 密钥交换性能良好\n");
    } else if (stats->success_rate >= 90.0f) {
        printf("  ⚠️ 良好 - 密钥交换性能可接受\n");
    } else if (stats->success_rate >= 80.0f) {
        printf("  ⚠️ 一般 - 密钥交换流程性能一般\n");
    } else {
        printf("  ❌ 差 - 密钥交换存在严重问题\n");
    }

    if (stats->avg_exchange_duration_ms > 60000) {
        printf("  ⚠️ 交换时间过长\n");
    }

    printf("\n最后更新时间: %d\n", stats->last_update_time);
}
```

### 🔄 **多模式通信和兼容性**

#### 模式间通信隔离原理
不同wM-Bus传输模式在物理层使用完全不同的参数，导致它们无法直接通信：

```c
// wM-Bus模式物理层参数对比表
typedef struct {
    char* mode_name;
    float frequency_mhz;
    uint32_t data_rate_bps;
    float freq_deviation_khz;
    uint16_t preamble_length;
    char* coding_scheme;
} wmbus_mode_spec_t;

static const wmbus_mode_spec_t mode_specs[] = {
    {"T1/T2", 868.95f, 66667,  50.0f, 48, "Manchester + 3/6 coding"},
    {"S1/S2", 868.30f, 32768,  50.0f, 19, "Manchester"},
    {"C1/C2", 868.95f, 100000, 50.0f, 32, "Manchester"},
    {"N1/N2", 169.40f, 2400,   2.4f,  15, "GFSK"},
    {"R2",    868.33f, 4800,   25.0f, 39, "Manchester"},
    {"F2",    433.82f, 2400,   2.4f,  15, "GFSK"}
};

// 模式兼容性检查
bool are_modes_compatible(const char* mode1, const char* mode2) {
    wmbus_mode_spec_t* spec1 = find_mode_spec(mode1);
    wmbus_mode_spec_t* spec2 = find_mode_spec(mode2);

    // 检查频率兼容性 (±100kHz容差)
    if (fabs(spec1->frequency_mhz - spec2->frequency_mhz) > 0.1f) {
        return false;
    }

    // 检查数据速率兼容性
    if (spec1->data_rate_bps != spec2->data_rate_bps) {
        return false;
    }

    // 检查编码方案兼容性
    if (strcmp(spec1->coding_scheme, spec2->coding_scheme) != 0) {
        return false;
    }

    return true;
}
```

#### 多模式主机实现策略
```c
// 多模式收集器架构
typedef struct {
    char mode_name[8];
    bool is_enabled;
    uint32_t scan_duration_ms;
    uint32_t last_activity_time;
    uint16_t discovered_devices;
    float avg_rssi;
    receiver_config_t rx_config;
} mode_scanner_t;

typedef struct {
    mode_scanner_t scanners[6];  // T, S, C, N, R, F
    uint8_t active_scanner_count;
    uint32_t total_scan_cycle_ms;
    bool adaptive_timing;
    bool priority_based_scanning;
} multi_mode_collector_t;

// 智能模式扫描算法
void intelligent_mode_scanning(multi_mode_collector_t* collector) {
    // 1. 初始全模式扫描 (发现阶段)
    for (int i = 0; i < 6; i++) {
        if (!collector->scanners[i].is_enabled) continue;

        configure_receiver(&collector->scanners[i].rx_config);
        uint16_t detected = scan_for_devices(30000); // 30秒扫描

        collector->scanners[i].discovered_devices = detected;
        if (detected > 0) {
            printf("发现 %s 模式设备: %d 个\n",
                   collector->scanners[i].mode_name, detected);
        }
    }

    // 2. 自适应扫描时间分配
    if (collector->adaptive_timing) {
        uint32_t total_devices = 0;
        for (int i = 0; i < 6; i++) {
            total_devices += collector->scanners[i].discovered_devices;
        }

        for (int i = 0; i < 6; i++) {
            if (total_devices > 0) {
                // 按设备数量比例分配扫描时间
                float ratio = (float)collector->scanners[i].discovered_devices / total_devices;
                collector->scanners[i].scan_duration_ms =
                    (uint32_t)(collector->total_scan_cycle_ms * ratio);

                // 最小扫描时间保证
                if (collector->scanners[i].discovered_devices > 0) {
                    collector->scanners[i].scan_duration_ms =
                        MAX(collector->scanners[i].scan_duration_ms, 5000);
                }
            }
        }
    }

    // 3. 优先级扫描循环
    while (true) {
        for (int i = 0; i < 6; i++) {
            mode_scanner_t* scanner = &collector->scanners[i];

            if (!scanner->is_enabled || scanner->scan_duration_ms == 0) {
                continue;
            }

            printf("🔄 扫描 %s 模式 (%d ms)...\n",
                   scanner->mode_name, scanner->scan_duration_ms);

            configure_receiver(&scanner->rx_config);
            scan_mode_for_duration(scanner, scanner->scan_duration_ms);
        }

        // 动态调整扫描策略
        if (collector->adaptive_timing) {
            adjust_scanning_strategy(collector);
        }
    }
}
```



---

## 常见问题解答 (FAQ)

### **Q1: EN 13757-4、OMS和P2标准有什么区别？**
**A:**
- **EN 13757-4**: 欧洲无线M-Bus基础标准，定义核心协议
- **OMS**: 开放计量系统规范，在EN 13757-4基础上增加互操作性和安全性
- **P2**: 荷兰Enexis智能电表标准，基于EN 60870-5-1，有特定要求

### **Q2: 如何选择合适的安全模式？**
**A:**
- **新项目**: 可选择Security Profile B (模式7) 或 Profile D (模式10)
- **P2应用**: 强制使用模式9 (AES-128 GCM)
- **高安全要求**: 使用Security Profile C (模式13, TLS)
- **测试阶段**: 可临时使用模式0 (无加密)

### **Q3: Frame Format A和B如何选择？**
**A:**
- **数据≤15字节**: 使用Format A (简单快速)
- **数据>15字节**: 使用Format B (高效率)
- **高可靠性要求**: 优先选择Format B
- **P2应用**: 使用FT3格式

### **Q4: 如何实现OMS合规性？**
**A:**
1. 支持至少一种安全配置文件
2. 实现标准CI字段 (7A/8A/8C等)
3. 正确处理AFL层和消息计数器
4. 支持标准DIF/VIF编码
5. 通过OMS合规性测试

### **Q5: P2调用计数器验证失败怎么办？**
**A:**
- **计数器过高**: E-meter接受但记录安全事件134
- **重放攻击**: 拒绝消息，检查网络安全
- **计数器溢出**: 需要更换密钥
- **验证规则**: 最大差值100，严格递增

### **Q6: 如何调试wM-Bus通信问题？**
**A:**
1. **物理层**: 检查频率、功率、调制参数
2. **帧同步**: 验证前导码和同步字
3. **CRC错误**: 检查数据完整性和计算方法
4. **地址过滤**: 确认设备地址配置
5. **安全验证**: 检查密钥和计数器状态

### **Q7: 支持哪些开发工具？**
**A:**
- **在线解析**: wmbusmeters.org
- **协议分析**: Wireshark + wM-Bus插件
- **测试工具**: OMS Test Tool
- **射频测试**: 频谱分析仪、矢量信号分析仪
- **开发板**: 各厂商wM-Bus开发套件

### **Q8: 如何获得认证？**
**A:**
- **OMS认证**: 通过OMS Group官方测试
- **CE认证**: 符合欧洲RED指令
- **国家认证**: 各国特定要求 (如荷兰P2)
- **互操作性**: 参与OMS互操作性测试

### **Q9: wM-Bus主机和从机的通信方式是什么？**
**A:**
wM-Bus通信**不完全是广播形式**，而是根据传输模式采用不同策略：

**从机到主机 (Meter → Collector):**
- **T1, C1, S1模式**: 单向广播，从机定期发送数据
- **T2, C2, S2模式**: 双向通信，从机广播后监听主机响应

**主机到从机 (Collector → Meter):**
- **广播命令**: 发送给所有设备 (目标地址=0xFFFFFFFF)
- **单播命令**: 发送给特定设备 (指定设备地址)
- **组播命令**: 发送给设备组

**主机设备发现机制:**
1. **被动监听**: 监听从机的定期广播数据
2. **主动扫描**: 发送广播查询命令
3. **安装模式**: 监听P2设备的安装请求 (SND_IR)
4. **网络扫描**: 系统性扫描地址空间

### **Q10: T模式主机能监听到S模式数据包吗？**
**A:**
**不能！** 不同wM-Bus模式间无法直接通信，原因如下：

**物理层参数差异:**
- **T模式**: 868.95 MHz (M→O), 868.3 MHz (O→M), 双向通信, 2-FSK调制
- **S模式**: 868.30 MHz, 32.768 kbps, 2-FSK调制
- **C模式**: 868.95 MHz, 100 kbps, 特殊前导码

**解决方案:**
1. **多模式主机**: 支持多种模式的循环扫描
2. **智能检测**: 自动发现网络中的活跃模式
3. **模式切换**: 根据需要动态切换接收器配置

```c
// 多模式扫描示例
void multi_mode_scan_loop() {
    char* modes[] = {"T", "S", "C"};
    for (int i = 0; i < 3; i++) {
        configure_receiver_for_mode(modes[i]);
        listen_for_devices(5000); // 监听5秒
    }
}
```

### **Q11: 同一网络中多个T模式从机同时发送会冲突吗？**
**A:**
**会产生严重冲突！** T模式的特点导致冲突风险：
- ❌ 无载波侦听 (CSMA)
- ❌ 无冲突检测
- ❌ 广播发送模式
- ❌ 不等待确认

T模式设备同时发送时会产生冲突，因为T模式采用广播发送方式，无载波侦听和冲突检测机制。根据EN 13757-4标准，T模式设备应实施适当的时间分散机制以减少冲突。

### **Q12: 如何实现密钥交换FAC过程？**
**A:**
密钥交换FAC是wM-Bus网络安全管理的核心功能，用于定期更新设备密钥：

**密钥交换流程:**
1. **FAC窗口通知**: 主机广播FAC窗口开启 (CI=90h)
2. **交换请求**: 从机发送密钥交换请求 (CI=90h)
3. **挑战发送**: 主机发送密钥交换挑战 (CI=91h)
4. **响应验证**: 从机发送加密响应 (CI=91h)
5. **确认激活**: 主机发送确认并设置激活时间 (CI=92h)
6. **密钥激活**: 双方同时激活新密钥

**关键安全特性:**
- **会话密钥**: 使用HKDF从主密钥派生
- **挑战响应**: HMAC-SHA256认证机制
- **加密传输**: 新密钥使用会话密钥加密
- **时间同步**: 防止重放攻击
- **版本管理**: 密钥版本号递增

**实现示例:**
```c
// 启动密钥交换FAC窗口
bool start_key_exchange_fac_window(duration_ms) {
    // 1. 广播FAC窗口通知
    broadcast_fac_announcement(duration_ms);

    // 2. 监听密钥交换请求
    listen_for_key_exchange_requests();

    // 3. 处理并发密钥交换
    process_concurrent_exchanges();
}

// 从机检测并请求密钥交换
bool detect_and_request_key_exchange() {
    if (fac_window_detected && key_needs_rotation()) {
        return send_key_exchange_request();
    }
}
```

**密钥轮换策略:**
- **定期轮换**: 每年自动轮换一次
- **预警机制**: 提前30天发出预警
- **紧急轮换**: 检测到安全事件时立即轮换
- **批量处理**: 支持大规模网络的批量密钥更新

**实施要求:**
- **超时管理**: 根据OMS规范设置适当超时
- **重试机制**: 按照标准要求实施重试

### **Q13: 如何验证AES-128加密实现的正确性？**
**A:**
在wM-Bus密钥交换实现中，AES-128加密的正确性至关重要。以下是验证方法：

**NIST标准测试向量验证:**
```c
// NIST AES-128 ECB标准测试向量
uint8_t nist_key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                        0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
uint8_t nist_plain[16] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                          0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
uint8_t expected[16] = {0x69, 0xc4, 0xe0, 0xd8, 0x6a, 0x7b, 0x04, 0x30,
                        0xd8, 0xcd, 0xb7, 0x80, 0x70, 0xb4, 0xc5, 0x5a};

// 验证加密结果
uint8_t result[16];
ql_aes_encrypt(nist_key, nist_plain, result);

bool is_correct = (memcmp(result, expected, 16) == 0);
ql_printf("NIST测试: %s\n", is_correct ? "通过" : "失败");
```

**平台特定实现验证:**
```c
// Quectel平台AES-128测试示例
uint8_t test_key[16] = {0x11, 0x22, 0x33, 0x55, 0x44, 0x66, 0x77, 0x88,
                        0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x77};
uint8_t test_data[16] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88,
                         0x99, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x77};

// Quectel平台标准结果
uint8_t quectel_expected[16] = {0xB8, 0x88, 0xCA, 0x1A, 0xE8, 0x2B, 0xF4, 0x5F,
                                0x13, 0x6D, 0xF7, 0xCE, 0xCD, 0xF2, 0x6F, 0xBA};

uint8_t encrypted[16], decrypted[16];
ql_aes_encrypt(test_key, test_data, encrypted);
ql_aes_decrypt(test_key, encrypted, decrypted);

// 验证加密结果
bool encrypt_ok = (memcmp(encrypted, quectel_expected, 16) == 0);
// 验证解密结果
bool decrypt_ok = (memcmp(decrypted, test_data, 16) == 0);

ql_printf("加密验证: %s\n", encrypt_ok ? "通过" : "失败");
ql_printf("解密验证: %s\n", decrypt_ok ? "通过" : "失败");
```

**验证要点:**
1. **NIST兼容性**: 确保与国际标准一致
2. **加解密对称性**: 加密后解密应恢复原始数据
3. **平台一致性**: 同一平台上结果应该可重现
4. **边界条件**: 测试全0、全1等特殊数据

**在wM-Bus中的应用:**
- 密钥交换过程中的会话密钥加密
- 设备认证数据的加密传输
- 敏感配置信息的安全存储


---

## 版本历史

| 版本 | 日期 | 修改内容 | 作者 |
|------|------|----------|------|
| 1.0 | 2024-12-26 | 初始版本 | 技术团队 |
| 1.1 | 2024-12-26 | 添加附录和案例 | 技术团队 |
| 1.2 | 2024-12-27 | 区分EN 13757-4与OMS/P2标准，修正C模式前导码 | 技术团队 |
| 1.3 | 2024-12-27 | 新增P2 Companion Standard完整章节 | 技术团队 |
| 2.0 | 2024-12-27 | **重大更新**: 基于全部PDF文档的完整规范 | 技术团队 |
|     |            | - 完整的OMS-Spec Vol.2 v5.0.1规范集成 |          |
|     |            | - P2 Companion Standard v5.0完整实现 |          |
|     |            | - EN 13757-3:2018 DIF/VIF编码完整规范 |          |
|     |            | - OMS安全模式13 (TLS)完整实现 |          |
|     |            | - OBIS代码映射规范 |          |
|     |            | - 三标准融合的权威技术文档 |          |
| 2.1 | 2024-12-27 | **文档优化**: 提升可用性和实用性 | 技术团队 |
|     |            | - 添加快速参考索引 |          |
|     |            | - 新增常见问题解答(FAQ) |          |
|     |            | - 修正目录链接 |          |
|     |            | - 完善开发者指导 |          |
| 2.2 | 2024-12-27 | **链接修复**: 修复章节链接问题 | 技术团队 |
|     |            | - 修复P2 Companion Standard章节链接 |          |
|     |            | - 重新组织附录编号 (A-H) |          |
|     |            | - 更新快速参考索引链接 |          |
|     |            | - 确保所有内部链接正常工作 |          |
| 2.3 | 2024-12-27 | **实际应用增强**: 添加关键技术问题解答 | 技术团队 |
|     |            | - 新增"实际应用中的关键技术问题"章节 |          |
|     |            | - 详细解答多模式通信兼容性问题 |          |
|     |            | - 深入分析T模式网络冲突和解决方案 |          |
|     |            | - 添加网络容量规划指导 |          |
|     |            | - 扩展FAQ部分，涵盖实际部署问题 |          |
|     |            | - 提供完整的代码示例和数学模型 |          |
| 2.4 | 2024-12-27 | **安全功能增强**: 实现密钥交换FAC过程 | 技术团队 |
|     |            | - 新增"密钥交换FAC过程实现"完整章节 |          |
|     |            | - 详细的主机端和从机端实现代码 |          |
|     |            | - 完整的密钥管理和安全功能 |          |
|     |            | - 密钥轮换策略和批量处理机制 |          |
|     |            | - 性能监控和统计分析功能 |          |
|     |            | - 添加密钥交换FAC相关FAQ (Q12) |          |
|     |            | - 基于HKDF、HMAC-SHA256的安全实现 |          |

---

*本文档基于EN 13757标准和OMS规范编写，适用于wM-Bus协议的学习和实际应用开发。如有技术问题，请参考相关标准文档或联系技术支持。*
