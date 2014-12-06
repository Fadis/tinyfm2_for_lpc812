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
#ifndef TINYFM2_VARIABLES_HPP
#define TINYFM2_VARIABLES_HPP

#include <tinyfm2/types.hpp>

namespace tinyfm2 {

  namespace variables {
    struct lfo {
      lfo() : memorized_level( 0 ) {}
      fixed memorized_level;
    };

    struct envelope {
      fixed note_off_level;
    };

    struct fm {
      fm() : feedback( 0 ), is_end( false ) {}
      fixed feedback;
      matrix tone_clock;
      matrix envelope_and_lfo;
      envelope envelope_;
      lfo lfo_;
      bool is_end;
    };
  
    struct tone {
      tone() : note_clock( fixed( 1 )/sample_rate, fixed( 0 ) ), tone_clock( fixed( 1 ), fixed( 0 ) ), key_clock_offset( 0 ), touch_level( 0 ) {}
      void note_on( const fixed &tone_freq, const fixed &touch_level_ ) {
        note_clock = matrix( fixed( 1 )/sample_rate, fixed( 0 ) );
        key_clock_offset = fixed( 0 );
        tone_clock = matrix( tone_freq, fixed( 0 ) );
        touch_level = touch_level_;
        for( uint32_t index = 0u; index != 4u; ++index ) {
          operators[ index ].feedback = 0;
          operators[ index ].is_end = false;
        }
      }
      void note_off() {
        //touch_level = fixed( 0 );
        key_clock_offset = -note_clock.y;
      }
      inline void advance( uint32_t block_size ) {
        note_clock.y += ( fixed( block_size )/sample_rate );
      }
      void pitch_bend( const fixed &new_hz ) {
        tone_clock = matrix(
          new_hz,
          ( tone_clock.x - new_hz ) * note_clock.y + tone_clock.y
        );
      }
      inline bool is_note_on() const {
        return key_clock_offset == fixed( 0 );
      }
      inline matrix get_tone_clock() const {
        return mul_lower_tangent( tone_clock, note_clock );
      }
      inline matrix get_key_clock() const {
        return matrix( note_clock.x, note_clock.y + key_clock_offset );
      }
      bool is_end() const {
        for( uint32_t index = 0; index != 4; ++index )
          if( !operators[ index ].is_end )
            return false;
        return true;
      }
      matrix note_clock;
      matrix tone_clock; // index to tone time
      fixed key_clock_offset;  // index to key time
      fixed touch_level;
      fm operators[ 4u ];
    };
  }
}

#endif

