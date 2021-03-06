/** @file ctx_switch_asm.S
 *
 * @brief Contains the core context switch routine.
 *
 * These routines all assume that the caller is following AAPCS, needs
 * no co-processor registers and that the caller is in SVC mode. Furthermore,
 * the SPSR is considered a spare/scratch register and hence, is not saved.
 *
 * @author Arjun Ankleshwaria <aanklesh>
 *         Ankit Chheda <achheda>
 *         Jiten Mehta <jitenm>
 *
 * @date   Nov. 17, 2013  7:00 pm
 */

#include <asm.h>
#include <arm/psr.h>

	.file "ctx_switch_asm.S"

/**
 * @brief Special exit routine from the scheduler that launches a task for the
 * first time.
 *
 * r4 contains the user entry point.
 * r5 contains the single argument to the user function called.
 * r6 contains the user-mode stack pointer.
 * Upon completion, we should be in user mode.
 */
FUNC(launch_task)
	mov     r0, r5
	mov     r1, #0
	mov     r2, #0
	mov     r3, #0
	mov     r5, #0
	mov     r8, #0
	mov     ip, #0
	msr     cpsr_c, #(PSR_MODE_USR)
	mov     sp, r6
	mov     r6, #0
	ldr     lr, =0xdeadbeef /* Causes a crash instead of calling reset vector */
	mov     pc, r4

/* r0 points to the target context, r1 to the current context. */
/* store context of current task and load context of target task */
FUNC(ctx_switch_full)
	 stmia r1, {r4, r5, r6, r7, r8, r9, r10, r11, sp, lr}
	 ldmia r0, {r4, r5, r6, r7, r8, r9, r10, r11, sp, pc}

/* r0 points to the target context */
/* load context of target task */
FUNC(ctx_switch_half)
	ldr r4, [r0]
   	ldr r5, [r0, #4]
    ldr r6, [r0, #8]
    ldr sp, [r0, #32]
    ldr pc, [r0, #36]

