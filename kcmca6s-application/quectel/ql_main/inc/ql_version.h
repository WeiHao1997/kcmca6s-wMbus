#ifndef __QL_VERSION_H__
#define __QL_VERSION_H__

#include "ql_include.h"

#define QL_OUTPUT_VERSION_COMPLETE 0
#define QL_OUTPUT_VERSION_SHORT    1
#define QL_OUTPUT_VERSION_SUB_VER  2

extern const uint8_t Version[];
extern const uint8_t Subversion[];

extern const uint8_t CusSoftwareVersion[];
extern const uint8_t CusHardwareVersion[];

void ql_output_customer_detail_version_info(void);
void ql_output_version_info(uint8_t param);

#endif /* __QL_VERSION_H__ */
