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

#define push  array_stack_push_back
#define pop   array_stack_pop_back
#define peek  array_stack_peek_back
#define empty array_stack_empty

struct array_stack {
  u64 end, capacity;
};

#define array_stack_header_size(a) \
  ALIGNUP(sizeof(struct array_stack), ALIGNOF(__typeof__(a[0])))

#define array_stack_header(a) \
  ((struct array_stack *)OFFSETBY(a, -array_stack_header_size(a)))

#define array_stack_free(a)                               \
  do {                                                    \
    if (a == NULL) break;                                 \
    struct array_stack *s__ = array_stack_header(a);      \
    memory_realloc(s__, s__->capacity * sizeof(a[0]), 0); \
    a = NULL;                                             \
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

// Scanner

struct token {
  enum token_kind : u32 {
    TOKEN_UNKNOWN,
    TOKEN_PLUS,
    TOKEN_MINUS,
    TOKEN_NEGATIVE,
    TOKEN_MULTIPLY,
    TOKEN_DIVIDE,
    TOKEN_POWER,
    TOKEN_EQ,
    TOKEN_GT,
    TOKEN_GEQ,
    TOKEN_LT,
    TOKEN_LEQ,
    TOKEN_PAREN_OPEN,
    TOKEN_PAREN_CLOSE,
    TOKEN_BRACE_OPEN,
    TOKEN_BRACE_CLOSE,
    TOKEN_SEMI,
    TOKEN_INTEGER,
    TOKEN_IDENT,
    TOKEN_LET,
    TOKEN_FUN,
    TOKEN_END,
  } kind;
  union {
    u64 ident_index;
    u64 integer_index;
  };
};

struct program {
  struct strings strings;
  struct token *tokens;

  struct integer {
    u64 val;
    bool overflow;
    u8 padding[7];
  } *integers;

  struct ident {
    struct string *name;
  } *idents;

  struct node {
    enum node_kind {
      NODE_UNKNOWN,
      NODE_LET_INTRODUCER,
      NODE_LET_INITIALIZER,
      NODE_LET,
      NODE_BINDING,
      NODE_EXPR_UNARY,
      NODE_EXPR_BINARY,
      NODE_EXPR_IDENT,
      NODE_EXPR_INTEGER,
    } kind;
    u64 token;
  } *nodes;
};

extern void scanner_tokenize(struct program *ctx, u64 input_count, c8 *input);

#ifdef AION_IMPLEMENTATION

static struct integer scanner__integer(u64 input_count, c8 *input, u64 *begin,
                                       c8 ch0) {
  // TODO: other bases, decimals, negatives?
  struct integer n = {(u64)ch0 - '0', false, {0}};
  u64 i = *begin;
  while (i != input_count) {
    c8 ch = input[i++];
    switch (ch) {
      case '0' ... '9':
        n.overflow |= __builtin_umull_overflow(n.val, 10, &n.val);
        n.overflow |= __builtin_uaddl_overflow(n.val, (u64)ch - '0', &n.val);
        *begin = i;
        continue;
    }
    break;
  }
  return n;
}

static struct ident scanner__ident(struct strings *strings, u64 input_count,
                                   c8 *input, u64 *i, c8 ch0) {
  struct murmur3 hash = {0};
  hash_murmur3(&hash, (u8)ch0);
  u64 begin = *i - 1;
  while (*i != input_count) {
    c8 ch = input[*i];
    switch (ch) {
      case '_':
      case 'a' ... 'z':
      case 'A' ... 'Z':
      case '0' ... '9':
        hash_murmur3(&hash, (u8)ch);
        *i = *i + 1;
        continue;
    }
    break;
  }
  struct ident id = {
      strings_intern(strings, hash_murmur3_fin(&hash), *i - begin,
                     &input[begin]),
  };
  return id;
}

void scanner_tokenize(struct program *ctx, u64 input_count, c8 *input) {
  enum keyword : u8 {
    KEYWORD_LET,
    KEYWORD_FUN,
    KEYWORD_COUNT,
  };
  struct string *keywords[KEYWORD_COUNT];
  for (enum keyword keyword = 0; keyword != KEYWORD_COUNT; ++keyword) {
    switch (keyword) {
      case KEYWORD_LET:
        keywords[keyword] = strings_intern_cstring(&ctx->strings, "let");
        push(ctx->idents, (struct ident){keywords[keyword]});
        break;
      case KEYWORD_FUN:
        keywords[keyword] = strings_intern_cstring(&ctx->strings, "fun");
        push(ctx->idents, (struct ident){keywords[keyword]});
        break;
      case KEYWORD_COUNT: ASSERT(false);
    }
  }

  u64 i = 0;
  enum token_kind prev_kind = TOKEN_UNKNOWN;
  while (i != input_count) {
    c8 ch = input[i++];
    struct token token = {0};
    switch (ch) {
      case '+': token.kind = TOKEN_PLUS; break;
      case '-':
        token.kind = prev_kind != TOKEN_IDENT && prev_kind != TOKEN_INTEGER
                         ? TOKEN_NEGATIVE
                         : TOKEN_MINUS;
        break;
      case '*': token.kind = TOKEN_MULTIPLY; break;
      case '/': token.kind = TOKEN_DIVIDE; break;
      case '=': token.kind = TOKEN_EQ; break;
      case '(': token.kind = TOKEN_PAREN_OPEN; break;
      case ')': token.kind = TOKEN_PAREN_CLOSE; break;
      case '{': token.kind = TOKEN_BRACE_OPEN; break;
      case '}': token.kind = TOKEN_BRACE_CLOSE; break;
      case '>':
        token.kind = TOKEN_GT;
        if (i != input_count && input[i] == '=') {
          token.kind = TOKEN_GEQ;
          ++i;
        }
        break;
      case '<':
        token.kind = TOKEN_LT;
        if (i != input_count && input[i] == '=') {
          token.kind = TOKEN_LEQ;
          ++i;
        }
        break;
      case '0' ... '9':
        token.kind = TOKEN_INTEGER;
        token.integer_index = array_stack_count(ctx->integers);
        push(ctx->integers, scanner__integer(input_count, input, &i, ch));
        break;
      case '_':
      case 'a' ... 'z':
      case 'A' ... 'Z':
        token.kind = TOKEN_IDENT;
        struct ident id =
            scanner__ident(&ctx->strings, input_count, input, &i, ch);
        for (enum keyword keyword = 0; keyword != KEYWORD_COUNT; ++keyword) {
          if (keywords[keyword] == id.name) {
            token.kind = TOKEN_LET + keyword;
            token.ident_index = (u64)keyword;
            break;
          }
        }

        if (token.kind == TOKEN_IDENT) {
          token.ident_index = array_stack_count(ctx->idents);
          push(ctx->idents, id);
        }
        break;
      case ' ':
      case '\0': continue;
      default: token.kind = TOKEN_UNKNOWN; break;
    }
    push(ctx->tokens, token);
    prev_kind = token.kind;
  }
  push(ctx->tokens, ((struct token){.kind = TOKEN_END}));
}

#endif

extern u64 parser_print_token(struct program *p, u64 t, u64 n, c8 *str);
extern u64 parse_print_nodes(struct program *p, u64 n, c8 *str,
                             u64 graph_count);
extern void parser_parse(struct program *p);

#ifdef AION_IMPLEMENTATION

u64 parser_print_token(struct program *p, u64 t, u64 n, c8 *str) {
  struct token token = p->tokens[t];
  switch (token.kind) {
    case TOKEN_UNKNOWN: n = snprintf(str, n, "Unknown"); break;
    case TOKEN_PLUS: n = snprintf(str, n, "+"); break;
    case TOKEN_NEGATIVE: n = snprintf(str, n, "-"); break;
    case TOKEN_MINUS: n = snprintf(str, n, "-"); break;
    case TOKEN_MULTIPLY: n = snprintf(str, n, "*"); break;
    case TOKEN_DIVIDE: n = snprintf(str, n, "/"); break;
    case TOKEN_POWER: n = snprintf(str, n, "^"); break;
    case TOKEN_EQ: n = snprintf(str, n, "="); break;
    case TOKEN_GT: n = snprintf(str, n, ">"); break;
    case TOKEN_GEQ: n = snprintf(str, n, ">="); break;
    case TOKEN_LT: n = snprintf(str, n, "<"); break;
    case TOKEN_LEQ: n = snprintf(str, n, "<="); break;
    case TOKEN_PAREN_OPEN: n = snprintf(str, n, "("); break;
    case TOKEN_PAREN_CLOSE: n = snprintf(str, n, ")"); break;
    case TOKEN_BRACE_OPEN: n = snprintf(str, n, "{"); break;
    case TOKEN_BRACE_CLOSE: n = snprintf(str, n, "}"); break;
    case TOKEN_INTEGER:
      n = snprintf(str, n, "%lu", p->integers[token.integer_index].val);
      break;
    case TOKEN_IDENT:
    case TOKEN_LET:
    case TOKEN_FUN:
      n = snprintf(str, n, "%s", p->idents[token.ident_index].name->val);
      break;
    case TOKEN_SEMI: n = snprintf(str, n, ";"); break;
    case TOKEN_END: n = snprintf(str, n, "$"); break;
  }
  return n;
}

u64 parse_print_nodes(struct program *p, u64 l, c8 *str, u64 graph_count) {
  u64 *stack = 0;
#define ATTR "color=\"white\" fontcolor=\"white\""

  u64 c = 0;
  c += snprintf(str + c, l - c, "digraph G_%lu {\nbgcolor=\"transparent\"\n",
                graph_count);
  array_stack_for(p->nodes, n) {
    struct node node = p->nodes[n];
    switch (node.kind) {
      case NODE_LET_INTRODUCER: {
        push(stack, n);
        c += snprintf(str + c, l - c,
                      "_%lx_%lx [label=\"let introducer: ", graph_count, n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
      } break;
      case NODE_LET_INITIALIZER: {
        push(stack, n);
        c += snprintf(str + c, l - c,
                      "_%lx_%lx [label=\"let initalizer: ", graph_count, n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
      } break;
      case NODE_LET: {
        u64 expression = pop(stack);
        u64 binding = pop(stack);
        u64 initializer = pop(stack);
        u64 introducer = pop(stack);
        push(stack, n);
        c +=
            snprintf(str + c, l - c, "_%lx_%lx [label=\"let: ", graph_count, n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, n, graph_count, (u64)introducer);
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, n, graph_count, (u64)initializer);
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, n, graph_count, (u64)binding);
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, n, graph_count, (u64)expression);
      } break;
      case NODE_BINDING: {
        push(stack, n);
        c += snprintf(str + c, l - c,
                      "_%lx_%lx [label=\"binding: ", graph_count, (u64)n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
      } break;
      case NODE_EXPR_BINARY: {
        u64 right = pop(stack);
        u64 left = pop(stack);
        push(stack, n);
        c += snprintf(str + c, l - c,
                      "_%lx_%lx [label=\"expr binary: ", graph_count, (u64)n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, (u64)n, graph_count, (u64)left);
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, (u64)n, graph_count, (u64)right);
      } break;
      case NODE_EXPR_UNARY: {
        u64 right = pop(stack);
        push(stack, n);
        c += snprintf(str + c, l - c,
                      "_%lx_%lx [label=\"expr unary: ", graph_count, (u64)n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
        c += snprintf(str + c, l - c, "_%lx_%lx -> _%lx_%lx [" ATTR "]\n",
                      graph_count, (u64)n, graph_count, (u64)right);
      } break;
      case NODE_EXPR_IDENT:
      case NODE_EXPR_INTEGER:
        push(stack, n);
        c +=
            snprintf(str + c, l - c, "_%lx_%lx [label=\"", graph_count, (u64)n);
        c += parser_print_token(p, node.token, l - c, str + c);
        c += snprintf(str + c, l - c, "\"" ATTR "]\n");
        break;
      case NODE_UNKNOWN: ASSERT(false); break;
    }
  }

#undef ATTR
  l = snprintf(str + c, l, "}");
  array_stack_free(stack);
  return l;
}

void parser_parse(struct program *p) {
  enum state : u32 {
    STATE_L0,
    STATE_LET,
    STATE_LET_BINDING,
    STATE_LET_AFTER_BINDING,
    STATE_LET_FINISH,
    STATE_EXPR,
    STATE_END,
  } *states = NULL;

  // clang-format off
  static struct parse_info {
    u8 prec;
    enum node_kind kind;
    enum assoc { ASSOC_LEFT, ASSOC_RIGHT } assoc;
  } parse_info[] = {
      [TOKEN_EQ] = {.prec = 1, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_RIGHT},
      [TOKEN_MINUS] = {.prec = 4, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_LEFT},
      [TOKEN_PLUS] = {.prec = 4, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_LEFT},
      [TOKEN_MULTIPLY] = {.prec = 8, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_LEFT},
      [TOKEN_DIVIDE] = {.prec = 8, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_LEFT},
      [TOKEN_POWER] = {.prec = 16, .kind = NODE_EXPR_BINARY, .assoc = ASSOC_RIGHT},
      [TOKEN_NEGATIVE] = {.prec = 16, .kind = NODE_EXPR_UNARY, .assoc = ASSOC_RIGHT},
  };
  // clang-format on

  u64 *operators = NULL;

  push(states, STATE_END);
  push(states, STATE_L0);

  u64 i = 0;
  while (!empty(states)) {
    enum state state = pop(states);
    struct token t = p->tokens[i];

    switch (state) {
      case STATE_L0: {
        if (t.kind == TOKEN_LET) {
          push(states, STATE_LET);
        } else {
          push(states, STATE_EXPR);
        }
      } break;
      case STATE_LET: {
        push(states, STATE_LET_FINISH);
        push(states, STATE_LET_AFTER_BINDING);
        push(states, STATE_LET_BINDING);

        struct node node = {.kind = NODE_LET_INTRODUCER, .token = i++};
        push(p->nodes, node);
      } break;
      case STATE_LET_BINDING: {
        if (t.kind == TOKEN_IDENT) {
          struct node node = {.kind = NODE_BINDING, .token = i++};
          push(p->nodes, node);
        } else {
          // syntax error
          ASSERT(false);
        }
      } break;
      case STATE_LET_AFTER_BINDING: {
        if (t.kind == TOKEN_EQ) {
          push(states, STATE_EXPR);

          struct node node = {.kind = NODE_LET_INITIALIZER, .token = i++};
          push(p->nodes, node);
        } else {
          // syntax error
          ASSERT(false);
        }
      } break;
      case STATE_LET_FINISH: {
        if (t.kind == TOKEN_SEMI) {
          struct node node = {.kind = NODE_LET, .token = i++};
          push(p->nodes, node);
        } else {
          // syntax error
          ASSERT(false);
        }
      } break;
      case STATE_EXPR: {
        bool end = false;
        // TODO: syntax errors
        do {
          switch (t.kind) {
            case TOKEN_PLUS:
            case TOKEN_MINUS:
            case TOKEN_MULTIPLY:
            case TOKEN_DIVIDE:
            case TOKEN_POWER: {
              struct parse_info p1 = parse_info[t.kind];
              while (!empty(operators)) {
                struct parse_info p2 =
                    parse_info[p->tokens[peek(operators)].kind];
                if (p2.prec > p1.prec ||
                    p2.prec == p1.prec && p1.assoc == ASSOC_LEFT) {
                  struct node node = {.kind = p2.kind, .token = pop(operators)};
                  push(p->nodes, node);
                  continue;
                }
                break;
              }
              push(operators, i);
            } break;
            case TOKEN_NEGATIVE: {
              struct parse_info p1 = parse_info[t.kind];
              while (!empty(operators)) {
                struct parse_info p2 =
                    parse_info[p->tokens[peek(operators)].kind];
                if (p2.prec > p1.prec ||
                    p2.prec == p1.prec && p1.assoc == ASSOC_LEFT) {
                  struct node node = {.kind = p2.kind, .token = pop(operators)};
                  push(p->nodes, node);
                  continue;
                }
                break;
              }
              push(operators, i);
            } break;
            case TOKEN_INTEGER: {
              struct node node = {.kind = NODE_EXPR_INTEGER, .token = i};
              push(p->nodes, node);
            } break;
            case TOKEN_IDENT: {
              struct node node = {.kind = NODE_EXPR_IDENT, .token = i};
              push(p->nodes, node);
            } break;
            case TOKEN_UNKNOWN:
            case TOKEN_EQ:
            case TOKEN_GT:
            case TOKEN_GEQ:
            case TOKEN_LT:
            case TOKEN_LEQ:
            case TOKEN_PAREN_OPEN:
            case TOKEN_PAREN_CLOSE:
            case TOKEN_BRACE_OPEN:
            case TOKEN_BRACE_CLOSE: ASSERT(false); break;
            case TOKEN_SEMI:
            case TOKEN_LET:
            case TOKEN_FUN:
            case TOKEN_END: end = true; break;
          }
          if (!end) {
            t = p->tokens[++i];
          }
        } while (!end);

        while (!empty(operators)) {
          u64 op = pop(operators);
          struct node node = {.kind = parse_info[p->tokens[op].kind].kind,
                              .token = op};
          push(p->nodes, node);
        }
      } break;
      case STATE_END: break;
    }
  }
  array_stack_free(operators);
}

#endif

// Interpreter

extern u64 interpret_program(struct program *p);

#ifdef AION_IMPLEMENTATION

u64 interpret_program(struct program *p) {
  u64 *stack = 0;
  array_stack_for(p->nodes, n) {
    struct node node = p->nodes[n];
    switch (node.kind) {
      case NODE_LET_INTRODUCER: {
        push(stack, n);
      } break;
      case NODE_LET_INITIALIZER: {
        push(stack, n);
      } break;
      case NODE_LET: {
        u64 expression = pop(stack);
        u64 binding = pop(stack);
        u64 initializer = pop(stack);
        u64 introducer = pop(stack);
        ASSERT(false);
      } break;
      case NODE_BINDING: {
        push(stack, n);
      } break;
      case NODE_EXPR_BINARY: {
        u64 right = pop(stack);
        u64 left = pop(stack);
        switch (p->tokens[node.token].kind) {
          case TOKEN_PLUS: push(stack, left + right); break;
          case TOKEN_MINUS: push(stack, left - right); break;
          case TOKEN_MULTIPLY: push(stack, left * right); break;
          case TOKEN_DIVIDE: push(stack, left / right); break;
          case TOKEN_POWER: ASSERT(false); break;
          case TOKEN_NEGATIVE:
          case TOKEN_EQ:
          case TOKEN_GT:
          case TOKEN_GEQ:
          case TOKEN_LT:
          case TOKEN_LEQ:
          case TOKEN_PAREN_OPEN:
          case TOKEN_PAREN_CLOSE:
          case TOKEN_BRACE_OPEN:
          case TOKEN_BRACE_CLOSE:
          case TOKEN_SEMI:
          case TOKEN_INTEGER:
          case TOKEN_IDENT:
          case TOKEN_LET:
          case TOKEN_FUN:
          case TOKEN_END:
          case TOKEN_UNKNOWN: ASSERT(false); break;
        }
      } break;
      case NODE_EXPR_UNARY: {
        u64 right = pop(stack);
        switch (p->tokens[node.token].kind) {
          case TOKEN_NEGATIVE: push(stack, -right); break;
          case TOKEN_PLUS:
          case TOKEN_MINUS:
          case TOKEN_MULTIPLY:
          case TOKEN_DIVIDE:
          case TOKEN_POWER:
          case TOKEN_EQ:
          case TOKEN_GT:
          case TOKEN_GEQ:
          case TOKEN_LT:
          case TOKEN_LEQ:
          case TOKEN_PAREN_OPEN:
          case TOKEN_PAREN_CLOSE:
          case TOKEN_BRACE_OPEN:
          case TOKEN_BRACE_CLOSE:
          case TOKEN_SEMI:
          case TOKEN_INTEGER:
          case TOKEN_IDENT:
          case TOKEN_LET:
          case TOKEN_FUN:
          case TOKEN_END:
          case TOKEN_UNKNOWN: ASSERT(false); break;
        }
      } break;
      case NODE_EXPR_IDENT: ASSERT(false); break;
      case NODE_EXPR_INTEGER:
        push(stack, p->integers[p->tokens[node.token].integer_index].val);
        break;
      case NODE_UNKNOWN: ASSERT(false); break;
    }
  }
  ASSERT(array_stack_count(stack) == 1);
  u64 result = pop(stack);
  array_stack_free(stack);
  return result;
}

#endif
