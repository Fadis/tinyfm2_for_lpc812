    .syntax unified
    .arch armv7-m

/* Memory Model
   The HEAP starts at the end of the DATA section and grows upward.
   
   The STACK starts at the end of the RAM and grows downward.
   
   The HEAP and stack STACK are only checked at compile time:
   (DATA_SIZE + HEAP_SIZE + STACK_SIZE) < RAM_SIZE
   
   This is just a check for the bare minimum for the Heap+Stack area before
   aborting compilation, it is not the run time limit:
   Heap_Size + Stack_Size = 0x80 + 0x80 = 0x100
 */
    .section .stack
    .align 3
#ifdef __STACK_SIZE
    .equ    Stack_Size, __STACK_SIZE
#else
    .equ    Stack_Size, 0x200
#endif
    .globl    __StackTop
    .globl    __StackLimit
__StackLimit:
    .space    Stack_Size
    .size __StackLimit, . - __StackLimit
__StackTop:
    .size __StackTop, . - __StackTop

    .section .heap
    .align 3
#ifdef __HEAP_SIZE
    .equ    Heap_Size, __HEAP_SIZE
#else
    .equ    Heap_Size, 0x000
#endif
    .globl    __HeapBase
    .globl    __HeapLimit
__HeapBase:
    .space    Heap_Size
    .size __HeapBase, . - __HeapBase
__HeapLimit:
    .size __HeapLimit, . - __HeapLimit
    
    .section .isr_vector
    .align 2
    .globl __isr_vector
__isr_vector:
    .long    __StackTop            /* Top of Stack */
    .long    Reset_Handler         /* Reset Handler */
    .long    NMI_Handler           /* NMI Handler */
    .long    HardFault_Handler     /* Hard Fault Handler */
    .long    0     /* MPU Fault Handler */
    .long    0      /* Bus Fault Handler */
    .long    0    /* Usage Fault Handler */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    0                     /* Reserved */
    .long    SVC_Handler           /* SVCall Handler */
    .long    0                     /* Debug Monitor Handler */
    .long    0                     /* Reserved */
    .long    PendSV_Handler        /* PendSV Handler */
    .long    SysTick_Handler       /* SysTick Handler */

    /* External interrupts */
    .long   SPI0_IRQHandler
    .long   SPI1_IRQHandler
    .long   0
    .long   UART0_IRQHandler
    .long   UART1_IRQHandler
    .long   UART2_IRQHandler
    .long   I2C_IRQHandler
    .long   SCT_IRQHandler
    .long   MRT_IRQHandler
    .long   CMP_IRQHandler
    .long   WDT_IRQHandler
    .long   BOD_IRQHandler
    .long   0
    .long   WKT_IRQHandler
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   0
    .long   PININT0_IRQHandler
    .long   PININT1_IRQHandler
    .long   PININT2_IRQHandler
    .long   PININT3_IRQHandler
    .long   PININT4_IRQHandler
    .long   PININT5_IRQHandler
    .long   PININT6_IRQHandler
    .long   PININT7_IRQHandler

    .size    __isr_vector, . - __isr_vector

    .text
    .thumb
    .thumb_func
    .align 2
    .globl    Reset_Handler
    .type    Reset_Handler, %function
Reset_Handler:
/*     Loop to copy data from read only memory to RAM. The ranges
 *      of copy from/to are specified by following symbols evaluated in 
 *      linker script.
 *      _etext: End of code section, i.e., begin of data sections to copy from.
 *      __data_start__/__data_end__: RAM address range that data should be
 *      copied to. Both must be aligned to 4 bytes boundary.  */
    ldr    r1, =__etext__
    ldr    r2, =__data_begin__
    ldr    r3, =__data_end__
    ldr    r0, =init_data
    blx    r0
    ldr    r0, =init_bss
    blx    r0
//    ldr    r0, =run_init
//    blx    r0
    ldr    r0, =run_preinit
    blx    r0
    ldr    r0, =run_init
    blx    r0
    ldr    r0, =SystemInit
    blx    r0
    ldr    r0, =main
    blx    r0
//    ldr    r0, =run_dtors
//    blx    r0
//    ldr    r0, =run_fini
//    blx    r0
    .pool
    .size Reset_Handler, . - Reset_Handler
    
/*    Macro to define default handlers. Default handler
 *    will be weak symbol and just dead loops. They can be
 *    overwritten by other handlers */
    .macro    def_default_handler    handler_name
    .align 1
    .thumb_func
    .weak    \handler_name
    .type    \handler_name, %function
\handler_name :
    b    .
    .size    \handler_name, . - \handler_name
    .endm
    
    def_default_handler    NMI_Handler
    def_default_handler    HardFault_Handler
    def_default_handler    SVC_Handler
    def_default_handler    PendSV_Handler
    def_default_handler    SysTick_Handler
    def_default_handler    Default_Handler
    
    def_default_handler     SPI0_IRQHandler
    def_default_handler     SPI1_IRQHandler
    def_default_handler     UART0_IRQHandler
    def_default_handler     UART1_IRQHandler
    def_default_handler     UART2_IRQHandler
    def_default_handler     I2C_IRQHandler
    def_default_handler     SCT_IRQHandler
    def_default_handler     MRT_IRQHandler
    def_default_handler     CMP_IRQHandler
    def_default_handler     WDT_IRQHandler
    def_default_handler     BOD_IRQHandler
    def_default_handler     WKT_IRQHandler
    def_default_handler     PININT0_IRQHandler
    def_default_handler     PININT1_IRQHandler
    def_default_handler     PININT2_IRQHandler
    def_default_handler     PININT3_IRQHandler
    def_default_handler     PININT4_IRQHandler
    def_default_handler     PININT5_IRQHandler
    def_default_handler     PININT6_IRQHandler
    def_default_handler     PININT7_IRQHandler

    .weak    DEF_IRQHandler
    .set    DEF_IRQHandler, Default_Handler

    .end

