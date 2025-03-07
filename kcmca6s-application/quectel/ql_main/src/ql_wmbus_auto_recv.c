#include "ql_wmbus_auto_recv.h"

#define QL_WMBUS_AUTO_RECV_DEV_MAX 64
#define QL_WMBUS_AUTO_RECV_PAYLOAD_LEN_MAX 512

ql_wmbus_auto_recv_callback g_ql_wmbus_auto_recv_cb = NULL;

ql_wmbus_auto_recv_dev_info_t *ql_wmbus_auto_recv_dev_info = NULL;

int16_t ql_wmbus_auto_recv_init(void)
{
    if(ql_wmbus_auto_recv_dev_info != NULL)
    {
        return -1;
    }

    ql_wmbus_auto_recv_dev_info = (ql_wmbus_auto_recv_dev_info_t *)sl_malloc(sizeof(ql_wmbus_auto_recv_dev_info_t));
    ql_wmbus_auto_recv_dev_info->node_list = NULL;
    ql_wmbus_auto_recv_dev_info->node_cnt = 0;

    if(!ql_wmbus_auto_recv_dev_info)
    {
        ql_printf("WMBUS AUTO RECV DEV INFO MALLOC FAILED\r\n");
        return -1;
    }

    return 0;
}

int16_t ql_wmbus_register_auto_recv_callback(ql_wmbus_auto_recv_callback recv_callback)
{
    if(recv_callback == NULL)
    {
        return -1;
    }

    g_ql_wmbus_auto_recv_cb = recv_callback;

    return 0;
}

static ql_wmbus_auto_recv_node_info_t *create_new_wmbus_auto_recv_node(ql_wmbus_auto_recv_dev_all_info_t *all_info)
{
    ql_wmbus_auto_recv_node_info_t *new_node = (ql_wmbus_auto_recv_node_info_t *)sl_malloc(sizeof(ql_wmbus_auto_recv_node_info_t));
    new_node->next = NULL;

    if(new_node == NULL)
    {
        ql_printf("CREATE NEW WMBUS AUTO RECV NODE FAILED\r\n");
        return NULL;
    }

    new_node->all_info.data.payload = (uint8_t *)sl_malloc(all_info->data.payload_len * sizeof(uint8_t));
    if(new_node->all_info.data.payload == NULL)
    {
        ql_printf("CREATE NEW WMBUS AUTO RECV NODE PAYLOAD FAILED\r\n");
        free(new_node);
        return NULL;
    }

    if(all_info->data.payload_len > QL_WMBUS_AUTO_RECV_PAYLOAD_LEN_MAX)
    {
        ql_printf("CREATE NEW WMBUS AUTO RECV NODE PAYLOAD LEN OVERFLOW\r\n");
        free(new_node->all_info.data.payload);
        free(new_node);
        return NULL;
    }

    memcpy(&new_node->all_info.cfg, &all_info->cfg, sizeof(ql_wmbus_auto_recv_config_t));

    memcpy(&new_node->all_info.dev_info, &all_info->dev_info, sizeof(ql_wmbus_auto_recv_base_dev_info_t));

    new_node->all_info.index_id         = all_info->index_id;
    new_node->all_info.data.length      = all_info->data.length;
    new_node->all_info.data.c_field     = all_info->data.c_field;
    new_node->all_info.data.ci_field    = all_info->data.ci_field;
    new_node->all_info.data.payload_len = all_info->data.payload_len;
    memcpy(new_node->all_info.data.payload, all_info->data.payload, all_info->data.payload_len);

    new_node->next = NULL;

    return new_node;
}

static int16_t ql_wmbus_check_config_is_valid(ql_wmbus_auto_recv_dev_all_info_t *all_info)
{
    if(all_info == NULL)
    {
        return -1;
    }

    if(all_info->data.length == 0)
    {
        return -1;
    }

    if(all_info->data.payload_len == 0)
    {
        return -1;
    }

    return 0;
}


/**
 * struct member compare
 * @param STR1
 * @param STR2
 * @param STR_BASE
 * @param MEMBER
 * @return int16_t equ == 1 not equ == 0
 */
#define STR_MEMBER_CMP(STR1, STR2, STR_BASE ,MEMBER) \
( (((STR_BASE*)STR1) -> MEMBER) == (((STR_BASE*)STR2) -> MEMBER) )


static int16_t ql_wmbus_check_config_data_is_available(ql_wmbus_auto_recv_dev_all_info_t *node_info, ql_wmbus_auto_recv_dev_all_info_t *all_info)
{
    if(node_info == NULL)
    {
        ql_printf("NODE INFO IS NULL\r\n");
        return -1;
    }

    if(ql_wmbus_check_config_is_valid(all_info) != 0)
    {
        return -1;
    }

    if((memcmp(&node_info->dev_info, &all_info->dev_info, sizeof(ql_wmbus_auto_recv_base_dev_info_t)) == 0) &&
        (memcmp(node_info->data.payload, all_info->data.payload, all_info->data.payload_len) == 0) &&
        (STR_MEMBER_CMP(&node_info->data, &all_info->data, ql_wmbus_auto_recv_data_info_t, length) == 1) &&
        (STR_MEMBER_CMP(&node_info->data, &all_info->data, ql_wmbus_auto_recv_data_info_t, c_field) == 1) &&
        (STR_MEMBER_CMP(&node_info->data, &all_info->data, ql_wmbus_auto_recv_data_info_t, ci_field) == 1) &&
        (STR_MEMBER_CMP(&node_info->data, &all_info->data, ql_wmbus_auto_recv_data_info_t, payload_len) == 1))
    {
        return -1;
    }

    return 0;
}

int16_t ql_wmbus_auto_recv_get_single_by_index(uint32_t index, ql_wmbus_auto_recv_data_info_t *info, ql_wmbus_auto_recv_config_t *cfg)
{
    ql_wmbus_auto_recv_node_info_t *current = ql_wmbus_auto_recv_dev_info->node_list;
    while(current != NULL)
    {
        if(current->all_info.index_id == index)
        {
            memcpy(info, &current->all_info.data, sizeof(ql_wmbus_auto_recv_data_info_t));
            memcpy(cfg, &current->all_info.cfg, sizeof(ql_wmbus_auto_recv_config_t));
            return 0;
        }

        current = current->next;
    }

    return -1;
}

int16_t ql_wmbus_auto_recv_set_is_enable_by_index(uint32_t index, uint8_t is_enable)
{
    ql_wmbus_auto_recv_node_info_t *current = ql_wmbus_auto_recv_dev_info->node_list;
    while(current != NULL)
    {
        if(current->all_info.index_id == index)
        {
            current->all_info.cfg.is_enable = is_enable;
            return 0;
        }

        current = current->next;
    }

    return -1;
}

int16_t ql_wmbus_auto_recv_set_config_by_index(uint32_t index, ql_wmbus_auto_recv_node_info_t *current ,ql_wmbus_auto_recv_data_info_t *recv_dev_info)
{
    if(current->all_info.index_id == index)
    {
        if(current->all_info.data.length == recv_dev_info->length && 
            current->all_info.data.c_field == recv_dev_info->c_field &&
            current->all_info.data.ci_field == recv_dev_info->ci_field &&
            current->all_info.data.payload_len == recv_dev_info->payload_len &&
            memcmp(current->all_info.data.payload, recv_dev_info->payload, recv_dev_info->payload_len) == 0)
        {
            return -1;
        }

        current->all_info.data.length      = recv_dev_info->length;
        current->all_info.data.c_field     = recv_dev_info->c_field;
        current->all_info.data.ci_field    = recv_dev_info->ci_field;
        current->all_info.data.payload_len = recv_dev_info->payload_len;
        memcpy(current->all_info.data.payload, recv_dev_info->payload, recv_dev_info->payload_len);

        return 0;
    }

    return -1;
}

int16_t add_new_wmbus_auto_recv_node_tail(ql_wmbus_auto_recv_dev_info_t *storage, ql_wmbus_auto_recv_dev_all_info_t *all_info)
{
    if(storage == NULL)
    {
        ql_printf("STORAGE IS NULL\r\n");
        return -1;
    }

    ql_wmbus_auto_recv_node_info_t *new_node = create_new_wmbus_auto_recv_node(all_info);
    if(new_node == NULL)
    {
        ql_printf("CREATE NEW WMBUS AUTO RECV NODE FAILED\r\n");
        return -1;
    }

    if(storage->node_list == NULL)
    {
        storage->node_list = new_node;
    }
    else
    {
        ql_wmbus_auto_recv_node_info_t *current = storage->node_list;
        while(current != NULL)
        {
            if(query_dev_wmbus_node_index_is_exist(all_info->index_id) != 0)
            {
                ql_printf("QUERY DEV WMBUS NODE INDEX IS EXIST\r\n");
                return -1;
            }
            
            if(ql_wmbus_check_config_data_is_available(current, all_info) != 0)
            {
                ql_printf("QUERY DEV WMBUS NODE CONFIG DATA IS NOT AVAILABLE\r\n");
                return -1;
            }
            else
            {
                int16_t ret = ql_wmbus_auto_recv_set_config_by_index(all_info->index_id, current, &all_info->data);
                if (ret == 0)
                {
                    ql_printf("SET CONFIG BY INDEX \r\n");
                    return 0;
                }
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
    ql_printf("ADD NEW WMBUS AUTO RECV NODE TAIL SUCCESS, NODE CNT:%d\r\n", storage->node_cnt);

    return 0;
}

uint32_t ql_wmbus_get_set_auto_recv_node_cnt(ql_wmbus_auto_recv_dev_info_t *storage)
{
    if(storage == NULL)
    {
        return 0;
    }

    return storage->node_cnt;
}

ql_wmbus_auto_recv_dev_info_t *ql_wmbus_get_auto_recv_dev_storage(void)
{
    return ql_wmbus_auto_recv_dev_info;
}

int16_t recognize_wmbus_auto_recv_is_active(ql_wmbus_auto_recv_dev_info_t *storage, ql_wmbus_auto_recv_base_dev_info_t *base_dev_info)
{
    if(!storage)
    {
        return -1;
    }

    if(!g_ql_wmbus_auto_recv_cb)
    {
        return -1;
    }

    if(storage->node_cnt <= 0)
    {
        return -1;
    }

    ql_wmbus_auto_recv_node_info_t *current = storage->node_list;

    uint32_t cnt = 0;
    while ((current != NULL) && (cnt < storage->node_cnt))
    {
        if(current->all_info.cfg.is_enable == 1)
        {
            if(memcmp(&current->all_info.dev_info, base_dev_info, sizeof(ql_wmbus_auto_recv_base_dev_info_t)) == 0)
            {
                g_ql_wmbus_auto_recv_cb(&current->all_info.dev_info, &current->all_info.data);
                return 0;
            }
        }
        cnt++;
        current = current->next;
    }
    
    return -1;
}

int16_t delete_wmbus_auto_recv_node_for_index(ql_wmbus_auto_recv_node_info_t **node_info_list, uint32_t index_id)
{
    if(query_dev_wmbus_node_index_is_exist(index_id) != 0)
    {
        return -1;
    }

    ql_wmbus_auto_recv_node_info_t *current = *node_info_list;
    ql_wmbus_auto_recv_node_info_t *last_node = NULL;

    while(current != NULL)
    {
        if(current->all_info.index_id == index_id)
        {
            if(last_node == NULL)
            {
                *node_info_list = current->next;
            }
            else
            {
                last_node->next = current->next;
            }

            sl_free(current->all_info.data.payload);
            sl_free(current);
            current = NULL;
            return 0;
        }

        last_node = current;
        current = current->next;
    }
    return -1;
}

