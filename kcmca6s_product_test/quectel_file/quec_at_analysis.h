/*
 * quec_at_analysis.h
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

#ifndef QUECTEL_FILE_QUEC_AT_ANALYSIS_H_
#define QUECTEL_FILE_QUEC_AT_ANALYSIS_H_

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"


/*
 * MACRO DEFINITIONS
 ****************************************************************************************
 */

#define QUEC_PRODUCT_AT_PARAM_MAX  5
#define QUEC_PRODUCT_AT_PARAM_SIZE 64

#define ATCMD_QUEC_AT_HEAD      "AT+"
#define ATCMD_QUEC_ATI_HEAD     "ATI"

#define ATCMD_PD_AT_PROD_ERROR_RESP     "ERROR\r\n"
#define ATCMD_PD_AT_PROD_ERROR_NO_ERROR "OK\r\n"

#define ATSVR_RET_CHAR                    '\r'
#define ATSVR_END_CHAR                    '\n'

#define ATSVR_END_STRING                  '\r\n'


#define AT_NULL                 0x00
#define AT_COMMAND              0x01
#define AT_MESSAGE              0x02
#define AT_PD_COMMAND           0x03


#define AT_RECEIVE_DATA_ERR     0x10
#define AT_COMMAND_ERR          0x11
#define AT_PARAM_ERR            0x12

typedef struct{
  unsigned inArg : 1;
  unsigned done : 1;
  unsigned isD : 2;
  unsigned limQ : 1;
  unsigned ischeck:1;
  unsigned isquote:1;
}_atsvr_handle_input_stat;


typedef struct
{
    const char *cmd;
    int (*Function)(int argc, char **argv);
}TypePDCommandFunction;

extern uint8_t g_AtIsEqual;
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

int quec_at_command_handle(char *inbuf,int cmdlen);

#endif /* QUECTEL_FILE_QUEC_AT_ANALYSIS_H_ */
