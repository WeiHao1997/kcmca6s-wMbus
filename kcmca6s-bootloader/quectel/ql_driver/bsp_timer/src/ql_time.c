/*
 * quec_softtime.c
 *
 *  Created on: 2024年11月5日
 *      Author: vic.cheng
 */
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT
#include "ql_time.h"




static volatile uint32_t s_uiDelayCount = 0;
static volatile uint8_t s_ucTimeOutFlag = 0;


static SOFT_TMR s_tTmr[TMR_COUNT];

volatile int32_t g_iRunTime = 0;

static void drv_timer_SoftTimerDec(SOFT_TMR *_tmr);

#define OUT_FREQ 1500  //1ms

void drv_soft_time_hook(void)
{
  uint8_t i;

  if (s_uiDelayCount > 0)
  {
    if (--s_uiDelayCount == 0)
    {
      s_ucTimeOutFlag = 1;
    }
  }


  for (i = 0; i < TMR_COUNT; i++)
  {
    drv_timer_SoftTimerDec(&s_tTmr[i]);
  }

}


static void drv_timer_SoftTimerDec(SOFT_TMR *_tmr)
{
  if (_tmr->Count > 0)
  {
    if (--_tmr->Count == 0)
    {
      _tmr->Flag = 1;

      _tmr->callback();

      if(_tmr->Mode == TMR_AUTO_MODE)
      {
        _tmr->Count = _tmr->PreLoad;
      }
    }
  }
}

void drv_timer_start(uint8_t _id, uint32_t _period , user_timer_callback_t call)
{
  if (_id >= TMR_COUNT)
  {
    return ;
  }
  s_tTmr[_id].Count = _period;
  s_tTmr[_id].PreLoad = _period;
  s_tTmr[_id].Flag = 0;
  s_tTmr[_id].Mode = TMR_ONCE_MODE;
  s_tTmr[_id].callback = call;
}

void drv_timer_StartAuto(uint8_t _id, uint32_t _period , user_timer_callback_t call)
{
  if (_id >= TMR_COUNT)
  {
    return ;
  }


  s_tTmr[_id].Count = _period;
  s_tTmr[_id].PreLoad = _period;
  s_tTmr[_id].Flag = 0;
  s_tTmr[_id].Mode = TMR_AUTO_MODE;
  s_tTmr[_id].callback = call;

 // ENABLE_INT();
}

void drv_timer_stop(uint8_t _id)
{
  if (_id >= TMR_COUNT)
  {
    return;
  }

  s_tTmr[_id].Count = 0;
  s_tTmr[_id].Flag = 0;
  s_tTmr[_id].Mode = TMR_ONCE_MODE;
}


uint8_t drv_timer_check(uint8_t _id)
{
  if (_id >= TMR_COUNT)
  {
    return 0;
  }

  if (s_tTmr[_id].Flag == 1)
  {
    s_tTmr[_id].Flag = 0;

    return 1;
  }
  else
  {
    return 0;
  }
}

int32_t drv_timer_GetRunTime(void)
{
  int32_t runtime;

  runtime = g_iRunTime;

  return runtime;
}

int32_t drv_timer_CheckRunTime(int32_t _LastTime)
{
  int32_t now_time;
  int32_t time_diff;

  now_time = g_iRunTime;


  if (now_time >= _LastTime)
  {
    time_diff = now_time - _LastTime;
  }
  else
  {
    time_diff = 0x7FFFFFFF - _LastTime + now_time;
  }

  return time_diff;
}

void initTIMER(void)
{
  uint32_t timerFreq, topValue;
  TIMER_Init_TypeDef timerInit = TIMER_INIT_DEFAULT;

  CMU_ClockEnable(cmuClock_TIMER0, true);

  // Don't start counter on initialization
  timerInit.enable = false;

  TIMER_Init(TIMER0, &timerInit);

  /*
   * Set the TOP register value.  Each time the counter overflows TOP
   * is one half of the signal period.
   */
  timerFreq = CMU_ClockFreqGet(cmuClock_TIMER0) / (timerInit.prescale + 1);
  topValue = timerFreq / (OUT_FREQ) - 1;
  TIMER_TopSet (TIMER0, topValue);

  TIMER_IntEnable(TIMER0,true);
  // Enable NVIC USART sources
  NVIC_ClearPendingIRQ(TIMER0_IRQn);
  NVIC_EnableIRQ(TIMER0_IRQn);

  // Now start the TIMER
  TIMER_Enable(TIMER0, true);
}
