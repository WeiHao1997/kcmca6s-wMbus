/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/
#ifndef __QL_SUPPORT_SDK__
#include "ql_main.h"
#endif /* __QL_SUPPORT_SDK__ */

void app_init(void)
{
#ifndef __QL_SUPPORT_SDK__
    ql_main();
#endif /* __QL_SUPPORT_SDK__ */
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
#ifndef __QL_SUPPORT_SDK__
    ql_main_loop();
#endif /* __QL_SUPPORT_SDK__ */
}
