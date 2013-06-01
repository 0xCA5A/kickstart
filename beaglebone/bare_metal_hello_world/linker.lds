ENTRY(_reset) /* entry point of the program, first instruction executed */
SECTIONS
{
 . = 0x402F0400;
 .text : {
 startup.o (INTERRUPT_VECTOR)
 *(.text)
 }
 .data : { *(.data) } /* global and static variables initialized by the application code */
 .bss : { *(.bss COMMON) } /* global and static variable that are not initialized by the application code. the compiler initialize this data to 0. */
 . = ALIGN(8);
 . = . + 0x1000; /* 4kB of stack memory */
 stack_top = .; /* stack grows upwards */
}