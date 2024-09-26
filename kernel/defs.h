// defs.h

#include "types.h"

// console.c
void console_init(void);

// kalloc.c
void kfree(void*);
void* kalloc(void);

// printf.c
void printf(char*, ...) __attribute__ ((format (printf, 1, 2)));
void panic(char*) __attribute__ ((noreturn));

// string.c
void memset(void*, int, int);

// uart.c
void uart_init(void);
void uart_putc(int);
void uart_putc_sync(int);
void uart_start(void);

// vm.c
void kvm_map(pagetable_t, uint64, uint64, uint64, int);
int map_pages(pagetable_t, uint64, uint64, uint64, int);
pte_t* walk(pagetable_t, uint64, int);
