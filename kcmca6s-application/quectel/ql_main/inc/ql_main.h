/*
 * ql_main.h
 *
 *  Created on: 2024年10月28日
 *      Author: kahn.wei
 */

#ifndef __QL_MAIN_H__
#define __QL_MAIN_H__

#include "ql_include.h"

typedef struct
{
    short int Year;
    short int Mon;
    short int Day;
    short int Hour;
    short int Min;
    short int Sec;
    short int Millisec;
} ql_build_time_t;

void ql_main(void);

void ql_handle_loop(void);

#endif /* __QL_MAIN_H__ */