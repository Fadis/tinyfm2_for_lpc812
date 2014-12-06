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
#ifndef TINYFM2_NOTE_MAPPER_HPP
#define TINYFM2_NOTE_MAPPER_HPP

#include <tinyfm2/tone.hpp>
#include <tinyfm2/normalizer.hpp>
#include <tinyfm2/variables.hpp>

namespace tinyfm2 {
  struct active_note {
    active_note() : id( 0u ) {}
    void reset( uint16_t id_, uint8_t instrument_, uint8_t scale_ ) {
      id = id_;
      instrument = instrument_;
      scale = scale_;
    }
    uint16_t id;
    uint8_t instrument;
    uint8_t scale;
  };
  class note_mapper {
  public:
    note_mapper() {}
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument(
      uint8_t instrument,
      const fixed &time_per_block_,
      InputIterator &iter,
      const WaveTableIterator &wt
    ) {
      tone_generator[ instrument ].change_instrument( time_per_block_, iter, wt );
      next_note_id = 0;
      for( uint32_t index = 0u; index != 3u; ++index ) {
        active_notes[ index ].id = 0;
      }
    }
    uint16_t note_on( uint8_t instrument, const fixed &touch_level, scale_t scale ) {
      return note_on( instrument, touch_level, exp2( ( fixed( scale + 3 ) / 12 ) ) * fixed( 6.875 ), scale );
    }
    uint16_t note_on( uint8_t instrument, const fixed &touch_level, const fixed &frequency, scale_t scale ) {
      const uint8_t slot = get_note_slot();
      const uint16_t id = next_note_id + slot + 4;
      next_note_id += 8;
      active_notes[ slot ].reset( id, instrument, scale );
      tone_info[ slot ].note_on( frequency, touch_level );
      return id;
    }
    void note_off( uint16_t id ) {
      if( active_notes[ id % 3 ].id == id ) {
        tone_info[ id % 3 ].note_off();
      }
    }
    void note_off( uint8_t instrument, scale_t scale ) {
      for( uint32_t index = 0u; index != 3u; ++index ) {
        if( active_notes[ index ].id ) {
          if( active_notes[ index ].instrument == instrument && active_notes[ index ].scale == scale ) {
            tone_info[ index ].note_off();
            return;
          }
        }
      }
    }
    void pitch_bend( const fixed &scale ) {
      for( uint32_t index = 0u; index != 3u; ++index ) {
        if( active_notes[ index ].id ) {
          tone_info[ index ].pitch_bend( exp2( ( ( fixed( active_notes[ index ].scale ) * scale + fixed( 3 ) ) / fixed( 12 ) ) ) * fixed( 6.875 ) );
        }
      }
    }
    void operator()( fixed *out ) {
      std::fill( out, out + block_size, tinyfm2::fixed( 0 ) );
      for( uint32_t index = 0u; index != 3u; ++index ) {
        if( active_notes[ index ].id ) {
          if( active_notes[ index ].instrument < 1 ) {
            tone_generator[ active_notes[ index ].instrument ].calc_simple( out, tone_info[ index ] );
            if( tone_info[ index ].is_end() ) {
              active_notes[ index ].id = 0u;
            }
            else {
              tone_info[ index ].advance( block_size );
            }
          }
          else {
          }
        }
      }
      norm( out );
      clean();
    }
  private:
    void clean() {
      for( uint32_t index = 0u; index != 3u; ++index ) {
        if( tone_info[ index ].is_end() ) {
          active_notes[ index ].id = 0u;
        }
      }
    }
    uint8_t get_note_slot() const {
      uint16_t min_id = active_notes[ 0u ].id;
      uint8_t min_slot = 0u;
      for( uint8_t index = 0u; index != 3u; ++index ) {
        if( min_id > active_notes[ index ].id ) {
          min_id = active_notes[ index ].id;
          min_slot = index;
          if( min_id == 0u )
            return index;
        }
      }
      return min_slot;
    }
    uint16_t next_note_id;
    active_note active_notes[ 3 ];
    variables::tone tone_info[ 3 ];
    tone tone_generator[ 1 ];
    normalizer norm;
  };
}

#endif

