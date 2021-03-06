/* kernel.h : Helper constants give us flexibility in case the
 *			  parameters change.
 *
 * Authors: Group Member 1: Arjun Ankleshwaria <aanklesh>
 *          Group Member 2: Jiten Mehta <jitenm>
 *		    Group Member 3: Ankit Chheda <achheda>
 * Date:    Oct 24, 2013 9:00 AM
 */
#ifndef _KERNEL_H
#define _KERNEL_H

#define USR_STACK_BASE 0xa3000000
#define USR_MAIN_ADDR 0xa2000000
#define SWI_VECTOR_ADDR 0x08
#define LDR_PC_PC_minus_4 0xe51ff004 // opcode for ldr pc, [pc, #-4]

#endif
