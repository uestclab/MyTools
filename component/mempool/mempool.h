#ifndef MEMPOOL_H
#define MEMPOOL_H

#include "buddy.h"

// void *zmalloc(size_t size) {
//     void *ptr = malloc(size+PREFIX_SIZE);

//     if (!ptr) zmalloc_oom_handler(size);
// #ifdef HAVE_MALLOC_SIZE
//     update_zmalloc_stat_alloc(zmalloc_size(ptr));
//     return ptr;
// #else
//     *((size_t*)ptr) = size;
//     update_zmalloc_stat_alloc(size+PREFIX_SIZE);
//     return (char*)ptr+PREFIX_SIZE;
// #endif
//}

typedef struct g_mempool_para{
    int level;
    int size;
    void *ptr;
    struct buddy *b;
}g_mempool_para;

g_mempool_para* createMemPool(int level); // 2^level
void mempool_delete(g_mempool_para* g_mempool);
void* mallocPool(int size);
void freePool(void *ptr);
int allocted_size(void *ptr);

#endif
