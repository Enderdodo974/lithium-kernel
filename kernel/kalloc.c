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
  for (; ptr + PAGE_SIZE <= (char*)pa_end; ptr += PAGE_SIZE) {
    kfree(ptr);
  }
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a call to kalloc().
// (The exception is when initializing the allocator; see kinit() above.)
void kfree(void *pa)
{
  struct run *r;

  if (((uint64)pa % PAGE_SIZE) != 0 || (uint64)pa < KERNEL_END || (uint64)pa >= PHYS_STOP) {
    panic("kfree in allocator init");
  }

  // fill with junk
  memset(pa, 1, PAGE_SIZE);

  r = (struct run*)pa;

  r->next = kmem.freelist;
  kmem.freelist = r;
}

// Allocate one 256-bytes page of physical memory.
// Returns a pointer that the kernel can use,
// or 0 if the memory cannot be allocated.
void * kalloc(void)
{
  struct run *r;

  r = kmem.freelist;
  if (r) {
    kmem.freelist = r->next;
  }

  if (r) {
    memset((char*)r, 0xff, PAGE_SIZE); // fill with junk
  }

  return (void*)r;
}
