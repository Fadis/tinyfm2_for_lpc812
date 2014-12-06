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

#include <stdint.h>
#include "init_LPC8xx.h"

extern unsigned char __data_values__;
extern unsigned char __data_begin__;
extern unsigned char __data_end__;
extern unsigned char __bss_begin__;
extern unsigned char __bss_end__;
extern int __preinit_array_start;
extern int __preinit_array_end;
extern int __init_array_start;
extern int __init_array_end;

void run_preinit(void) {
  int *cur = &__preinit_array_start;
  for( ; cur < &__preinit_array_end; cur++ ) {
    void (*f)(void) = (void *)*cur;
    (*f)();
  }
}
void run_init(void) {
  int *cur = &__init_array_start;
  for( ; cur < &__init_array_end; cur++ ) {
    void (*f)(void) = (void *)*cur;
    (*f)();
  }
}

void init_data(void) {
  unsigned char *src = &__data_values__;
  unsigned char *dest = &__data_begin__;
  unsigned int len = &__data_end__ - &__data_begin__;
  while( len-- ) *dest++ = *src++;
}

void init_bss(void) {
  unsigned char *dest = &__bss_begin__;
  unsigned int len = &__bss_end__ - &__bss_begin__;
  while( len-- ) *dest++ = 0;
}

void _init() {}
void _fini() {}
