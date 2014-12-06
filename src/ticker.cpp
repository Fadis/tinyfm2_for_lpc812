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
#include "dac.hpp"
#include "uart.hpp"
#include <tinyfm2/lpcsink.hpp>

uint16_t step = 0;
uint16_t last_value = 0;

extern "C"
__attribute__((interrupt("IRQ")))
void SysTick_Handler(void) {
  if( tinyfm2::read_head != tinyfm2::write_head ) {
    if( step & 0x01 ) {
      set_dac( last_value = tinyfm2::audio_buffer[ ( tinyfm2::read_head & 0x7F ) + (step>>1) ] );
    }
    else {
      set_dac( last_value = ( tinyfm2::audio_buffer[ ( tinyfm2::read_head & 0x7F ) + (step>>1) ] + last_value ) >> 1 );
    }
    ++step;
    if( step == tinyfm2::block_size*2 ) {
      tinyfm2::read_head += 32;
      step = 0;
    }
  }
  else {
    set_dac( last_value );
    ++step;
    if( step == tinyfm2::block_size*2 ) {
      step = 0;
    }
  }
}

