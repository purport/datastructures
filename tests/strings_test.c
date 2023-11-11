#include <aion.h>

s32 main(void) {
  CHECK_START();

  struct strings s;
  struct string *s1 = strings_intern(&s, 0x10000, 3, "abc");
  struct string *s2 = strings_intern(&s, 0x20000, 1, "b");
  struct string *s3 = strings_intern(&s, 0x10000, 1, "c");
  struct string *s4 = strings_intern(&s, 0x10000, 3, "abc");
  struct string *s5 = strings_intern(&s, 0x20000, 1, "b");
  struct string *s6 = strings_intern(&s, 0x10000, 1, "c");

  CHECK(strcmp(s1->val, "abc"), ==, 0);
  CHECK(strcmp(s2->val, "b"), ==, 0);
  CHECK(strcmp(s3->val, "c"), ==, 0);
  CHECK(strcmp(s4->val, "abc"), ==, 0);
  CHECK(strcmp(s5->val, "b"), ==, 0);
  CHECK(strcmp(s6->val, "c"), ==, 0);
  CHECK(s1, ==, s4);
  CHECK(s2, ==, s5);
  CHECK(s3, ==, s6);
  CHECK(s1, !=, s2);
  CHECK(s1, !=, s3);
  CHECK(s2, !=, s3);

  struct string *s7 = strings_intern(&s, 0x10001, 1, "e");
  CHECK(strcmp(s7->val, "e"), ==, 0);
  CHECK(s1, !=, s7);
  CHECK(s2, !=, s7);
  CHECK(s3, !=, s7);
  CHECK_END();
}
