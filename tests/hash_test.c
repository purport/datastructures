#include <aion.h>

#define CHECK_MURMUR3(expected, seed, ...)                               \
  do {                                                                   \
    u8 data[] = {__VA_ARGS__};                                           \
    struct murmur3 hash = {.value = seed};                               \
    for (u64 i = 0; i != COUNT(data); ++i) hash_murmur3(&hash, data[i]); \
    u32 h = hash_murmur3_fin(&hash);                                     \
    CHECK(h, ==, expected);                                              \
    /*printf("%x\n", h);*/                                               \
  } while (0)

s32 main() {
  CHECK_START();
  CHECK(hash_murmur3_fin(&(struct murmur3){.value = 0}), ==, 0);
  CHECK(hash_murmur3_fin(&(struct murmur3){.value = 1}), ==, 0x514e28b7);
  CHECK(hash_murmur3_fin(&(struct murmur3){.value = (u32)-1}), ==, 0x81f16f39);
  CHECK_MURMUR3(0x76293b50, 0, 0xff, 0xff, 0xff, 0xff);
  CHECK_MURMUR3(0xf55b516b, 0, 0x21, 0x43, 0x65, 0x87);
  CHECK_MURMUR3(0x2362f9de, 0x5082edee, 0x21, 0x43, 0x65, 0x87);
  CHECK_MURMUR3(0x7e4a8634, 0, 0x21, 0x43, 0x65);
  CHECK_MURMUR3(0xa0f7b07a, 0, 0x21, 0x43);
  CHECK_MURMUR3(0x477d9216, 0, 'y');
  CHECK_MURMUR3(0x6ec17566, 0, 'y', '_', '0');
  CHECK_MURMUR3(0xcbe65824, 0, '_', '1');
  CHECK_MURMUR3(0x070e6949, 0, 'a', 'B', 'C', 'Z', 'x');

  CHECK_END();
}
