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
#ifndef TINYFM2_ADPCM_DECODER_HPP
#define TINYFM2_ADPCM_DECODER_HPP

#include <algorithm>
#include <tinyfm2/adpcm/adpcm.hpp>

template< typename InputIterator >
class bit_unpacker {
public:
  bit_unpacker( const InputIterator &begin, const InputIterator &end_ ) : iter( begin ), end( end_ ), temp( 0u ), pos( 0 ) {}
  adaptive_difference_t pop( unsigned int length ) {
    adaptive_difference_t value = 0u;
    for( unsigned int index = 0u; index != length; ++index ) {
      value <<= 1;
      value |= ( pop() ? 1 : 0 );
    }
    return value;
  }
  bool is_end() const {
    return iter == end && pos == 0;
  }
private:
  bool pop() {
    if( pos == 0 ) {
      if( iter != end ) {
        temp = *iter;
        ++iter;
        pos = 32;
      }
      else
        return false;
    }
    return temp & ( 1 << --pos );
  }
  InputIterator iter;
  InputIterator end;
  compressed_t temp;
  int pos;
};

template< typename InputIterator >
class uncompress {
public:
  uncompress( const InputIterator &iter, const InputIterator &end ) : current_mode( 0 ), unpacker( iter, end ) {}
  adaptive_difference_t operator()() {
    int value = parse_compressed_sample();
    if( value == 15 ) {
      const int switch_to = unpacker.pop( 2 );
      current_mode = switch_to;
      value = parse_compressed_sample();
    }
    return value;
  }
  bool is_end() const {
    return unpacker.is_end();
  }
private:
  int parse_compressed_sample() {
    const auto raw_value = unpacker.pop( current_mode + 1 );
    if( current_mode == 0 )
      return ( raw_value << 3 ) + 7;
    else if( current_mode == 1 )
      return ( raw_value == 3 ) ? 15 : raw_value + 6;
    else if( current_mode == 2 )
      return ( raw_value == 7 ) ? 15 : raw_value + 4;
    else
      return raw_value;
  }
  int current_mode;
  bit_unpacker< InputIterator > unpacker;
};

class adpcm_decoder {
public:
  adpcm_decoder() : prev( 0 ), tangent( 0 ) {}
  raw_sample_t operator()( adaptive_difference_t value ) {
    const int diff = decode_table[ value ];
    const int estimated = prev + tangent;
    const int decoded = std::min( std::max( estimated + diff, -127 ), 127 );
    tangent = decoded - prev;
    prev = decoded;
    return decoded;
  }
private:
  int prev;
  int tangent;
};

template< typename InputIterator, typename OutputIterator >
void decode(
  const InputIterator &iter,
  const InputIterator &end,
  size_t length,
  const OutputIterator &out_
) {
  OutputIterator out = out_;
  uncompress< InputIterator > uncomp( iter, end );
  adpcm_decoder decoder;
  for( size_t index = 0u; index != length && !uncomp.is_end(); ++index ) {
    *out = decoder( uncomp() );
    ++out;
  }
}

#endif

