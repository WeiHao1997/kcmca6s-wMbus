/*
 * ql_nvm.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_NVM_H__
#define __QL_NVM_H__

#include "ql_include.h"

int16_t ql_nvm_init(void);

int16_t ql_nvm_deinit(void);

int16_t ql_nvm_write_data(uint32_t data_key, uint8_t *data, uint32_t data_len);

int16_t ql_nvm_read_data(uint32_t data_key, uint8_t *data, uint32_t *data_len);

#endif /* __QL_NVM_H__ */
