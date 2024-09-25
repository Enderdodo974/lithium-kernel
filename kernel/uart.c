// uart.c

/*
 * Low-level driver routines for the 16550a UART.
*/

#include "types.h"
#include "param.h"
#include "defs.h"
#include "memlayout.h"

// the UART control registers are memory-mapped
// at address UART0. this macro returns the
// address of one of the registers.
#define Reg(reg) ((volatile uchar*)(UART0 + (reg)))

// the UART control registers. some have different meanings
// for read vs write. see http://byterunner.com/16550.html
#define RHR                    0 // recieve holding register (for input bytes)
#define THR                    0 // transmit holding register (for output bytes)
#define IER                    1 // interrupt enable register
#define IER_RX_ENABLE     (1<<0) // enable input
#define IER_TX_ENABLE     (1<<1) // enable output
#define FCR                    2 // FIFO control register
#define FCR_FIFO_ENABLE   (1<<0) // enable FIFOs
#define FCR_FIFO_CLEAR    (3<<1) // clear the content of the two FIFOs
#define ISR                    2 // interrupt status register
#define LCR                    3 // line control register
#define LCR_EIGHT_BITS    (3<<0) // 8-bit mode for signal transmission
#define LCR_BAUD_LATCH    (1<<7) // special mode to set baud rate
#define LSR                    5 // line status register
#define LSR_RX_READY      (1<<0) // input is waiting to be read from RHR
#define LSR_TX_IDLE       (1<<5) // THR can accept another character to send

#define ReadReg(reg)     (*(Reg(reg)))
#define WriteReg(reg, v) (*(Reg(reg)) = (v))


// the transmit output buffer
#define UART_TX_BUFFER_SIZE 16
char uart_tx_buffer[UART_TX_BUFFER_SIZE];
uint16 uart_tx_w; // write next to the transmit output buffer
uint16 uart_tx_r; // read next from the transmit output buffer

void uart_init(void)
{
  // disable interrupts
  WriteReg(IER, 0x00);

  // special mode to set baud rate
  WriteReg(LCR, LCR_BAUD_LATCH);

  // LSB set-up for baud rate of 1.024K
  // the baud rate determines how many bits/sec are transmitted
  // from and to the UART device
  // Note: the UART implementation differs in baud rate values
  // and in the way those values are interpreted.
  WriteReg(0, 0x00);

  // MSB for baud rate of 1.024K
  WriteReg(1, 0x40);

  // leave set-baud mode,
  // and set word length to 8 bits, no parity
  WriteReg(LCR, LCR_EIGHT_BITS);

  // reset and enable FIFOs
  WriteReg(FCR, FCR_FIFO_ENABLE | FCR_FIFO_CLEAR);

  // enable transmit and receive interrupts
  WriteReg(IER, IER_TX_ENABLE | IER_RX_ENABLE);
}

// add a character to the output buffer and tell the UART to
// start sending if it isn't already.
// blocks if the output buffer is full. because it may block,
// it can't be called from interrupts; it's only suitable for use
// by write().
void uart_putc(int c)
{
  while (uart_tx_w == uart_tx_r + UART_TX_BUFFER_SIZE) {
    // buffer is full.
    // wait for uart_start() to open up space in the buffer.
    sleep(&uart_tx_r);
  }
  uart_tx_buffer[uart_tx_w % UART_TX_BUFFER_SIZE] = c;
  uart_tx_w++;
  uart_start();
}

// alternate version of uart_putc() that doesn't
// use interrupts, for use by kernel printf() and
// to echo characters. it spins waiting for the uart's
// output register to be empty.
void uart_putc_sync(int c)
{
  // wait for Transmit Holding Empty to be set in LSR
  while ((ReadReg(LSR) & LSR_TX_IDLE) == 0)
    ;
  WriteReg(THR, c);
}

// if the UART is idle, and a character is waiting in the transmit
// buffer, send it. called from both the top- and bottom-half.
void uart_start(void)
{
  for (;;) {
    if (uart_tx_w == uart_tx_r) {
      // transmit buffer is empty.
      ReadReg(ISR);
      return;
    }

    if ((ReadReg(LSR) & LSR_TX_IDLE) == 0) {
      // the UART transmit holding register is full,
      // so we cannot give it another byte.
      // it will interrupt when it's ready for a new byte.
      return;
    }

    int c = uart_tx_buffer[uart_tx_r % UART_TX_BUFFER_SIZE];
    uart_tx_r++;

    // maybe uart_putc() is waiting for space in the buffer.
    wakeup(&uart_tx_r);

    WriteReg(THR, c);
  }
}
