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
#include "dac.hpp"

void init_dac() {
  NVIC_DisableIRQ( (IRQn_Type)( SPI0_IRQn ) );
  NVIC_SetPriority(SPI0_IRQn, 0);                 //make sure this is the top priority isr
  do                                //clear pending SPI0 interrupt...
  {                               //...
    NVIC_ClearPendingIRQ(SPI0_IRQn);              //...
  }
  while(NVIC_GetPendingIRQ(SPI0_IRQn) != 0);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 11);
  LPC_SPI0->DLY = 4;
  LPC_SPI0->DIV = 2;
  LPC_SPI0->TXCTRL = (16-1)<<24 | 1<<22 | ( 1 << 20 );
  LPC_SPI0->CFG = 0<<8 | 0<<7 | 1 << 5 | 1<<4 | 0<<3 | 1<<2 | 1<<0;
  switch_matrix::bind_SPI0_SSEL_IO( 8 );
  switch_matrix::bind_SPI0_SCK_IO( 9 );
  switch_matrix::bind_SPI0_MOSI_IO( 1 );
  LPC_SPI0->INTENSET = 0x0;
//  NVIC_EnableIRQ( (IRQn_Type)( SPI0_IRQn ) );
  LPC_SPI0->TXDAT = 2048 | ( 1 << 13 )|( 1 << 12 );
}

void set_dac( uint16_t value ) {
  value &= 4095;
  value |= ( 1 << 13 )|( 1 << 12 );
  LPC_SPI0->TXDAT = ( value );
}

uint16_t count = 0;

extern "C"
__attribute__((interrupt("IRQ")))
void SPI0_IRQHandler(void) {
//  if( LPC_SPI0->INTSTAT & 0x02 ) {
//    count += 512;
//    uint16_t value = ( count & 4095 ) | ( 1 << 13 )|( 1 << 12 );
//    LPC_SPI0->TXDAT = ( value );
//  }

}

