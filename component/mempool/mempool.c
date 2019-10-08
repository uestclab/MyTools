#include "mempool.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>
#include <sys/time.h>

g_mempool_para* g_mempool = NULL;

// size = 1 << level
g_mempool_para* createMemPool(int level){
	g_mempool = (g_mempool_para*)malloc(sizeof(g_mempool_para));
	g_mempool->level = level;
	g_mempool->size = 1 << level;
	g_mempool->ptr = malloc(g_mempool->size);
	g_mempool->b = buddy_new(level);
	return g_mempool;
}

void mempool_delete(g_mempool_para* g_mempool){
	if(g_mempool->ptr != NULL)
		free(g_mempool->ptr);
	if(g_mempool->b != NULL)
		buddy_delete(g_mempool->b);
	if(g_mempool != NULL)
		free(g_mempool);
}

void* mallocPool(int size){
	int offset = buddy_alloc(g_mempool->b,size);
	if(-1 == offset)
		return NULL;
	return g_mempool->ptr + offset;
}

void freePool(void *ptr){
	assert( ptr != NULL);
	int offset = ptr - g_mempool->ptr;
	buddy_free(g_mempool->b,offset);
	//ptr = NULL;
}

int allocted_size(void *ptr){
	assert(ptr != NULL);
	int offset = ptr - g_mempool->ptr;
	int s = buddy_size(g_mempool->b,offset);
	return s;
}

/* ------------------------------- Benchmark ---------------------------------*/

long long timeInMilliseconds(void) {
    struct timeval tv;

    gettimeofday(&tv,NULL);
    return (((long long)tv.tv_sec)*1000)+(tv.tv_usec/1000);
}

#ifdef BENCHMARK_MAIN

#define start_benchmark() start = timeInMilliseconds()
#define end_benchmark(msg) do { \
    elapsed = timeInMilliseconds()-start; \
    printf(msg ": in %lld ms\n", elapsed); \
} while(0);

int
main() {
	g_mempool_para* tmp_pool = createMemPool(14);

	printf("header ptr = 0x%x , size = %d\n",tmp_pool->ptr,tmp_pool->size);

	long long start, elapsed;
	int j;
	int number = 100000;

	start_benchmark();
	for(j=0;j<number;j++){
		int* tmp_i = (int*)malloc(100*sizeof(int));
		char* tmp_c = (char*)malloc(100);
		free(tmp_i);
		free(tmp_c);
		tmp_i = NULL;
		double* tmp_d = (double*)malloc(4*sizeof(double));
		tmp_i = (int*)malloc(8*sizeof(int));
		free(tmp_d);
		free(tmp_i);
	}
	end_benchmark("malloc");


	start_benchmark();
	for(j=0;j<number;j++){
		int* tmp_i = (int*)mallocPool(100*sizeof(int));
		char* tmp_c = (char*)mallocPool(100);
		freePool(tmp_i);
		freePool(tmp_c);
		tmp_i = NULL;
		double* tmp_d = (double*)mallocPool(4*sizeof(double));
		tmp_i = (int*)mallocPool(8*sizeof(int));
		freePool(tmp_d);
		freePool(tmp_i);
	}
	end_benchmark("mempool");

	return 0;
}
#endif