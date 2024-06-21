#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"


void *func(void *arg)
{
  sleep(1);
  printf("func with argument %d\n", *(int *)arg);

  return NULL;
}


int main(){
  int arr[100] = {0};

  for(int i = 0; i < 100; i++){
    arr[i] = i;
  }

  thread_pool_t *thread_pool = thread_pool_create(10);

  for(int i = 0; i < 100; i++)
    thread_pool_add_task(thread_pool, func, arr + i);

  thread_pool_ctl(thread_pool);
  thread_pool_destroy(thread_pool);
}
