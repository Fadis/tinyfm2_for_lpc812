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
#ifndef TINYFM2_MIXER_HPP
#define TINYFM2_MIXER_HPP

#include <tinyfm2/types.hpp>
#include <tinyfm2/variables.hpp>

namespace tinyfm2 {
  // l0 l1 l2 l3

  class mixer {
  public:
    template< typename InputIterator >
    void change_instrument(
      InputIterator &iter
    ) {
      const fixed l0_level = fixed( *iter ) / 255;
      ++iter;
      const fixed l1_level = fixed( *iter ) / 255;
      ++iter;
      const fixed l2_level = fixed( *iter ) / 255;
      ++iter;
      const fixed l3_level = fixed( *iter ) / 255;
      ++iter;
      if( l0_level + l1_level + l2_level + l3_level > fixed( 1 ) ) {
        const fixed scale = fixed( 1 ) / ( l0_level + l1_level + l2_level + l3_level );
        l0 = l0_level * scale;
        l1 = l1_level * scale;
        l2 = l2_level * scale;
        l3 = l3_level * scale;
      }
      else {
        l0 = l0_level;
        l1 = l1_level;
        l2 = l2_level;
        l3 = l3_level;
      }
    }
    inline fixed operator()( const fixed &i0, const fixed &i1, const fixed &i2, const fixed &i3 ) const {
      return
        mul_lower( i0, l0 ) +
        mul_lower( i1, l1 ) +
        mul_lower( i2, l2 ) +
        mul_lower( i3, l3 );
    }
    inline fixed operator()( const variables::fm *fmstats ) const {
      return
        mul_lower( fmstats[ 0 ].feedback, l0 ) +
        mul_lower( fmstats[ 1 ].feedback, l1 ) +
        mul_lower( fmstats[ 2 ].feedback, l2 ) +
        mul_lower( fmstats[ 3 ].feedback, l3 );
    }
  private:
    fixed l0;
    fixed l1;
    fixed l2;
    fixed l3;
  };
}

#endif

