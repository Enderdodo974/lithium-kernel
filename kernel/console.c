// console.c

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"

void console_init(void)
{

  uart_init();

  // connect read and write system calls
  // to consoleread and consolewrite
}
