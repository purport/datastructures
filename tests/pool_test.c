#include <aion.h>

s32 main(void) {
  CHECK_START();
  {
    struct pool p = {.size = 11};
    void *addr1 = pool_alloc_aligned(&p, 1, 4);
    CHECK(addr1, !=, NULL);
    CHECK((u64)addr1 & (4 - 1), ==, 0);
    void *addr2 = pool_alloc_aligned(&p, 1, 1);
    CHECK(addr2, !=, NULL);
    CHECK((u64)addr2 + 1, ==, (u64)addr1);
    void *addr3 = pool_alloc_aligned(&p, 1, 4);
    CHECK(addr3, !=, NULL);
    CHECK((u64)addr3 & (4 - 1), ==, 0);

    CHECK_TRUE(pool_free(&p));
    CHECK(p.free, ==, NULL);
    CHECK(p.buffer, ==, NULL);
  }
  {
    struct pool p = {.size = 2 * PAGESIZE - 16};
    void *addr1 = pool_alloc_aligned(&p, PAGESIZE, PAGESIZE);
    CHECK(addr1, !=, NULL);
    CHECK((u64)addr1 & (PAGESIZE - 1), ==, 0);
    CHECK(p.size, ==, 2 * PAGESIZE);

    pool_alloc_aligned(&p, 1, 1);

    void *addr2 = pool_alloc_aligned(&p, PAGESIZE, PAGESIZE);
    CHECK(addr2, ==, NULL);
    void *addr3 = pool_alloc_aligned(&p, PAGESIZE, 1);
    CHECK(addr3, ==, NULL);
    void *addr4 = pool_alloc_aligned(&p, PAGESIZE - 16, 0x1000000);
    CHECK(addr4, ==, NULL);
    void *addr5 = pool_alloc_aligned(&p, PAGESIZE - 16, PAGESIZE);
    CHECK(addr5, !=, NULL);
    CHECK((u64)addr5 & (PAGESIZE - 1), ==, 0);
    void *addr6 = pool_alloc_aligned(&p, 1, 1);
    CHECK(addr6, ==, NULL);

    CHECK_TRUE(pool_free(&p));
    CHECK(p.free, ==, NULL);
    CHECK(p.buffer, ==, NULL);
  }
  {
    typedef double v4d __attribute__((vector_size(32)));
    _Static_assert(_Alignof(int) == 4, "Ints have 4 byte alignment");
    _Static_assert(_Alignof(long) == 8, "Longs have 8 byte alignment");
    _Static_assert(_Alignof(__int128) == 16, "Int128 have 16 byte alignment");
    _Static_assert(_Alignof(v4d) == 32, "v4d have 32 byte alignment");
    struct pool p = {.size = PAGESIZE};

    for (int i = 0; i != 16; ++i) {
      void *addr1 = pool_alloc(&p, int);
      CHECK(addr1, !=, NULL);
      CHECK((u64)addr1 & (4 - 1), ==, 0);
    }
    for (int i = 0; i != 8; ++i) {
      void *addr2 = pool_alloc(&p, long);
      CHECK(addr2, !=, NULL);
      CHECK((u64)addr2 & (8 - 1), ==, 0);
    }
    for (int i = 0; i != 4; ++i) {
      void *addr2 = pool_alloc(&p, __int128);
      CHECK(addr2, !=, NULL);
      CHECK((u64)addr2 & (16 - 1), ==, 0);
    }
    for (int i = 0; i != 2; ++i) {
      void *addr2 = pool_alloc(&p, v4d);
      CHECK(addr2, !=, NULL);
      CHECK((u64)addr2 & (32 - 1), ==, 0);
    }

    CHECK_TRUE(pool_free(&p));
    CHECK(p.free, ==, NULL);
    CHECK(p.buffer, ==, NULL);
  }
  CHECK_END();
}
