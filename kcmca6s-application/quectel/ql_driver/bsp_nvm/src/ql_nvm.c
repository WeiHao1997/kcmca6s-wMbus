#include "ql_nvm.h"
#include "nvm3.h"
#include "nvm3_default.h"

#define NVM_CHECK_DATA_KEY_IS_AVAILABLE(x)  (((x) && 0x000FFFFF) > 0 ? 1 : 0)

int16_t ql_nvm_init(void)
{
    Ecode_t ret = nvm3_initDefault();

    if(ECODE_NVM3_OK != ret)
    {
        return -1;
    }
    return 0;
}

int16_t ql_nvm_deinit(void)
{
    Ecode_t ret = nvm3_deinitDefault();

    if(ECODE_NVM3_OK != ret)
    {
        return -1;
    }
    return 0;
}

int16_t ql_nvm_write_data(uint32_t data_key, uint8_t *data, uint32_t data_len)
{
    if(!NVM_CHECK_DATA_KEY_IS_AVAILABLE(data_key))
    {
        return -1;
    }
    Ecode_t ret = nvm3_writeData(nvm3_defaultHandle, (nvm3_ObjectKey_t)data_key, (const void *)data, data_len);
    if(ECODE_NVM3_OK != ret)
    {
        return -1;
    }
    return 0;
}

int16_t ql_nvm_read_data(uint32_t data_key, uint8_t *data, uint32_t *data_len)
{
    if(!NVM_CHECK_DATA_KEY_IS_AVAILABLE(data_key))
    {
        return -1;
    }
    Ecode_t ret = 0;

    uint32_t nvm_type = 0;
    uint32_t rd_len = 0;

    ret = nvm3_getObjectInfo(nvm3_defaultHandle, data_key, &nvm_type, &rd_len);
    if((ECODE_NVM3_OK != ret) || (nvm_type != NVM3_OBJECTTYPE_DATA))
    {
        return -1;
    }

    ret = nvm3_readData(nvm3_defaultHandle, (nvm3_ObjectKey_t)data_key, data, rd_len);
    if(ECODE_NVM3_OK != ret)
    {
        return -1;
    }

    *data_len = rd_len;

    return 0;
}



