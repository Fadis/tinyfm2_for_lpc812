/********************************************************************************
 * Copyright (c) 2013 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * ******************************************************************************/

#include "LPC8xx.h"
#include "switch_matrix.hpp"
#include "uart.hpp"

void init_uart0() {
  NVIC_DisableIRQ( (IRQn_Type)( UART0_IRQn ) );
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 14);
  LPC_USART0->CFG = 1 | ( 1 << 2 );
  LPC_USART0->CTRL = 0;
  switch_matrix::bind_U0_RXD_I( 0 );
  switch_matrix::bind_U0_TXD_O( 4 );
  LPC_SYSCON->UARTCLKDIV = 1;
  LPC_SYSCON->UARTFRGDIV=255;
  LPC_SYSCON->UARTFRGMULT=244;
  LPC_USART0->BRG=199;
  LPC_USART0->INTENSET = 0x1|0x4|0x40;
  NVIC_EnableIRQ( (IRQn_Type)( UART0_IRQn ) );
}

void print( const char *str ) {
  txhead = str;
  init_uart0();
}

volatile const char *txhead;
char *rxhead;
uint32_t rxsize;

extern "C"
__attribute__((interrupt("IRQ")))
void UART0_IRQHandler(void) {
  if( LPC_USART0->INTSTAT & 0x01 ) {
    //const auto value = LPC_USART0->RXDATA;
    LPC_GPIO_PORT->NOT0 = 1 << 15;
  }
  else if( LPC_USART0->INTSTAT & 0x04 ) {
    if( txhead ) {
      if( *txhead != '\0' ) {
        LPC_USART0->TXDATA = *txhead;
        ++txhead;
      }
      else {
        LPC_USART0->INTENCLR = 1 << 2;
        LPC_USART0->CTRL = 1;
        txhead = nullptr;
      }
    }
  }
}

