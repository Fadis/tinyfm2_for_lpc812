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

#ifndef TINYFM2_LPC8XX_SWITCH_MATRIX_HPP
#define TINYFM2_LPC8XX_SWITCH_MATRIX_HPP

#include "LPC8xx.h"

#define PIN_BINDER( index, name0, name1, name2, name3 ) \
  static void bind_ ## name0 ( uint8_t pin ) { LPC_SWM->PINASSIGN[ index ] = ( LPC_SWM->PINASSIGN[ index ] & 0xFFFFFF00 ) | ( static_cast< uint32_t >( pin ) << 0 ); } \
  static void bind_ ## name1 ( uint8_t pin ) { LPC_SWM->PINASSIGN[ index ] = ( LPC_SWM->PINASSIGN[ index ] & 0xFFFF00FF ) | ( static_cast< uint32_t >( pin ) << 8 ); } \
  static void bind_ ## name2 ( uint8_t pin ) { LPC_SWM->PINASSIGN[ index ] = ( LPC_SWM->PINASSIGN[ index ] & 0xFF00FFFF ) | ( static_cast< uint32_t >( pin ) << 16 ); } \
  static void bind_ ## name3 ( uint8_t pin ) { LPC_SWM->PINASSIGN[ index ] = ( LPC_SWM->PINASSIGN[ index ] & 0x00FFFFFF ) | ( static_cast< uint32_t >( pin ) << 24 ); }

class switch_matrix {
  public:
    PIN_BINDER( 0, U0_TXD_O, U0_RXD_I, U0_RTS_O, U0_CTS_I )
      PIN_BINDER( 1, U0_SCLK_IO, U1_TXD_O, U1_RXD_I, U1_RTS_O )
      PIN_BINDER( 2, U1_CTS_I, U1_SCLK_IO, U2_TXD_O, U2_RXD_I )
      PIN_BINDER( 3, U2_RTS_O, U2_CTS_I, U2_SCLK_IO, SPI0_SCK_IO )
      PIN_BINDER( 4, SPI0_MOSI_IO, SPI0_MISO_IO, SPI0_SSEL_IO, SPI1_SCK_IO )
      PIN_BINDER( 5, SPI1_MOSI_IO, SPI1_MISO_IO, SPI1_SSEL_IO, CTIN_0_I )
      PIN_BINDER( 6, CTIN_1_I, CTIN_2_I, CTIN_3_I, CTOUT_0_O )
      PIN_BINDER( 7, CTOUT_1_O, CTOUT_2_O, CTOUT_3_O, I2C_SDA_IO )
      PIN_BINDER( 8, I2C_SCL_IO, ACMP_O_O, CLKOUT_O, GPIO_INT_BMAT_O )
};

#endif

