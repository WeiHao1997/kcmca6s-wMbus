#include "sl_event_handler.h"

#include "em_chip.h"
#include "sl_interrupt_manager.h"
#include "sl_memory_manager.h"

void sl_platform_init(void)
{
  CHIP_Init();
  sl_interrupt_manager_init();
  sl_memory_init();
}

void sl_driver_init(void)
{
}

void sl_service_init(void)
{
}

void sl_stack_init(void)
{
}

void sl_internal_app_init(void)
{
}

void sl_platform_process_action(void)
{
}

void sl_service_process_action(void)
{
}

void sl_stack_process_action(void)
{
}

void sl_internal_app_process_action(void)
{
}

