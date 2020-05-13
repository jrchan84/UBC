#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "uthread.h"
#include "uthread_sem.h"

#define MAX_ITEMS      10
#define NUM_ITERATIONS 200
#define NUM_PRODUCERS  2
#define NUM_CONSUMERS  2
#define NUM_PROCESSORS 4

// histogram [i] == # of times list stored i items
int histogram [MAX_ITEMS+1];

// number of items currently produced but not yet consumed
// invariant that you must maintain: 0 >= items >= MAX_ITEMS

struct Group {
  uthread_sem_t mx;
  uthread_sem_t hasItems;
  uthread_sem_t notFull;
  int items;
};

struct Group* createGroup() {
  struct Group* group = malloc (sizeof (struct Group));
  group->mx = uthread_sem_create(1);
  group->hasItems = uthread_sem_create(0);
  group->notFull = uthread_sem_create(MAX_ITEMS);
  group->items = 0;
  return group;
}

// if necessary wait until items < MAX_ITEMS and then increment items
// assertion checks the invariant that 0 >= items >= MAX_ITEMS
void* producer (void* v) {
  struct Group* g = v;
  for (int i=0; i<NUM_ITERATIONS; i++) {
    uthread_sem_wait(g->notFull);
    uthread_sem_wait(g->mx);
    g->items++;
    assert (g->items >= 0 && g->items <= MAX_ITEMS);
    histogram [g->items] ++;
    uthread_sem_signal(g->hasItems);
    uthread_sem_signal(g->mx);
  }
  return NULL;
}

// if necessary wait until items > 0 and then decrement items
// assertion checks the invariant that 0 >= items >= MAX_ITEMS
void* consumer (void* v) {
  struct Group* g = v;
  for (int i=0; i<NUM_ITERATIONS; i++) {
    uthread_sem_wait(g->hasItems);
    uthread_sem_wait(g->mx);
    g->items--;
    assert (g->items >= 0 && g->items <= MAX_ITEMS);
    histogram [g->items] ++;
    uthread_sem_signal(g->notFull);
    uthread_sem_signal(g->mx);
  }
  return NULL;
}

int main (int argc, char** argv) {

  // init the thread system
  uthread_init (NUM_PROCESSORS);
  struct Group* g = createGroup();

  // start the threads
  uthread_t threads [NUM_PRODUCERS + NUM_CONSUMERS];
  for (int i = 0; i < NUM_PRODUCERS; i++)
    threads [i] = uthread_create (producer, g);
  for (int i = NUM_PRODUCERS; i < NUM_PRODUCERS + NUM_CONSUMERS; i++)
    threads [i] = uthread_create (consumer, g);

  // wait for threads to complete
  for (int i=0; i < NUM_PRODUCERS + NUM_CONSUMERS; i++)
    uthread_join (threads [i], NULL);

  // sum up
  printf ("items value histogram:\n");
  int sum=0;
  for (int i = 0; i <= MAX_ITEMS; i++) {
    printf ("  items=%d, %d times\n", i, histogram [i]);
    sum += histogram [i];
  }
  // checks invariant that ever change to items was recorded in histogram exactly one
  assert (sum == (NUM_PRODUCERS + NUM_CONSUMERS) * NUM_ITERATIONS);
}
