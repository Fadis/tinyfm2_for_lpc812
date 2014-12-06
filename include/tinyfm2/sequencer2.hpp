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
#ifndef TINYFM2_SEQUENCER2_HPP
#define TINYFM2_SEQUENCER2_HPP

#include <tinyfm2/note_mapper.hpp>
#include <tinyfm2/smf_parser.hpp>

namespace tinyfm2 {
  class sequencer2 {
  public:
    sequencer2( const uint8_t *begin, const uint8_t *end ) : current_step( 0u ), sleep_until( 0u ), parser( begin, end ) {}
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument(
      uint8_t instrument,
      const fixed &time_per_block_,
      InputIterator &iter,
      const WaveTableIterator &wt
    ) {
      map.change_instrument( instrument, time_per_block_, iter, wt );
    }
    void operator()( fixed *out ) {
      if( sleep_until <= current_step )
        control();
      map( out );
      current_step += 32 * 1000 * 1000 / 8000;
    }
  private:
    void control() {
      while( !parser.is_end() ) {
        parser.next();
        if( parser.get_event_type() == smf_parser< const uint8_t* >::event_type::note_on ) {
          map.note_on( instrument, fixed( parser.get_velocity() )/fixed( 255 ), parser.get_scale() -12 );
        }
        else if( parser.get_event_type() == smf_parser< const uint8_t* >::event_type::note_off ) {
          map.note_off( instrument, parser.get_scale() - 12 );
        }
        else if( parser.get_event_type() == smf_parser< const uint8_t* >::event_type::pitch_bend ) {
        }
        else if( parser.get_event_type() == smf_parser< const uint8_t* >::event_type::sleep ) {
          sleep_until += parser.get_length();
          break;
        }
      }
    }
    uint32_t current_step;
    uint32_t sleep_until;
    uint8_t instrument;
    smf_parser< const uint8_t* > parser;
    note_mapper map;
  };
}

#endif

