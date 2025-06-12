#include "ql_utils.h"

void ql_bootloader_query_build_time(ql_build_time_t *time)
{
   char mon_str[5] = { 0 };
   static char month_names[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
   // hh:mm:ss
   sscanf(__TIME__, "%hd:%hd:%hd", &time->Hour, &time->Min, &time->Sec);

   // Mmm dd yyyy
   sscanf(__DATE__, "%s %hd %hd", mon_str, &time->Day, &time->Year);
   time->Mon = ((strstr(month_names, mon_str) - &month_names[0]) / 3 + 1);
}

void memory_pool_init(MemoryPool **pool)
{
    *pool = (MemoryPool *)sl_malloc(sizeof(MemoryPool));
    (*pool)->offset = 0;
}

void *memory_pool_alloc(MemoryPool *pool, size_t size)
{
    if(pool->offset + size > MEMORY_POOL_SIZE)
    {
        return NULL;
    }

    void *ptr = pool->pool + pool->offset;
    pool->offset += size;
    return ptr;
}

void memory_pool_reset(MemoryPool *pool)
{
    pool->offset = 0;
}

void memory_pool_free(MemoryPool *pool)
{
    if (pool != NULL) {
        sl_free(pool);
        pool = NULL;
    }
}