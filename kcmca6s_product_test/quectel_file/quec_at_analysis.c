/*
 * quec_at_analysis.c
 *
 *  Created on: 2024年6月24日
 *      Author: vic.cheng
 */

#include "string.h"
#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "app.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif // SL_CATALOG_POWER_MANAGER_PRESENT
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

#include "app_debug.h"
#include "quectel_file/quec_at_analysis.h"
#include "quectel_file/quec_product_test.h"
#include "quectel_file/quec_uart.h"

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
uint8_t g_AtIsEqual = 0;

const TypePDCommandFunction CommandFuncTable[] = {
    /**********************product FT command*************************/
    {"QVERSION",      quec_sr_version},
    {"QPROD",         quec_sr_prod_entry},
    {"EGMR",          quec_sr_prod_sn},   //=CGM SN = ADVERTISING NAME
    {"QGMR",          quec_sr_prod_version},
    {"QFTM",          quec_sr_ftm_mode},
    {"CSUB",          quec_sr_csub},
    {"QSUB",          quec_sr_qsub},
    {"QCTUNE",        quec_sr_qctune},
};

int quec_at_param_analysis(char *inbuf , int len , int command_point)
{
  _atsvr_handle_input_stat stat;
  char *argv[QUEC_PRODUCT_AT_PARAM_MAX+1];
  int argc = 1;
  int i = 0;
  int offset = 0;
  int str_conut = 0;
  memset((void *)&argv, 0, sizeof(argv));
  memset(&stat, 0, sizeof(stat));
  stat.isquote = 0;

  do
  {
    offset++;
    if(offset > len){
        app_log("The data is incomplete\r\n");
        return -1;  ////error
    }

    switch (inbuf[i])
    {
      case '\0':
      {
          if(((argc == 0)||(stat.isD == 1))||(stat.limQ))
          {
              app_log("The data does not conform to the regulations %d\r\n",__LINE__);
              return 2;
          }
          stat.done = 1;
      }break;

      case '=':
      {
        if(argc == 1)
        {
            inbuf[i] = '\0';
            stat.inArg = 0;
            stat.isD = 1;
            stat.ischeck = 1;
            app_log("quec_at_param_analysis1\r\n");
        }
        else if(argc == 0)
        {
            app_log("The data does not conform to the regulations %d\r\n",__LINE__);
            return -1;
        }
      }break;

      case '?':
      {
        if((stat.ischeck ==1) && (argc <= 1))
        {
             argc = 3;
             argv[1] = "=";
             argv[2] = "?";
             stat.ischeck = 0;
             stat.done = 1;
        }
        else if (argc <= 1)
        {
             inbuf[i] = '\0';
             argc = 2;
             argv[1] = "?";
             stat.done = 1;
         }

        if(inbuf[i+1] != '\r')
          return -1;

     }break;


     case '\\':  ////"\"
     {
         offset += 1;
         if((offset + 1) > len)
         {
             app_log("The data does not conform to the regulations %d\r\n",__LINE__);
             return 6;  ////error
         }

         memmove(&inbuf[i],&inbuf[i+1],(len-offset) + 1);
     }break;

     case ',':
     {
        if((stat.isD == 1)&&(argc == 1))  ///=,
        {
            app_log("The data does not conform to the regulations %d\r\n",__LINE__);
            return -1;
        }

        if(stat.isquote)
        {
            app_log("********** quote is in string \n");
            break;
        }

        if(stat.inArg)
        {
             stat.inArg = 0;
             inbuf[i] = '\0';
             stat.limQ = 1;
        }
      }break;


      case '"':
      {
        str_conut++;
        if(str_conut == 1)
        {
            stat.isquote = 1;
        }
        else if(str_conut == 2)
        {
            stat.isquote = 0;
            str_conut = 0;
        }
      }


        default:
            if(!stat.inArg)
            {
                stat.inArg = 1;
                argc++;
                argv[argc -1] = (char*)&inbuf[i];
                stat.limQ = 0;
                if(stat.isD == 1)
                {
                   stat.isD = 2;
                }
            }
            break;
        }
    }
    while (!stat.done && (++i < 1024));

    app_log("end\r\n");

    if (argc < 1)
    {
        app_log("Data parsing exception\r\n");
        return -1;
    }


    //for(i = 0 ; i < argc ; i++)
    //{
    //    app_log("argv[%d]:%s, argc:%d\n", i , argv[i] , i);
    //}

    g_AtIsEqual = stat.ischeck;
    return CommandFuncTable[command_point].Function(argc , argv);
}



int quec_at_command_handle(char *inbuf,int cmdlen)
{
    int ret = -1;

    if(!memcmp(inbuf , ATCMD_QUEC_ATI_HEAD , strlen(ATCMD_QUEC_ATI_HEAD)))
    {
        quec_sr_ati();
    }
    else if(!memcmp(inbuf , ATCMD_QUEC_AT_HEAD , strlen(ATCMD_QUEC_AT_HEAD)))
    {
        for(uint16_t i=0 ; i<sizeof(CommandFuncTable)/sizeof(CommandFuncTable[0]); i++)
        {
            if(!memcmp(CommandFuncTable[i].cmd,inbuf+3,strlen(CommandFuncTable[i].cmd)))
            {
                ret = quec_at_param_analysis(inbuf + strlen(ATCMD_QUEC_AT_HEAD) + strlen(CommandFuncTable[i].cmd) , cmdlen+1-strlen(ATCMD_QUEC_AT_HEAD)-strlen(CommandFuncTable[i].cmd) , i);
                break;
            }
        }

       app_log("ret = %d\r\n" , ret);
       if(0 == ret)
       {
           quec_uart_send_string(ATCMD_PD_AT_PROD_ERROR_NO_ERROR);
       }
       else if(ret == 1)
       {

       }
       else
       {
           quec_uart_send_string(ATCMD_PD_AT_PROD_ERROR_RESP);
       }
    }
    else
    {
        return -1;
    }

    return 0;
}

