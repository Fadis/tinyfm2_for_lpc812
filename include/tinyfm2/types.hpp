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
#ifndef TINYFM2_TYPES_HPP
#define TINYFM2_TYPES_HPP

#include <boost/mpl/size_t.hpp>
#include <tinyfm2/fixed.hpp>

namespace tinyfm2 {
  typedef rapidfp::fixed< boost::mpl::bool_< true >, boost::mpl::size_t< 32u >, boost::mpl::size_t< 16u > > fixed;
  struct matrix {
    inline constexpr matrix(){};
    inline constexpr matrix( const fixed &x_, const fixed &y_ ) : x( x_ ), y( y_ ) {};
    inline constexpr matrix( const matrix & ) = default;
    inline matrix &operator=( const matrix & ) = default;
    fixed x;
    fixed y;
  };
  inline constexpr matrix operator+( const matrix &left, const matrix &right ) {
    return matrix( left.x + right.x, right.y + left.y );
  }
  inline constexpr matrix operator-( const matrix &left, const matrix &right ) {
    return matrix( left.x - right.x, right.y - left.y );
  }
  inline constexpr matrix operator*( const matrix &left, const matrix &right ) {
    return matrix( left.x * right.x, left.x*right.y+left.y );
  }
  inline constexpr matrix mul_lower( const matrix &left, const matrix &right ) {
    return matrix( mul_lower( left.x, right.x ), mul_lower( left.x, right.y ) + left.y );
  }
  inline constexpr matrix mul_lower_tangent( const matrix &left, const matrix &right ) {
    return matrix( mul_lower( left.x, right.x ), left.x * right.y + left.y );
  }
  inline constexpr fixed operator*( const matrix &left, const fixed &right ) {
    return left.x * right + left.y;
  }
  inline constexpr fixed mul_lower( const matrix &left, const fixed &right ) {
    return mul_lower( left.x, right ) + left.y;
  }
  inline constexpr fixed operator*( const matrix &left, const fixed::single_type &right ) {
    return left.x * right + left.y;
  }
  inline constexpr matrix inverse( const matrix &left ) {
    return matrix( fixed(1)/left.x, -left.y/left.x );
  }
  inline constexpr matrix mix( const matrix &left, const matrix &right ) {
    return matrix( left.x * right.y + left.y * right.x, left.y * right.y );
  }
  inline constexpr matrix mix( const matrix &left, const matrix &right, const fixed &interpolate ) {
    return matrix(
      ( ( left * interpolate ) * ( right * interpolate ) - left.y * right.y )/interpolate,
      left.y * right.y
    );
  }
  typedef uint8_t scale_t;
  constexpr fixed::single_type sample_rate = 8000u;
  constexpr fixed::single_type block_size = 32u;
}

#endif

