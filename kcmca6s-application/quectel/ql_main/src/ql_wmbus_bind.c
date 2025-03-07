#include "ql_wmbus_bind.h"

#define QL_WMBUS_BIND_DEV_MAX 64
#define QL_NVM_SAVE_WMBUS_DEV_INFO 0x02

ql_wmbus_bind_info_t *g_wmbus_bind_info = NULL;

void ql_wmbus_bind_init(void)
{
    if(g_wmbus_bind_info != NULL)
    {
        return;
    }
    g_wmbus_bind_info = (ql_wmbus_bind_info_t *)sl_malloc(sizeof(ql_wmbus_bind_info_t));
    g_wmbus_bind_info->node_list = NULL;
    g_wmbus_bind_info->node_cnt = 0;

    uint8_t rbuf[64 * 12] = { 0 };
    uint32_t rlen = 0;
    int16_t ret = ql_nvm_read_data(QL_NVM_SAVE_WMBUS_DEV_INFO, rbuf, &rlen);

    if(ret != 0 || rlen <= 0)
    {
        ql_printf("[NVM]READ WM-BUS DEVICE NODE PARAM ERROR,RET: %d, LENTH:%d\r\n", ret, rlen);
        return ;
    }
     

    for (size_t i = 0; i < (rlen / sizeof(ql_wmbus_dev_info_t)); i++)
    {
        ql_wmbus_dev_info_t *dev_info = (ql_wmbus_dev_info_t *)&rbuf[i * sizeof(ql_wmbus_dev_info_t)];
        add_dev_wmbus_node_tail(g_wmbus_bind_info, *dev_info);

        ql_printf("[APP]DEV INFO:INDEX_ID:%d DEV_TYPE:%02X, ID:%08X, MANUF:%04X, VER:%02X, IS_ENCRY:%d\r\n",
            dev_info->index_id, dev_info->device_type, dev_info->id, dev_info->manufacturer, dev_info->version, dev_info->is_encryption
        );

    }
}

int16_t ql_wmbus_bind_dev_save_to_flash(void)
{
    if((g_wmbus_bind_info->node_cnt) == 0 || (g_wmbus_bind_info == NULL))
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = g_wmbus_bind_info->node_list;

    uint8_t *buf = sl_malloc(ql_wmbus_get_bind_cnt() * sizeof(ql_wmbus_dev_info_t));
    uint32_t off_set = 0;
    if(!buf)
    {
        return -1;
    }

    uint32_t node_cnt = 0;
    while ((current != NULL) && (node_cnt <= ql_wmbus_get_bind_cnt()))
    {
        memcpy(buf + off_set, &current->info, sizeof(ql_wmbus_dev_info_t));
        off_set += sizeof(ql_wmbus_dev_info_t);
        current = current->next;
        node_cnt++;
    }

    ql_printf("[APP]SAVE WM-BUS DEVICE NODE CNT:%d\r\n", ql_wmbus_get_bind_cnt());
    
    int16_t ret = ql_nvm_write_data(QL_NVM_SAVE_WMBUS_DEV_INFO, buf, off_set);
    if(ret != 0)
    {
        return -1;
    }
    return 0;
}

ql_wmbus_bind_info_t* ql_wmbus_get_dev_storage(void)
{
    return g_wmbus_bind_info;
}

uint32_t ql_wmbus_get_bind_cnt(void)
{
    if(!g_wmbus_bind_info)
    {
        return 0;
    }
    return g_wmbus_bind_info->node_cnt;
}

static ql_wmbus_bind_node_t* create_dev_wmbus_node(ql_wmbus_dev_info_t *dev_wmbus_info)
{
    ql_wmbus_bind_node_t *new_node = (ql_wmbus_bind_node_t*)sl_malloc(sizeof(ql_wmbus_bind_node_t));

    if(!new_node)
    {
        return NULL;
    }

    new_node->info.device_type = dev_wmbus_info->device_type;
    new_node->info.id = dev_wmbus_info->id;
    new_node->info.index_id = dev_wmbus_info->index_id;
    new_node->info.manufacturer = dev_wmbus_info->manufacturer;
    new_node->info.version = dev_wmbus_info->version;
    new_node->info.is_encryption = dev_wmbus_info->is_encryption;

    new_node->next = NULL;

    return new_node;
}

int16_t add_dev_wmbus_node_tail(ql_wmbus_bind_info_t* storage, ql_wmbus_dev_info_t dev_wmbus_info)
{
    if (!storage)
    {
        return -1;
    }

    if(ql_wmbus_get_bind_cnt() > QL_WMBUS_BIND_DEV_MAX)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* new_node = create_dev_wmbus_node(&dev_wmbus_info);
    new_node->next = NULL;

    if (storage->node_list == NULL)
    {
        storage->node_list = new_node;
    }
    else
    {
        ql_wmbus_bind_node_t* current = storage->node_list;

        while (current != NULL)
        {
            if ((current->info.device_type == dev_wmbus_info.device_type) &&
                (current->info.id == dev_wmbus_info.id) &&
                (current->info.manufacturer == dev_wmbus_info.manufacturer) &&
                (current->info.version == dev_wmbus_info.version)
                )
            {
                sl_free(new_node);
                return -1;
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
            if(current->info.index_id == dev_wmbus_info.index_id)
            {
                current->info.device_type = dev_wmbus_info.device_type;
                current->info.id = dev_wmbus_info.id;
                current->info.manufacturer = dev_wmbus_info.manufacturer;
                current->info.version = dev_wmbus_info.version;
                current->info.is_encryption = dev_wmbus_info.is_encryption;
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

int8_t query_dev_wmbus_node_is_encryption_by_index(uint32_t index)
{
    if(!g_wmbus_bind_info)
    {
        ql_printf("[APP]BIND INFO IS NULL\r\n");
        return -1;
    }

    if(g_wmbus_bind_info->node_cnt == 0)
    {
        ql_printf("[APP]BIND INFO NODE CNT IS 0\r\n");
        return -1;
    }

    if(!g_wmbus_bind_info->node_list)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = g_wmbus_bind_info->node_list;

    while (current != NULL)
    {
        if (current->info.index_id == index)
        {
            ql_printf("[APP]QUERY ENCRYPTION INDEX:%d, IS_ENCRY:%d\r\n", current->info.index_id, current->info.is_encryption);
            return current->info.is_encryption;
        }

        current = current->next;
    }

    return -1;
}

int16_t set_dev_wmbus_node_is_encryption_by_index(uint32_t index)
{
    if(!g_wmbus_bind_info)
    {
        return -1;
    }

    if(g_wmbus_bind_info->node_cnt == 0)
    {
        return -1;
    }

    if(!g_wmbus_bind_info->node_list)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = g_wmbus_bind_info->node_list;

    while (current != NULL)
    {
        if (current->info.index_id == index)
        {
            current->info.is_encryption = 1;
            ql_printf("[APP]SET ENCRYPTION INDEX:%d, IS_ENCRY:%d\r\n", index, current->info.is_encryption);
            return 0;
        }

        current = current->next;
    }
    return -1;
}

int16_t query_dev_wmbus_node_index_is_exist(uint32_t index)
{
    if(!g_wmbus_bind_info)
    {
        return -1;
    }

    if(g_wmbus_bind_info->node_cnt == 0)
    {
        return -1;
    }

    if(!g_wmbus_bind_info->node_list)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = g_wmbus_bind_info->node_list;

    while (current != NULL)
    {
        if (current->info.index_id == index)
        {
            return 0;
        }

        current = current->next;
    }

    return -1;
}

int16_t query_dev_wmbus_node_index(ql_wmbus_dev_info_t dev_wmbus_info)
{
    if(!g_wmbus_bind_info)
    {
        return -1;
    }

    if(g_wmbus_bind_info->node_cnt == 0)
    {
        return -1;
    }

    if(!g_wmbus_bind_info->node_list)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = g_wmbus_bind_info->node_list;

    while (current != NULL)
    {
        if ((current->info.device_type == dev_wmbus_info.device_type) &&
            (current->info.id == dev_wmbus_info.id) &&
            (current->info.manufacturer == dev_wmbus_info.manufacturer) &&
            (current->info.version == dev_wmbus_info.version)
            )
        {
            return current->info.index_id;
        }

        current = current->next;
    }

    return -1;
}

int16_t query_dev_wmbus_node(ql_wmbus_bind_info_t* storage, uint32_t index_id, ql_wmbus_dev_info_t *dev_wmbus_info)
{
    if(storage->node_cnt == 0)
    {
        return -1;
    }

    ql_wmbus_bind_node_t* current = storage->node_list;
    while (current != NULL)
    {
        if(index_id == current->info.index_id)
        {
            dev_wmbus_info->index_id = current->info.index_id;
            dev_wmbus_info->device_type = current->info.device_type;
            dev_wmbus_info->id = current->info.id;
            dev_wmbus_info->manufacturer = current->info.manufacturer;
            dev_wmbus_info->version = current->info.version;
            return 0;
        }
        current = current->next;
    }
    return -1;
}

int16_t check_device_is_register(ql_wmbus_bind_info_t* storage, ql_wmbus_dev_info_t dev_wmbus_info)
{
    ql_wmbus_bind_node_t* current = storage->node_list;

    while (current != NULL)
    {
        if ((current->info.device_type == dev_wmbus_info.device_type) &&
            (current->info.id == dev_wmbus_info.id) &&
            (current->info.manufacturer == dev_wmbus_info.manufacturer) &&
            (current->info.version == dev_wmbus_info.version)
            )
        {
            return 0;
        }

        current = current->next;
    }

    return -1;
}

void free_data_storage(ql_wmbus_bind_info_t* storage)
 {
    if (storage == NULL) 
    {
        return;
    }

    ql_wmbus_bind_node_t* current_node = storage->node_list;
    ql_wmbus_bind_node_t* next_service;

    while (current_node != NULL) 
    {
        next_service = current_node->next;
        sl_free(current_node);
        current_node = next_service;
    }
    storage->node_list = NULL;

    sl_free(storage);
    storage = NULL;
}