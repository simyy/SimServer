/*
 * pool.h
 * =========
 * (c) Copyright xianda yu 2013/11/19
 * memory pool
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "pool.h"

struct pool* createPool(size_t size)
{
	struct pool*       pp;
	struct pool_block* p;

	p =(struct pool_block*)malloc(sizeof(struct pool_block));
	if( p == NULL){
		printf("malloc fail .\n");
		return NULL;
	}

	p->space = (char*)malloc(size);

	p->last = p->space;
	p->end  = p->space + size; 
	p->next = NULL;	

	pp = (struct pool*)malloc(sizeof(struct pool));
	pp->n    = 1;
	pp->head = p; 
	
	printf("create success.\n");
	return pp;
}

void destroyPool(struct pool* m_pool)
{
	struct pool_block* p;
	struct pool_block* ptr;
	int i;
	
	p = m_pool->head;
	for(i = 0; i < m_pool->n; ++i){
		ptr = p->next;
		free(p->space);
		free(p);
		p = ptr;
	}
	m_pool = NULL;
	free(m_pool);

	printf("destroy success.\n");
}

void* palloc(struct pool* m_pool, size_t size)
{
	char*       s;
	struct pool_block* p;
	struct pool_block* q;
	int i;

	p = m_pool->head;
	for(i = 0; i < m_pool->n; ++i){
		if(size < (size_t)(p->end - p->last)){
			s = p->last;
			p->last += size;
			printf("palloc success.\n");	
			return s;	
		}
		if(p->next != NULL)
			p = p->next;	
	}

	q = (struct pool_block*)malloc(sizeof(struct pool_block));
	if(size < MAX_SIZE){	
		q->space = (char*)malloc(MAX_SIZE);
		q->last  = q->space;
		q->end   = (char*)q + MAX_SIZE;
	}
	else{
		q->space = (char*)malloc(size + 1);
		q->last  = q->space;
		q->end   = (char*)q + size+1;
	}
	q->next = NULL;

	p->next = q;
	m_pool->n += 1;
	
	printf("palloc success.\n");	
	return (void*)q;
}
