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

	p =	malloc(size);
	if( p == NULL){
		printf("malloc fail .\n");
		return NULL;
	}

	p->last = (char*)p;
	p->end  = (char*)p + size; 
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
		free(p);
		p = ptr;
	}
	/*
	for(p = m_pool->head; p != NULL; p = ptr){
		ptr = p->next;
		free(p);
	}	
*/
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

	if(size < MAX_SIZE)	
		q = (struct pool_block*)malloc(MAX_SIZE);
	else
		q = (struct pool_block*)malloc(size);
	q->last  = (char*)q;
	q->end   = (char*)q + MAX_SIZE;
	q->next = NULL;

	p->next = q;
	m_pool->n += 1;
	
	printf("palloc success.\n");	
	return (void*)q;
}
