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

MEMORY {
  flash (rx) : ORIGIN = 0x00000000, LENGTH = 4K
  ram (rwx) : ORIGIN = 0x10000000, LENGTH = 1K
}
ENTRY(Reset_Handler)
SECTIONS {
  .text : {
    KEEP(*(.isr_vector))
    *(.text*)
    KEEP(*(.init))
    KEEP(*(.fini))
    *crtbegin.o(.ctors)
    *crtbegin?.o(.ctors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
    *(SORT(.ctors.*))
    *(.ctors)
    *crtbegin.o(.dtors)
    *crtbegin?.o(.dtors)
    *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
    *(SORT(.dtors.*))
    *(.dtors)
    *(.rodata*)
    KEEP(*(.eh_frame*))
  } > flash
  .ARM.extab : {
    *(.ARM.extab* .gnu.linkonce.armextab.*)
  } > flash
  __exidx_start = .;
  .ARM.exidx : {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } > flash
  __exidx_end = .;
  __etext__ = .;
  .data : AT(__etext__) {
    __data_values__ = LOADADDR(.data);
    __data_begin__ = .;
    *(vtable)
    *(.data*)
    . = ALIGN(4);
    KEEP(*(.preinit_array))
    . = ALIGN(4);
    KEEP(*(SORT(.init_array.*)))
    KEEP(*(.init_array))
    . = ALIGN(4);
    KEEP(*(SORT(.fini_array.*)))
    KEEP(*(.fini_array))
    . = ALIGN(4);
    __data_end__ = .;
  } > ram
  .bss : {
    __bss_begin__ = .;
    *(.bss*)
    *(COMMON)
    __bss_end__ = .;
  } > ram
  .heap : {
    __end__ = .;
    end = __end__;
    *(.heap*)
    __HeapLimit = .;
  } > ram
  .stack_dummy : {
    *(.stack)
  } > ram
}
