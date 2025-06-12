/*
 * ql_crc.h
 *
 *  Created on: 2024年10月29日
 *      Author: Administrator
 */

#ifndef __QL_CRC_H__
#define __QL_CRC_H__

#include "ql_include.h"

uint16_t crc16_calculate(const uint8_t *cp, int32_t len);
uint16_t crc16_x25(const uint8_t *data, int length);
uint32_t Ql_Check_CRC32(uint32_t Val, const uint8_t* Data, int32_t Offset, int32_t Length);

#endif /* __QL_CRC_H__ */
