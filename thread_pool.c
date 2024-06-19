#include <pthread.h>
#include <stdlib.h>

#include "thread_pool.h"


static void *thread_pool_routine(void *arg)
{
  thread_pool_t *thread_pool = (thread_pool_t *)arg;
  thread_task_t *thread_task = NULL;

  for(;;)
  {
    pthread_mutex_lock(&thread_pool->mutex);
    
    while(thread_pool->queue->size == 0)
      pthread_cond_wait(&thread_pool->cond_var, &thread_pool->mutex);
    
    if(queue_size(thread_pool->queue) > 0) {
      thread_task = (thread_task_t *)queue_back(thread_pool->queue);
      queue_pop(thread_pool->queue);
    }

    pthread_mutex_unlock(&thread_pool->mutex);

    if(thread_task){
      if(*(int *)thread_task->arg == 99)
        thread_pool->active = 0;
      thread_task->task(thread_task->arg);
      thread_task_destroy(thread_task);
    }

    if(!thread_pool->active)
      break;
  }

  return NULL;
}

thread_pool_t *thread_pool_create(int num_threads)
{
  thread_pool_t *thread_pool = malloc(sizeof(thread_pool_t));

  if(!thread_pool)
    return NULL;

  thread_pool->num_threads  = num_threads;
  thread_pool->active       = 0;
  thread_pool->pool         = malloc(sizeof(pthread_t) * num_threads);
  thread_pool->queue        = queue_create();

  pthread_mutex_init(&thread_pool->mutex, NULL);
  pthread_cond_init(&thread_pool->cond_var, NULL);

  for(int i = 0; i < num_threads; i++){
    pthread_create(thread_pool->pool + i, NULL, thread_pool_routine, thread_pool);
  }

  return thread_pool;
}


void thread_pool_ctl(thread_pool_t *thread_pool)
{
  thread_pool->active = 1;

  for(int i = 0; i < thread_pool->num_threads; i++){
    pthread_join(thread_pool->pool[i], NULL);
  } 
}


void thread_pool_add_task(thread_pool_t *thread_pool, task task, void *arg)
{
  thread_task_t *thread_task = thread_task_create(task, arg);

  if(thread_task == NULL)
    return;

  pthread_mutex_lock(&thread_pool->mutex);
  queue_push(thread_pool->queue, thread_task);
  pthread_mutex_unlock(&thread_pool->mutex);

  pthread_cond_broadcast(&thread_pool->cond_var);
}


void thread_pool_destroy(thread_pool_t *thread_pool)
{
  pthread_mutex_destroy(&thread_pool->mutex);
  pthread_cond_destroy(&thread_pool->cond_var);

  queue_destroy(thread_pool->queue);
  free(thread_pool->pool);
  free(thread_pool);
}


thread_task_t *thread_task_create(task task, void *arg)
{
  thread_task_t *thread_task = malloc(sizeof(thread_task_t));

  if(!thread_task)
    return NULL;

  thread_task->task = task;
  thread_task->arg  = arg;

  return thread_task;
}


inline void thread_task_destroy(thread_task_t *thread_task)
{
  free(thread_task);
}


