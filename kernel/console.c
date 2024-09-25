// console.c

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"

#define BACKSPACE 0x100
#define CONTROL(x) ((x) - '@') // Control-x

void console_putc(int c)
{
  if (c == BACKSPACE) {
    // if the user typed backspace, overwrite with a space
    uart_putc_sync('\b'); uart_putc_sync(' '); uart_putc_sync('\b');
  } else {
    uart_putc_sync(c);
  }
}

void console_init(void)
{

  uart_init();

  // connect read and write system calls
  // to consoleread and consolewrite (TODO)
}
