#include <aion.h>

s32 main() {
  CHECK_START();

  {
    c8* b = NULL;
    CHECK(array_gap_insert(b, 'a'), ==, true);
    CHECK(b[0], ==, 'a');
    CHECK(array_gap_insert(b, 'b'), ==, true);
    CHECK(b[0], ==, 'a');
    CHECK(b[1], ==, 'b');
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(array_gap_header(b)->end, ==, 31);
    CHECK(b[0], ==, 'a');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_insert(b, 'c'), ==, true);
    CHECK(b[0], ==, 'a');
    CHECK(b[1], ==, 'c');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(b[0], ==, 'a');
    CHECK(b[30], ==, 'c');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(b[29], ==, 'a');
    CHECK(b[30], ==, 'c');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_cursor_left(b), ==, false);
    CHECK(array_gap_insert(b, 'd'), ==, true);
    CHECK(array_gap_insert(b, 'e'), ==, true);
    CHECK(array_gap_insert(b, 'f'), ==, true);
    CHECK(b[0], ==, 'd');
    CHECK(b[1], ==, 'e');
    CHECK(b[2], ==, 'f');
    CHECK(b[29], ==, 'a');
    CHECK(b[30], ==, 'c');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_cursor_right(b), ==, true);
    CHECK(b[0], ==, 'd');
    CHECK(b[1], ==, 'e');
    CHECK(b[2], ==, 'f');
    CHECK(b[3], ==, 'a');
    CHECK(b[30], ==, 'c');
    CHECK(b[31], ==, 'b');
    CHECK(array_gap_cursor_right(b), ==, true);
    CHECK(array_gap_cursor_right(b), ==, true);
    CHECK(b[0], ==, 'd');
    CHECK(b[1], ==, 'e');
    CHECK(b[2], ==, 'f');
    CHECK(b[3], ==, 'a');
    CHECK(b[4], ==, 'c');
    CHECK(b[5], ==, 'b');
    CHECK(array_gap_header(b)->end, ==, 32);
    CHECK(array_gap_cursor_right(b), ==, false);
    CHECK(array_gap_insert(b, 'g'), ==, true);
    CHECK(b[0], ==, 'd');
    CHECK(b[1], ==, 'e');
    CHECK(b[2], ==, 'f');
    CHECK(b[3], ==, 'a');
    CHECK(b[4], ==, 'c');
    CHECK(b[5], ==, 'b');
    CHECK(b[6], ==, 'g');
    for (u32 i = 0; i != 32 - 6; ++i) {
      array_gap_insert(b, ' ');
    }
    CHECK(array_gap_header(b)->capacity, ==, 64);
    CHECK(b[0], ==, 'd');
    CHECK(b[1], ==, 'e');
    CHECK(b[2], ==, 'f');
    CHECK(b[3], ==, 'a');
    CHECK(b[4], ==, 'c');
    CHECK(b[5], ==, 'b');
    CHECK(b[6], ==, 'g');
    CHECK(b[30], ==, ' ');
    CHECK(b[31], ==, ' ');
    CHECK(b[32], ==, ' ');
    CHECK(b[33], ==, '\0');
    array_gap_free(b);
  }
  {
    c8* b = NULL;
    for (u32 i = 0; i != 32; ++i) {
      array_gap_insert(b, 'a');
    }
    CHECK(array_gap_full(b), ==, true);
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(array_gap_insert(b, 'b'), ==, true);
    CHECK(b[30], ==, 'a');
    CHECK(b[31], ==, 'b');
    CHECK(b[63], ==, 'a');
  }
  {
    c8* b = NULL;
    for (u32 i = 0; i != 32; ++i) {
      array_gap_insert(b, 'a');
    }
    CHECK(array_gap_full(b), ==, true);
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(array_gap_insert(b, 'b'), ==, true);
    CHECK(b[28], ==, 'a');
    CHECK(b[29], ==, 'b');
    CHECK(b[61], ==, 'a');
    CHECK(b[62], ==, 'a');
    CHECK(b[63], ==, 'a');
    CHECK(array_gap_cursor_right(b), ==, true);
    CHECK(array_gap_delete(b), ==, true);
    CHECK(array_gap_cursor_left(b), ==, true);
    CHECK(b[28], ==, 'a');
    CHECK(b[61], ==, 'b');
    CHECK(b[62], ==, 'a');
    CHECK(b[63], ==, 'a');
  }
  {
    s32* b = NULL;
    for (u32 i = 0; i != 32; ++i) {
      array_gap_insert(b, i + 1);
      array_gap_cursor_left(b);
    }
    CHECK(array_gap_full(b), ==, true);
    CHECK(array_gap_insert(b, 33), ==, true);
    CHECK(b[0], ==, 33);
    CHECK(b[32], ==, 32);
    CHECK(b[63], ==, 1);
  }
  {
    s32* b = NULL;
    for (u32 i = 0; i != 32; ++i) {
      array_gap_insert(b, 0xdea300 | i + 1);
    }
    CHECK(array_gap_full(b), ==, true);
    array_gap_cursor_left(b);
    CHECK(array_gap_insert(b, 33), ==, true);
    CHECK(b[0], ==, 0xdea300 | 1);
    CHECK(b[30], ==, 0xdea300 | 31);
    CHECK(b[31], ==, 33);
    CHECK(b[63], ==, 0xdea300 | 32);
    CHECK(array_gap_insert(b, 34), ==, true);
    CHECK(b[32], ==, 34);
  }

  CHECK_END();
}
