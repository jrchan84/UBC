#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <assert.h>
#include "uthread.h"
#include "queue.h"
#include "disk.h"

queue_t pending_read_queue;

void interrupt_service_routine() {
  void* val;
  void* counter;
  void (*callback)(void*, void*);
  queue_dequeue(pending_read_queue, &val, &counter, &callback);
  callback(val, counter);
}

void handleOtherReads (void* resultv, void* countv) {
  int* result = resultv;
  int resultNum = *result;
  int* counter = countv;
  *counter = *counter - 1;

  //printf("handleOtherReads count: %d result: %d\n", *counter, *result);

  if(!*counter) {                 //found last block, print value
    printf ("%d\n", resultNum);
    exit (EXIT_SUCCESS);
  } else {                      // go to next block *result away
    disk_schedule_read(result, *result);
    queue_enqueue(pending_read_queue, result, counter, handleOtherReads);
  }
}

void handleFirstRead (void* resultv, void* countv) {
    int* result = resultv;
    int* counter = countv;
    *counter = *result;

    //printf("handleFirstRead %d\n", *result);

    if (*counter) {               // keep searching
      disk_schedule_read(result, *result);
      queue_enqueue (pending_read_queue, result, counter, handleOtherReads);
    } else {                    // stop and print
      printf("%d\n", *result);
      exit (EXIT_SUCCESS);
    }
}

int main (int argc, char** argv) {
  // Command Line Arguments
  static char* usage = "usage: treasureHunt starting_block_number";
  int starting_block_number;
  char *endptr;
  if (argc == 2)
    starting_block_number = strtol (argv [1], &endptr, 10);
  if (argc != 2 || *endptr != 0) {
    printf ("argument error - %s \n", usage);
    return EXIT_FAILURE;
  }

  // Initialize
  uthread_init (1);
  disk_start (interrupt_service_routine);
  pending_read_queue = queue_create();

  int result;
  int counter;

  disk_schedule_read(&result, starting_block_number);     // scheduel first search
  queue_enqueue(pending_read_queue, &result, &counter, handleFirstRead);


  while (1); // inifinite loop so that main doesn't return before hunt completes
}
