
#include "mem_alloc.h"
#include "stm32f4xx.h"

#ifndef NULL
#define NULL  0
#endif

#define MEM1_BLOCK_SIZE		    32 //内存块大小为32字节

typedef struct{
    uint32_t start;///<起始块
    uint32_t end;///<结束块
    uint8_t flag;///<使用标记
    void *p;///<分配的地址
}MEM_MANAGE_INF;

MEM_MANAGE_INF mem_manage_pool[10];

#define EX_RAM_SIZE         (2 * 1024 * 1024)
#define GET_EX_RAM_ADDR(B)  ((uint8_t*)0x60000000 + (B));

void* malloc_ex_mem(uint32_t bytes)
{
    uint32_t n = sizeof(mem_manage_pool) / sizeof(mem_manage_pool[0]);
    uint32_t s = 0;
    uint32_t half_num;
    int32_t i = 0;
    
    half_num = (bytes + 1)/2;//半字个数
    
    for(i = 0; i < n; i++)
    {
        if(mem_manage_pool[i].flag == 1)
        {
            if(s < mem_manage_pool[i].end)
            {
                s = mem_manage_pool[i].end;
            }
        }
    }
    
    if(EX_RAM_SIZE < (bytes + s))
    {
        return NULL;
    }
    
    for(i = 0; i < n; i++)
    {
        if(mem_manage_pool[i].flag == 0)
        {
            mem_manage_pool[i].flag = 1;
            mem_manage_pool[i].start = s;
            mem_manage_pool[i].end = mem_manage_pool[i].start + half_num * 2;
            mem_manage_pool[i].p = GET_EX_RAM_ADDR(mem_manage_pool[i].start);
            return mem_manage_pool[i].p;
        }
    }
    
    return NULL;
}


void free_ex_mem(void *p)
{
    int32_t i = 0;
    uint32_t n = sizeof(mem_manage_pool) / sizeof(mem_manage_pool[0]);
    
    for(i = 0; i < n; i++)
    {
        if(mem_manage_pool[i].flag == 1)
        {
            if(p == mem_manage_pool[i].p)
            {
                mem_manage_pool[i].p = NULL;
                mem_manage_pool[i].flag = 0;
                mem_manage_pool[i].start = 0;
                mem_manage_pool[i].end = 0;
            }
        }
    }
}










