/**
 * @file chat_thread_poll.h
 */

#ifndef _CHAT_SRC_SERVER_CHAT_THREAD_POOL_H_
#define _CHAT_SRC_SERVER_CHAT_THREAD_POOL_H_

#include <pthread.h>

#include "queue.h"


typedef void *(* task)(void *);



typedef struct thread_task_s {
  task task;
  void *arg;
} thread_task_t;

thread_task_t *thread_task_create(task task, void *arg);

void thread_task_destroy(thread_task_t *thread_task);

typedef struct thread_pool_s
{
  queue_t     * queue;
  pthread_t        * pool;
  pthread_mutex_t    mutex;
  pthread_cond_t     cond_var;
  int                num_threads;
  volatile int       active;
} thread_pool_t;


thread_pool_t *thread_pool_create(int num_threads);

void thread_pool_ctl(thread_pool_t *thread_pool);

void thread_pool_add_task(thread_pool_t *thread_pool, task task, void *arg);

void thread_pool_destroy(thread_pool_t *thread_pool);

#endif
