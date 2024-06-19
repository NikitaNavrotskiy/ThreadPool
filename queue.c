#include <stdlib.h>

#include "queue.h"


static struct dnode_s *__dnode_create(void *data, struct dnode_s *next,
          struct dnode_s *prev)
{
  struct dnode_s *nd = malloc(sizeof(struct dnode_s));

  if(!nd)
    return NULL;

  nd->data = data;
  nd->next = next;
  nd->prev = prev;

  return nd;
}


queue_t *queue_create()
{
  queue_t *q = malloc(sizeof(queue_t));

  if(!q)
    return NULL;

  q->front = NULL;
  q->back  = NULL;
  q->size  = 0;

  return q;
}


void queue_push(queue_t *q, const void *data)
{
  q->front = __dnode_create((void *)data, q->front, NULL);

  if(q->back == NULL)
    q->back = q->front;
  else 
    q->front->next->prev = q->front;

  q->size++;
}


void queue_pop(queue_t *q)
{
  struct dnode_s *tmp = q->back;

  q->back = tmp->prev;

  if(tmp->prev)
    q->back->next = NULL;
  else 
    q->front = NULL;

  q->size--;

  free(tmp);
}


inline void *queue_front(const queue_t *q)
{
  return q->front->data;
}


inline void *queue_back(const queue_t *q)
{
  return q->back->data;
}


inline size_t queue_size(const queue_t *q)
{
  return q->size;
}


void queue_destroy(queue_t *q)
{
  struct dnode_s *tmp = q->front;

  while(tmp){
    struct dnode_s *tmp_next = tmp->next;
    free(tmp);
    tmp = tmp_next;
  }

  free(q);
}

