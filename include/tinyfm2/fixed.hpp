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
#ifndef TINYFM2_FIXED_HPP
#define TINYFM2_FIXED_HPP

#include <cstdint>
#include <boost/mpl/if.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/greater_equal.hpp>
#include <boost/mpl/max.hpp>
#include <boost/mpl/minus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer.hpp>
#include <boost/utility/enable_if.hpp>

namespace rapidfp {
  constexpr long double pi = 3.141592653589793238462643383279502884L;
  constexpr long double e = 2.718281828459045235360287471352662498L;
  constexpr long double ln2 = 0.693147180559945309417232121458176568L;
  constexpr long double ln10 = 2.302585092994045684017991454684364208L;
  struct raw{};
  template< typename Sign, int length, typename Enable = void >
  struct int_type {};
  template< typename Sign, int length >
  struct int_type< Sign, length, typename boost::enable_if< Sign >::type > { typedef typename boost::int_t< length >::exact type; };
  template< typename Sign, int length >
  struct int_type< Sign, length, typename boost::disable_if< Sign >::type > { typedef typename boost::uint_t< length >::exact type; };

  template< typename Sign, typename Length, typename Shift >
  struct fixed {
  public:
    typedef fixed< Sign, Length, Shift > self_type;
    typedef typename int_type< Sign, Length::value >::type single_type;
    typedef typename int_type< Sign, Length::value * 2u >::type double_type;
    inline constexpr fixed() : base( 0 ) {}
    template< typename T >
    inline constexpr fixed( T value, typename boost::enable_if< boost::is_float< T > >::type* = 0 ) : base( value * ( 1 << Shift::value ) ) {}
    template< typename T >
    inline constexpr fixed( T value, typename boost::enable_if< boost::is_integral< T > >::type* = 0 ) : base( static_cast< single_type >( value ) << Shift::value ) {}
    template< typename T >
    inline constexpr fixed( T value, const raw& ) : base( value ) {}
    template< typename Sign_, typename Length_, typename Shift_ >
    inline constexpr fixed( const fixed< Sign_, Length_, Shift_ > &value, typename boost::disable_if< boost::is_same< fixed< Sign_, Length_, Shift_ >, self_type > >::type* = 0 ) : base( static_cast< self_type >( value.base ) ) {}
    inline constexpr fixed( const self_type &value ) : base( value.base ) {}
    static constexpr single_type pi() {
      return static_cast< single_type >( rapidfp::pi * ( 1u << Shift::value ) );
    }
    static constexpr single_type e() {
      return static_cast< single_type >( rapidfp::e * ( 1u << Shift::value ) );
    }
    static constexpr single_type ln2() {
      return static_cast< single_type >( rapidfp::ln2 * ( 1u << Shift::value ) );
    }
    static constexpr single_type ln10() {
      return static_cast< single_type >( rapidfp::ln10 * ( 1u << Shift::value ) );
    }
    static constexpr single_type one() {
      return static_cast< single_type >( 1 ) << Shift::value;
    }
    static constexpr single_type max() {
      return std::numeric_limits< single_type >::max();
    }
    static constexpr single_type min() {
      return std::numeric_limits< single_type >::min();
    }
    inline constexpr self_type operator-() const {
      return self_type( -base, raw() );
    }
    inline constexpr self_type operator+() const {
      return *this;
    }
    inline self_type operator--() {
      base -= one();
      return *this;
    }
    inline self_type operator++() {
      base += one();
      return *this;
    }
    inline self_type operator--(int) {
      self_type old( *this );
      base -= one();
      return old;
    }
    inline self_type operator++(int) {
      self_type old( *this );
      base += one();
      return old;
    }
    inline constexpr self_type operator+( const self_type &y ) const {
      return self_type( base + y.base, raw() );
    }
    inline constexpr self_type operator-( const self_type &y ) const {
      return self_type( base - y.base, raw() );
    }
    inline constexpr self_type operator*( const self_type &y ) const {
      return self_type(
      ( ( static_cast< double_type >( base ) ) * static_cast< double_type >( y.base ) ) >> Shift::value, raw()
      );
    }
    inline constexpr self_type operator/( const self_type &y ) const {
      return self_type(
      ( ( static_cast< double_type >( base ) << Shift::value ) / static_cast< double_type >( y.base ) ), raw()
      );
    }
    inline constexpr bool operator>( const self_type &y ) const {
      return base > y.base;
    }
    inline constexpr bool operator<( const self_type &y ) const {
      return base < y.base;
    }
    inline constexpr bool operator>=( const self_type &y ) const {
      return base >= y.base;
    }
    inline constexpr bool operator<=( const self_type &y ) const {
      return base <= y.base;
    }
    inline constexpr bool operator==( const self_type &y ) const {
      return base == y.base;
    }
    inline constexpr bool operator!=( const self_type &y ) const {
      return base != y.base;
    }
    inline constexpr self_type operator<<( int y ) const {
      return self_type( base << y, raw() );
    }
    inline constexpr self_type operator>>( int y ) const {
      return self_type( base >> y, raw() );
    }
    inline constexpr self_type shift( int y ) const {
      return ( y < 0 ) ? self_type( base >> -y, raw() ) : self_type( base << y, raw() );
    }
    inline self_type &operator+=( const self_type &y ) {
      base += y.base;
      return *this;
    }
    inline self_type &operator-=( const self_type &y ) {
      base -= y.base;
      return *this;
    }
    inline self_type &operator*=( const self_type &y ) {
      base = ( static_cast< double_type >( base ) * static_cast< double_type >( y.base ) ) >> Shift::value;
      return *this;
    }
    inline self_type &operator/=( const self_type &y ) {
      base = ( ( static_cast< double_type >( base ) << Shift::value ) / static_cast< double_type >( y.base ) );
      return *this;
    }
    inline self_type &operator<<=( int y ) {
      base <<= y;
      return *this;
    }
    inline self_type &operator>>=( int y ) {
      base >>= y;
      return *this;
    }
    template< typename T >
    inline self_type &operator=( const T &y ) {
      assign( y );
      return *this;
    }
    inline constexpr operator bool() const { return base != 0u; }
    inline constexpr operator char() const { return base >> Shift::value; }
    inline constexpr operator unsigned char() const { return base >> Shift::value; }
    inline constexpr operator short() const { return base >> Shift::value; }
    inline constexpr operator unsigned short() const { return base >> Shift::value; }
    inline constexpr operator int() const { return base >> Shift::value; }
    inline constexpr operator unsigned int() const { return base >> Shift::value; }
    inline constexpr operator long() const { return base >> Shift::value; }
    inline constexpr operator unsigned long() const { return base >> Shift::value; }
    //inline constexpr operator long long() const { return base >> Shift::value; }
   // inline constexpr operator unsigned long long() const { return base >> Shift::value; }
    inline constexpr operator float() const { return static_cast< float >( base ) / ( 1 << Shift::value ); }
    inline constexpr operator double() const { return static_cast< double >( base ) / ( 1 << Shift::value ); }
    inline constexpr operator long double() const { return static_cast< long double >( base ) / ( 1 << Shift::value ); }
    template< typename Sign_, typename Length_, typename Shift_ >
    inline constexpr operator fixed< Sign_, Length_, Shift_ >() const {
      return cast< Sign_, Length_, Shift_ >();
    }
    single_type base;
  private:
    template< typename T >
    inline void assign( T value, typename boost::enable_if< boost::is_float< T > >::type* = 0 ) {
      base = value * ( 1 << Shift::value );
    }
    template< typename T >
    inline void assign( T value, typename boost::enable_if< boost::is_integral< T > >::type* = 0 ) {
      base = static_cast< single_type >( value ) << Shift::value;
    }
    template< typename Sign_, typename Length_, typename Shift_ >
    inline void assign( const fixed< Sign_, Length_, Shift_ > &value, typename boost::disable_if< boost::is_same< fixed< Sign_, Length_, Shift_ >, self_type > >::type* = 0 ) {
      base = static_cast< self_type >( value ).base;
    }
    inline void assign( const self_type &value ) {
      base = value.base;
    }
    template< typename Sign_, typename Length_, typename Shift_ >
    inline constexpr fixed< Sign_, Length_, Shift_ > cast( typename boost::enable_if< boost::mpl::less< Shift, Shift_ > >::type* = 0 ) const {
      return fixed< Sign_, Length_, Shift_ >(
        static_cast< typename int_type< Sign_, boost::mpl::max< Length, Length_ >::value >::type >( base ) << Shift_::value - Shift::value, raw()
      );
    }
    template< typename Sign_, typename Length_, typename Shift_ >
    inline constexpr fixed< Sign_, Length_, Shift_ > cast( typename boost::enable_if< boost::mpl::greater_equal< Shift, Shift_ > >::type* = 0 ) const {
      return fixed< Sign_, Length_, Shift_ >(
        static_cast< typename int_type< Sign_, boost::mpl::max< Length, Length_ >::value >::type >( base ) >> ( Shift::value - Shift_::value ), raw()
      );
    }
    inline constexpr single_type abs_base() {
      return base >= 0 ? base : -base; 
    }
  };

  constexpr uint8_t sin_table[ 65u ] = {
    0,   6,  13,  19,  25,  31,  37,  44,
   50,  56,  62,  68,  74,  80 , 86,  92,
   98, 103, 109, 115, 120, 126, 131, 136,
  142, 147, 152, 157, 162, 167, 171, 176,
  180, 185, 189, 193, 197, 201, 205, 208,
  212, 215, 219, 222, 225, 228, 231, 233,
  236, 238, 240, 242, 244, 246, 247, 249,
  250, 251, 252, 253, 254, 254, 255, 255, 
  255
  };

  constexpr uint8_t asin_table[ 65u ] = {
     0,   2,   5,   7,  10,  12,  15,  17,
    20,  22,  25,  28,  30,  33,  35,  38,
    41,  43,  46,  48,  51,  54,  56,  59,
    62,  65,  67,  70,  73,  76,  79,  82,
    85,  87,  90,  93,  96, 100, 103, 106,
   109, 112, 116, 119, 123, 126, 130, 133,
   137, 141, 145, 149, 153, 158, 163, 167,
   172, 178, 184, 190, 197, 205, 214, 226,
   255
  };

  constexpr uint8_t exp_table[ 65u ] = {
     0,   2,   5,   7,  10,  12,  15,  17,
    20,  22,  25,  28,  31,  34,  36,  39,
    42,  45,  48,  51,  55,  58,  61,  64,
    68,  71,  75,  78,  82,  85,  89,  93,
    97, 101, 104, 108, 112, 117, 121, 125,
   129, 134, 138, 143, 147, 152, 157, 162,
   166, 171, 176, 182, 187, 192, 197, 203,
   208, 214, 220, 226, 231, 237, 244, 250,
   255,
  };
  constexpr uint8_t log_table[ 65u ] = {
     0,   5,  11,  16,  22,  27,  33,  38,
    43,  48,  53,  58,  63,  68,  73,  77,
    82,  87,  91,  96, 100, 105, 109, 113,
   117, 122, 126, 130, 134, 138, 142, 146,
   150, 153, 157, 161, 165, 168, 172, 176,
   179, 183, 186, 190, 193, 197, 200, 203,
   207, 210, 213, 216, 220, 223, 226, 229,
   232, 235, 238, 241, 244, 247, 250, 253,
   255
  };
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > mul_lower(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    /*return x * y*/;
    return fixed< Sign, Length, Shift >(
      int32_t( int32_t( x.base >> 1 ) * int32_t( y.base ) ) >> ( Shift::value - 1 ), raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > div_lower(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return fixed< Sign, Length, Shift >(
      ( ( x.base << ( Shift::value - 2 ) ) / y.base ) << 2, raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > operator*(
    const fixed< Sign, Length, Shift > &x,
    const typename fixed< Sign, Length, Shift >::single_type &y
  ) {
   /* return x * fixed< Sign, Length, Shift >( y );*/
    return fixed< Sign, Length, Shift >(
      x.base * y, raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > operator*(
    const typename fixed< Sign, Length, Shift >::single_type &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    /*return fixed<  Sign, Length, Shift  >( x ) * y;*/
    return fixed< Sign, Length, Shift >(
      x * y.base, raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > operator/(
    const fixed< Sign, Length, Shift > &x,
    const typename fixed< Sign, Length, Shift >::single_type &y
  ) {
    return fixed< Sign, Length, Shift >(
      x.base / y, raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > loop(  const fixed< Sign, Length, Shift > &x ) {
    return fixed< Sign, Length, Shift >( x.base & ( fixed< Sign, Length, Shift >::one() - 1u ), raw() );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > floor(  const fixed< Sign, Length, Shift > &x ) {
    return fixed< Sign, Length, Shift >( x.base & ~( fixed< Sign, Length, Shift >::one() - 1u ), raw() );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > round(  const fixed< Sign, Length, Shift > &x ) {
    return ( loop( x ).base < ( fixed< Sign, Length, Shift >::one() >> 1 ) ) ?
      floor( x ) :
      floor( x ) + fixed< Sign, Length, Shift >( 1 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > ceil(  const fixed< Sign, Length, Shift > &x ) {
    return ( loop( x ).base ) ?
      floor( x ) + fixed< Sign, Length, Shift >( 1 ) :
      floor( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > trunc(  const fixed< Sign, Length, Shift > &x ) {
    return ( loop( x ).base && x.base < 0 ) ?
      floor( x ) + fixed< Sign, Length, Shift >( 1 ) :
      floor( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fmod(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x - trunc( x / y ) * y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > remainder(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x - round( x / y ) * y;
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > remquo(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y,
    int *quot
  ) {
    *quot = round( x / y );
    return x - round( x / y ) * y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline long long int llround(
    const fixed< Sign, Length, Shift > &x
  ) {
    return round( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline long int lround(
    const fixed< Sign, Length, Shift > &x
  ) {
    return round( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > rectangle( const fixed< Sign, Length, Shift > &x, const fixed< Sign, Length, Shift > &duty ) {
    return ( x < duty ) ?
      fixed< Sign, Length, Shift >( 1 ) :
      fixed< Sign, Length, Shift >( 0 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > copysign( const fixed< Sign, Length, Shift > &x, const fixed< Sign, Length, Shift > &y ) {
    return ( y < fixed< Sign, Length, Shift >( 0 ) ) ?
      -abs( x ) :
      abs( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > triangle(
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::greater_equal< boost::mpl::minus< Length, Shift >, boost::mpl::int_< 2u > > >::type* = 0
  ) {
    return ( fixed< Sign, Length, Shift >( 1 ) > ( loop( x ) << 2 ) ) ?
      ( loop( x ) << 2 ):
      (
        ( fixed< Sign, Length, Shift >( 3 ) > ( loop( x ) << 2 ) ) ?
        -( loop( x ) << 2 ) + fixed< Sign, Length, Shift >( 2 ):
        ( loop( x ) << 2 ) - fixed< Sign, Length, Shift >( 4 )
      );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_positive_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ triangle( x ).base >> ( Shift::value - 6 ) ]
    ) << ( Shift::value - 8 ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_positive_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( triangle( x ).base >> ( Shift::value - 6 ) ) + 1 ]
    ) << ( Shift::value - 8 ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_negative_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    return ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ -( triangle( x ).base >> ( Shift::value - 6 ) ) - 1 ]
    ) << ( Shift::value - 8 ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_negative_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    return ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ -( triangle( x ).base >> ( Shift::value - 6 ) ) ]
    ) << ( Shift::value - 8 ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_sine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      ( triangle( x ).base >= 0 ) ?
      (
        fetch_from_sine_table_positive_lower( table, x ) + ( fetch_from_sine_table_positive_higher( table, x ) - fetch_from_sine_table_positive_lower( table, x ) ) * ( triangle( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) )
      ) :
      (
        fetch_from_sine_table_negative_higher( table, x ) + ( fetch_from_sine_table_negative_lower( table, x ) - fetch_from_sine_table_negative_higher( table, x ) ) * ( triangle( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) )
      ),
      raw()
    );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_positive_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ triangle( x ).base >> ( Shift::value - 6 ) ]
    ) >> ( 8 - Shift::value ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_positive_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( triangle( x ).base >> ( Shift::value - 6 ) ) + 1 ]
    ) >> ( 8 - Shift::value ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_negative_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ -( triangle( x ).base >> ( Shift::value - 6 ) ) - 1 ]
    ) >> ( 8 - Shift::value ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_negative_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ -( triangle( x ).base >> ( Shift::value - 6 ) ) ]
    ) >> ( 8 - Shift::value ) ) - 1;
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_sine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      ( triangle( x ).base >= 0 ) ?
      (
        fetch_from_sine_table_positive_lower( table, x ) + ( fetch_from_sine_table_positive_higher( table, x ) - fetch_from_sine_table_positive_lower( table, x ) ) * ( triangle( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) )
      ) :
      (
        fetch_from_sine_table_negative_lower( table, x ) + ( fetch_from_sine_table_negative_lower( table, x ) - fetch_from_sine_table_negative_higher( table, x ) ) * ( triangle( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) )
      ),
      raw()
    );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_positive(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ triangle( x ).base << ( 6 - Shift::value ) ]
    ) >> ( 8u - Shift::value );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_sine_table_negative(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    return -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ -triangle( x ).base << ( 6 - Shift::value ) ]
    ) >> ( 8u - Shift::value );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_sine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      ( triangle( x ).base > 0 ) ?
      fetch_from_sine_table_positive( table, x ) :
      fetch_from_sine_table_negative( table, x ),
      raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > sine(
    const fixed< Sign, Length, Shift > &x
  ) {
    return fetch_from_sine_table( sin_table, x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > cosine(
    const fixed< Sign, Length, Shift > &x
  ) {
    return sine( x + ( fixed< Sign, Length, Shift >( 1 ) >> 2 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > tangent(
    const fixed< Sign, Length, Shift > &x
  ) {
    return sine( x )/cosine( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > sin(
    const fixed< Sign, Length, Shift > &x
  ) {
    return sine( x / fixed< Sign, Length, Shift >( pi * 2.L ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > cos(
    const fixed< Sign, Length, Shift > &x
  ) {
    return cosine( x / fixed< Sign, Length, Shift >( pi * 2.L ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > tan(
    const fixed< Sign, Length, Shift > &x
  ) {
    return tangent( x / fixed< Sign, Length, Shift >( pi * 2.L ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > abs(
    const fixed< Sign, Length, Shift > &x
  ) {
    return ( x >= fixed< Sign, Length, Shift >( 0 ) ) ? x : -x;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fabs(
    const fixed< Sign, Length, Shift > &x
  ) {
    return abs( x );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > fetch_from_arcsine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > >
    >::type* = 0
  ) {
    fixed< Sign, Length, Shift > temp;
    const auto fragment_max = 1 << ( Shift::value - 6 );
    const auto fragment = x.base & ( fragment_max - 1 );
    const auto index = x.base >> ( Shift::value - 6 );
    if( x >= fixed< Sign, Length, Shift >( 1 ) )
      temp.base = fixed< Sign, Length, Shift >::one();
    else if( x <= fixed< Sign, Length, Shift >( -1 ) )
      temp.base = -fixed< Sign, Length, Shift >::one();
    else if( x.base >= 0 ) {
      const auto a =
        ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ index ]
        ) << ( Shift::value - 8 ) );
      const auto b =
        ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ index + 1 ]
        ) << ( Shift::value - 8 ) );
      temp.base = a + ( b - a ) * fragment / fragment_max;
    }
    else {
      const auto a =
        ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ -index - 1 ]
        ) << ( Shift::value - 8 ) );
      const auto b =
        ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ -index ]
        ) << ( Shift::value - 8 ) ) - 1;
      temp.base = b - ( b - a ) * fragment / fragment_max;
    }
    return temp;
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > fetch_from_arcsine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    fixed< Sign, Length, Shift > temp;
    const auto fragment_max = 1 << ( Shift::value - 6 );
    const auto fragment = x.base & ( fragment_max - 1 );
    const auto index = x.base >> ( Shift::value - 6 );
    if( abs( x ) > fixed< Sign, Length, Shift >( 1 ) )
      temp.base = fixed< Sign, Length, Shift >::max();
    else if( x.base >= 0 ) {
      const auto a =
        ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ index ]
        ) << ( 8 - Shift::value ) );
      const auto b =
        ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ index + 1 ]
        ) << ( 8 - Shift::value ) );
      temp.base = a + ( b - a ) * fragment / fragment_max;
    }
    else {
      const auto a =
        ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ -index - 1 ]
        ) << ( 8 - Shift::value ) );
      const auto b =
        ( -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ -index ]
        ) << ( 8 - Shift::value ) ) - 1;
      temp.base = b - ( b - a ) * fragment / fragment_max;
    }
    return temp;
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > fetch_from_arcsine_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    fixed< Sign, Length, Shift > temp;
    if( abs( x ) > fixed< Sign, Length, Shift >( 1 ) )
      temp.base = fixed< Sign, Length, Shift >::max();
    else if( x.base >= 0 )
      temp.base =
        static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ x.base << ( 6 - Shift::value ) ]
        ) >> ( 8u - Shift::value );
    else 
      temp.base =
        -static_cast< typename fixed< Sign, Length, Shift >::single_type >(
          table[ -x.base << ( 6 - Shift::value ) ]
        ) >> ( 8u - Shift::value );
    return temp;
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > arcsine(
    const fixed< Sign, Length, Shift > &x
  ) {
    return fetch_from_arcsine_table( asin_table, x );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > arccosine(
    const fixed< Sign, Length, Shift > &x
  ) {
    return -arcsine( x ) + fixed< Sign, Length, Shift >( 1 );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > asin(
    const fixed< Sign, Length, Shift > &x
  ) {
    return arcsine( x ) * fixed< Sign, Length, Shift > ( pi / 2.L );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > acos(
    const fixed< Sign, Length, Shift > &x
  ) {
    return arccosine( x ) * fixed< Sign, Length, Shift > ( pi / 2.L );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > atan(
    const fixed< Sign, Length, Shift > &x
  ) {
    return asin( x / sqrt( fixed< Sign, Length, Shift >( 1 ) + x * x ) );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > arctangent2(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    if( x > fixed< Sign, Length, Shift >( 0 ) )
      return arctangent( x / y );
    else if( x < fixed< Sign, Length, Shift >( 0 ) ) {
      if( y >= fixed< Sign, Length, Shift >( 0 ) )
        return arctangent( x/y ) + ( fixed< Sign, Length, Shift >( 0 ) >> 1 );
      else
        return arctangent( x/y ) - ( fixed< Sign, Length, Shift >( 0 ) >> 1 );
    }
    else {
      if( y >= fixed< Sign, Length, Shift >( 0 ) )
        return fixed< Sign, Length, Shift >( 0 ) >> 2;
      else
        return -fixed< Sign, Length, Shift >( 0 ) >> 2;
    }
  }
  template< typename Sign, typename Length, typename Shift, typename T >
  inline fixed< Sign, Length, Shift > pow(
    const fixed< Sign, Length, Shift > &x,
    T y,
    typename boost::enable_if< boost::is_integral< T > >::type* = 0
  ) {
    fixed< Sign, Length, Shift > temp( 1 );
    if( y >= 0 )
      for( unsigned int count = 0; count != static_cast<unsigned int>( y ); ++count )
        temp *= x;
    else
      for( unsigned int count = 0; count != static_cast<unsigned int>( -y ); ++count )
        temp /= x;
    return temp;
  } 
  template< typename Sign, typename Length, typename Shift, typename T >
  constexpr inline fixed< Sign, Length, Shift > pow(
    const fixed< Sign, Length, Shift > &x,
    T y,
    typename boost::enable_if< boost::is_float< T > >::type* = 0
  ) {
    return pow( x, fixed< Sign, Length, Shift >( y ) );
  } 
  template< typename Table, typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > pow(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    if( y == floor( y ) )
      return pow( x, static_cast< int >( y ) );
    else
      return pow( x, static_cast< float >( y ) );
  } 
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_exp_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      static_cast< typename fixed< Sign, Length, Shift >::single_type >(
        table[ loop( x ).base << ( 6 - Shift::value ) ]
      ) >> ( 8u - Shift::value ), raw()
    );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_exp_table_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ loop( x ).base >> ( Shift::value - 6 ) ]
    ) >> ( 8u - Shift::value );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_exp_table_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( loop( x ).base >> ( Shift::value - 6 ) ) + 1 ]
    ) >> ( 8u - Shift::value );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_exp_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      fetch_from_exp_table_lower( table, x ) + ( fetch_from_exp_table_higher( table, x ) - fetch_from_exp_table_lower( table, x ) ) * ( loop( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) ),
      raw()
    );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_exp_table_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
        table[ loop( x ).base >> ( Shift::value - 6 ) ]
      ) << ( Shift::value - 8 );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_exp_table_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( loop( x ).base >> ( Shift::value - 6 ) ) + 1 ]
    ) << ( Shift::value - 8 );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_exp_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      fetch_from_exp_table_lower( table, x ) + ( fetch_from_exp_table_higher( table, x ) - fetch_from_exp_table_lower( table, x ) ) * ( loop( x ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) ),
      raw()
    );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > exp(
    const fixed< Sign, Length, Shift > &x
  ) {
    return (
      fetch_from_exp_table( exp_table, x ) *
      ( fixed< Sign, Length, Shift >( e ) - fixed< Sign, Length, Shift >( 1 ) ) +
      fixed< Sign, Length, Shift >( 1 )
    ) *
    pow(
      fixed< Sign, Length, Shift >( e ),
      static_cast< typename fixed< Sign, Length, Shift >::single_type >( floor( x ) )
    );
  }
  template< typename Sign, typename Length, typename Shift >  
  inline fixed< Sign, Length, Shift > frexp(
    const fixed< Sign, Length, Shift > &significand,
    int *exponent_
  ) {
    fixed< Sign, Length, Shift > temp = significand;
    int exponent = 0;
    if( temp > fixed< Sign, Length, Shift >( 1 ) )
      while( temp >= fixed< Sign, Length, Shift >( 1 ) ) {
        temp >>= 1;
        ++exponent;
      }
    else
      while( temp < ( fixed< Sign, Length, Shift >( 1 ) >> 1 ) ) {
        temp <<= 1;
        --exponent;
      }
    *exponent_ = exponent;
    return temp;
  }
  template< typename Sign, typename Length, typename Shift >  
  constexpr inline fixed< Sign, Length, Shift > ldexp(
    const fixed< Sign, Length, Shift > &significand,
    int exponent
  ) {
    return ( exponent > 0 ) ? ( significand << exponent ) : ( significand >> -exponent );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_log_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if< boost::mpl::less< Shift, boost::mpl::int_< 6u > > >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base << ( 6 - Shift::value ) ]
    ) >> ( 8u - Shift::value ), raw() );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_log_table_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base >> ( Shift::value - 6 ) ]
    ) >> ( 8u - Shift::value ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_log_table_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return ( static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base >> ( Shift::value - 6 ) ) + 1 ]
    ) >> ( 8u - Shift::value ) );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_log_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if<
      boost::mpl::and_<
        boost::mpl::greater_equal< Shift, boost::mpl::int_< 6u > >,
        boost::mpl::less< Shift, boost::mpl::int_< 8u > >
      >
    >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      fetch_from_log_table_lower( table, x_ ) + ( fetch_from_log_table_higher( table, x_ ) - fetch_from_log_table_lower( table, x_ ) ) * ( ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) ), raw()
    );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_log_table_lower(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base >> ( Shift::value - 6 ) ]
    ) << ( Shift::value - 8 );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline typename fixed< Sign, Length, Shift >::single_type fetch_from_log_table_higher(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return static_cast< typename fixed< Sign, Length, Shift >::single_type >(
      table[ ( ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base >> ( Shift::value - 6 ) ) + 1 ] 
    ) << ( Shift::value - 8 );
  }
  template< typename Table, typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fetch_from_log_table(
    const Table &table,
    const fixed< Sign, Length, Shift > &x_,
    typename boost::enable_if< boost::mpl::greater_equal< Shift, boost::mpl::int_< 8u > > >::type* = 0
  ) {
    return fixed< Sign, Length, Shift >(
      fetch_from_log_table_lower( table, x_ ) + ( fetch_from_log_table_higher( table, x_ ) - fetch_from_log_table_lower( table, x_ ) ) * ( ( x_ - fixed< Sign, Length, Shift >( 1 ) ).base & ( ( 1 << ( Shift::value - 6 ) ) - 1 ) ) / ( 1 << ( Shift::value - 6 ) ),
      raw()
    ) * ( fixed< Sign, Length, Shift >( 177 )/fixed< Sign, Length, Shift >( 255 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > log( const fixed< Sign, Length, Shift > &x_ ) {
    return fetch_from_log_table( log_table, x_.shift( -ilogb( x_ ) ) ) + fixed< Sign, Length, Shift >( ilogb( x_ ) ) * fixed< Sign, Length, Shift >( ln2 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > log(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return log( y ) / log( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > log10(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( x ) / fixed< Sign, Length, Shift >( ln10 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > log2(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( x ) / fixed< Sign, Length, Shift >( ln2 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > pow(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return exp( y * log( x ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > sqrt(
    const fixed< Sign, Length, Shift > &x
  ) {
    return exp( log( x ) >> 1 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > cbrt(
    const fixed< Sign, Length, Shift > &x
  ) {
    return exp( log( x ) / fixed< Sign, Length, Shift >( 3 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > hypot(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return exp( log( x * x + y * y ) >> 1 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > exp2(
    const fixed< Sign, Length, Shift > &y
  ) {
    return exp( y * fixed< Sign, Length, Shift >( ln2 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > exp10(
    const fixed< Sign, Length, Shift > &y
  ) {
    return exp( y * fixed< Sign, Length, Shift >( ln10 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > sinh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return ( exp( x ) - exp( -x ) )/fixed< Sign, Length, Shift >( 2 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > cosh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return ( exp( x ) + exp( -x ) )/fixed< Sign, Length, Shift >( 2 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > tanh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return sinh( x )/cosh( x );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > asinh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( x + sqrt( x * x + fixed< Sign, Length, Shift >( 1 ) ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > acosh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( x + sqrt( x * x - fixed< Sign, Length, Shift >( 1 ) ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > atanh(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( ( fixed< Sign, Length, Shift >( 1 ) + x )/( fixed< Sign, Length, Shift >( 1 ) - x ) )/fixed< Sign, Length, Shift >( 2 );
  }
  template< typename Sign, typename Length, typename Shift >
  inline fixed< Sign, Length, Shift > modf(
    const fixed< Sign, Length, Shift > &x,
    fixed< Sign, Length, Shift > *intpart
  ) {
    const fixed< Sign, Length, Shift > fraction( x.base & ( fixed< Sign, Length, Shift >::one() - 1 ), raw() );
    intpart->base = x.base & ~( fixed< Sign, Length, Shift >::one() - 1 );
    return fraction;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > expm1(
    const fixed< Sign, Length, Shift > &x
  ) {
    return exp( x ) - fixed< Sign, Length, Shift >( 1 );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int ilogb_right( const fixed< Sign, Length, Shift > &x ) {
    return ( x >= fixed< Sign, Length, Shift >( 2 )  ) ?
      ( ilogb_right( x >> 1 ) + 1 ) : 0; 
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int ilogb_left( const fixed< Sign, Length, Shift > &x ) {
    return ( x < fixed< Sign, Length, Shift >( 1 )  ) ?
      ( ilogb_left( x << 1 ) - 1 ) : 0; 
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int ilogb( const fixed< Sign, Length, Shift > &x ) {
    return ( abs( x ) == fixed< Sign, Length, Shift >( 0 ) ) ?
      0 : (
        ( abs( x ) >= fixed< Sign, Length, Shift >( 2 ) ) ?
        ilogb_right( abs( x ) ) : ilogb_left( abs( x ) )
      );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int silogb_right( const fixed< Sign, Length, Shift > &x ) {
    return ( x >= fixed< Sign, Length, Shift >( 2 )  ) ?
      ( silogb_right( x >> 1 ) + 1 ) : 0; 
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int silogb_left( const fixed< Sign, Length, Shift > &x ) {
    return ( x < fixed< Sign, Length, Shift >( 1 )  ) ?
      ( silogb_right( x << 1 ) - 1 ) : 0; 
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline int silogb( const fixed< Sign, Length, Shift > &x ) {
    return ( x == fixed< Sign, Length, Shift >( 0 ) ) ?
      0 : (
        ( x >= fixed< Sign, Length, Shift >( 2 ) ) ?
        silogb_right( x ) : silogb_left( x )
      );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > log1p(
    const fixed< Sign, Length, Shift > &x
  ) {
    return log( x + fixed< Sign, Length, Shift >( 1 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > logb(
    const fixed< Sign, Length, Shift > &x
  ) {
    return fixed< Sign, Length, Shift >( ilogb( x ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > scalbn(
    const fixed< Sign, Length, Shift > &x,
    int value
  ) {
    return ( value >= 0 ) ? ( x << value ) : ( x >> value );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > scalbln(
    const fixed< Sign, Length, Shift > &x,
    long int value
  ) {
    return ( value >= 0l ) ? ( x << value ) : ( x >> value );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > nextafter(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return ( x < y )  ? fixed< Sign, Length, Shift >( x.base + 1, raw() ) : fixed< Sign, Length, Shift >( x.base - 1, raw() );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > nexttoward(
    const fixed< Sign, Length, Shift > &x,
    long double y
  ) {
    return ( static_cast< long double >( x ) < y )  ? fixed< Sign, Length, Shift >( x.base + 1, raw() ) : fixed< Sign, Length, Shift >( x.base - 1, raw() );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > max(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x > y ? x : y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fmax(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x > y ? x : y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > min(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x < y ? x : y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fmin(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return x < y ? x : y;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > dim(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y
  ) {
    return max( x - y, fixed< Sign, Length, Shift >( 0 ) );
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > ma(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y,
    const fixed< Sign, Length, Shift > &z
  ) {
    return x * y + z;
  }
  template< typename Sign, typename Length, typename Shift >
  constexpr inline fixed< Sign, Length, Shift > fma(
    const fixed< Sign, Length, Shift > &x,
    const fixed< Sign, Length, Shift > &y,
    const fixed< Sign, Length, Shift > &z
  ) {
    return ma( x, y, z );
  }
}
#endif


