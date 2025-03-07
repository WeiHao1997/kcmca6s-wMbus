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

#endif /* __QL_CRC16_H__ */
