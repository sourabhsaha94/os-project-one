#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <sched.h>

//Sets the CPU number on which to execute the next instrutions
void setCPUNumber(int cpu){
  cpu_set_t mask;//variable to hold CPU number
  CPU_ZERO(&mask);
  CPU_SET(cpu,&mask);//use cpu number 7 for running current process
  int result = sched_setaffinity(0,sizeof(mask),&mask);
  printf("Set cpu to %i\n", cpu);
}

void main(void){
  uint64_t start, end, number_of_cycles;
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  long iterations = 1000000;
  long take_iterations = iterations/2;
  long sum = 0;
  long loop_iterations = 1;
  //Setting up CPU
  setCPUNumber(3);

  //Preparing OUTPUT file - results.out-----------------------------------------
  FILE *output_file = fopen("results.out", "w");
  fprintf(output_file, "%s", "");
  fclose(output_file);
  output_file = fopen("results.out", "a");

  for(long i = 1; i<=iterations; i++){

    //Get Start Cycle count
    asm volatile ("CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");

    //The function to benchmark
    for(long i = 1; i<loop_iterations; i++){}

    //Get End Cycle count
    asm volatile("RDTSCP\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t"
    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
    "%rax", "%rbx", "%rcx", "%rdx");

    //Take the last 50% of results
    //So that the cache effects are reduced and the CPU is "warmed up"
    if(i>take_iterations){
      //Calculating number of Cycles
      start = ( ((uint64_t)cycles_high << 32) | cycles_low );
      end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
      number_of_cycles = end - start;
      number_of_cycles /= loop_iterations;
      //Exclude outliers
      if(number_of_cycles < 500){
        fprintf(output_file, "%lu\n", number_of_cycles);
      }
    }
  }

  //Closing output file
  fclose(output_file);
  return;
}
