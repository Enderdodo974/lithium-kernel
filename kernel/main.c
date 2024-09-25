// main.c

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"

void main(void)
{

  // console initialisation for logs and debug output
  console_init();
  printf("lithium-kernel is now booting...");
  printf("\n");
  kinit();
}
