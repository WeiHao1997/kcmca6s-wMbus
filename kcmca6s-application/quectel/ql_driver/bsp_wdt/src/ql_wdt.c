#include "ql_wdt.h" 

void ql_wdg_init(void)
{
    WDOG_Init_TypeDef wdg = WDOG_INIT_DEFAULT; //default 8s

    CMU_ClockEnable(cmuClock_WDOG0, true);

    wdg.perSel = wdogPeriod_128k; //128k/32k(LFRCO)
    wdg.em2Run = true;
    WDOGn_Init(WDOG0,&wdg);
    WDOGn_Enable(WDOG0,true);
}


void ql_wdg_feed(void)
{
    WDOGn_Feed(WDOG0);
}