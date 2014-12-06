/********************************************************************************
 * Copyright (c) 2014 Naomasa Matsubayashi
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

#define CLOCK_CONTROL( name, bit ) \
  static inline void enable_ ##  name () { LPC_SYSCON->SYSAHBCLKCTRL |= ( 1 << bit ); } \
  static inline void disable_ ## name () { LPC_SYSCON->SYSAHBCLKCTRL ^= ( 1 << bit ); }

enum class output_clock_source {
  IRC_OSCILLATOR = 0,
  CRYSTAL_OSCILLATOR = 1,
  WATCHDOG_OSCILLATOR = 2,
  MAIN_CLOCK = 3
};

class system_clock {
  CLOCK_CONTROL( SYS, 0 )
  CLOCK_CONTROL( ROM, 1 )
  CLOCK_CONTROL( RAM, 2 )
  CLOCK_CONTROL( FLASHREG, 3 )
  CLOCK_CONTROL( FLASH, 4 )
  CLOCK_CONTROL( I2C, 5 )
  CLOCK_CONTROL( GPIO, 6 )
  CLOCK_CONTROL( SWM, 7 )
  CLOCK_CONTROL( SCT, 8 )
  CLOCK_CONTROL( WKT, 9 )
  CLOCK_CONTROL( MRT, 10 )
  CLOCK_CONTROL( SPI0, 11 )
  CLOCK_CONTROL( SPI1, 12 )
  CLOCK_CONTROL( CRC, 13 )
  CLOCK_CONTROL( UART0, 14 )
  CLOCK_CONTROL( UART1, 15 )
  CLOCK_CONTROL( UART2, 16 )
  CLOCK_CONTROL( WWDT, 17 )
  CLOCK_CONTROL( IOCON, 0 )
  CLOCK_CONTROL( ACMP, 0 )
  static inline void select_CLKOUT_source( const output_clock_source &clock ) {
    LPC_SYSCON->CLKOUTUEN = 0;
    LPC_SYSCON->CLKOUTSEL = uint32_t( clock );
    LPC_SYSCON->CLKOUTUEN = 1;
  }
};

