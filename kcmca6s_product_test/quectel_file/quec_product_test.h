/*
 * quec_product_test.h
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

#ifndef QUECTEL_FILE_QUEC_PRODUCT_TEST_H_
#define QUECTEL_FILE_QUEC_PRODUCT_TEST_H_

#define QUEC_PD_CTUNE_MIN  0
#define QUEC_PD_CTUNE_MAX  100

#define QUEC_PD_SN_MAX_SIZE  32

#define QUEC_PD_SN_OPTION_TYPE      5
#define QUEC_PD_SN_OPTION_GET       0
#define QUEC_PD_SN_OPTION_SET       1

typedef enum
{
  EM_UART_AT_MODE,
  EM_UART_PROD_MODE = 5,
  EM_UART_FTM_MODE = 6,
}EM_UART_WORK_MODE;

#define QUEC_VERSION "KCMCA6SAAR01A01K02"
#define QUEC_SUB_VERSION "V01"
#define QUEC_PRODUCT_VERSION "KCMCA6SAAR01A01K02"

void quec_uart_pd_send_string(char* str , uint32_t len);
void quec_prod_param_init(void);
EM_UART_WORK_MODE app_quec_product_get_workmode(void);
int quec_product_command_task(char *cmd, uint16_t cmdlen);

int quec_sr_ati(void);
int quec_sr_prod_entry(int argc, char **argv);
int quec_sr_prod_sn(int argc, char **argv);
int quec_sr_prod_version(int argc, char **argv);
int quec_sr_ftm_mode(int argc, char **argv);
int quec_sr_csub(int argc, char **argv);
int quec_sr_version(int argc, char **argv);
int quec_sr_qsub(int argc, char **argv);
int quec_sr_version(int argc, char **argv);
int quec_sr_qctune(int argc, char **argv);
int quec_sr_ctune_get(void);

int quec_sr_prod_update_sn(uint8_t *sn , uint8_t sn_length);
#endif

