#ifndef __QL_UTILS_H__
#define __QL_UTILS_H__

#include "ql_include.h"
#include "sl_memory_manager.h"

typedef struct
{
    short int Year;
    short int Mon;
    short int Day;
    short int Hour;
    short int Min;
    short int Sec;
    short int Millisec;
} ql_build_time_t;

#define MEMORY_POOL_SIZE 1024

typedef struct
{
    uint8_t pool[MEMORY_POOL_SIZE];
    uint32_t offset;
} MemoryPool;

void ql_bootloader_query_build_time(ql_build_time_t *time);

void memory_pool_init(MemoryPool **pool);

void *memory_pool_alloc(MemoryPool *pool, size_t size);

void memory_pool_reset(MemoryPool *pool);

void memory_pool_free(MemoryPool *pool);

#endif /* __QL_UTILS_H__ */
