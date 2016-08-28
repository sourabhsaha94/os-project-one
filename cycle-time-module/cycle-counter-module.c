/*Based on How to Benchmark Code Execution Times on Intel IA-32 and IA-64 Instruction Set Architectures by Gabriele Paolini*/

#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/hardirq.h>
#include<linux/preempt.h>
#include<linux/sched.h>

static int __init hello_start(void)
{
	unsigned long flags;
	uint64_t start,end;
	int variable =0,i=0;
	unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
	printk(KERN_INFO "Loading test module...\n");

	/*To make CPU exclusive*/
	preempt_disable();
	raw_local_irq_save(flags);

	/*now CPU is exclusive*/
	asm volatile (
		"cpuid\n\t"
		"rdtsc\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		:"=r"(cycles_high),"=r"(cycles_low)
		::"%rax","%rbx","%rcx","%rdx");
	
	for(i=0;i<5;i++)
	{
		variable = variable +5;
	}


	asm volatile (
		"rdtsc\n\t"
		"mov %%edx, %0\n\t"
		"mov %%eax, %1\n\t"
		"cpuid\n\t"
		:"=r"(cycles_high1),"=r"(cycles_low1)
		::"%rax","%rbx","%rcx","%rdx");

	raw_local_irq_restore(flags);
	preempt_enable();

	start = (((uint64_t)cycles_high << 32) | cycles_low);
	end = (((uint64_t)cycles_high1 << 32) | cycles_low1);

	printk(KERN_INFO "\n code executed in %llu clock cycles",(end-start));
	return 0;
}

static void __exit hello_end(void)
{
	printk(KERN_INFO "Goodbye\n");
}

module_init(hello_start);
module_exit(hello_end);
