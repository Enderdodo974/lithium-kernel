// vm.c

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"
#include "arch.h"

// the kernel page table
pagetable_t kernel_pagetable;

extern char trampoline[];

// make a direct-map page table for the kernel
pagetable_t kvm_make(void)
{
  pagetable_t kernel_pt;

  kernel_pt = (pagetable_t) kalloc();
  memset(kernel_pt, 0, PAGE_SIZE);

  // UART registers
  kvm_map(kernel_pt, UART0, UART0, PAGE_SIZE, PTE_R | PTE_W);

  // virtio mmio disk interface
  kvm_map(kernel_pt, VIRTIO0, VIRTIO0, PAGE_SIZE, PTE_R | PTE_W);

  // map the kernel data
  kvm_map(kernel_pt, KERNEL_BASE, KERNEL_BASE, KERNEL_END, PTE_R | PTE_W);

  // physical RAM the system will use
  kvm_map(kernel_pt, 0x4000, 0x4000, PHYS_STOP - 0x4000, PTE_R | PTE_W);

  // map the trampoline for trap entry/exit to
  // the highest virtual address in the kernel
  kvm_map(kernel_pt, TRAMPOLINE, (uint64)trampoline, PAGE_SIZE, PTE_R | PTE_X);

  // allocate and map a kernel stack for each process
  proc_map_stacks(kernel_pt);

  return kernel_pt;
}

void kvminit(void)
{
  kernel_pagetable = kvm_make();
}

// add a mapping to the kernel page table
// used only when booting, does not enable paging.
void kvm_map(pagetable_t kernel_pt, uint64 va, uint64 pa, uint64 size, int perm)
{
  if (map_pages(kernel_pt, va, size, pa, perm) != 0) {
    panic("kvm_map failed");
  }
}

pte_t* walk(pagetable_t page_table, uint64 va, int alloc)
{
  if (va >= MAX_VA) {
    panic("walk");
  }

  int level;
  pte_t *pte;

  for (level = 2; level > 0; level--) {
    pte = &page_table[PX(level, va)];
    if (*pte & PTE_V) {
      page_table = (pagetable_t) PTE_TO_PA(*pte);
    } else {
      if (!alloc || (page_table = (pte_t*)kalloc()) == 0) {
        return 0;
      }
      memset(page_table, 0, PAGE_SIZE);
      *pte = PA_TO_PTE(page_table) | PTE_V;
    }
  }

  return &page_table[PX(0, va)];
}

int map_pages(pagetable_t page_table, uint64 va, uint64 size, uint64 pa, int perm)
{
  uint64 first, last;
  pte_t *pte;

  if ((va % PAGE_SIZE) != 0) {
    panic("map_pages: va not aligned properly");
  }

  if ((size % PAGE_SIZE) != 0) {
    panic("map_pages: size not aligned properly");
  }

  if (size == 0) {
    panic("map_pages: invalid size");
  }

  first = va;
  last = va + size - PAGE_SIZE;

  for (;;) {
    if ((pte = walk(page_table, first, 1)) == 0) {
      return -1;
    }

    if (*pte & PTE_V) {
      panic("map_pages: remap");
    }
    *pte = PA_TO_PTE(pa) | perm | PTE_V;

    if (first == last) break;

    first += PAGE_SIZE;
    pa    += PAGE_SIZE;
  }

  return 0;
}
