#include <aion.h>

static bool fail_next_memory_operation = false;
extern void *__real_mremap(void *address, u64 prev, u64 new, s32, ...);
extern void *__wrap_mremap(void *address, u64 prev, u64 new, s32 flags, ...) {
  if (fail_next_memory_operation) {
    fail_next_memory_operation = false;
    return NULL;
  }
  return __real_mremap(address, prev, new, flags);
}

extern void *__real_mmap(void *address, u64 new, s32 prot, s32 flags, s32 fd,
                         s64 offset);
extern void *__wrap_mmap(void *address, u64 new, s32 prot, s32 flags, s32 fd,
                         s64 offset) {
  if (fail_next_memory_operation) {
    fail_next_memory_operation = false;
    return NULL;
  }
  return __real_mmap(address, new, prot, flags, fd, offset);
}

s32 main(void) {
  CHECK_START();

  {
    u8 *mem = NULL;
    fail_next_memory_operation = true;
    CHECK(array_stack_push_back(mem, 1), ==, false);
    CHECK(mem, ==, NULL);
    CHECK(array_stack_push_back(mem, 1), ==, true);
    CHECK(mem, !=, NULL);
    array_stack_free(mem);
    CHECK(mem, ==, NULL);
  }

  {
    u8 *mem = NULL;
    array_stack_free(mem);
    CHECK(mem, ==, NULL);
    array_stack_push_back(mem, 1);
    CHECK(mem[0], ==, 1);
    array_stack_push_back(mem, 2);
    CHECK(mem[0], ==, 1);
    CHECK(mem[1], ==, 2);
    array_stack_free(mem);
    CHECK(mem, ==, NULL);
  }

  {
    u8 *mem = NULL;
    for (u32 i = 0; i != 32; ++i) {
      array_stack_push_back(mem, (u8)i);
    }
    CHECK(mem[31], ==, 31);
    struct array_stack *s = array_stack_header(mem);
    CHECK(s->capacity, ==, 32);
    CHECK(s->end, ==, 32);
    array_stack_push_back(mem, 100);
    CHECK(s->capacity, ==, 64);
    CHECK(s->end, ==, 33);
    CHECK(mem[32], ==, 100);
    array_stack_free(mem);
  }

  {
    u32 *mem = NULL;
    u32 val = 0;
    CHECK(array_stack_try_pop_back(mem, val), ==, false);
    CHECK(array_stack_push_back(mem, 1), ==, true);
    CHECK(array_stack_try_pop_back(mem, val), ==, true);
    CHECK(val, ==, 1);
  }
  {
    u32 *mem = NULL;
    CHECK(array_stack_empty(mem), ==, true);
    CHECK(array_stack_full(mem), ==, true);
    array_stack_push_back(mem, 0);
    CHECK(array_stack_empty(mem), ==, false);
    CHECK(array_stack_full(mem), ==, false);
    array_stack_pop_back(mem);
    CHECK(array_stack_empty(mem), ==, true);
    CHECK(array_stack_full(mem), ==, false);
    array_stack_free(mem);
    CHECK(array_stack_full(mem), ==, true);
  }

  {
    u32 *mem = NULL;
    for (u32 i = 0; i != 5 * KILOBYTE; ++i) {
      array_stack_push_back(mem, i);
    }
    struct array_stack *s = array_stack_header(mem);
    CHECK(s->capacity, ==, 8 * KILOBYTE);
    CHECK(s->end, ==, 5 * KILOBYTE);
    u32 i = 5 * KILOBYTE;
    while (!array_stack_empty(mem)) {
      CHECK(array_stack_pop_back(mem), ==, --i);
    }
    CHECK(s->capacity, ==, 8 * KILOBYTE);
    CHECK(s->end, ==, 0);
    array_stack_free(mem);
  }

  {
    u16 *mem = NULL;
    while (array_stack_push_back(mem, 1)) {
      CHECK(mem, !=, NULL);
      fail_next_memory_operation = true;
    }
    CHECK(mem, !=, NULL);
    CHECK(array_stack_header(mem)->end, ==, 32);
    CHECK(array_stack_header(mem)->capacity, ==, 32);
    CHECK(array_stack_full(mem), ==, true);
    array_stack_free(mem);
  }
  CHECK_END();
}
