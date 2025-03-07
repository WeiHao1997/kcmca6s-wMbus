/*
 * quec_product_test.c
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

/*
 * quec_product_test.c
 *
 *  Created on: 2023年9月22日
 *      Author: vic.cheng
 */
#include <stdlib.h>
#include <string.h>
#include "em_common.h"
#include "em_core.h"
#include "sl_status.h"
#include "em_msc.h"
#include "app.h"
#include "em_gpio.h"
#include "em_msc.h"
#include "quectel_file/quec_product_test.h"
#include "quectel_file/quec_at_analysis.h"
#include "app_debug.h"


#define QUEC_PRODUCT_INFO_SN_LENGTH      64
#define QUEC_PRODUCT_PARAM_BASE_ADDRESS  0x803c000

typedef struct tag_quec_product_info{
  uint32_t ctune_flag;  //0xA5A5A5A5 vaild
  uint32_t ctune_value;
  uint32_t sn_len;
  uint8_t sn[QUEC_PRODUCT_INFO_SN_LENGTH];
}QUEC_PRODUCT_INFO_T;

QUEC_PRODUCT_INFO_T g_tQuecProduct;
QUEC_PRODUCT_INFO_T g_ucCheckBuffer;
EM_UART_WORK_MODE g_emWorkMode = EM_UART_AT_MODE;



void flash_param_read(uint8_t *buffer , uint32_t len)
{
  memcpy(buffer,(void *)(QUEC_PRODUCT_PARAM_BASE_ADDRESS) , len);
}


void flash_param_write(uint8_t *buffer,uint16_t len)
{
    uint16_t templ;

    templ = len;

    while((templ%4) != 0)
      templ++;

    templ = templ > 1024?1024:templ;

    MSC_ErasePage((uint32_t *)QUEC_PRODUCT_PARAM_BASE_ADDRESS);
    MSC_Init();
    MSC_WriteWord((uint32_t *)QUEC_PRODUCT_PARAM_BASE_ADDRESS,buffer,templ);
    MSC_Deinit();
}

void quec_uart_pd_send_string(char* str , uint32_t len)
{
  quec_uart_send(str , len);
  quec_uart_send((char *)"\r\n" , 2);
}


EM_UART_WORK_MODE app_quec_product_get_workmode(void)
{
  return g_emWorkMode;
}

void app_quec_product_set_workmode(EM_UART_WORK_MODE mode)
{
  g_emWorkMode = mode;
}

int quec_sr_version(int argc, char **argv)
{
  (void)argc;

  char rsp[64] = {0};

  if((argv[1][0] != '\r') || (argv[1][1] != '\n'))
  {
      return -1;
  }

  memset(rsp , 0 , sizeof(rsp));
  sprintf(rsp,"+QVERSION: %s\r\n" , QUEC_VERSION);
  quec_uart_send_string(rsp);

  return 0;
}

int quec_sr_prod_entry(int argc, char **argv)
{
    uint8_t mode = 0;
    EM_UART_WORK_MODE get_mode = 0;
    char resp_buf[128] = {0};

    (void)argc;

    if((argv[1][0] != '0' && argv[1][0] != '1') && argv[1][0] != '?')
    {
        return -1;
    }

    if(strlen(argv[1]) !=3 && strlen(argv[1]) !=1)
    {
        return -1;
    }

    if(argv[1][0] == '?')
    {
        get_mode = app_quec_product_get_workmode();
        if(get_mode == EM_UART_PROD_MODE)
        {
            sprintf(resp_buf , "+QPROD: %d" , 1);
        }
        else
        {
            sprintf(resp_buf , "+QPROD: %d" , 0);
        }
        quec_uart_pd_send_string(resp_buf , strlen(resp_buf));
        return 0;
    }
    else
    {
        mode = atoi(argv[1]);
        if(mode == 0)
        {
            app_quec_product_set_workmode(EM_UART_AT_MODE);
            app_log("exit prod mode\r\n");
            return 0;
        }
        else if(mode == 1)
        {
            app_quec_product_set_workmode(EM_UART_PROD_MODE);
            app_log("enter prod mode\r\n");
            return 0;
        }
        else
        {
            return -1;
        }
    }

    return 0;
}


int quec_sr_prod_update_sn(uint8_t *sn , uint8_t sn_length)
{
  flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
  memset(g_tQuecProduct.sn , 0 , sizeof(g_tQuecProduct.sn));
  memcpy(g_tQuecProduct.sn,sn,sn_length);

  flash_param_write((uint8_t *)&g_tQuecProduct ,sizeof(g_tQuecProduct));

  return 0;
}

int quec_sr_prod_sn(int argc, char **argv)
{
  uint16_t sn_length = 0;
  char resp_buf[128] = {0};
  uint8_t mode = 0;// 0:get , 1:set
  uint8_t option = 0; //5:sn

  if(argc != 3 && argc != 4)
  {
      return -1;
  }

  mode = atoi(argv[1]);
  option = atoi(argv[2]);

  if(option != QUEC_PD_SN_OPTION_TYPE)
  {
      return -1;
  }

//  if(argv[1][0] == '?')
  if(mode == QUEC_PD_SN_OPTION_GET) //get
  {
      memset(resp_buf , 0 , sizeof(resp_buf));
      flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
      sprintf(resp_buf , "+EGMR: \"%s\"" , g_tQuecProduct.sn);
      quec_uart_pd_send_string(resp_buf , strlen(resp_buf));
      return 0;
  }
//  else if(argv[3][0] == '\"' )
  else if(mode == QUEC_PD_SN_OPTION_SET) //set
  {
      if(app_quec_product_get_workmode() != EM_UART_PROD_MODE)
      {
          return -1;
      }

      app_log("sn length = %d , sn = %s\r\n" , strlen(&argv[3][0]) - 4 , &argv[3][0]);

      //if((argv[3][0] != '\"') || (argv[3][strlen(argv[3])-3] != '\"'))
      if(argv[3][0] != '\"')
      {
          return -1;
      }

      sn_length = strlen(&argv[3][0]) - 4;
      if(sn_length > QUEC_PD_SN_MAX_SIZE)
      {
          return -1;
      }

      flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
      memset(g_tQuecProduct.sn , 0 , sizeof(g_tQuecProduct.sn));
      memcpy(g_tQuecProduct.sn,&argv[3][1],sn_length);
      g_tQuecProduct.sn_len = sn_length;

      flash_param_write((uint8_t *)&g_tQuecProduct ,sizeof(g_tQuecProduct));

      memset(g_ucCheckBuffer.sn , 0 , sizeof(g_ucCheckBuffer.sn));

      flash_param_read((uint8_t *)&g_ucCheckBuffer,sizeof(g_ucCheckBuffer));

      if(!memcmp(&argv[3][1],g_ucCheckBuffer.sn,sn_length))
      {
          return 0;
      }
      else
      {
          return -1;
      }
  }
  else
  {
      return -1;
  }

  return 0;
}

int quec_sr_prod_version(int argc, char **argv)
{
  char resp_buf[128] = {0};

  if(argc != 2)
  {
      return -1;
  }

  //if(argv[1][0] == '?')
  if(argv[1][0] == '\r')
  {
      memset(resp_buf , 0 , sizeof(resp_buf));
      sprintf(resp_buf , "%s" , QUEC_PRODUCT_VERSION);
      quec_uart_pd_send_string(resp_buf , strlen(resp_buf));
      return 0;
  }
  else
  {
      return -1;
  }

  return 0;
}


int quec_sr_ftm_mode(int argc, char **argv)
{
  uint8_t mode = 0;
  EM_UART_WORK_MODE get_mode = 0;
  char resp_buf[128] = {0};

  (void)argc;

  if((argv[1][0] != '0' && argv[1][0] != '1') && argv[1][0] != '?')
  {
      return -1;
  }

  if(strlen(argv[1]) !=3 && strlen(argv[1]) !=1)
  {
      return -1;
  }


  if(argv[1][0] == '?')
  {
      get_mode = app_quec_product_get_workmode();
      if(get_mode == EM_UART_FTM_MODE)
      {
          sprintf(resp_buf , "+QFTM: %d" , 1);
      }
      else
      {
          sprintf(resp_buf , "+QFTM: %d" , 0);
      }
      quec_uart_pd_send_string(resp_buf , strlen(resp_buf));
      return 0;
  }
  else
  {
      mode = atoi(argv[1]);
      if(mode == 0)
      {
          app_quec_product_set_workmode(EM_UART_AT_MODE);
          app_log("exit ftm mode\r\n");
          return 0;
      }
      else if(mode == 1)
      {
          app_quec_product_set_workmode(EM_UART_FTM_MODE);
          app_log("enter ftm mode\r\n");
          return 0;
      }
      else
      {
          return -1;
      }

      quec_uart_pd_send_string(ATCMD_PD_AT_PROD_ERROR_NO_ERROR,strlen(ATCMD_PD_AT_PROD_ERROR_NO_ERROR));
  }

  return 0;
}


int quec_sr_csub(int argc, char **argv)
{
  (void)argc;

  char rsp[64] = {0};

  if((argv[1][0] != '\r') || (argv[1][1] != '\n'))
  {
      return -1;
  }

  memset(rsp , 0 , sizeof(rsp));
  sprintf(rsp,"SubEditi: %s\r\n" , QUEC_SUB_VERSION);
  quec_uart_pd_send_string(rsp,strlen(rsp));

  return 0;
}


int quec_sr_ati(void)
{
  char rsp[64] = {0};

  quec_uart_pd_send_string("Quectel",strlen("Quectel"));
  quec_uart_pd_send_string("KCMCA6S",strlen("KCMCA6S"));

  memset(rsp , 0 , sizeof(rsp));
  sprintf(rsp,"Revision: %s" , QUEC_VERSION);
  quec_uart_pd_send_string(rsp,strlen(rsp));

  quec_uart_send_string(ATCMD_PD_AT_PROD_ERROR_NO_ERROR);

  return 0;
}

int quec_sr_qsub(int argc, char **argv)
{
  (void)argc;

  char rsp[64] = {0};

  if((argv[1][0] != '\r') || (argv[1][1] != '\n'))
  {
      return -1;
  }

  memset(rsp , 0 , sizeof(rsp));
  sprintf(rsp,"+QSUB: %s" , QUEC_SUB_VERSION);
  quec_uart_pd_send_string(rsp,strlen(rsp));

  return 0;
}

int quec_sr_qctune(int argc, char **argv)
{
  (void)argc;
  int ctune_value = 0;
  char rsp[64] = {0};

  if(argc != 2)
    return -1;

  if(argv[1][0] == '?')
  {
      flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
      app_log("g_tQuecProduct.ctune_value = %d\r\n" , g_tQuecProduct.ctune_value);
      memset(rsp , 0 , sizeof(rsp));
      sprintf(rsp,"+QCTUNE: %d" , g_tQuecProduct.ctune_value);
      quec_uart_pd_send_string(rsp,strlen(rsp));
      return 0;
  }
  else
  {
      if(app_quec_product_get_workmode() != EM_UART_PROD_MODE)
      {
          return -1;
      }

      //app_log("ctune_value1 = %d\r\n" , ctune_value);
      for(uint16_t i = 0; (argv[1][i] != '\r') ; i++ )
      {
          if((argv[1][i] > '9' || argv[1][i] < '0') && (argv[1][i] != '-') )
          {
              return -1;
          }

          //app_log("ctune_value2 = %d\r\n" , ctune_value);

          //if(((argv[1][i] == '0') && (argv[1][i+1] != '\r')) || ((argv[1][i] == '-') && (argv[1][i+1] == '\r')))
          //{
          //    return -1;
          //}
          if((argv[1][i] == '-') && (argv[1][i+1] == '\r'))
          {
              return -1;
          }
      }

      if(1 != g_AtIsEqual)
        return -1;

      ctune_value = atoi(argv[1]);

      app_log("ctune_value = %d\r\n" , ctune_value);

      if(ctune_value < QUEC_PD_CTUNE_MIN || ctune_value > QUEC_PD_CTUNE_MAX)
        return -1;

      flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
      g_tQuecProduct.ctune_value = ctune_value;
      g_tQuecProduct.ctune_flag = 0xA5A5A5A5;

      flash_param_write((uint8_t *)&g_tQuecProduct ,sizeof(g_tQuecProduct));

      flash_param_read((uint8_t *)&g_ucCheckBuffer,sizeof(g_ucCheckBuffer));
      if(g_tQuecProduct.ctune_value == g_ucCheckBuffer.ctune_value)
      {
          return 0;
      }
      else
      {
          return -1;
      }
  }

}

int quec_sr_ctune_get(void)
{
  flash_param_read((uint8_t *)&g_tQuecProduct,sizeof(g_tQuecProduct));
  if(g_tQuecProduct.ctune_flag != 0xA5A5A5A5)
    return -100;
  else
    return g_tQuecProduct.ctune_value;
}


