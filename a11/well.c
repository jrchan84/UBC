#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

#define MAX_OCCUPANCY      3
#define NUM_ITERATIONS     100
#define NUM_PEOPLE         20
#define FAIR_WAITING_COUNT 4

/**
 * You might find these declarations useful.
 */
enum Endianness {LITTLE = 0, BIG = 1};
const static enum Endianness oppositeEnd [] = {BIG, LITTLE};

struct Well {
  uthread_mutex_t mx;
  uthread_cond_t little;
  uthread_cond_t big;
  int count;
  int curr;
  enum Endianness type;
};

struct Well* createWell() {
  struct Well* Well = malloc (sizeof (struct Well));
  Well->mx = uthread_mutex_create();
  Well->little = uthread_cond_create(Well->mx);
  Well->big = uthread_cond_create(Well->mx);
  Well->count = 0;
  Well->curr = 0;
  Well->type = 0;
  return Well;
}

struct Well* Well;

#define WAITING_HISTOGRAM_SIZE (NUM_ITERATIONS * NUM_PEOPLE)
int             entryTicker;                                          // incremented with each entry
int             waitingHistogram         [WAITING_HISTOGRAM_SIZE];
int             waitingHistogramOverflow;
uthread_mutex_t waitingHistogrammutex;
int             occupancyHistogram       [2] [MAX_OCCUPANCY + 1];

void recordWaitingTime (int waitingTime) {
  uthread_mutex_lock (waitingHistogrammutex);
  if (waitingTime < WAITING_HISTOGRAM_SIZE)
    waitingHistogram [waitingTime] ++;
  else
    waitingHistogramOverflow ++;
  uthread_mutex_unlock (waitingHistogrammutex);
}

void enterWell (struct Well* well, enum Endianness g) {
  uthread_mutex_lock(well->mx);
  if (well->count == 0) {
    well->type = g;
  } else {
    int entered = well->curr;
    if (g == LITTLE) {
      uthread_cond_wait(well->big);
    } else if (g == BIG) {
      uthread_cond_wait(well->little);
    }
    int waitingTime = well->curr - entered; // w->curr may have changed as this thread waits for its turn. entered is original curr
    recordWaitingTime(waitingTime);
  }
  well->curr++;
  well->count++;
  occupancyHistogram[g][well->count]++;
  uthread_mutex_unlock(well->mx);
}

void leaveWell(struct Well* well, enum Endianness g) {
  uthread_mutex_lock(well->mx);
  well->count--;
  occupancyHistogram[g][well->count]--;
  if (well->count == 0) {
    well->type = (well->type + 1) % 2;
    for (int i=0; i<3; i++) {
      if (well->type == LITTLE) {
        uthread_cond_signal(well->big);
      } else if (well->type == BIG) {
        uthread_cond_signal(well->little);
      }
    }
  }
  uthread_mutex_unlock(well->mx);
}

//
// TODO
// You will probably need to create some additional produres etc.
//

void* people(void* wellv) {
  struct Well* well = wellv;
  enum Endianness type = random() % 2;

  for (int i =0; i<NUM_ITERATIONS; i++) {
    enterWell(well, type);
    for (int i = 0; i<NUM_PEOPLE; i++) {
      uthread_yield();
    }
    leaveWell(well, type);
    for (int i = 0; i<NUM_PEOPLE; i++) {
      uthread_yield();
    }
  }
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (1);
  struct Well* well = createWell(); // changed to pointer to fix warnings
  uthread_t pt [NUM_PEOPLE];
  waitingHistogrammutex = uthread_mutex_create ();

  // TODO
  for (int i=0; i<NUM_PEOPLE; i++) {
    pt[i] = uthread_create(people, well);
  }

  for (int i=0; i<NUM_PEOPLE; i++) {
    uthread_join(pt[i], 0);
  }

  printf ("Times with 1 little endian %d\n", occupancyHistogram [LITTLE]   [1]);
  printf ("Times with 2 little endian %d\n", occupancyHistogram [LITTLE]   [2]);
  printf ("Times with 3 little endian %d\n", occupancyHistogram [LITTLE]   [3]);
  printf ("Times with 1 big endian    %d\n", occupancyHistogram [BIG] [1]);
  printf ("Times with 2 big endian    %d\n", occupancyHistogram [BIG] [2]);
  printf ("Times with 3 big endian    %d\n", occupancyHistogram [BIG] [3]);
  printf ("Waiting Histogram\n");
  for (int i=0; i<WAITING_HISTOGRAM_SIZE; i++)
    if (waitingHistogram [i])
      printf ("  Number of times people waited for %d %s to enter: %d\n", i, i==1?"person":"people", waitingHistogram [i]);
  if (waitingHistogramOverflow)
    printf ("  Number of times people waited more than %d entries: %d\n", WAITING_HISTOGRAM_SIZE, waitingHistogramOverflow);
}
