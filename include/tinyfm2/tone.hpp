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
#ifndef TINYFM2_TONE_HPP
#define TINYFM2_TONE_HPP

#include <vector>
#include <boost/preprocessor.hpp>
#include <tinyfm2/types.hpp>
#include <tinyfm2/variables.hpp>
#include <tinyfm2/fm.hpp>

namespace tinyfm2 {
  class tone {
  public:
    template< typename InputIterator, typename WaveTableIterator >
    void change_instrument(
      const fixed &time_per_block_,
      InputIterator &iter,
      const WaveTableIterator &wt
    ) {
      mixer_.change_instrument( iter );
      for( uint32_t index = 0u; index != 4u; ++index )
        operators[ index ].change_instrument( time_per_block_, iter, wt );
    }
    void calc_simple(
      fixed *out,
      variables::tone &tstat
    ) const {
      setup_fm( tstat );
      for( uint32_t sample_index = 0; sample_index != block_size; ++sample_index ) {
        calc_fm_simple( tstat, sample_index );
        output_fm( out, tstat, sample_index );
      }
    }
    void calc(
      fixed *out,
      variables::tone &tstat
    ) const {
      setup_fm( tstat );
      for( uint32_t sample_index = 0; sample_index != block_size; ++sample_index ) {
        calc_fm( tstat, sample_index );
        output_fm( out, tstat, sample_index );
      }
    }
  private:
    inline void setup_fm(
      variables::tone &tstat
    ) const {
      for( uint32_t index = 0; index != 4; ++index )
        operators[ index ].setup( tstat.operators[ index ], tstat );
    }
    inline void calc_fm(
      variables::tone &tstat,
      uint32_t index
    ) const {
      for( uint32_t operator_index = 0; operator_index != 4u; ++operator_index ) {
        tstat.operators[ operator_index ].feedback = operators[ operator_index ].calc( tstat.operators, operator_index, index );
      }
    }
    inline void calc_fm_simple(
      variables::tone &tstat,
      uint32_t index
    ) const {
      for( uint32_t operator_index = 0; operator_index != 4u; ++operator_index ) {
        tstat.operators[ operator_index ].feedback = operators[ operator_index ].calc_simple( tstat.operators, operator_index, index );
      }
    }
    inline void output_fm(
      fixed *out,
      variables::tone &tstat,
      uint32_t index
    ) const {
      out[ index ] += mixer_(
        tstat.operators[ 0 ].feedback,
        tstat.operators[ 1 ].feedback,
        tstat.operators[ 2 ].feedback,
        tstat.operators[ 3 ].feedback
      );
    }
    mixer mixer_;
    fm operators[ 4 ];
  };
}

#endif

