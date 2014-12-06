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
#ifndef TINYFM2_LPCSINK_HPP
#define TINYFM2_LPCSINK_HPP

#include <tinyfm2/note_mapper.hpp>
#include <tinyfm2/types.hpp>

namespace tinyfm2 {

  static constexpr uint32_t buffer_count = 4u;

  extern volatile uint32_t read_head;
  extern volatile uint32_t write_head;
  extern volatile uint16_t audio_buffer[ 128 ];
  
  class lpcsink {
  public:
    lpcsink() {}
    inline void play_if_not_playing() {}
    inline bool buffer_is_ready() const {
      return write_head != read_head + 128;
    }
    inline void operator()( const tinyfm2::fixed *data ) {
      for( uint32_t index = 0u; index != block_size; ++index )
        audio_buffer[ ( write_head + index ) & 0x7F ] = ( data[ index ] * fixed( 0.5f ) + fixed( 0.5f ) ).base >> 4;
      write_head += 32;
    }
  };
}

#endif

