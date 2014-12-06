/********************************************************************************
 * Copyright (c) 2013 Naomasa Matsubayashi
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

#include "config.hpp"
#include <cstdint>

const uint8_t conf[72] = {
  255u, 0u, 128u, 0u,
  255u, 2u, 64u, 64u, 32u, 32u,
  128u, 32u, 0u,
  0u, 128u, 0u, 0u,
  0x00u, 0x08u, 0u, 0u,
  255u, 8u, 8u, 255u, 16u, 128u,
  0u, 64u, 0u,
  0u, 0u, 0u, 0u,
  0x40u, 0x08u, 0u, 0u,
  255u, 4u, 32u, 255u, 16u, 128u,
  128u, 32u, 0u,
  0u, 0u, 0u, 128u,
  0x02u, 0x08u, 0u, 0u,
  255u, 4u, 32u, 255u, 16u, 128u,
  0u, 64u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x08u, 0u, 0u
};

const uint8_t piano[72] = {
  0u, 0u, 0u, 255u,
  240u, 0u, 40u, 128u, 32u, 0u,
  0u, 0u, 0u,
  100u, 0u, 0u, 0u,
  0x00u, 0x10u, 0u, 0u,
  240u, 0u, 120u, 128u, 16u, 0u,
  0u, 0u, 0u,
  160u, 0u, 0u, 0u,
  0x01u, 0x08u, 0u, 0u,
  240u, 0u, 80u, 40u, 16u, 0u,
  0u, 0u, 0u,
  0u, 160u, 20u, 0u,
  0x00u, 0x10u, 0u, 0u,
  240u, 0u, 64u, 80u, 20u, 40u,
  0u, 0u, 0u,
  0u, 80u, 0u, 80u,
  0x00u, 0x08u, 0u, 0u
};
const uint8_t epiano[72] = {
  127u, 127u, 0u, 0u,
  240u, 0u, 32u, 64u, 8u, 16u,
  0u, 0u, 0u,
  0u, 0u, 120u, 120u,
  0x00u, 0x04u, 0u, 0u,
  240u, 0u, 10u, 32u, 8u, 16u,
  0u, 0u, 0u,
  0u, 0u, 120u, 120u,
  0x01u, 0x04u, 0u, 0u,
  240u, 0u, 64u, 0u, 32u, 0u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x01u, 0x20u, 0u, 0u,
  240u, 0u, 40u, 0u, 32u, 0u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x40u, 0u, 0u
};
const uint8_t organ[72] = {
  63u, 63u, 63u, 63u,
  180u, 2u, 128u, 0u, 4u, 250u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x08u, 0u, 0u,
  180u, 2u, 16u, 0u, 4u, 250u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x04u, 0u, 0u,
  180u, 2u, 64u, 0u, 4u, 250u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x10u, 0u, 0u,
  180u, 2u, 32u, 0u, 4u, 250u,
  0u, 0u, 0u,
  0u, 0u, 0u, 0u,
  0x00u, 0x20u, 0u, 0u
};
const uint8_t sax[72] = {
  0u, 0u, 127u, 127u,
  
  240u, 0u, 80u, 0u, 4u, 0u,
  0u, 0u, 0u,
  0u, 200u, 0u, 0u,
  0x01u, 0x04u, 0u, 0u,
  
  240u, 0u, 40u, 0u, 4u, 0u,
  0u, 0u, 0u,
  100u, 100u, 0u, 0u,
  0x00u, 0x08u, 0u, 0u,
  
  240u, 2u, 80u, 64u, 4u, 64u,
  0u, 0u, 0u,
  120u, 40u, 0u, 0u,
  0x00u, 0x08u, 0u, 0u,
  
  240u, 1u, 100u, 32u, 4u, 0u,
  0u, 0u, 0u,
  120u, 40u, 0u, 0u,
  0x01u, 0x10u, 0u, 0u,
};
const uint8_t guiter[72] = {
  200u, 0u, 0u, 0u,
  127u, 0u, 80u, 0u, 80u, 0u,
  0u, 0u, 0u,
  0u, 64u, 96u, 64u,
  0x00u, 0x08u, 0u, 0u,
  255u, 0u, 16u, 0u, 80u, 0u,
  0u, 0u, 0u,
  0u, 200u, 30u, 0u,
  0x00u, 0x10u, 0u, 0u,
  255u, 0u, 40u, 0u, 80u, 0u,
  0u, 0u, 0u,
  0u, 0u, 40u, 0u,
  0x00u, 0x08u, 0u, 0u,
  255u, 0u, 100u, 0u, 80u, 16u,
  0u, 0u, 0u,
  0u, 0u, 100u, 100u,
  0x01u, 0x08u, 0u, 0u
};
const uint8_t strings[72] = {
  127u, 0u, 0u, 127u,
  160u, 0u, 128u, 0u, 4u, 32u,
  0u, 0u, 0u,
  0u, 127u, 127u, 0u,
  0x00u, 0x08u, 0u, 0u,
  255u, 0u, 16u, 0u, 4u, 32u,
  0u, 0u, 0u,
  0u, 0u, 32u, 0u,
  0x00u, 0x10u, 0u, 0u,
  255u, 0u, 16u, 0u, 4u, 32u,
  0u, 0u, 0u,
  0u, 32u, 0u, 0u,
  0x00u, 0x04u, 0u, 0u,
  160u, 0u, 32u, 0u, 4u, 32u,
  0u, 0u, 0u,
  0u, 127u, 127u, 0u,
  0x00u, 0x30u, 0u, 0u
};
const uint32_t wave_table[11] = {
  2068443146, 1118053233, 3962086855, 2963445534, 3364923672, 45355, 321347218, 3407706699, 745924812, 1254457344
};

