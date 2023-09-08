#pragma once
#include <os.h>

struct array_stack {
  u64 end, capacity;
};

#define array_stack_header_size(a) \
  ALIGNUP(sizeof(struct array_stack), ALIGNOF(__typeof__(a[0])))

#define array_stack_header(a) \
  ((struct array_stack *)OFFSETBY(a, -array_stack_header_size(a)))

#define array_stack_free(a)                           \
  do {                                                \
    if (a == NULL) break;                             \
    struct array_stack *s__ = array_stack_header(a);    \
    memory_realloc(s__, s__->capacity * sizeof(a[0]), 0); \
    a = NULL;                                         \
  } while (0)

static void *array_stack_grow(void *a, u64 header_size, u64 item_size) {
  struct array_stack *s = NULL;
  if (a == NULL) {
    s = memory_realloc(NULL, 0, header_size + 32 * item_size);
    if (s == NULL) {
      return NULL;
    }
    s->capacity = 32;
    s->end = 0;
  } else {
    s = OFFSETBY(a, -header_size);
    if (s->end == s->capacity) {
      s = memory_realloc(s, header_size + s->capacity * item_size,
                         header_size + 2 * s->capacity * item_size);
      if (s == NULL) {
        return a;
      }
      s->capacity *= 2;
    }
  }
  return OFFSETBY(s, header_size);
}

#define array_stack_full(a) \
  (a == NULL || array_stack_header(a)->end == array_stack_header(a)->capacity)

#define array_stack_empty(a) (a == NULL || array_stack_header(a)->end == 0)

#define array_stack_push_back(a, value)                                       \
  ((void)(a = array_stack_grow(a, array_stack_header_size(a), sizeof(a[0]))), \
   array_stack_full(a)                                                        \
       ? false                                                                \
       : ((void)(a[array_stack_header(a)->end++] = value), true))

#define array_stack_pop_back(a) \
  ((void)ASSERT(!array_stack_empty(a)), a[--array_stack_header(a)->end])

#define array_stack_try_pop_back(a, var) \
  (!array_stack_empty(a) ? (var = a[--array_stack_header(a)->end], true) : false)
