/*
 * app_debug.h
 *
 *  Created on: 2024年12月5日
 *      Author: vic.cheng
 */

#ifndef APP_DEBUG_H_
#define APP_DEBUG_H_

int32_t app_debug_init(void);
void ql_printf(const char *fmt, ...);

#define app_log(format, ...)        ql_printf(format , ##__VA_ARGS__)


#endif /* APP_DEBUG_H_ */
