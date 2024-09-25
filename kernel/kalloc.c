// kalloc.c

/*
 * Physical memory allocator, for user processes,
 * kernel stack, page-table pages, and pipe buffers.
 * Allocate 256 bytes pages.
*/

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "arch.h"

void freerange(void *pa_start, void *pa_end);

struct run {
  struct run *next;
};

struct {
  struct run *freelist;
} kmem;

void kinit()
{
  freerange((void*) KERNEL_END, (void*) PHYS_STOP);
}

void freerange(void *pa_start, void *pa_end)
{
  char *ptr;
  ptr = (char*)PAGE_ROUND_UP((uint64)pa_start);
}
