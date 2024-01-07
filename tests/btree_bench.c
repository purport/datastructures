#include <aion.h>
#include <stdlib.h>

static struct bench {
  u32 count;
  u32 *key;
  struct btree tree;
} bench = {0};

static void bench_init(u32 count) {
  bench.key = memory_realloc(bench.key, bench.count * sizeof(*bench.key),
                             count * sizeof(*bench.key));
  bench.count = count;

  srand(7);
  for (u32 i = 0; i != count; ++i) {
    bench.key[i] = rand();
  }

  btree_clear(&bench.tree);
}

static void bench_run_insert() {
  for (u32 i = 0; i != bench.count; ++i) {
    btree_insert(&bench.tree, bench.key[i]);
  }
  printf("tree height is %d\n", bench.tree.height);
}

static void bench_run_find() {
  u32 key1 = bench.key[100];
  u32 key2 = bench.key[101];
  if (key2 < key1) {
    SWAP(key1, key2);
  }

  u32 count = 0;
  struct btree_iterator it = {0};
  btree_iterator(&bench.tree, &it);
  while (btree_iterator_next(&it)) {
    u32 key = btree_iterator_key(&it);
    if (key1 <= key) {
      do {
        key = btree_iterator_key(&it);
        if (key < key2) {
          ++count;
        } else {
          break;
        }
      } while (btree_iterator_next(&it));
      break;
    }
  }

  printf("there are %d keys between %d and %d\n", count, key1, key2);
}

s32 main() {
  u128 start, end;
  bench_init(5000000);

  start = now();
  bench_run_insert();
  end = now();
  printf("%-10s: %f s\n", "insert", (end - start) / 1000000000.0);

  start = now();
  bench_run_find();
  end = now();
  printf("%-10s: %f s\n", "find", (end - start) / 1000000000.0);
}
