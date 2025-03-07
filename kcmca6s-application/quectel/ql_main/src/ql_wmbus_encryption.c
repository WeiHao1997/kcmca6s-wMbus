#include "ql_wmbus_encryption.h"

#define WMBUS_ENCRYPTION_KEY_SIZE       16
#define WMBUS_ENCRYPTION_BIND_DEV_MAX   64

#define WMBUS_ENCRYPTION_KEY_CNT_SAVE_OBJECT_IDENTIFY   0x03
#define WMBUS_ENCRYPTION_KEY_SAVE_OBJECT_IDENTIFY       0x04

static ql_wmbus_encryption_t *g_wmbus_encryption = NULL;

void print_arr(uint8_t arr[], int size, int i) {
    if (i < size) {
        ql_printf("%02X ", arr[i]);
        print_arr(arr, size, i + 1);
    }
}

void ql_wmbus_bind_encryption_init(void)
{
    if(g_wmbus_encryption)
    {
        return ;
    }

    g_wmbus_encryption = (ql_wmbus_encryption_t *)sl_malloc(sizeof(ql_wmbus_encryption_t));
    if (g_wmbus_encryption == NULL)
    {
        ql_printf("[ERROR] MEMORY ALLOCATION FAIL\r\n");
        return;
    }
    g_wmbus_encryption->node_list = NULL;
    g_wmbus_encryption->node_cnt = 0;

    uint32_t enc_cnt = 0;
    uint32_t data_lens = sizeof(enc_cnt);;
    int16_t ret = ql_nvm_read_data(WMBUS_ENCRYPTION_KEY_CNT_SAVE_OBJECT_IDENTIFY, &enc_cnt, &data_lens);
    if(ret != 0 || (data_lens != sizeof(enc_cnt)))
    {
        ql_printf("[NVM]READ WM-BUS DEVICE ENCRYPTION NODE PARAM ERROR,RET: %d, LENTH:%d\r\n", ret, enc_cnt);
        return ;
    }

    uint8_t *encryption_buf = sl_malloc(sizeof(ql_wmbus_encryption_info_t) * enc_cnt);
    if (encryption_buf == NULL)
    {
        ql_printf("[ERROR] MEMORY ALLOCATION FAIL\r\n");
        return;
    }

    uint32_t encryption_size = 0;
    ret = ql_nvm_read_data(WMBUS_ENCRYPTION_KEY_SAVE_OBJECT_IDENTIFY, encryption_buf, &encryption_size);
    if(ret != 0 || (encryption_size <= 0))
    {
        sl_free(encryption_buf);
        sl_free(g_wmbus_encryption);
        g_wmbus_encryption= NULL;
        ql_printf("[NVM]READ WM-BUS DEVICE NODE PARAM ERROR,RET: %d, CNT:%d\r\n", ret, encryption_size);
        return ;
    }

    for (int32_t i = 0; i < (encryption_size / sizeof(ql_wmbus_encryption_info_t)); i++)
    {
        ql_wmbus_encryption_info_t *dev_info = (ql_wmbus_encryption_info_t *)&encryption_buf[i * sizeof(ql_wmbus_encryption_info_t)];
        add_dev_wmbus_encryption_node_tail(ql_wmbus_get_encryption_storage(), *dev_info);
        ql_printf("[APP]INDEX:%d, ENCRY:", dev_info->node_index);
        print_arr(dev_info->enc_key, WMBUS_ENCRYPTION_KEY_SIZE, 0);
        ql_printf(",ENCRY_METHOD:%04X\r\n", dev_info->enc_type);
    }

    sl_free(encryption_buf);
    encryption_buf = NULL;
}

int16_t ql_wmbus_bind_dev_encryption_save_to_flash(void)
{
    int16_t ret = 0;
    if(!g_wmbus_encryption)
    {
        return -1;
    }

    if(g_wmbus_encryption->node_cnt <= 0)
    {
        return 0;
    }

    uint8_t *encryption_buf = sl_malloc(g_wmbus_encryption->node_cnt * sizeof(ql_wmbus_encryption_info_t));
    if(!encryption_buf)
    {
        return -1;
    }

    ret = ql_nvm_write_data(WMBUS_ENCRYPTION_KEY_CNT_SAVE_OBJECT_IDENTIFY, (uint8_t *)&g_wmbus_encryption->node_cnt, sizeof(g_wmbus_encryption->node_cnt));
    if(ret != 0)
    {
        sl_free(encryption_buf);
        encryption_buf = NULL;
        return -1;
    }

    ql_wmbus_encryption_node_t *current = g_wmbus_encryption->node_list;
    uint32_t off_set = 0;
    uint32_t node_cnt = 0;
    while ((current != NULL) && (node_cnt <= g_wmbus_encryption->node_cnt))
    {
        memcpy((void *)encryption_buf + off_set, (void *)&current->enc_info, sizeof(ql_wmbus_encryption_info_t));
        off_set += sizeof(ql_wmbus_encryption_info_t);
        current = current->next;
        node_cnt++;
    }
        
    ret = ql_nvm_write_data(WMBUS_ENCRYPTION_KEY_SAVE_OBJECT_IDENTIFY, encryption_buf, off_set);
    if(ret != 0)
    {
        sl_free(encryption_buf);
        encryption_buf = NULL;
        return -1;
    }

    sl_free(encryption_buf);
    encryption_buf = NULL;
    return 0;
}

ql_wmbus_encryption_t* ql_wmbus_get_encryption_storage(void)
{
    return g_wmbus_encryption;
}

int16_t ql_wmbus_get_encryption_bind_cnt(void)
{
    if(!g_wmbus_encryption)
    {
        return 0;
    }
    return g_wmbus_encryption->node_cnt;
}

static ql_wmbus_encryption_node_t* create_dev_wmbus_encryption_node(ql_wmbus_encryption_info_t *dev_wmbus_encryption_info)
{
    ql_wmbus_encryption_node_t *new_node = (ql_wmbus_encryption_node_t*)sl_malloc(sizeof(ql_wmbus_encryption_node_t));

    if(!new_node)
    {
        return NULL;
    }

    new_node->enc_info.node_index   = dev_wmbus_encryption_info->node_index;
    new_node->enc_info.enc_type     = dev_wmbus_encryption_info->enc_type;
    memcpy((void *)&new_node->enc_info.enc_key, (void *)&dev_wmbus_encryption_info->enc_key, WMBUS_ENCRYPTION_KEY_SIZE);

    new_node->next = NULL;

    return new_node;
}

int16_t add_dev_wmbus_encryption_is_encryption_flag(uint32_t index, uint16_t method_encryption)
{
    if(!g_wmbus_encryption)
    {
        return -1;
    }

    if(g_wmbus_encryption->node_cnt <= 0)
    {
        return -1;
    }

    if(!g_wmbus_encryption->node_list)
    {
        return -1;
    }

    ql_wmbus_encryption_node_t *current_node = g_wmbus_encryption->node_list;

    while (current_node != NULL)
    {
        if(current_node->enc_info.node_index == index)
        {
            current_node->enc_info.enc_type = method_encryption;
            return 0;
        }
        current_node = current_node->next;
    }
    
    return -1;
}

int16_t add_dev_wmbus_encryption_node_tail(ql_wmbus_encryption_t* storage, ql_wmbus_encryption_info_t dev_wmbus_encryption_info)
{
    switch (ql_wmbus_get_current_role())
    {
        case E_ROLE_SLAVE:
        {

        }
        break;
        case E_ROLE_MASTER:
        {
            int16_t ret = set_dev_wmbus_node_is_encryption_by_index(dev_wmbus_encryption_info.node_index);
            if (ret != 0)
            {
                return -1;
            } 

            if(ql_wmbus_get_bind_cnt() <= 0)
            {
                ql_printf("[APP]NO BIND OTHER DEVICE\r\n");
                return -1;
            }
            
            if(query_dev_wmbus_node_index_is_exist(dev_wmbus_encryption_info.node_index) != 0)
            {
                return -1; 
            }
        }
        break;
    
    default:
        break;
    }

    if(!storage)
    {
        ql_printf("[APP]NO MALLOC\r\n");
        return -1;
    }

    if(ql_wmbus_get_encryption_bind_cnt() > WMBUS_ENCRYPTION_BIND_DEV_MAX)
    {
        return -1;
    }

    ql_wmbus_encryption_node_t* new_node = create_dev_wmbus_encryption_node(&dev_wmbus_encryption_info);
    if (new_node == NULL)
    {
        return -1;
    }

    new_node->next = NULL;

    if (storage->node_list == NULL)
    {
        storage->node_list = new_node;
    }
    else
    {
        ql_wmbus_encryption_node_t* current = storage->node_list;

        while (current != NULL)
        {
            if(current->enc_info.node_index == dev_wmbus_encryption_info.node_index)
            {
                if(memcpy(current->enc_info.enc_key, dev_wmbus_encryption_info.enc_key, WMBUS_ENCRYPTION_KEY_SIZE) == 0)
                {
                    sl_free(new_node);
                    return -1;
                }
            }
            if(current->next == NULL)
            {
                break;
            }
            current = current->next;
        }

        current = storage->node_list;

        while (current != NULL)
        {
            if(current->enc_info.node_index == dev_wmbus_encryption_info.node_index)
            {
                memcpy(current->enc_info.enc_key, dev_wmbus_encryption_info.enc_key, WMBUS_ENCRYPTION_KEY_SIZE);
                sl_free(new_node);
                return 0;
            }
            if(current->next == NULL)
            {
                break;
            }
            current = current->next;
        }

        current->next = new_node;
    }

    storage->node_cnt++;

    return 0;
}

int16_t query_encryption_flag_by_index(int32_t index, uint16_t *flag)
{
    if(!g_wmbus_encryption)
    {
        return -1;
    }

    if(g_wmbus_encryption->node_cnt <= 0)
    {
        return -1;
    }

    if(!g_wmbus_encryption->node_list)
    {
        return -1;
    }

    ql_wmbus_encryption_node_t *current_node = g_wmbus_encryption->node_list;

    while (current_node != NULL)
    {
        if(current_node->enc_info.node_index == index)
        {
            *flag = current_node->enc_info.enc_type;
            return 0;
        }
        current_node = current_node->next;
    }
    
    return -1;
}

int8_t query_encryption_by_index(int32_t index, uint8_t *encryption_key)
{    
    if(!g_wmbus_encryption)
    {
        return -1;
    }

    if(g_wmbus_encryption->node_cnt <= 0)
    {
        return -1;
    }

    if(!g_wmbus_encryption->node_list)
    {
        return -1;
    }

    if(!encryption_key)
    {
        return -1;
    }

    ql_wmbus_encryption_node_t *current_node = g_wmbus_encryption->node_list;

    while (current_node != NULL)
    {
        if(current_node->enc_info.node_index == index)
        {
            memcpy(encryption_key, current_node->enc_info.enc_key, WMBUS_ENCRYPTION_KEY_SIZE);
            return 0;
        }
        current_node = current_node->next;
    }
    
    return -1;
}

void free_encryption_data_storage(ql_wmbus_encryption_t* storage)
 {
    if (storage == NULL) 
    {
        return;
    }

    ql_wmbus_encryption_node_t* current_node = storage->node_list;
    ql_wmbus_encryption_node_t* next_node = NULL;

    while (current_node != NULL) 
    {
        next_node = current_node->next;
        sl_free(current_node);
        current_node = next_node;
    }
    storage->node_list = NULL;

    sl_free(storage);
    storage = NULL;
}