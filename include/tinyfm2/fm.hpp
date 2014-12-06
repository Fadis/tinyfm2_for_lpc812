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
#ifndef TINYFM2_FM_HPP
#define TINYFM2_FM_HPP

#include <tinyfm2/types.hpp>
#include <tinyfm2/adpcm/decoder.hpp>
#include <tinyfm2/envelope.hpp>
#include <tinyfm2/lfo.hpp>
#include <tinyfm2/mixer.hpp>
#include <tinyfm2/variables.hpp>

namespace tinyfm2 {
  // dt   dt   wt   wt 

  class fm_core {
  public:
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument( InputIterator &iter, const WaveTableIterator &wt ) {
      fixed::single_type raw_detune = fixed::single_type( *iter ) << 5;
      ++iter;
      raw_detune |= fixed::single_type( *iter ) << 13;
      ++iter;
      detune = matrix( fixed( raw_detune, rapidfp::raw() ), fixed( 0 ) );
      uint16_t wt_offset = *iter;
      ++iter;
      wt_offset |= uint16_t( *iter ) << 8;
      ++iter;
      const auto wt_begin = std::next( wt, wt_offset );
      auto wt_end = std::next( wt_begin, 64 );
      decode( wt_begin, wt_end, 128, &*wave_table );
    }
    inline void setup( variables::fm &fmstat, const matrix &tone_clock ) const {
      fmstat.tone_clock = mul_lower( detune, tone_clock );
    }
    inline fixed operator()( const variables::fm &fmstat, const fixed::single_type &index, const fixed &modulator_value ) const {
      //static int32_t step = 0u;
      //return fixed( int32_t( wave_table[ ( ( ++step /*+ int32_t( modulator_value * fixed( 128 ) )*/ ) & 0x7F ) ] ) << 9, rapidfp::raw() );
      return fixed( fixed::single_type( wave_table[ ( ( fmstat.tone_clock * index + modulator_value ).base >> 9 ) & 0x7F ] ) << 9, rapidfp::raw() );

      //return sine( loop( fmstat.tone_clock * index + modulator_value ) );
    }
  private:
    matrix detune;
    int8_t wave_table[ 128 ];
  };

  class fm {
  public:
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument(
      const fixed &time_per_block_,
      InputIterator &iter,
      const WaveTableIterator &wt
    ) {
      envelope_.change_instrument( iter );
      lfo_.change_instrument( time_per_block_, iter );
      mixer_.change_instrument( iter );
      fm_.change_instrument( iter, wt );
    }
    void setup( variables::fm &fmstat, const variables::tone &tstat ) const {
      const matrix key_clock = tstat.get_key_clock();
      const matrix current_envelope = mul_lower( envelope_( fmstat.envelope_, tstat.is_note_on(), key_clock.y ), key_clock );
      fixed note_off_level = std::max( current_envelope * block_size, fixed( 0u ) );
      if( tstat.is_note_on() )
        fmstat.envelope_.note_off_level = note_off_level;
      if( note_off_level == fixed( 0u ) )
        fmstat.is_end = true;
      //fmstat.envelope_and_lfo = mul_lower( matrix( tstat.touch_level, fixed( 0 ) ), current_envelope );
      fmstat.envelope_and_lfo = matrix( fixed( 0 ),  mul_lower( tstat.touch_level, current_envelope.y ) );
      fm_.setup( fmstat, tstat.get_tone_clock() );
    }
    inline fixed calc_simple( const variables::fm *fmstats, uint32_t operator_index, const fixed::single_type &index ) const {
      return mul_lower(
        (
          fmstats[ operator_index ].envelope_and_lfo.y
        ),
        fm_(
          fmstats[ operator_index ],
          index,
          mixer_( fmstats )
        )
      );
    }
    inline fixed calc( const variables::fm *fmstats, uint32_t operator_index, const fixed::single_type &index ) const {
      return mul_lower(
        (
          fmstats[ operator_index ].envelope_and_lfo*operator_index
        ),
        fm_(
          fmstats[ operator_index ],
          index,
          mixer_( fmstats )
        )
      );
    }
    /*inline fixed operator()( const variables::fm &fmstat, const fixed &index, const fixed &, const fixed &, const fixed &, const fixed & ) const {
      return (fmstat.envelope_and_lfo*index);
    }*/
  private:
    envelope envelope_;
    lfo lfo_;
    mixer mixer_;
    fm_core fm_;
  };
}

#endif

