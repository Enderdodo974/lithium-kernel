
#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"

#include <stdarg.h>

const char digits[] = "0123456789abcdef";

void print_int(int value, int base, int sign)
{
  char buffer[16];
  int i;
  unsigned int x;

  if (sign && (sign = (value < 0))) {
    x = -value;
  } else {
    x =  value;
  }

  i = 0;
  do {
    buffer[i++] = digits[x % base];
  } while ((x /= base) != 0);

  if (sign) {
    buffer[i++] = '-';
  }

  while (--i >= 0) {
    console_putc(buffer[i]);
  }
}

void print_ptr(uint16 ptr)
{
  unsigned int i;
  console_putc('0');
  console_putc('x');
  for (i = 0; i < (sizeof(uint16)) * 2; i++, ptr <<= 4) {
    console_putc(digits[ptr >> (sizeof(uint16) * 8 - 4)]);
  }
}

void printf(char *fmt, ...)
{
  va_list args_p;
  int i, cx, format;
  char *s;

  va_start(args_p, fmt);

  for (i = 0; (cx = fmt[i]) != 0; i++) {
    if (cx != '%') {
      console_putc(cx);
      continue;
    }
    format = fmt[++i];
    switch (format) {
    case 'd':
      print_int(va_arg(args_p, int), 10, 1);
      break;
    case 'u':
      print_int(va_arg(args_p, int), 10, 0);
      break;
    case 'x':
      print_int(va_arg(args_p, int), 16, 1);
      break;
    case 'p':
      print_ptr(va_arg(args_p, int));
      break;
    case 's':
      if ((s = va_arg(args_p, char*)) == 0) {
        s = "null";
      }
      while (*s) {
        console_putc(*s);
        s++;
      }
      break;
    case '%':
      console_putc('%');
      break;
    default:
      // print unknown % sequence to draw attention
      console_putc('%');
      console_putc(format);
      break;
    }
  }
  va_end(args_p);
}

void panic(char *s)
{
  printf("panic: ");
  printf("%s\n", s);

  for(;;); // freeze the CPU
}
