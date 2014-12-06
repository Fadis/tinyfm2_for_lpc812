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
#ifndef TINYFM2_LFO_HPP
#define TINYFM2_LFO_HPP

#include <tinyfm2/types.hpp>
#include <tinyfm2/variables.hpp>

namespace tinyfm2 {
  // g   f    ar   
  //          4sec

  class lfo {
  public:
    template< typename InputIterator >
    void change_instrument(
      const fixed &time_per_block_,
      InputIterator &iter
    ) {
      time_per_block = time_per_block_;
      gain = ( fixed( *iter ) / 511 ) / 2;
      ++iter;
      frequency = fixed( *iter ) / 4;
      ++iter;
      attack_until = ( fixed( *iter ) * 4 ) / 255;
      ++iter;
      if( attack_until != fixed( 0 ) ) {
        attack.x = gain / attack_until;
        attack.y = fixed( 0 );
      }
    }
    inline matrix operator()( variables::lfo &status, fixed note_time ) const {
      if( frequency != fixed( 0 ) ) {
        const auto next_note_time = note_time + time_per_block;
        fixed next_level;
        if( next_note_time < attack_until ) {
          const fixed current_gain = attack * next_note_time;
          next_level = ( fixed( 1 ) - gain ) + mul_lower( sine( next_note_time * frequency ), current_gain );
        }
        else
          next_level = ( fixed( 1 ) - gain ) + mul_lower( sine( next_note_time * frequency ), gain );
        const auto tangent = div_lower( ( next_level - status.memorized_level ), time_per_block );
        const auto shift = status.memorized_level - tangent * note_time;
        status.memorized_level = next_level;
        return matrix( tangent, shift );
      }
      else {
        return matrix( fixed( 0 ), fixed( 1 ) - gain );
      }
    }
  private:
    fixed time_per_block;
    fixed frequency;
    fixed gain;
    fixed attack_until;
    matrix attack;
  };
}

#endif

