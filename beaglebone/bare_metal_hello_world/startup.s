.section INTERRUPT_VECTOR, "x"
.global _reset
_reset:
  B reset_handler /* reset */
  B . /* undefined instruction */
  B . /* swi */
  B . /* prefetch abort */
  B . /* data abort */
  B . /* reserved */
  B . /* irq */
  B . /* fiq */

reset_handler:
  LDR sp, =stack_top /* set stack pointer */
  BL c_entry /* c code entry point */
  B . /* loop forever after return from c code */
