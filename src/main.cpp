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
#include "system_clock.hpp"
#include "uart.hpp"
#include "dac.hpp"
#include "score.hpp"
#include "config.hpp"
#include <tinyfm2/note_mapper.hpp>
#include <tinyfm2/sequencer2.hpp>
#include <tinyfm2/lpcsink.hpp>

extern uint16_t counter;

void init_system_clock() {
  LPC_SYSCON->SYSAHBCLKDIV = 2;
  LPC_SYSCON->SYSPLLCTRL = 0x24;
}

tinyfm2::sequencer2 seq( score, score + 2468 );

int main() {
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 6);
  LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 18);
  LPC_GPIO_PORT->DIR0 |= ( 1 << 16 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 15 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 14 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 8 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 9 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 1 );
  LPC_GPIO_PORT->DIR0 |= ( 1 << 1 );
  init_system_clock();
  init_dac();
  SysTick_Config( 1875 );
  //seq.setup( events, events + 18 );
  auto iter = sax;
  seq.change_instrument( 0, tinyfm2::fixed( 32 ), iter, wave_table );
  tinyfm2::fixed out[ 32 ];
  tinyfm2::lpcsink sink;
  for( uint32_t step = 0; ; ++step ) {
    sink.play_if_not_playing();
    if( sink.buffer_is_ready() ) {
      seq( out );
      sink( out );
    }
    else {
      LPC_GPIO_PORT->NOT0 = 1 << 15;
    }
  }
  while( 1 );
}
