#pragma once

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
#define ASSERT(cond) ((cond) || (__debugbreak(), 0))

#define OFFSETBY(a, b)         ((void *)(((u8 *)(a)) + (b)))
#define OFFSETOF(st, m)        __builtin_offsetof(st, m)
#define COUNT(a)               (sizeof((a)) / sizeof((a)[0]))
#define ALIGNOF(a)             _Alignof(a)
#define ALIGNUP(size, align)   (((size) + (align)-1) & ~((align)-1))
#define ALIGNDOWN(size, align) ((size) & ~((align)-1))

extern void *memory_realloc(void *address, u64 prev, u64 new);