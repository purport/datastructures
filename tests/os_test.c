#include <aion.h>

s32 main(void) {
  u8 *mem;
  CHECK_START();
  // null allocation
  CHECK(mem = memory_realloc(NULL, 0, 0), ==, NULL);

  // allocation and free
  CHECK(mem = memory_realloc(NULL, 0, GIGABYTE), !=, NULL);
  mem[0] = 1;
  mem[GIGABYTE - 1] = 1;
  mem[GIGABYTE - 2] = 1;
  CHECK(mem = memory_realloc(mem, GIGABYTE, 0), ==, NULL);

  // resizing
  CHECK(mem = memory_realloc(NULL, 0, MEGABYTE), !=, NULL);
  mem[0] = 2;
  mem[MEGABYTE - 1] = 3;
  mem[MEGABYTE - 2] = 4;
  CHECK(mem = memory_realloc(mem, MEGABYTE, GIGABYTE), !=, NULL);
  mem[GIGABYTE - 1] = 5;
  mem[GIGABYTE - 2] = 6;

  CHECK(mem[0], ==, 2);
  CHECK(mem[MEGABYTE - 1], ==, 3);
  CHECK(mem[MEGABYTE - 2], ==, 4);
  CHECK(mem[GIGABYTE - 1], ==, 5);
  CHECK(mem[GIGABYTE - 2], ==, 6);

  CHECK(mem = memory_realloc(mem, GIGABYTE, 0), ==, NULL);

  CHECK_END();
}
