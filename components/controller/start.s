
.globl _start
_start:

  ldr r0,=0x29000000
  ldr r1,=0xDEADBEEF
  ldr r2,[r0]
  cmp r1,r2
  beq _main

// ---------------------------------------------------------------------- Switch to Supervisor mode
  mrs r0, cpsr
  bic r0, r0, #0x1F     // clear mode bits
  orr r0, r0, #0x13     // set Supervisor mode
  msr spsr_cxsf, r0
  add r0, pc, #4        // hold (in ELR_hyp) the address to return to  (to make 'eret' working right)
  msr ELR_hyp, r0       // save the address in ELR_hyp
  eret                  // apply the mode change (Exception return)

  // ---------------------------------------------------------------------- Init cache
  mov r12,#0
  mcr p15, 0, r12, c7, c10, 1   // DCCMVAC - Clean data or unified cache line by virtual address to PoC.
  dsb                           // Data sync barrier
  mov r12, #0
  mcr p15, 0, r12, c7, c5, 0    // ICIALLU - Invalidate all instruction caches to PoU. If branch predictors are architecturally visible, also flush them.
  mov r12, #0
  mcr p15, 0, r12, c7, c5, 6    // BPIALL  - Invalidate all entries from branch predictors.
  dsb                           // Data sync barrier
  isb                           // Instruction sync barrier

  // ---------------------------------------------------------------------- Turn on Instruction cache
  mrc p15,0,r2,c1,c0,0
  orr r2, #0x1000    // Instruction cache
  mcr p15,0,r2,c1,c0,0
  ldr sp,=0x30000000

  ldr r0,=0x29000000
  ldr r1,=0xDEADBEEF
  str r1,[r0]

_main:
  ldr r0,=0xDEADBEEF
  ldr r1,=0x400000bc
  str r0,[r1]
  bl main
  ldr r0,=0xFFFFFFFF
  ldr r1,=0x400000fc
  str r0,[r1]
  mov r3, #0           // Comparison value
_check_loop:             // Loop to check jump address
    ldr r0, [r1]         // Get its value
    cmp r0, r3           // Is it null?
    beq _check_loop      // If yes, loop around
    bx  r0               // If not, jump to the address
