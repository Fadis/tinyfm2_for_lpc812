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
#ifndef TINYFM2_SMF_PARSER_HPP
#define TINYFM2_SMF_PARSER_HPP

#include <cstdint>
#include <algorithm>

namespace tinyfm2 {
  template< typename Iterator >
  class smf_parser {
  public:
    enum class event_type {
      sleep,
      note_on,
      note_off,
      pitch_bend,
      program_change
    };
    enum class parser_state {
      delta_time,
      event
    };
    enum class midi_event {
      note_off = 0x80,
      note_on = 0x90,
      polyphonic_pressure = 0xA0,
      control_change = 0xB0,
      program_change = 0xC0,
      channel_pressure = 0xD0,
      pitch_bend = 0xE0,
      meta_event = 0xF0
    };
    enum class midi_meta_event {
      track_end = 0x2F,
      tempo = 0x51
    };
    struct event {
      constexpr event( const event_type &type_, uint32_t param_ ) : type( type_ ), param( param_ ) {}
      event( const event& ) = default;
      event( event&& ) = default;
      event &operator=( const event& ) = default;
      event &operator=( event&& ) = default;
      event_type type;
      uint32_t param;
    };
    smf_parser( const Iterator &begin, const Iterator &end_ ) : cur( begin ), end( end_ ), state( parser_state::delta_time ), tempo( 120 ), current_event( event_type::sleep, 0x0000 ), broken( false ) {
      static const uint8_t magic[]= { 0x4d, 0x54, 0x68, 0x64, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01 };
      if( std::equal( magic, magic + 12, cur ) ) {
        std::advance( cur, 12 );
        division = *cur << 8;
        ++cur;
        division |= *cur;
        ++cur;
        static const uint8_t track_magic[]= { 0x4d, 0x54, 0x72, 0x6b };
        if( std::equal( track_magic, track_magic + 4, cur ) ) {
          std::advance( cur, 4 );
          uint32_t length = ( *cur << 24 );
          ++cur;
          length |= *cur << 16;
          ++cur;
          length |= *cur << 8;
          ++cur;
          length |= *cur;
          ++cur;
          end = std::next( cur, length );
        }
        else {
          end = cur;
          broken = true;
        }
      }
      else {
        end = cur;
        broken = true;
      }
    }
    void next() {
      current_event = read_next_event(); 
    }
    event_type get_event_type() const {
      return current_event.type;
    }
    uint32_t get_length() const {
      return tempo * current_event.param / division;
    }
    uint8_t get_velocity() const {
      return current_event.param & 0xFF;
    }
    uint8_t get_scale() const {
      return ( current_event.param >> 8 ) & 0xFF;
    }
    uint8_t get_track() const {
      return ( current_event.param >> 16 ) & 0xF;
    }
    int16_t get_pitch() const {
      return ( current_event.param >= 0x8000u ) ? -( 0x10000 - int32_t( current_event.param ) ): int32_t( current_event.param );
    }
    bool is_end() const {
      return cur == end;
    }
    bool is_broken() const {
      return broken;
    }
  private:
    event read_next_event() {
      if( cur == end ) {
        return event( event_type::sleep, 0xFFFF );
      }
      else if( state == parser_state::delta_time ) {
        uint32_t length = 0;
        for( ; cur != end; ++cur ) {
          length <<= 7;
          length |= *cur & 0x7F;
          if( !( *cur & 0x80 ) ) {
            ++cur;
            break;
          }
        }
        state = parser_state::event;
        if( length == 0 )
          return read_next_event();
        else
          return event( event_type::sleep, length );
      }
      else {
        if( *cur >= 0x80 ) {
          status = *cur;
          ++cur;
        }
        switch( midi_event( status & 0xF0 ) ) {
        case midi_event::note_off:
          {
            uint32_t param = ( status & 0x0F ) << 16;
            param |= *cur << 8;
            std::advance( cur, 2 );
            state = parser_state::delta_time;
            return event( event_type::note_off, param );
          }
        case midi_event::note_on:
          {
            uint32_t param = ( status & 0x0F ) << 16;
            param |= *cur << 8;
            ++cur;
            if( *cur != 0 ) {
              param |= *cur;
              ++cur;
              state = parser_state::delta_time;
              return event( event_type::note_on, param );
            }
            else {
              ++cur;
              state = parser_state::delta_time;
              return event( event_type::note_off, param );
            }
          }
        case midi_event::polyphonic_pressure:
          std::advance( cur, 2 );
          state = parser_state::delta_time;
          return read_next_event();
        case midi_event::control_change:
          if( *cur == 0x7E )
            ++cur;
          std::advance( cur, 2 );
          state = parser_state::delta_time;
          return read_next_event();
        case midi_event::program_change:
          {
            uint32_t inst = *cur;
            ++cur;
            state = parser_state::delta_time;
            return event( event_type::program_change, inst );
          }
        case midi_event::channel_pressure:
          ++cur;
          state = parser_state::delta_time;
          return read_next_event();
        case midi_event::pitch_bend:
          {
            uint32_t pitch = *cur;
            ++cur;
            pitch |= ( uint32_t( *cur ) << 8 );
            ++cur;
            state = parser_state::delta_time;
            return event( event_type::pitch_bend, pitch );
          }
        case midi_event::meta_event:
          {
            switch( status ) {
            case 0xF0:
              {
                const uint8_t length = *cur;
                cur += length;
                state = parser_state::delta_time;
                return read_next_event();
              }
            case 0xF7:
              {
                const uint8_t length = *cur;
                cur += length;
                state = parser_state::delta_time;
                return read_next_event();
              }
            case 0xFF:
              {
                uint8_t meta_event = *cur;
                ++cur;
                const uint8_t length = *cur;
                ++cur;
                switch( midi_meta_event( meta_event ) ) {
                case midi_meta_event::track_end:
                  cur = end;
                  state = parser_state::delta_time;
                  return event( event_type::sleep, 0 );
                case midi_meta_event::tempo:
                  {
                    tempo = 0;
                    tempo |= uint32_t( *cur ) << 16;
                    ++cur;
                    tempo |= uint32_t( *cur ) << 8;
                    ++cur;
                    tempo |= uint32_t( *cur );
                    ++cur;
                    state = parser_state::delta_time;
                    return read_next_event();
                  }
                default:
                  cur += length;
                  state = parser_state::delta_time;
                  return read_next_event();
                }
              }
            default:
              invalid_sequence();
              return event( event_type::sleep, 0 );
            };
          }
        default:
          invalid_sequence();
          return event( event_type::sleep, 0 );
        };
      }
    }
  private:
    void invalid_sequence() {
      cur = end;
      broken = true;
    }
    uint16_t division;
    Iterator cur;
    Iterator end;
    parser_state state;
    uint8_t status;
    uint32_t tempo;
    event current_event;
    bool broken;
  };
}

#endif

