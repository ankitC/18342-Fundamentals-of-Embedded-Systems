@ init.S: initilizing the evn for main user_space app
@
@ Authors: Group Member 1: Arjun Ankleshwaria <aanklesh>
@          Group Member 2: Jiten Mehta <jitenm>
@		   Group Member 3: Ankit Chheda <achheda>
@ Date:    Oct 24, 2013 9:00 AM

#include <kernel_helper.h>
#include <bits/swi.h>

	.file	"init.S"
	.text
	.align 2

	.global	init

init:
	MRS r12, cpsr
 	BIC r12, r12, #0x03				@ Changing to user_mode
 	MSR cpsr, r12

	mov sp, r0						@ Userspace SP point to stk ptr
									@ which is the argument to init.

	mov lr, pc						@ Pointing lr to swi EXIT_SWI
	LDR pc, =USR_MAIN_ADDR

