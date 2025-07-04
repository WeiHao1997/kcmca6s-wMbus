/***************************************************************************//**
 * @brief RAIL Configuration
 * @details
 *   WARNING: Auto-Generated Radio Config  -  DO NOT EDIT
 *   Radio Configurator Version: 2402.6.1
 *   RAIL Adapter Version: 2.4.33
 *   RAIL Compatibility: 2.x
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/
#include "em_device.h"
#include "rail_config.h"

uint32_t RAILCb_CalcSymbolRate(RAIL_Handle_t railHandle)
{
  (void) railHandle;
  return 0U;
}

uint32_t RAILCb_CalcBitRate(RAIL_Handle_t railHandle)
{
  (void) railHandle;
  return 0U;
}

void RAILCb_ConfigFrameTypeLength(RAIL_Handle_t railHandle,
                                  const RAIL_FrameType_t *frameType)
{
  (void) railHandle;
  (void) frameType;
}

uint32_t frameCodingTable[] = {
  2155905152, 2155905152, 58753152, 2147615104,
  125862016, 2147909760, 2147485056, 2155905028,
  192970880, 2148142976, 2155874688, 2155905032,
  2148404608, 2155905036, 2155905152, 2155905152,
  185470234, 320215324, 589703468, 624046388,
};

static const uint8_t irCalConfig[] = {
  20, 41, 2, 0, 0, 49, 17, 0, 0, 0, 1, 0, 2, 100, 0, 1, 1, 47, 0, 0, 7
};

static const int32_t timingConfig_0[] = {
  23455, 23455, 5000, 0
};

static const int32_t timingConfig_1[] = {
  23455, 23455, 15259, 0
};

static const int32_t timingConfig_2[] = {
  64447, 64447, 5000, 0
};

static const int32_t timingConfig_3[] = {
  48093, 48093, 20000, 0
};

static const uint8_t hfxoRetimingConfigEntries[] = {
  1, 0, 0, 0, 0xc0, 0x17, 0x53, 0x02, 4, 12, 0, 0, 0xe0, 0x02, 0, 0, 0, 0, 0x3c, 0x03, 1, 2, 5, 4, 0x98, 0x03, 1, 2, 5, 5, 0xf4, 0x03, 1, 2, 6, 5
};

static RAIL_ChannelConfigEntryAttr_t channelConfigEntryAttr = {
#if RAIL_SUPPORTS_OFDM_PA
  {
#ifdef RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS
    { 0xFFFFFFFFUL, 0xFFFFFFFFUL, },
#else
    { 0xFFFFFFFFUL },
#endif // RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS
    { 0xFFFFFFFFUL, 0xFFFFFFFFUL }
  }
#else // RAIL_SUPPORTS_OFDM_PA
#ifdef RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS
  { 0xFFFFFFFFUL, 0xFFFFFFFFUL, },
#else
  { 0xFFFFFFFFUL },
#endif // RADIO_CONFIG_ENABLE_IRCAL_MULTIPLE_RF_PATHS
#endif // RAIL_SUPPORTS_OFDM_PA
};

static const uint32_t phyInfo_0[] = {
  17UL,
  0x00539782UL, // 83.59183673469387
  (uint32_t) NULL,
  (uint32_t) irCalConfig,
  (uint32_t) timingConfig_0,
  0x00000000UL,
  8UL,
  0UL,
  100000UL,
  0x00F50101UL,
  0x071021DEUL,
  (uint32_t) NULL,
  (uint32_t) hfxoRetimingConfigEntries,
  (uint32_t) NULL,
  0UL,
  0UL,
  99999UL,
  (uint32_t) NULL,
  (uint32_t) NULL,
  (uint32_t) NULL,
};

static const uint32_t phyInfo_1[] = {
  17UL,
  0x00492492UL, // 73.14285714285714
  (uint32_t) NULL,
  (uint32_t) irCalConfig,
  (uint32_t) timingConfig_1,
  0x00000000UL,
  8UL,
  0UL,
  32767UL,
  0x00F50201UL,
  0x07102225UL,
  (uint32_t) NULL,
  (uint32_t) hfxoRetimingConfigEntries,
  (uint32_t) NULL,
  0UL,
  0UL,
  32765UL,
  (uint32_t) NULL,
  (uint32_t) NULL,
  (uint32_t) NULL,
};

static const uint32_t phyInfo_2[] = {
  17UL,
  0x00400000UL, // 64.0
  (uint32_t) NULL,
  (uint32_t) irCalConfig,
  (uint32_t) timingConfig_2,
  0x00000000UL,
  8UL,
  0UL,
  100000UL,
  0x00F40101UL,
  0x075020AEUL,
  (uint32_t) NULL,
  (uint32_t) hfxoRetimingConfigEntries,
  (uint32_t) NULL,
  0UL,
  0UL,
  100000UL,
  (uint32_t) NULL,
  (uint32_t) NULL,
  (uint32_t) NULL,
};

static const uint32_t phyInfo_3[] = {
  17UL,
  0x00369D03UL, // 54.61333333333334
  (uint32_t) NULL,
  (uint32_t) irCalConfig,
  (uint32_t) timingConfig_3,
  0x00000000UL,
  8UL,
  0UL,
  50000UL,
  0x00F20101UL,
  0x07101507UL,
  (uint32_t) NULL,
  (uint32_t) hfxoRetimingConfigEntries,
  (uint32_t) NULL,
  0UL,
  0UL,
  49999UL,
  (uint32_t) NULL,
  (uint32_t) NULL,
  (uint32_t) NULL,
};

const uint32_t wmbus_mode_t_modemConfigBase[] = {
  0x00014010UL, 0x000040FFUL,
  0x00024020UL, 0x00000000UL,
  /*    4024 */ 0x00000000UL,
  0x00034040UL, (uint32_t) &frameCodingTable,
  /*    4044 */ 0x00004000UL,
  /*    4048 */ 0x000007A4UL,
  0x0002405CUL, 0x00000000UL,
  /*    4060 */ 0x00000000UL,
  0x000140A8UL, 0x00000007UL,
  0x000440BCUL, 0x00000000UL,
  /*    40C0 */ 0x00000000UL,
  /*    40C4 */ 0x00000000UL,
  /*    40C8 */ 0x00000000UL,
  0x1001C020UL, 0x0007F800UL,
  0x3001C020UL, 0x002801FEUL,
  0x1001C024UL, 0x000000FFUL,
  0x3001C024UL, 0x00001300UL,
  0x0005C028UL, 0x03B380ECUL,
  /*    C02C */ 0x51407543UL,
  /*    C030 */ 0xF8000FA0UL,
  /*    C034 */ 0x00004030UL,
  /*    C038 */ 0x0007AAA8UL,
  0x0002C040UL, 0x00000000UL,
  /*    C044 */ 0x00000000UL,
  0x0001C054UL, 0x00302187UL,
  0x000DC070UL, 0x000010BAUL,
  /*    C074 */ 0x003F0000UL,
  /*    C078 */ 0x00EE008DUL,
  /*    C07C */ 0x03AC01F6UL,
  /*    C080 */ 0x079604F5UL,
  /*    C084 */ 0x0D9C09DEUL,
  /*    C088 */ 0x179311C3UL,
  /*    C08C */ 0x26F51DFEUL,
  /*    C090 */ 0x3FFF32BDUL,
  /*    C094 */ 0x1BF815FEUL,
  /*    C098 */ 0x2DB423DCUL,
  /*    C09C */ 0x3FFF39D0UL,
  /*    C0A0 */ 0x00003FFFUL,
  0x0005C0A8UL, 0x15724BBDUL,
  /*    C0AC */ 0x0518A311UL,
  /*    C0B0 */ 0x76543210UL,
  /*    C0B4 */ 0x00000A98UL,
  /*    C0B8 */ 0x00000000UL,
  0x0003C0D0UL, 0x00000000UL,
  /*    C0D4 */ 0x000A0001UL,
  /*    C0D8 */ 0x00280001UL,
  0x01010008UL, 0x000007A6UL,
  0x01010018UL, 0x00000000UL,
  0x01010020UL, 0x0000A6BCUL,
  0x0103405CUL, 0x03000000UL,
  /*    4060 */ 0x20000000UL,
  /*    4064 */ 0x00000000UL,
  0x010F409CUL, 0x00000000UL,
  /*    40A0 */ 0x00000000UL,
  /*    40A4 */ 0x00000000UL,
  /*    40A8 */ 0x00000000UL,
  /*    40AC */ 0x00000000UL,
  /*    40B0 */ 0x00000000UL,
  /*    40B4 */ 0x00000000UL,
  /*    40B8 */ 0x00000000UL,
  /*    40BC */ 0x00000000UL,
  /*    40C0 */ 0x00000000UL,
  /*    40C4 */ 0x00000000UL,
  /*    40C8 */ 0x00000000UL,
  /*    40CC */ 0x00000000UL,
  /*    40D0 */ 0x00000000UL,
  /*    40D4 */ 0x00000000UL,
  0x110140E0UL, 0x000001F8UL,
  0x310140E0UL, 0x00000201UL,
  0x01024110UL, 0x00051E33UL,
  /*    4114 */ 0x00000000UL,
  0x01074120UL, 0x00000000UL,
  /*    4124 */ 0x078304FFUL,
  /*    4128 */ 0x3AC81388UL,
  /*    412C */ 0x0C6606FFUL,
  /*    4130 */ 0x078304FFUL,
  /*    4134 */ 0x03FF1388UL,
  /*    4138 */ 0xF00A20BCUL,
  0x01024158UL, 0x00000000UL,
  /*    415C */ 0x0000FDFFUL,
  0x01014164UL, 0x0000010CUL,
  0x010B416CUL, 0x40000000UL,
  /*    4170 */ 0x00000000UL,
  /*    4174 */ 0x00000000UL,
  /*    4178 */ 0x00000000UL,
  /*    417C */ 0x00000000UL,
  /*    4180 */ 0x00000000UL,
  /*    4184 */ 0x00000101UL,
  /*    4188 */ 0x00000000UL,
  /*    418C */ 0x00000000UL,
  /*    4190 */ 0x00000000UL,
  /*    4194 */ 0x00000000UL,
  0x010241A4UL, 0x00000000UL,
  /*    41A8 */ 0x00000000UL,
  0x010141B0UL, 0x00000000UL,
  0x010641B8UL, 0x00000000UL,
  /*    41BC */ 0x00000000UL,
  /*    41C0 */ 0x003C0000UL,
  /*    41C4 */ 0x0006AAAAUL,
  /*    41C8 */ 0x00000000UL,
  /*    41CC */ 0x00000000UL,
  0x010141DCUL, 0x00000000UL,
  0x01044228UL, 0x00000000UL,
  /*    422C */ 0x40001860UL,
  /*    4230 */ 0x00000000UL,
  /*    4234 */ 0x00000000UL,
  0x0101423CUL, 0x00000000UL,
  0x01024244UL, 0x00000014UL,
  /*    4248 */ 0x00000000UL,
  0x01018010UL, 0x00000003UL,
  0x0101803CUL, 0x00000003UL,
  0x0103809CUL, 0x00000000UL,
  /*    80A0 */ 0x0003B870UL,
  /*    80A4 */ 0x0003B870UL,
  0x110180A8UL, 0x000001F6UL,
  0x310180A8UL, 0x01014201UL,
  0x110180ACUL, 0x000001F6UL,
  0x310180ACUL, 0x01014201UL,
  0x010280B0UL, 0x02000300UL,
  /*    80B4 */ 0x02000300UL,
  0x02030098UL, 0x00000000UL,
  /*    009C */ 0x04000C00UL,
  /*    00A0 */ 0x0000044CUL,
  0x020200D8UL, 0xAA400005UL,
  /*    00DC */ 0x00000188UL,
  0x120100ECUL, 0x00000FE0UL,
  0x320100ECUL, 0x5151200DUL,
  0x020100F0UL, 0x0000052BUL,
  0x02010100UL, 0x00000110UL,
  0x12010104UL, 0x00000000UL,
  0x32010104UL, 0x00000110UL,
  0x12010110UL, 0x000FFF00UL,
  0x32010110UL, 0x42000002UL,
  0x1201012CUL, 0x001FFC00UL,
  0x3201012CUL, 0x008000A9UL,
  0x02010140UL, 0x0000003FUL,
  0x02020168UL, 0x00060010UL,
  /*    016C */ 0x00062000UL,
  0x02010174UL, 0x0C100169UL,
  0x12010178UL, 0x001C0000UL,
  0x32010178UL, 0xCFE00440UL,
  0x12010180UL, 0x00000779UL,
  0x32010180UL, 0x00000006UL,
  0x02020188UL, 0x00000090UL,
  /*    018C */ 0x00000000UL,
  0x120101ACUL, 0x000001F8UL,
  0x320101ACUL, 0x00010204UL,
  0x020101B0UL, 0x00000000UL,
  0x03014FF8UL, 0x00000000UL,
  0xFFFFFFFFUL,
};

const uint32_t t_m2o_tx_o2m_rx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_0,
  0x0001400CUL, 0x000A8001UL,
  0x00044030UL, 0x00000300UL,
  /*    4034 */ 0x00000020UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000007UL,
  0x00014050UL, 0x00800900UL,
  0x00044108UL, 0x00004C09UL,
  /*    410C */ 0x00004C0FUL,
  /*    4110 */ 0x00004D09UL,
  /*    4114 */ 0x00004D0FUL,
  0x0001C03CUL, 0x0109040EUL,
  0x0006C058UL, 0xE655005BUL,
  /*    C05C */ 0x00000111UL,
  /*    C060 */ 0x524D4432UL,
  /*    C064 */ 0x00000055UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x00000520UL,
  0x0001C0CCUL, 0x000000EEUL,
  0x01074040UL, 0xF0F00000UL,
  /*    4044 */ 0xC1F00000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x00000000UL,
  /*    4050 */ 0x0081C009UL,
  /*    4054 */ 0x20000000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x000400C3UL,
  /*    406C */ 0x00000041UL,
  /*    4070 */ 0x00000030UL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x000002F0UL,
  /*    407C */ 0x00000000UL,
  /*    4080 */ 0x00002410UL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x003B0395UL,
  /*    408C */ 0x60000000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x00000000UL,
  /*    4098 */ 0x00000000UL,
  0x0101411CUL, 0x04000000UL,
  0x0106413CUL, 0x00508778UL,
  /*    4140 */ 0x001D4084UL,
  /*    4144 */ 0x123556B7UL,
  /*    4148 */ 0x5000001DUL,
  /*    414C */ 0x00003B80UL,
  /*    4150 */ 0x00000000UL,
  0x010141B4UL, 0x00200000UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x000001D0UL,
  /*    41D8 */ 0x00020000UL,
  0x011141E4UL, 0x1492F42DUL,
  /*    41E8 */ 0x003858D3UL,
  /*    41EC */ 0x007AFDBEUL,
  /*    41F0 */ 0x0041732CUL,
  /*    41F4 */ 0x0DD53E7FUL,
  /*    41F8 */ 0x03283A26UL,
  /*    41FC */ 0x302424E2UL,
  /*    4200 */ 0x1492F42DUL,
  /*    4204 */ 0x003858D3UL,
  /*    4208 */ 0x007AFDBEUL,
  /*    420C */ 0x0041732CUL,
  /*    4210 */ 0x0DD53E7FUL,
  /*    4214 */ 0x03283A26UL,
  /*    4218 */ 0x302424E2UL,
  /*    421C */ 0x00000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x8000FFFFUL,
  0x0101424CUL, 0x04000008UL,
  0x010F4330UL, 0x0146E46AUL,
  /*    4334 */ 0x304A56EBUL,
  /*    4338 */ 0x16C0C081UL,
  /*    433C */ 0x00A00C07UL,
  /*    4340 */ 0x40000000UL,
  /*    4344 */ 0xD2240012UL,
  /*    4348 */ 0x40004000UL,
  /*    434C */ 0x80000002UL,
  /*    4350 */ 0x00000020UL,
  /*    4354 */ 0x00000478UL,
  /*    4358 */ 0xDAA34004UL,
  /*    435C */ 0x9520740EUL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x001E0000UL,
  /*    4368 */ 0x9A9B5555UL,
  0x01018038UL, 0x001021DEUL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510063UL,
  0xFFFFFFFFUL,
};

const uint32_t t_m2o_rx_o2m_tx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_1,
  0x0001400CUL, 0x000A8001UL,
  0x00044030UL, 0x00000000UL,
  /*    4034 */ 0x00000000UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000000UL,
  0x00014050UL, 0x000008FFUL,
  0x00044108UL, 0x00004C09UL,
  /*    410C */ 0x00004C0FUL,
  /*    4110 */ 0x00004D09UL,
  /*    4114 */ 0x00004D0FUL,
  0x0001C03CUL, 0x01090E35UL,
  0x0006C058UL, 0xE654005AUL,
  /*    C05C */ 0x0000010EUL,
  /*    C060 */ 0x514D4432UL,
  /*    C064 */ 0x00000054UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x00000520UL,
  0x0001C0CCUL, 0x000000EEUL,
  0x01074040UL, 0x11D00000UL,
  /*    4044 */ 0x00000000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x00000012UL,
  /*    4050 */ 0x0081C011UL,
  /*    4054 */ 0x20000000UL,
  /*    4058 */ 0x000AD000UL,
  0x010D4068UL, 0x00C270BEUL,
  /*    406C */ 0x00000041UL,
  /*    4070 */ 0x00000030UL,
  /*    4074 */ 0x000F0011UL,
  /*    4078 */ 0x00025A47UL,
  /*    407C */ 0x0003FFFFUL,
  /*    4080 */ 0x00001412UL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x00080392UL,
  /*    408C */ 0x62000000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x00000000UL,
  /*    4098 */ 0x00000000UL,
  0x0101411CUL, 0x04000000UL,
  0x0106413CUL, 0x00508894UL,
  /*    4140 */ 0x00A14284UL,
  /*    4144 */ 0x123556B7UL,
  /*    4148 */ 0x500000A1UL,
  /*    414C */ 0x00003B80UL,
  /*    4150 */ 0x00000000UL,
  0x010141B4UL, 0x00200000UL,
  0x010341D0UL, 0xAAAAAAABUL,
  /*    41D4 */ 0x000001D0UL,
  /*    41D8 */ 0x00020000UL,
  0x011141E4UL, 0x1492F42DUL,
  /*    41E8 */ 0x003858D3UL,
  /*    41EC */ 0x007AFDBEUL,
  /*    41F0 */ 0x0041732CUL,
  /*    41F4 */ 0x0DD53E7FUL,
  /*    41F8 */ 0x03283A26UL,
  /*    41FC */ 0x302424E2UL,
  /*    4200 */ 0x0C81901EUL,
  /*    4204 */ 0x0006490CUL,
  /*    4208 */ 0x006DDFA8UL,
  /*    420C */ 0x00B10BC0UL,
  /*    4210 */ 0x00A53D18UL,
  /*    4214 */ 0x05020AE8UL,
  /*    4218 */ 0x1DD71B27UL,
  /*    421C */ 0x80000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x8000FFFFUL,
  0x0101424CUL, 0x04000008UL,
  0x010F4330UL, 0x09C2422BUL,
  /*    4334 */ 0x80E38121UL,
  /*    4338 */ 0xF6C0C081UL,
  /*    433C */ 0x00A40C06UL,
  /*    4340 */ 0x00000000UL,
  /*    4344 */ 0xD2600023UL,
  /*    4348 */ 0x4000C350UL,
  /*    434C */ 0x80000002UL,
  /*    4350 */ 0x00000020UL,
  /*    4354 */ 0x00000478UL,
  /*    4358 */ 0xDA834504UL,
  /*    435C */ 0x9518840FUL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x00210000UL,
  /*    4368 */ 0x9AC85555UL,
  0x01018038UL, 0x00102225UL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510063UL,
  0xFFFFFFFFUL,
};

const uint32_t t_m2o_rx_t_c_combo_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_0,
  0x0001400CUL, 0x000A8001UL,
  0x00044030UL, 0x00000300UL,
  /*    4034 */ 0x00000020UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000007UL,
  0x00014050UL, 0x00800900UL,
  0x00044108UL, 0x00004C09UL,
  /*    410C */ 0x00004C0FUL,
  /*    4110 */ 0x00004D09UL,
  /*    4114 */ 0x00004D0FUL,
  0x0001C03CUL, 0x0109040EUL,
  0x0006C058UL, 0xE655005BUL,
  /*    C05C */ 0x00000111UL,
  /*    C060 */ 0x524D4432UL,
  /*    C064 */ 0x00000055UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x00000520UL,
  0x0001C0CCUL, 0x000000EEUL,
  0x01074040UL, 0xF0F00000UL,
  /*    4044 */ 0xC1F00000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x00000000UL,
  /*    4050 */ 0x0081C009UL,
  /*    4054 */ 0x20000000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x000400C3UL,
  /*    406C */ 0x00000041UL,
  /*    4070 */ 0x00000030UL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x000002F0UL,
  /*    407C */ 0x00000000UL,
  /*    4080 */ 0x00002410UL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x003B0395UL,
  /*    408C */ 0x60000000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x00000000UL,
  /*    4098 */ 0x00000000UL,
  0x0101411CUL, 0x04000000UL,
  0x0106413CUL, 0x00508778UL,
  /*    4140 */ 0x001C3C7CUL,
  /*    4144 */ 0x123556B7UL,
  /*    4148 */ 0x5000001CUL,
  /*    414C */ 0x00003B80UL,
  /*    4150 */ 0x00000000UL,
  0x010141B4UL, 0x00200000UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x000001D0UL,
  /*    41D8 */ 0x00020000UL,
  0x011141E4UL, 0x1492F42DUL,
  /*    41E8 */ 0x003858D3UL,
  /*    41EC */ 0x007AFDBEUL,
  /*    41F0 */ 0x0041732CUL,
  /*    41F4 */ 0x0DD53E7FUL,
  /*    41F8 */ 0x03283A26UL,
  /*    41FC */ 0x302424E2UL,
  /*    4200 */ 0x1492F42DUL,
  /*    4204 */ 0x003858D3UL,
  /*    4208 */ 0x007AFDBEUL,
  /*    420C */ 0x0041732CUL,
  /*    4210 */ 0x0DD53E7FUL,
  /*    4214 */ 0x03283A26UL,
  /*    4218 */ 0x302424E2UL,
  /*    421C */ 0x00000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x8000FFFFUL,
  0x0101424CUL, 0x04000008UL,
  0x010F4330UL, 0x0146E46AUL,
  /*    4334 */ 0x304A56EBUL,
  /*    4338 */ 0x16C0C081UL,
  /*    433C */ 0x00A00C07UL,
  /*    4340 */ 0x40000000UL,
  /*    4344 */ 0xD2240012UL,
  /*    4348 */ 0x40004000UL,
  /*    434C */ 0x80000002UL,
  /*    4350 */ 0x00000020UL,
  /*    4354 */ 0x00000478UL,
  /*    4358 */ 0xDAA34004UL,
  /*    435C */ 0x9520700EUL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x001C0000UL,
  /*    4368 */ 0x9AA85555UL,
  0x01018038UL, 0x001021DEUL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510063UL,
  0xFFFFFFFFUL,
};

const uint32_t c_frame_a_m2o_tx_o2m_rx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_2,
  0x0001400CUL, 0x000A8001UL,
  0x00044030UL, 0x00000000UL,
  /*    4034 */ 0x00000000UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000000UL,
  0x00014050UL, 0x00000000UL,
  0x00044108UL, 0x00004C09UL,
  /*    410C */ 0x00004C0FUL,
  /*    4110 */ 0x00004D09UL,
  /*    4114 */ 0x00004D0FUL,
  0x0001C03CUL, 0x00000000UL,
  0x0006C058UL, 0xE657005EUL,
  /*    C05C */ 0x0000011AUL,
  /*    C060 */ 0x55504734UL,
  /*    C064 */ 0x00000057UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x000004A0UL,
  0x0001C0CCUL, 0x000000EEUL,
  0x01074040UL, 0x70100000UL,
  /*    4044 */ 0x00000000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x00000000UL,
  /*    4050 */ 0x0082C019UL,
  /*    4054 */ 0x00000000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x00FC2FFDUL,
  /*    406C */ 0x00000C41UL,
  /*    4070 */ 0x0000002AUL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x02CCAAF0UL,
  /*    407C */ 0x02F0AAF0UL,
  /*    4080 */ 0x00000E4DUL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x00080393UL,
  /*    408C */ 0x62040000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x00000000UL,
  /*    4098 */ 0x00000000UL,
  0x0101411CUL, 0x8CA49000UL,
  0x0106413CUL, 0x0050AE4BUL,
  /*    4140 */ 0x40AA64E5UL,
  /*    4144 */ 0x543D54CDUL,
  /*    4148 */ 0x43F91EB0UL,
  /*    414C */ 0x00403B89UL,
  /*    4150 */ 0x800003C0UL,
  0x010141B4UL, 0xC03F9658UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x806E01E6UL,
  /*    41D8 */ 0x00C60007UL,
  0x011141E4UL, 0x11E107F9UL,
  /*    41E8 */ 0x0005E9CCUL,
  /*    41EC */ 0x006C8E32UL,
  /*    41F0 */ 0x004EB132UL,
  /*    41F4 */ 0x0E1C0182UL,
  /*    41F8 */ 0x0265F774UL,
  /*    41FC */ 0x3350259BUL,
  /*    4200 */ 0x0C81901EUL,
  /*    4204 */ 0x0006490CUL,
  /*    4208 */ 0x006DDFA8UL,
  /*    420C */ 0x00B10BC0UL,
  /*    4210 */ 0x00A53D18UL,
  /*    4214 */ 0x05020AE8UL,
  /*    4218 */ 0x1DD71B27UL,
  /*    421C */ 0x80000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x0000004DUL,
  0x0101424CUL, 0x04060008UL,
  0x010F4330UL, 0x00000000UL,
  /*    4334 */ 0x00000000UL,
  /*    4338 */ 0x00000000UL,
  /*    433C */ 0x00000000UL,
  /*    4340 */ 0x00000000UL,
  /*    4344 */ 0x00000000UL,
  /*    4348 */ 0x00000000UL,
  /*    434C */ 0x00000000UL,
  /*    4350 */ 0x00000000UL,
  /*    4354 */ 0x00000000UL,
  /*    4358 */ 0x00000000UL,
  /*    435C */ 0x38000000UL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x00000000UL,
  /*    4368 */ 0x58FF0000UL,
  0x01018038UL, 0x001020AEUL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510063UL,
  0xFFFFFFFFUL,
};

const uint32_t c_frame_b_m2o_tx_o2m_rx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_2,
  0x0001400CUL, 0x001C8001UL,
  0x00044030UL, 0x00000000UL,
  /*    4034 */ 0x00000000UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000000UL,
  0x00014050UL, 0x00000000UL,
  0x00044108UL, 0x00004C7DUL,
  /*    410C */ 0x00004CFFUL,
  /*    4110 */ 0x00004D7DUL,
  /*    4114 */ 0x00004DFFUL,
  0x0001C03CUL, 0x00000000UL,
  0x0006C058UL, 0xE657005EUL,
  /*    C05C */ 0x0000011AUL,
  /*    C060 */ 0x55504734UL,
  /*    C064 */ 0x00000057UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x000004A0UL,
  0x0001C0CCUL, 0x000000EEUL,
  0x01074040UL, 0x70100000UL,
  /*    4044 */ 0x00000000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x00000000UL,
  /*    4050 */ 0x0082C019UL,
  /*    4054 */ 0x00000000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x00FC2FFDUL,
  /*    406C */ 0x00000C41UL,
  /*    4070 */ 0x0000002AUL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x02F0AAF0UL,
  /*    407C */ 0x02CCAAF0UL,
  /*    4080 */ 0x00000E4DUL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x00080393UL,
  /*    408C */ 0x62040000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x00000000UL,
  /*    4098 */ 0x00000000UL,
  0x0101411CUL, 0x8CA49000UL,
  0x0106413CUL, 0x0050AE4BUL,
  /*    4140 */ 0x40AA64E5UL,
  /*    4144 */ 0x543D543DUL,
  /*    4148 */ 0x43F91EB0UL,
  /*    414C */ 0x00403B89UL,
  /*    4150 */ 0x800003C0UL,
  0x010141B4UL, 0xC03F9658UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x806E01E6UL,
  /*    41D8 */ 0x00C60007UL,
  0x011141E4UL, 0x11E107F9UL,
  /*    41E8 */ 0x0005E9CCUL,
  /*    41EC */ 0x006C8E32UL,
  /*    41F0 */ 0x004EB132UL,
  /*    41F4 */ 0x0E1C0182UL,
  /*    41F8 */ 0x0265F774UL,
  /*    41FC */ 0x3350259BUL,
  /*    4200 */ 0x0C81901EUL,
  /*    4204 */ 0x0006490CUL,
  /*    4208 */ 0x006DDFA8UL,
  /*    420C */ 0x00B10BC0UL,
  /*    4210 */ 0x00A53D18UL,
  /*    4214 */ 0x05020AE8UL,
  /*    4218 */ 0x1DD71B27UL,
  /*    421C */ 0x80000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x0000004DUL,
  0x0101424CUL, 0x04060008UL,
  0x010F4330UL, 0x00000000UL,
  /*    4334 */ 0x00000000UL,
  /*    4338 */ 0x00000000UL,
  /*    433C */ 0x00000000UL,
  /*    4340 */ 0x00000000UL,
  /*    4344 */ 0x00000000UL,
  /*    4348 */ 0x00000000UL,
  /*    434C */ 0x00000000UL,
  /*    4350 */ 0x00000000UL,
  /*    4354 */ 0x00000000UL,
  /*    4358 */ 0x00000000UL,
  /*    435C */ 0x38000000UL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x00000000UL,
  /*    4368 */ 0x58FF0000UL,
  0x01018038UL, 0x001020AEUL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510063UL,
  0xFFFFFFFFUL,
};

const uint32_t c_frame_a_m2o_rx_o2m_tx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_3,
  0x0001400CUL, 0x000A8001UL,
  0x00044030UL, 0x00000000UL,
  /*    4034 */ 0x00000000UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000000UL,
  0x00014050UL, 0x00000000UL,
  0x00044108UL, 0x00004C09UL,
  /*    410C */ 0x00004C0FUL,
  /*    4110 */ 0x00004D09UL,
  /*    4114 */ 0x00004D0FUL,
  0x0001C03CUL, 0x00000000UL,
  0x0006C058UL, 0xE6880092UL,
  /*    C05C */ 0x000001B6UL,
  /*    C060 */ 0x837C6E50UL,
  /*    C064 */ 0x00000088UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x000004C0UL,
  0x0001C0CCUL, 0x000001FEUL,
  0x01074040UL, 0x30B00000UL,
  /*    4044 */ 0x00000000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x04000000UL,
  /*    4050 */ 0x0082C019UL,
  /*    4054 */ 0x20005000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x00FE60BDUL,
  /*    406C */ 0x00000C41UL,
  /*    4070 */ 0x00000070UL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x02CCAAF0UL,
  /*    407C */ 0x02F0AAF0UL,
  /*    4080 */ 0x00000E35UL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x002B037FUL,
  /*    408C */ 0x62040000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x22140A04UL,
  /*    4098 */ 0x4F4A4132UL,
  0x0101411CUL, 0x8CF3D000UL,
  0x0106413CUL, 0x0050541CUL,
  /*    4140 */ 0x409A48B7UL,
  /*    4144 */ 0x543D54CDUL,
  /*    4148 */ 0x43F9FF9BUL,
  /*    414C */ 0x00403B89UL,
  /*    4150 */ 0x800003C0UL,
  0x010141B4UL, 0xC03F9658UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x805780E6UL,
  /*    41D8 */ 0x00C60007UL,
  0x011141E4UL, 0x11E107F9UL,
  /*    41E8 */ 0x0005E9CCUL,
  /*    41EC */ 0x006C8E32UL,
  /*    41F0 */ 0x004EB132UL,
  /*    41F4 */ 0x0E1C0182UL,
  /*    41F8 */ 0x0265F774UL,
  /*    41FC */ 0x3350259BUL,
  /*    4200 */ 0x0A00ABFFUL,
  /*    4204 */ 0x000FF15CUL,
  /*    4208 */ 0x000241D3UL,
  /*    420C */ 0x00B1ED95UL,
  /*    4210 */ 0x0FD87B19UL,
  /*    4214 */ 0x04B90812UL,
  /*    4218 */ 0x1F6D1BEAUL,
  /*    421C */ 0x80000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x0000004DUL,
  0x0101424CUL, 0x14060008UL,
  0x010F4330UL, 0x00000000UL,
  /*    4334 */ 0x00000000UL,
  /*    4338 */ 0x00000000UL,
  /*    433C */ 0x00000000UL,
  /*    4340 */ 0x00000000UL,
  /*    4344 */ 0x00000000UL,
  /*    4348 */ 0x00000000UL,
  /*    434C */ 0x00000000UL,
  /*    4350 */ 0x00000000UL,
  /*    4354 */ 0x00000000UL,
  /*    4358 */ 0x00000000UL,
  /*    435C */ 0x38000000UL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x00000000UL,
  /*    4368 */ 0x58FF0000UL,
  0x01018038UL, 0x00101507UL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510062UL,
  0xFFFFFFFFUL,
};

const uint32_t c_frame_b_m2o_rx_o2m_tx_modemConfig[] = {
  0x03014FFCUL, (uint32_t) &phyInfo_3,
  0x0001400CUL, 0x001C8001UL,
  0x00044030UL, 0x00000000UL,
  /*    4034 */ 0x00000000UL,
  /*    4038 */ 0x00000000UL,
  /*    403C */ 0x00000000UL,
  0x00014050UL, 0x00000000UL,
  0x00044108UL, 0x00004C7DUL,
  /*    410C */ 0x00004CFFUL,
  /*    4110 */ 0x00004D7DUL,
  /*    4114 */ 0x00004DFFUL,
  0x0001C03CUL, 0x00000000UL,
  0x0006C058UL, 0xE6880092UL,
  /*    C05C */ 0x000001B6UL,
  /*    C060 */ 0x837C6E50UL,
  /*    C064 */ 0x00000088UL,
  /*    C068 */ 0x0002C688UL,
  /*    C06C */ 0x000004C0UL,
  0x0001C0CCUL, 0x000001FEUL,
  0x01074040UL, 0x30B00000UL,
  /*    4044 */ 0x00000000UL,
  /*    4048 */ 0x00000010UL,
  /*    404C */ 0x04000000UL,
  /*    4050 */ 0x0082C019UL,
  /*    4054 */ 0x20005000UL,
  /*    4058 */ 0x00000000UL,
  0x010D4068UL, 0x00FE60BDUL,
  /*    406C */ 0x00000C41UL,
  /*    4070 */ 0x00000070UL,
  /*    4074 */ 0x00130012UL,
  /*    4078 */ 0x02F0AAF0UL,
  /*    407C */ 0x02CCAAF0UL,
  /*    4080 */ 0x00000E35UL,
  /*    4084 */ 0x00000000UL,
  /*    4088 */ 0x002B037FUL,
  /*    408C */ 0x62040000UL,
  /*    4090 */ 0x00000000UL,
  /*    4094 */ 0x22140A04UL,
  /*    4098 */ 0x4F4A4132UL,
  0x0101411CUL, 0x8CF3D000UL,
  0x0106413CUL, 0x0050541CUL,
  /*    4140 */ 0x409A48B7UL,
  /*    4144 */ 0x543D543DUL,
  /*    4148 */ 0x43F9FF9BUL,
  /*    414C */ 0x00403B89UL,
  /*    4150 */ 0x800003C0UL,
  0x010141B4UL, 0xC03F9658UL,
  0x010341D0UL, 0x55555555UL,
  /*    41D4 */ 0x805780E6UL,
  /*    41D8 */ 0x00C60007UL,
  0x011141E4UL, 0x11E107F9UL,
  /*    41E8 */ 0x0005E9CCUL,
  /*    41EC */ 0x006C8E32UL,
  /*    41F0 */ 0x004EB132UL,
  /*    41F4 */ 0x0E1C0182UL,
  /*    41F8 */ 0x0265F774UL,
  /*    41FC */ 0x3350259BUL,
  /*    4200 */ 0x0A00ABFFUL,
  /*    4204 */ 0x000FF15CUL,
  /*    4208 */ 0x000241D3UL,
  /*    420C */ 0x00B1ED95UL,
  /*    4210 */ 0x0FD87B19UL,
  /*    4214 */ 0x04B90812UL,
  /*    4218 */ 0x1F6D1BEAUL,
  /*    421C */ 0x80000000UL,
  /*    4220 */ 0x00000000UL,
  /*    4224 */ 0x0000004DUL,
  0x0101424CUL, 0x14060008UL,
  0x010F4330UL, 0x00000000UL,
  /*    4334 */ 0x00000000UL,
  /*    4338 */ 0x00000000UL,
  /*    433C */ 0x00000000UL,
  /*    4340 */ 0x00000000UL,
  /*    4344 */ 0x00000000UL,
  /*    4348 */ 0x00000000UL,
  /*    434C */ 0x00000000UL,
  /*    4350 */ 0x00000000UL,
  /*    4354 */ 0x00000000UL,
  /*    4358 */ 0x00000000UL,
  /*    435C */ 0x38000000UL,
  /*    4360 */ 0x00000000UL,
  /*    4364 */ 0x00000000UL,
  /*    4368 */ 0x58FF0000UL,
  0x01018038UL, 0x00101507UL,
  0x12010150UL, 0x0000C000UL,
  0x32010150UL, 0x00510062UL,
  0xFFFFFFFFUL,
};

const RAIL_ChannelConfigEntry_t wmbus_mode_t_channels[] = {
  {
    .phyConfigDeltaAdd = t_m2o_tx_o2m_rx_modemConfig,
    .baseFrequency = 868950000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 0,
    .channelNumberStart = 0,
    .channelNumberEnd = 0,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = t_m2o_rx_o2m_tx_modemConfig,
    .baseFrequency = 868300000,
    .channelSpacing = 300000,
    .physicalChannelOffset = 1,
    .channelNumberStart = 1,
    .channelNumberEnd = 1,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = t_m2o_rx_t_c_combo_modemConfig,
    .baseFrequency = 868950000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 2,
    .channelNumberStart = 2,
    .channelNumberEnd = 2,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = c_frame_a_m2o_tx_o2m_rx_modemConfig,
    .baseFrequency = 868950000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 3,
    .channelNumberStart = 3,
    .channelNumberEnd = 3,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = c_frame_b_m2o_tx_o2m_rx_modemConfig,
    .baseFrequency = 868950000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 4,
    .channelNumberStart = 4,
    .channelNumberEnd = 4,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = c_frame_a_m2o_rx_o2m_tx_modemConfig,
    .baseFrequency = 869525000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 5,
    .channelNumberStart = 5,
    .channelNumberEnd = 5,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
  {
    .phyConfigDeltaAdd = c_frame_b_m2o_rx_o2m_tx_modemConfig,
    .baseFrequency = 869525000,
    .channelSpacing = 1000000,
    .physicalChannelOffset = 6,
    .channelNumberStart = 6,
    .channelNumberEnd = 6,
    .maxPower = RAIL_TX_POWER_MAX,
    .attr = &channelConfigEntryAttr,
#ifdef RADIO_CONFIG_ENABLE_CONC_PHY
    .entryType = 0,
#endif
#ifdef RADIO_CONFIG_ENABLE_STACK_INFO
    .stackInfo = NULL,
#endif
    .alternatePhy = NULL,
  },
};

const RAIL_ChannelConfig_t wmbus_mode_t_channelConfig = {
  .phyConfigBase = wmbus_mode_t_modemConfigBase,
  .phyConfigDeltaSubtract = NULL,
  .configs = wmbus_mode_t_channels,
  .length = 7U,
  .signature = 0UL,
  .xtalFrequencyHz = 39000000UL,
};

const RAIL_ChannelConfig_t *channelConfigs[] = {
  &wmbus_mode_t_channelConfig,
  NULL
};

uint32_t wmbusAccelerationBuffer[299];
