/** @file proc.c
 *
 * @brief Implementation of `process' syscalls
 *
 * @author: Group Member 1: Arjun Ankleshwaria <aanklesh>
 *          Group Member 2: Jiten Mehta <jitenm>
 *		    Group Member 3: Ankit Chheda <achheda>
 *
 * @date:   Nov 20, 2013 9:00 PM
 */

#include <exports.h>
#include <bits/errno.h>
#include <config.h>
#include <kernel.h>
#include <syscall.h>
#include <sched.h>
#include <lock.h>
#include <arm/reg.h>
#include <arm/psr.h>
#include <arm/exception.h>
#include <arm/physmem.h>
#include <device.h>
#include <assert.h>


int task_create(task_t* tasks, size_t num_tasks)
{
	unsigned int i = 0;
	unsigned int j = 0;
	unsigned int ret = 0;

	/* check for total number of tasks. return error if greater than 64 */
	if(num_tasks > 62)
		return -EINVAL;

	/* check if task pointer lies outside the valid address space */
	if(!(valid_addr((void *)tasks, sizeof(task_t)*num_tasks, \
					USR_START_ADDR, USR_END_ADDR)))
		return -EFAULT;

	for(i = 0; i < num_tasks; i++)
	{
		/* check if function pointer is NULL */
		if(tasks[i].lambda == null)
			return -EINVAL;

		/* check if stack pointers of two tasks are same */
		for(j = 0; j < num_tasks; j++)
			if((tasks[i].stack_pos == tasks[j].stack_pos) && (i != j))
				return -EINVAL;

		/* check if stack pointer lies outside the valid address space */
		if(!(valid_addr(tasks[i].stack_pos, (size_t)OS_USTACK_SIZE, \
						USR_START_ADDR, USR_END_ADDR)))
			return -EFAULT;
	}

	/* Check for schedulability of the task set */
	ret = assign_schedule(&tasks, num_tasks);

	/* If task set not schedulable, return error */
	if(!ret)
		return -ESCHED;

	/* Allocate all the tasks */
	allocate_tasks(&tasks, num_tasks);

	/* Context switch to the highest priority task */
	disable_interrupts();
	dispatch_nosave();

	assert(0); /* should never reach here */
}

int event_wait(unsigned int dev)
{
	/* Return invalid if the device does not exist */
	if(dev >= NUM_DEVICES)
		return -EINVAL;

	/* Check if task is holding lock and trying to wait on a device */
	if(get_cur_tcb()->holds_lock)
		return -EHOLDSLOCK;

	/* Add the task to the sleep queue of the device and context switch */
	dev_wait(dev);
	disable_interrupts();
	dispatch_sleep();
	return 0;
}

/* An invalid syscall causes the kernel to exit. */
void invalid_syscall(unsigned int call_num)
{
	printf("Kernel panic: invalid syscall -- 0x%08x\n", call_num);
	disable_interrupts();
	while(1);
}

