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
#ifndef TINYFM2_ENVELOPE_HPP
#define TINYFM2_ENVELOPE_HPP

#include <tinyfm2/types.hpp>
#include <tinyfm2/variables.hpp>

namespace tinyfm2 {
  // g   ar   dr   sr   rr   sl gt
  //     4sec 4sec 4sec 4sec 

  class envelope {
  public:
    template< typename InputIterator >
    void change_instrument(
      InputIterator &iter
    ) {
      const fixed gain = fixed( *iter ) / 255;
      ++iter;
      attack_until = ( fixed( *iter ) * 4 ) / 255;
      ++iter;
      const fixed decay_length = ( fixed( *iter ) * 4 ) / 255;
      decay_until = attack_until + decay_length;
      ++iter;
      const uint8_t raw_sustain_length = *iter;
      const fixed sustain_length = ( fixed( raw_sustain_length ) * 4 ) / 255;
      sustain_until = decay_until + sustain_length;
      ++iter;
      const fixed release_length = ( fixed( *iter ) * 4 ) / 255;
      ++iter;
      const fixed sustain_level = mul_lower( fixed( *iter )/255, gain );
      ++iter;
      if( attack_until != fixed( 0 ) ) {
        attack.x = gain / attack_until;
        attack.y = fixed( 0 );
      }
      if( decay_length != fixed( 0 ) ) {
        decay.x = ( sustain_level - gain ) / decay_length;
        decay.y = gain - decay.x * attack_until;
      }
      if( raw_sustain_length == 0 ) {
        sustain.x = 0;
        sustain.y = sustain_level;
      }
      else {
        sustain.x = -sustain_level / sustain_length;
        sustain.y = sustain_level - sustain.x * decay_until;
      }
      if( release_length != fixed( 0 ) ) {
        release_tangent = -sustain_level / release_length;
      }
      else
        release_tangent = fixed( 0 );
    }
    inline matrix operator()( const variables::envelope &status, bool is_note_on, fixed key_time ) const {
      if( is_note_on ) {
        if( key_time < attack_until )
          return attack;
        else if( key_time < decay_until )
          return decay;
        else if( sustain_until == fixed( 0 ) || key_time < sustain_until )
          return sustain;
        else
          return matrix( fixed( 0 ), fixed( 0 ) );
      }
      else if( release_tangent != fixed( 0 ) ) {
        matrix release( release_tangent, status.note_off_level );
        if( release * key_time > fixed( 0 ) )
          return release;
        else {
          return matrix( fixed( 0 ), fixed( 0 ) );
        }
      }
      else
        return matrix( release_tangent, fixed( 0 ) );
    }
  private:
    fixed attack_until;
    matrix attack;
    fixed decay_until;
    matrix decay;
    fixed sustain_until;
    matrix sustain;
    fixed release_tangent; 
  };

}

#endif

