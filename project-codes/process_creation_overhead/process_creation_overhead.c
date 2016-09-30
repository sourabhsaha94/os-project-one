#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
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
  uint64_t start;
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  long iterations = 100000;
  long take_iterations = iterations/2;
  long sum = 0;

  //Setting up CPU
  setCPUNumber(1);

  //Preparing OUTPUT file - results.out-----------------------------------------
  FILE *output_start_file = fopen("start_results.out", "w");
  fprintf(output_start_file, "%s", "");
  fclose(output_start_file);
  FILE *output_end_file = fopen("end_results.out", "w");
  fprintf(output_end_file, "%s", "");
  fclose(output_end_file);
  output_start_file = fopen("start_results.out", "a");

  for(long i = 1; i<=iterations; i++){

    //Get Start Cycle count
    asm volatile ("CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");

    int pid = fork();
    if(pid == 0){
      execvp("./process_to_be_created", NULL);
    }
    start = ( ((uint64_t)cycles_high << 32) | cycles_low );
    int status;
    waitpid(pid, &status, 0);
    fprintf(output_start_file, "%lu\n", start);
  }
  fclose(output_start_file);
  return;
}
