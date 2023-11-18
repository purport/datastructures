#pragma once

#include <stdio.h>
#include <string.h>

typedef char c8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
typedef char s8;
typedef short s16;
typedef int s32;
typedef long int s64;
typedef float f32;
typedef double f64;

#define U32_MAX 0xffffffff

_Static_assert(sizeof(c8) == 1, "incorrect integer width");
_Static_assert(sizeof(u8) == 1, "incorrect integer width");
_Static_assert(sizeof(s8) == 1, "incorrect integer width");
_Static_assert(sizeof(u16) == 2, "incorrect integer width");
_Static_assert(sizeof(s16) == 2, "incorrect integer width");
_Static_assert(sizeof(u32) == 4, "incorrect integer width");
_Static_assert(sizeof(s32) == 4, "incorrect integer width");
_Static_assert(sizeof(u64) == 8, "incorrect integer width");
_Static_assert(sizeof(s64) == 8, "incorrect integer width");

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmacro-redefined"
#define NULL ((void *)0)
#pragma clang diagnostic pop

#define KILOBYTE 1024ull
#define MEGABYTE 1024ull * KILOBYTE
#define GIGABYTE 1024ull * MEGABYTE
#define TERABYTE 1024ull * GIGABYTE

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreserved-identifier"
static void __debugbreak(void) { __asm__("int $3"); }
#pragma clang diagnostic pop

#define ASSERT_(cond) (cond) || (__debugbreak(), 0)
#define ASSERT(cond)  ((cond) || (__debugbreak(), 0))

#define PAGESIZE               4 * KILOBYTE
#define OFFSETBY(a, b)         ((void *)(((u64)(void *)(a)) + (b)))
#define OFFSETOF(st, m)        __builtin_offsetof(st, m)
#define COUNT(a)               (sizeof((a)) / sizeof((a)[0]))
#define ALIGNOF(a)             _Alignof(a)
#define ALIGNUP(size, align)   (((size) + (align)-1) & ~((align)-1))
#define ALIGNDOWN(size, align) ((size) & ~((align)-1))
#define SWAP(a, b)          \
  do {                      \
    __typeof__(a) temp = a; \
    a = b;                  \
    b = temp;               \
  } while (0)

extern void *memory_realloc(void *address, u64 prev, u64 new);

#ifdef AION_IMPLEMENTATION_LINUX
#include <sys/mman.h>

void *memory_realloc(void *address, u64 prev, u64 new) {
  if (new == 0) {
    return munmap(address, prev) == 0 ? NULL : address;
  }

  if (address == NULL) {
    return mmap(NULL, new, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
                -1, 0);
  }
  return mremap(address, prev, new, MREMAP_MAYMOVE);
}

#endif

// Check

#ifndef CHECK_PRINT_PASSES
#define CHECK_PRINT_PASSES 0
#endif
#define CHECK_START() \
  u32 failed = 0;     \
  u32 total = 0
#define CHECK_EARLY_EXIT() \
  if (failed) goto CHECK_EARLY_EXIT_POINT
#define CHECK_END()                                               \
  CHECK_EARLY_EXIT_POINT:                                         \
  if (failed) {                                                   \
    printf("\x1b[31m[FAIL]\x1b[0m %d/%d tests\n", failed, total); \
  } else {                                                        \
    if (CHECK_PRINT_PASSES) {                                     \
      printf("\x1b[32m[PASS]\x1b[0m %d tests\n", total);          \
    }                                                             \
  }                                                               \
  return (s32)failed

#define STRINGIZE(x)  STRINGIZE2(x)
#define STRINGIZE2(x) #x
#define LINE_STRING   STRINGIZE(__LINE__)

#define CHECK(lhs, op, rhs)                                               \
  do {                                                                    \
    ++total;                                                              \
    if ((lhs)op(rhs)) {                                                   \
      if (CHECK_PRINT_PASSES) {                                           \
        printf(__FILE__ ":" LINE_STRING ":1: \x1b[32m[PASS]\x1b[0m " #lhs \
                        " " #op " " #rhs "\n");                           \
      }                                                                   \
    } else {                                                              \
      printf(__FILE__ ":" LINE_STRING ":1: \x1b[31m[FAIL]\x1b[0m " #lhs   \
                      " " #op " " #rhs "\n");                             \
      ++failed;                                                           \
    }                                                                     \
  } while (0)

#define CHECK_TRUE(lhs) CHECK(lhs, ==, true)
#define CHECK_FAIL(msg) \
  printf(__FILE__ ":" LINE_STRING ":1: \x1b[31m[FAIL]\x1b[0m " #msg "\n")
#define CHECK_FAIL_F(msg, ...)                                            \
  printf(__FILE__ ":" LINE_STRING ":1: \x1b[31m[FAIL]\x1b[0m " #msg "\n", \
         __VA_ARGS__)

// Hash

struct murmur3 {
  u32 value;
  u32 size;
  u32 k;
  u8 k_shift;
  u8 padding[3];
};

extern u32 hash_murmur3_cstring(c8 *chars);
extern void hash_murmur3(struct murmur3 *hash, u8 ch);
extern u32 hash_murmur3_fin(struct murmur3 *hash);

#ifdef AION_IMPLEMENTATION

u32 hash_murmur3_cstring(c8 *chars) {
  c8 ch;
  struct murmur3 hash = {0};
  while ((ch = *chars++) != '\0') {
    hash_murmur3(&hash, (u8)ch);
  }
  return hash_murmur3_fin(&hash);
}

void hash_murmur3(struct murmur3 *hash, u8 ch) {
  u32 c1 = 0xcc9e2d51, c2 = 0x1b873593, m = 5, n = 0xe6546b64;
  ++hash->size;
  hash->k |= (u32)(ch << hash->k_shift);
  hash->k_shift += 8;
  if (hash->k_shift == 32) {
    hash->k *= c1;
    hash->k = (hash->k << 15) | (hash->k >> 17);
    hash->k *= c2;

    hash->value ^= hash->k;
    hash->value = (hash->value << 13) | (hash->value >> 19);
    hash->value *= m;
    hash->value += n;

    hash->k_shift = 0;
    hash->k = 0;
  }
}

u32 hash_murmur3_fin(struct murmur3 *hash) {
  u32 c1 = 0xcc9e2d51, c2 = 0x1b873593;
  u32 value = hash->value;
  if (hash->k_shift != 0) {
    u32 k = hash->k * c1;
    k = (k << 15) | (k >> 17);
    k *= c2;
    value ^= k;
  }

  value ^= hash->size;
  value ^= value >> 16;
  value *= 0x85ebca6b;
  value ^= value >> 13;
  value *= 0xc2b2ae35;
  value ^= value >> 16;
  return value;
}

#endif

// Pool

struct pool {
  u64 size;
  u8 *free;
  u8 *buffer;
};

#define pool_alloc(pool, type) \
  pool_alloc_aligned((pool), sizeof(type), _Alignof(type))

#define pool_alloc_array(pool, n, type) \
  pool_alloc_aligned((pool), (n) * sizeof(type), _Alignof(type))

extern bool pool_free(struct pool *pool);
extern void *pool_alloc_aligned(struct pool *pool, u64 size, u64 align);

#ifdef AION_IMPLEMENTATION

bool pool_free(struct pool *pool) {
  pool->buffer = memory_realloc(pool->buffer, pool->size, 0);
  if (pool->buffer == NULL) {
    pool->free = NULL;
    return true;
  }
  return false;
}

void *pool_alloc_aligned(struct pool *pool, u64 size, u64 align) {
  if (pool->free == NULL) {
    pool->size = ALIGNUP(pool->size + 1, PAGESIZE);
    pool->buffer = memory_realloc(NULL, 0, pool->size);
    if (pool->buffer != NULL) {
      pool->free = &pool->buffer[pool->size];
    }
  }
  u64 ptr = (u64)pool->free;
  if (ptr < size) {
    return NULL;
  }
  ptr = ALIGNDOWN(ptr - size, align);

  if (ptr < (u64)pool->buffer) {
    return NULL;
  }
  pool->free = (void *)ptr;
  return pool->free;
}

#endif

// Array Stack

struct array_stack {
  u64 end, capacity;
};

#define array_stack_header_size(a) \
  ALIGNUP(sizeof(struct array_stack), ALIGNOF(__typeof__(a[0])))

#define array_stack_header(a) \
  ((struct array_stack *)OFFSETBY(a, -array_stack_header_size(a)))

#define array_stack_free(a)                                                 \
  do {                                                                      \
    if (a == NULL) break;                                                   \
    struct array_stack *s__ = array_stack_header(a);                        \
    memory_realloc(                                                         \
        s__, array_stack_header_size(a) + s__->capacity * sizeof(a[0]), 0); \
    a = NULL;                                                               \
  } while (0)

#define array_stack_full(a) \
  (a == NULL || array_stack_header(a)->end == array_stack_header(a)->capacity)

#define array_stack_empty(a) (a == NULL || array_stack_header(a)->end == 0)

#define array_stack_push_back(a, value)                                       \
  ((void)(a = array_stack_grow(a, array_stack_header_size(a), sizeof(a[0]))), \
   array_stack_full(a)                                                        \
       ? false                                                                \
       : ((void)(a[array_stack_header(a)->end++] = (value)), true))

#define array_stack_peek_back(a) \
  ((void)ASSERT(!array_stack_empty(a)), a[array_stack_header(a)->end - 1])

#define array_stack_pop_back(a) \
  ((void)ASSERT(!array_stack_empty(a)), a[--array_stack_header(a)->end])

#define array_stack_try_pop_back(a, var)                                 \
  (!array_stack_empty(a) ? (var = a[--array_stack_header(a)->end], true) \
                         : false)

#define array_stack_count(a) \
  (array_stack_empty(a) ? 0 : array_stack_header(a)->end)

#define array_stack_for(a, index) \
  for (u64 index = 0; a != NULL && index != array_stack_header(a)->end; ++index)

extern void *array_stack_grow(void *a, u64 header_size, u64 item_size);

#ifdef AION_IMPLEMENTATION

void *array_stack_grow(void *a, u64 header_size, u64 item_size) {
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

#endif

// Array Gap Buffer

struct array_gap {
  u64 capacity;
  u64 cursor, end;
};

#define array_gap_header_size(a) \
  ALIGNUP(sizeof(struct array_gap), ALIGNOF(__typeof__(a[0])))

#define array_gap_header(a) \
  ((struct array_gap *)OFFSETBY(a, -array_gap_header_size(a)))

#define array_gap_free(a)                                                 \
  do {                                                                    \
    if (a == NULL) break;                                                 \
    struct array_gap *s__ = array_gap_header(a);                          \
    memory_realloc(                                                       \
        s__, array_gap_header_size(a) + s__->capacity * sizeof(a[0]), 0); \
    a = NULL;                                                             \
  } while (0)

#define array_gap_full(a) \
  ((a) == NULL || array_gap_header(a)->cursor == array_gap_header(a)->end)

#define array_gap_insert(a, ch)                                           \
  ((void)(a = array_gap_grow(a, array_gap_header_size(a), sizeof(a[0]))), \
   array_gap_full(a)                                                      \
       ? false                                                            \
       : ((void)(a[array_gap_header(a)->cursor++] = (ch)), true))

#define array_gap_cursor_left(a)                        \
  ((a) == NULL || array_gap_header(a)->cursor == 0      \
       ? false                                          \
       : ((void)(a[--array_gap_header(a)->end] =        \
                     a[--array_gap_header(a)->cursor]), \
          true))

#define array_gap_cursor_right(a)                                           \
  ((a) == NULL || array_gap_header(a)->end == array_gap_header(a)->capacity \
       ? false                                                              \
       : ((void)(a[array_gap_header(a)->cursor++] =                         \
                     a[array_gap_header(a)->end++]),                        \
          true))

#define array_gap_delete(a)                        \
  ((a) == NULL || array_gap_header(a)->cursor == 0 \
       ? false                                     \
       : (array_gap_header(a)->cursor--, true))

extern void *array_gap_grow(void *a, u64 header_size, u64 item_size);

#ifdef AION_IMPLEMENTATION

void *array_gap_grow(void *a, u64 header_size, u64 item_size) {
  struct array_gap *s = NULL;
  if (a == NULL) {
    s = memory_realloc(NULL, 0, header_size + 32 * item_size);
    if (s == NULL) {
      return NULL;
    }
    s->cursor = 0;
    s->capacity = s->end = 32;
  } else {
    s = OFFSETBY(a, -header_size);
    if (s->cursor == s->end) {
      s = memory_realloc(s, header_size + s->capacity * item_size,
                         header_size + 2 * s->capacity * item_size);
      if (s == NULL) {
        return a;
      }
      u64 end_count = s->capacity - s->end;
      s->capacity *= 2;

      // this could be a memcpy, as we are doubling the capacity each time.
      memmove(OFFSETBY(s, header_size + (s->capacity - end_count) * item_size),
              OFFSETBY(a, s->end * item_size), end_count * item_size);

      s->end = s->capacity - end_count;
    }
  }
  return OFFSETBY(s, header_size);
}

#endif

// String

struct string {
  u64 count;
  c8 val[];
};

// Strings

struct strings {
  struct pool memory;
  u64 count, capacity;
  u32 *keys;
  struct string **values;
};

#define strings_intern_cstring(strings, str) \
  strings_intern(strings, hash_murmur3_cstring(str), COUNT(str), str)

extern struct string *strings_intern(struct strings *strings, u32 hash,
                                     u64 count, c8 *value);

#ifdef AION_IMPLEMENTATION

struct string *strings_intern(struct strings *strings, u32 hash, u64 count,
                              c8 *value) {
  if (2 * (strings->count + 1) > strings->capacity) {
    u32 cap = 1024;
    while (cap < 3 * strings->count) {
      cap <<= 1;
    }
    u32 *keys = memory_realloc(NULL, 0, sizeof(u32) * cap);
    struct string **values = memory_realloc(NULL, 0, sizeof(void *) * cap);
    if (keys == NULL || values == NULL) {
      memory_realloc(keys, 0, 0);
      memory_realloc(values, 0, 0);
      return NULL;
    }

    for (u32 j = 0; j != strings->capacity; ++j) {
      if (strings->values[j] == NULL) {
        continue;
      }
      u32 i = strings->keys[j] & (cap - 1);
      while (values[i] != NULL) {
        i = (i + 1) & (cap - 1);
      }
      keys[i] = strings->keys[j];
      values[i] = strings->values[j];
    }

    SWAP(strings->keys, keys);
    SWAP(strings->values, values);
    memory_realloc(keys, strings->capacity * sizeof(*keys), 0);
    memory_realloc(values, strings->capacity * sizeof(void *), 0);
    strings->capacity = cap;
  }

  {
    u32 i;
    u32 j = i = hash & (strings->capacity - 1);
    while (strings->values[i] != NULL) {
      if (strings->keys[i] == hash) {
        if (strncmp(strings->values[i]->val, value, count) == 0) {
          return strings->values[i];
        }
      }
      i = (i + 1) & (strings->capacity - 1);
    }

    struct string *s = pool_alloc_aligned(
        &strings->memory, OFFSETOF(struct string, val) + count + 1,
        ALIGNOF(struct string));
    if (s != NULL) {
      s->count = count;
      memcpy(s->val, value, count);
      s->val[count] = '\0';

      strings->keys[i] = hash;
      strings->values[i] = s;
      ++strings->count;
    }
    return s;
  }
}

#endif

// B-Tree

enum {
  BTREE_MIN = 4,
  BTREE_MAX = 8,
};

struct btree {
  u32 height;

  struct btree_node {
    u32 length;
    u32 key[BTREE_MAX];
    union {
      void *value[BTREE_MAX];
      struct btree_node *children[BTREE_MAX];
    };
  } root;
};

extern void btree_insert(struct btree *tree, u64 key, void *value);

#ifdef AION_IMPLEMENTATION

static struct btree_node *btree_insert_leaf(struct btree_node *node, u32 key,
                                            void *value) {
  u32 index;
  for (index = 0; index != node->length; ++index) {
    if (key <= node->key[index]) {
      if (key == node->key[index]) {
        // replace value
        node->value[index] = value;
        return NULL;
      }
      break;
    }
  }

  struct btree_node *right = NULL;
  if (node->length == BTREE_MAX) {
    right = memory_realloc(NULL, 0, sizeof(*right));
    for (u32 i = BTREE_MIN, j = 0; j != BTREE_MIN; ++i, ++j) {
      right->key[j] = node->key[i];
      right->value[j] = node->value[i];
    }
    right->length = BTREE_MIN;
    node->length = BTREE_MIN;
    if (index >= BTREE_MIN) {
      node = right;
      index -= BTREE_MIN;
    }
  }

  node->length++;
  for (u32 i = index; i != node->length; ++i) {
    SWAP(node->key[i], key);
    SWAP(node->value[i], value);
  }

  return right;
}

static struct btree_node *btree_insert_node(struct btree_node *node, u32 height,
                                            u32 key, void *value) {
  u32 index;
  for (index = 0; index != node->length; ++index) {
    if (key <= node->key[index]) {
      break;
    }
  }
  // increment end key
  if (index == node->length) {
    node->key[--index] = key;
  }

  struct btree_node *new_child;
  if (height == 1) {
    // child will be a leaf
    new_child = btree_insert_leaf(node->children[index], key, value);
  } else {
    new_child =
        btree_insert_node(node->children[index], height - 1, key, value);
  }

  struct btree_node *sibling = NULL;
  if (new_child) {
    if (node->length == BTREE_MAX) {
      sibling = memory_realloc(NULL, 0, sizeof(*sibling));
      for (u32 i = BTREE_MIN, j = 0; j != BTREE_MIN; ++i, ++j) {
        sibling->key[j] = node->key[i];
        sibling->value[j] = node->value[i];
      }
      sibling->length = BTREE_MIN;
      node->length = BTREE_MIN;
      if (index >= BTREE_MIN) {
        node = sibling;
        index -= BTREE_MIN;
      }
    }
    node->key[index] =
        node->children[index]->key[node->children[index]->length - 1];

    u32 new_key = new_child->key[new_child->length - 1];

    node->length++;
    for (u32 i = index + 1; i != node->length; ++i) {
      SWAP(node->key[i], new_key);
      SWAP(node->children[i], new_child);
    }
  }

  return sibling;
}

void btree_insert(struct btree *tree, u64 key, void *value) {
  struct btree_node *new = NULL;
  if (tree->height == 0) {
    new = btree_insert_leaf(&tree->root, key, value);
  } else {
    new = btree_insert_node(&tree->root, tree->height, key, value);
  }

  if (new) {
    struct btree_node *root_copy = memory_realloc(NULL, 0, sizeof(*root_copy));
    *root_copy = tree->root;
    tree->root.length = 2;
    tree->root.key[0] = root_copy->key[root_copy->length - 1];
    tree->root.children[0] = root_copy;
    tree->root.key[1] = new->key[new->length - 1];
    tree->root.children[1] = new;
    tree->height++;
  }
}
#endif
