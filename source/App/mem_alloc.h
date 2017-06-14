#ifndef __MEM_ALLOC_H__
#define __MEM_ALLOC_H__
/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"


extern void free_ex_mem(void *p);
extern void* malloc_ex_mem(uint32_t bytes);




#endif //__MEM_ALLOC_H__
