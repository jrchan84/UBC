#include <stdlib.h>
#include <stdio.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_THREADS 3
uthread_t threads[NUM_THREADS];
uthread_mutex_t mx;
uthread_cond_t a;

void randomStall() {
  int i, r = random() >> 16;
  while (i++<r);
}

void waitForAllOtherThreads() {
  if(uthread_self() == threads[NUM_THREADS-1]) {
    uthread_cond_broadcast(a);
  } else {
    uthread_cond_wait(a);
  }
}

void* p(void* v) {
  uthread_mutex_lock(mx);
  randomStall();
  printf("a\n");
  waitForAllOtherThreads();
  printf("b\n");
  uthread_mutex_unlock(mx);
  return NULL;
}

int main(int arg, char** arv) {
  mx = uthread_mutex_create();
  a = uthread_cond_create(mx);
  uthread_init(4);
  for (int i=0; i<NUM_THREADS; i++)
    threads[i] = uthread_create(p, NULL);
  for (int i=0; i<NUM_THREADS; i++)
    uthread_join (threads[i], NULL);
  printf("------\n");
}
