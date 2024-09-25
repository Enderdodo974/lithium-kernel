// defs.h

// console.c
void console_init(void);

// printf.c
void printf(char*, ...) __attribute__ ((format (printf, 1, 2)));
void panic(char*) __attribute__ ((noreturn));

// uart.c
void uart_init(void);
void uart_putc(int);
void uart_putc_sync(int);
void uart_start(void);
