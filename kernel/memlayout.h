// memlayout.h

/*
 * Physical RAM layout
 *
 * 0000 -- kernel RAM and stack space
 * 1fff -- end of kernel space
 * 2000 -- UART0
 * 3100 -- virtio disk
 *
 * 7fff -- end of physical RAM (PHY_STOP)
*/

// UART
#define UART0 0x2000
#define UART_IRQ 10

// virtio mmio interface
#define VIRTIO0 0x3100
#define VIRTIO0_IRQ 1

// kernel
#define KERNEL_BASE 0x0000
#define PHYS_STOP 0x7fff

// trampoline
// map the trampoline page to the highest address,
// in both user and kernel space
#define TRAMPOLINE (MAXVA - PGSIZE)

// map kernel stack beneath the trampoline,
// surrounded by invalid guard pages
#define KERNEL_STACK_VA(p) (TRAMPOLINE - ((p) + 1) * 2 * PGSIZE)

// User memory layout
// Address 0 first:
//   text
//   original data
//   fixed-sized stack
//   expandable heap
//   ...
//   TRAPFRAME (p->trapframe, used by the trampoline)
//   TRAMPOLINE (the same page as in the kernel)
#define TRAPFRAME (TRAMPOLINE - PGSIZE)
