/**
 * @file proto.h
 */

#ifndef _SRC_SERVER_QUEUE_H_
#define _SRC_SERVER_QUEUE_H_

#include <stddef.h>


struct dnode_s {
  void *data;
  struct dnode_s *next;
  struct dnode_s *prev;
};


typedef struct queue_s {
  struct dnode_s * front;
  struct dnode_s * back;
  size_t                size;
} queue_t;


queue_t *queue_create();


void queue_push(queue_t *q, const void *data);


void queue_pop(queue_t *q);


void *queue_front(const queue_t *q);


void *queue_back(const queue_t *q);


size_t queue_size(const queue_t *q);


void queue_destroy(queue_t *q);

#endif
