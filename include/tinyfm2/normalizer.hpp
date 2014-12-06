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
#ifndef TINYFM2_NORMALIZER_HPP
#define TINYFM2_NORMALIZER_HPP

#include <tinyfm2/types.hpp>

namespace tinyfm2 {
  class normalizer {
  public:
    normalizer() : scale( 1 ) {}
    inline void operator()( fixed *buffer ) {
      fixed max = std::max( fixed( 1 ), scale - fixed( block_size, rapidfp::raw() ) );
      for( uint32_t index = 0u; index != block_size; ++index ) {
        const fixed abs_sample = fabs( buffer[ index ] + mul_lower( buffer[ index ], fixed( 0.2f ) ) );
        if( abs_sample > max )
          max = abs_sample;
      }
      const auto diff = max - scale;
      if( diff != fixed( 0 ) ) {
        const matrix scale_( diff, scale );
        for( uint32_t index = 0u; index != block_size; ++index ) {
          buffer[ index ] = div_lower( buffer[ index ], ( scale_ * index ) );
        }
        scale = max;
      }
      else {
        const auto vol = div_lower( fixed( 1 ), scale );
        for( uint32_t index = 0u; index != block_size; ++index ) {
          buffer[ index ] = mul_lower( buffer[ index ], vol );
        }
      
      }
    }
  private:
    fixed scale;
  };
}

#endif

