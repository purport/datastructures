#include <os.h>

#define _GNU_SOURCE
#include <sys/mman.h>

extern void *memory_realloc(void *address, u64 prev, u64 new) {
  if (new == 0) {
    return munmap(address, prev) == 0 ? NULL : address;
  }

  if (address == NULL) {
    return mmap(NULL, new, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_PRIVATE,
                -1, 0);
  }
  return mremap(address, prev, new, MREMAP_MAYMOVE);
}