#include <stdlib.h>
#include <string.h>
#include "element.h"
#include "refcount.h"

struct element {
  char*           value;  // Possibly referenced counted; you decide
  struct element* prev;
  struct element* next;
};

/**
 * Create an element
 */
struct element* element_create (char* value) {
  struct element* e = rc_malloc (sizeof (struct element));
  int value_length = strlen (value);
  e->value = rc_malloc (value_length + 1);
  strncpy (e->value, value, value_length);
  e->prev = NULL;
  e->next = NULL;
  return e;
}

/**
 * Get element's value
 */
char* element_get_value (struct element* e) {
  return e->value;
}

/**
 * Set element's value
 */
void element_set_value (struct element* e, char* value) {
  e->value = value;
}

/**
 * Get prev element of list.
 */
struct element* element_get_prev (struct element* e) {
  return e->prev;
}

/**
 * Set prev element of list.
 */
void element_set_prev(struct element* e, struct element* prev) {
  e->prev = prev;
}

/**
 * Get next element of list.
 */
struct element* element_get_next (struct element* e) {
  return e->next;
}

/**
 * Set next element of list.
 */
void element_set_next (struct element* e, struct element* next) {
  e->next = next;
}

/**
 * Increment element's reference count
 */
void element_keep_ref (struct element* e) {
  rc_keep_ref (element_get_value(e));
  rc_keep_ref (e);
}

/**
 * Decrement element's reference count
 */
void element_free_ref (struct element* e) {
  rc_keep_ref (element_get_value(e));
  rc_free_ref (e);
}
