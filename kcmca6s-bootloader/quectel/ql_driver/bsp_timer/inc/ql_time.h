/*
 * quec_softtime.h
 *
 *  Created on: 2024年11月5日
 *      Author: vic.cheng
 */

#ifndef QUECTEL_AT_QUEC_SOFTTIME_H_
#define QUECTEL_AT_QUEC_SOFTTIME_H_

#include "ql_include.h"

#define USE_SYSTICK_TIME 0

#define TMR_COUNT 1

#define UART_RECV          (0)

typedef enum
{
  TMR_ONCE_MODE = 0,
  TMR_AUTO_MODE = 1
}TMR_MODE_E;


typedef void(* user_timer_callback_t)(void);

typedef struct
{
  volatile uint8_t Mode;
  volatile uint8_t Flag;
  volatile uint32_t Count;
  volatile uint32_t PreLoad;
  user_timer_callback_t callback;
}SOFT_TMR;

void initTIMER(void);

void drv_timer_stop(uint8_t _id);
void drv_soft_time_hook(void);
void drv_timer_start(uint8_t _id, uint32_t _period , user_timer_callback_t call);
void drv_timer_StartAuto(uint8_t _id, uint32_t _period , user_timer_callback_t call);
#endif /* QUECTEL_AT_QUEC_SOFTTIME_H_ */
