#include "ql_main.h"

static void ql_bootloader_query_build_time(ql_build_time_t *time)
{
   char mon_str[5] = { 0 };
   static char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
   // hh:mm:ss
   sscanf(__TIME__, "%hd:%hd:%hd", &time->Hour, &time->Min, &time->Sec);

   // Mmm dd yyyy
   sscanf(__DATE__, "%s %hd %hd", mon_str, &time->Day, &time->Year);
   time->Mon = ((strstr(month_names, mon_str) - &month_names[0]) / 3 + 1);
}

void ql_main(void)
{
    ql_build_time_t build_time;
    ql_bootloader_query_build_time(&build_time);
    ql_wdg_init();
    ql_uart_open(QL_UART_UART1_PORT);
    ql_uart_open(QL_UART_UART0_PORT);
    ql_printf("\r\n[APP]QUECTEL APPLICATION INIT\r\n");
    ql_printf("[APP]QUECTEL APP BUILD TIME: [%04d/%02d/%02d,%02d:%02d:%02d]\r\n",
        build_time.Year,
        build_time.Mon,
        build_time.Day,
        build_time.Hour,
        build_time.Min,
        build_time.Sec
    );
    ql_nvm_init();
    ql_param_cfg_init();
    ql_wmbus_init();
    ql_printf("[APP]QUECTEL APPLICATION INIT COMPLETE\r\n");
    
    ql_output_customer_detail_version_info();
}

void ql_handle_loop(void)
{
    ql_cmd_handle_loop();
    ql_wmbus_handle_loop();
    ql_wdg_feed();
}
