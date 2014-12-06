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
#ifndef TINYFM2_SEQUENCER_HPP
#define TINYFM2_SEQUENCER_HPP

#include <tinyfm2/note_mapper.hpp>

namespace tinyfm2 {
  struct event_t {
    enum class event_type {
      sleep,
      note_on,
      note_off,
      pitch_bend
    };
    constexpr event_t( event_type e, uint8_t i, uint16_t p ) : event( e ), instrument( i ), param( p ) {}
    event_t( const event_t& ) = default;
    event_t( event_t&& ) = default;
    event_t &operator=( const event_t& ) = default;
    event_t &operator=( event_t&& ) = default;
    event_type event;
    uint8_t instrument;
    uint16_t param;
  };
  class sequencer {
  public:
    void setup( const event_t *begin, const event_t *end_ ) {
      current_step = 0u;
      sleep_until = 0u;
      cur = begin;
      end = end_;
    }
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument(
      uint8_t instrument,
      const fixed &time_per_block_,
      InputIterator &iter,
      const WaveTableIterator &wt
    ) {
      map.change_instrument( instrument, time_per_block_, iter, wt );
        //map.note_on( cur->instrument, fixed( 1 ), 60 );
    }
    void operator()( fixed *out ) {
      if( sleep_until < ++current_step )
        control();
      map( out );
    }
  private:
    void control() {
      for( ;cur != end; ++cur ) {
        if( cur->event == event_t::event_type::note_on )
          map.note_on( cur->instrument, fixed( cur->param >> 8 )/fixed( 255 ), scale_t( cur->param & 0xFF ) );
        else if( cur->event == event_t::event_type::note_off )
          map.note_off( cur->instrument, scale_t( cur->param & 0xFF ) );
        else if( cur->event == event_t::event_type::pitch_bend )
          map.pitch_bend( fixed( ( fixed::single_type( cur->param ) + 1 ) << 5, rapidfp::raw() ) );
        else if( cur->event == event_t::event_type::sleep ) {
          const fixed sleep_length( ( fixed::single_type( cur->param ) + 1 ), rapidfp::raw() );
          sleep_until += uint32_t( ceil( ( sleep_length * sample_rate ) / fixed( block_size ) ) );
          ++cur;
          break;
        }
      }
    }
    uint32_t current_step;
    uint32_t sleep_until;
    const event_t *cur;
    const event_t *end;
    note_mapper map;
  };
}

#endif

