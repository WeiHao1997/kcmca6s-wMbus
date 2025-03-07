/*
 * ql_crc16.h
 *
 *  Created on: 2024年10月29日
 *      Author: Administrator
 */

#ifndef __QL_CRC16_H__
#define __QL_CRC16_H__

#include "ql_include.h"

uint16_t crc16_calculate(const uint8_t *cp, int32_t len);
uint16_t crc16_x25(const uint8_t *data, int length);
unsigned int Ql_Check_CRC32(unsigned int InitVal, const unsigned char* pData, const unsigned int Length);

#endif /* __QL_CRC16_H__ */
