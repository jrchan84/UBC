#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>
#include "uthread.h"
#include "uthread_mutex_cond.h"

#define NUM_ITERATIONS 1000

#ifdef VERBOSE
#define VERBOSE_PRINT(S, ...) printf (S, ##__VA_ARGS__);
#else
#define VERBOSE_PRINT(S, ...) ;
#endif

struct Agent {
  uthread_mutex_t mutex;
  uthread_cond_t  match;
  uthread_cond_t  paper;
  uthread_cond_t  tobacco;
  uthread_cond_t  smoke;
};

struct Agent* createAgent() {
  struct Agent* agent = malloc (sizeof (struct Agent));
  agent->mutex   = uthread_mutex_create();
  agent->paper   = uthread_cond_create (agent->mutex);
  agent->match   = uthread_cond_create (agent->mutex);
  agent->tobacco = uthread_cond_create (agent->mutex);
  agent->smoke   = uthread_cond_create (agent->mutex);
  return agent;
}

/**
 * You might find these declarations helpful.
 *   Note that Resource enum had values 1, 2 and 4 so you can combine resources;
 *   e.g., having a MATCH and PAPER is the value MATCH | PAPER == 1 | 2 == 3
 */
enum Resource            {    MATCH = 1, PAPER = 2,   TOBACCO = 4};
char* resource_name [] = {"", "match",   "paper", "", "tobacco"};

int signal_count [5];  // # of times resource signalled
int smoke_count  [5];  // # of times smoker with resource smoked

//
// TODO
// You will probably need to add some procedures and struct etc.
//

struct ThreeSmokers {
  struct Agent* agent;
  int match;
  int tobacco;
  int paper;
};

struct Smoker {
  struct ThreeSmokers* group;
  int type;
};

struct ThreeSmokers* createSmokerGroup(struct Agent* agent) {
  struct ThreeSmokers* smokerGroup = malloc (sizeof (struct ThreeSmokers));
  smokerGroup->agent = agent;
  smokerGroup->match = 0;
  smokerGroup->tobacco = 0;
  smokerGroup->paper = 0;
  return smokerGroup;
}

struct Smoker* createSmoker(struct ThreeSmokers* s, int type) {
  struct Smoker* smoker = malloc (sizeof (struct Smoker));
  smoker->group = s;
  smoker->type = type;
}

void* smoke (void* sv) {
  struct Smoker* s = sv;
  struct ThreeSmokers* smokerGroup = s->group;
  struct Agent* a = smokerGroup->agent;

  uthread_mutex_lock(a->mutex);

  while(1) {
    switch (s->type) {
      case MATCH:
           uthread_cond_wait(a->match);
           //printf("smoke wait(match)");
           if (smokerGroup->tobacco > 0 && smokerGroup->paper > 0) {
             smokerGroup->tobacco--;
             smokerGroup->paper--;
             smoke_count[s->type]++;
             uthread_cond_signal(a->smoke);
           } else {
             smokerGroup->match++;
           }
           break;
      case TOBACCO:
           uthread_cond_wait(a->tobacco);
           //printf("smoke wait(tobacco)");
           if (smokerGroup->paper > 0 && smokerGroup->match > 0) {
             smokerGroup->paper--;
             smokerGroup->match--;
             smoke_count[s->type]++;
             uthread_cond_signal(a->smoke);
           } else {
             smokerGroup->tobacco++;
           }
           break;
      case PAPER:
           //printf("smoke wait(paper)");
           uthread_cond_wait(a->paper);
           if (smokerGroup->tobacco > 0 && smokerGroup->match > 0) {
             smokerGroup->tobacco--;
             smokerGroup->match--;
             smoke_count[s->type]++;
             uthread_cond_signal(a->smoke);
           } else {
             smokerGroup->paper++;
           }
           break;
      default:
            break;
    }

    if (smokerGroup->tobacco > 0 && smokerGroup->paper > 0) {
      uthread_cond_signal(a->match);
    } else if (smokerGroup->paper > 0 && smokerGroup->match > 0) {
      uthread_cond_signal(a->tobacco);
    } else if (smokerGroup->match > 0 && smokerGroup->tobacco > 0) {
      uthread_cond_signal(a->paper);
    }
  }

  uthread_mutex_unlock(a->mutex);
  return NULL;
}

/**
 * This is the agent procedure.  It is complete and you shouldn't change it in
 * any material way.  You can re-write it if you like, but be sure that all it does
 * is choose 2 random reasources, signal their condition variables, and then wait
 * wait for a smoker to smoke.
 */
void* agent (void* av) {
  struct Agent* a = av;
  static const int choices[]         = {MATCH|PAPER, MATCH|TOBACCO, PAPER|TOBACCO};
  static const int matching_smoker[] = {TOBACCO,     PAPER,         MATCH};

  uthread_mutex_lock (a->mutex);
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      int r = random() % 3;
      signal_count [matching_smoker [r]] ++;
      int c = choices [r];
      if (c & MATCH) {
        VERBOSE_PRINT ("match available\n");
        uthread_cond_signal (a->match);
      }
      if (c & PAPER) {
        VERBOSE_PRINT ("paper available\n");
        uthread_cond_signal (a->paper);
      }
      if (c & TOBACCO) {
        VERBOSE_PRINT ("tobacco available\n");
        uthread_cond_signal (a->tobacco);
      }
      VERBOSE_PRINT ("agent is waiting for smoker to smoke\n");
      uthread_cond_wait (a->smoke);
    }
  uthread_mutex_unlock (a->mutex);
  return NULL;
}

int main (int argc, char** argv) {
  uthread_init (7);
  struct Agent*  a = createAgent();
  struct ThreeSmokers* s = createSmokerGroup(a);

  uthread_t match = uthread_create(smoke, createSmoker(s, MATCH));
  uthread_t paper = uthread_create(smoke, createSmoker(s, PAPER));
  uthread_t tobacco = uthread_create(smoke, createSmoker(s, TOBACCO));

  // TODO
  uthread_join (uthread_create (agent, a), 0);
  assert (signal_count [MATCH]   == smoke_count [MATCH]);
  assert (signal_count [PAPER]   == smoke_count [PAPER]);
  assert (signal_count [TOBACCO] == smoke_count [TOBACCO]);
  assert (smoke_count [MATCH] + smoke_count [PAPER] + smoke_count [TOBACCO] == NUM_ITERATIONS);
  printf ("Smoke counts: %d matches, %d paper, %d tobacco\n",
          smoke_count [MATCH], smoke_count [PAPER], smoke_count [TOBACCO]);
}
