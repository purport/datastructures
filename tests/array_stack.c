#include <array_stack.h>
#include <check.h>

#include "os.h"

int main(void) {
  CHECK_START();

  {
    u8 *mem = NULL;
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
    CHECK(array_stack_empty(mem), ==, true);
    array_stack_push_back(mem, 0);
    CHECK(array_stack_empty(mem), ==, false);
    array_stack_pop_back(mem);
    CHECK(array_stack_empty(mem), ==, true);
    array_stack_free(mem);
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
    struct {
      u8 buffer[512];
    } *mem = NULL, item = {0};

    while (array_stack_push_back(mem, item)) {
      CHECK(mem, !=, NULL);
    }
    CHECK(mem, !=, NULL);
    CHECK(array_stack_header(mem)->end, !=, 0);
    CHECK(array_stack_header(mem)->capacity, !=, 0);
    array_stack_free(mem);
  }
  CHECK_END();
}