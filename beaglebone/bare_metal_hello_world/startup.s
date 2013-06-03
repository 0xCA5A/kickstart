.section INTERRUPT_VECTOR, "x"
.global _asm_entry

_asm_entry:
  LDR sp, =stack_top /* set stack pointer */
  BL c_entry /* c code entry point */
  B . /* loop forever after return from c code */
