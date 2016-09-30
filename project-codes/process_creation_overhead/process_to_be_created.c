#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>

void main(void)
{
  //THIS INITILIZATION WILL CAUSE SOME CYCLE TIME INCREASE, but is very little
  //Compared to context switch
  unsigned cycles_low, cycles_high;

  //Get Start Cycle count
  asm volatile ("CPUID\n\t"
  "RDTSC\n\t"
  "mov %%edx, %0\n\t"
  "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
  "%rax", "%rbx", "%rcx", "%rdx");

  uint64_t end;
  FILE *output_end_file = fopen("end_results.out", "a");
  end = ( ((uint64_t)cycles_high << 32) | cycles_low );
  fprintf(output_end_file, "%lu\n", end);
  fclose(output_end_file);
  return;
}
