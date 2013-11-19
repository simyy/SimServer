/*
 * pool.h
 * =========
 * (c) Copyright xianda yu 2013/11/19
 * memory pool
 *
 */

#ifndef _POOL_H
#define _POOL_H

#include <stdio.h>

#define MAX_SIZE 1000

struct pool_block{
	char* last; /* the last of used space */
	char* end;  /* the end of pool */
	struct pool_block* next; /* connect to next pool */
};

struct pool{
	int   n;
	struct pool_block* head;
};

struct pool* createPool(size_t size);

void  destroyPool(struct pool* m_pool);

void* palloc(struct pool* m_pool, size_t size);


#endif
