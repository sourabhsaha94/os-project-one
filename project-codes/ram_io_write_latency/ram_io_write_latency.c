#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <strings.h>
#include <sched.h>
#include <stdlib.h>

//Sets the CPU number on which to execute the next instrutions
void setCPUNumber(int cpu){
  //Setting priority of process to max
  //to give more control of CPU
  //Needs SU privelege
  struct sched_param param;
  param.sched_priority = 99;
  if (sched_setscheduler(getpid(), SCHED_FIFO, & param) != 0) {
    perror("sched_setscheduler");
    exit(EXIT_FAILURE);
  }
  cpu_set_t mask;//variable to hold CPU number
  CPU_ZERO(&mask);
  CPU_SET(cpu,&mask);//use cpu number 7 for running current process
  int result = sched_setaffinity(0,sizeof(mask),&mask);
}


void main(){
  setCPUNumber(1);
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  uint64_t start, end, number_of_cycles;
  int *array;
  long size = sizeof(int)*1024;
  array = (int *)valloc(size);
  long iterations = 200000000;
  bzero(array, size);

  asm volatile ("CPUID\n\t"
  "RDTSC\n\t"
  "mov %%edx, %0\n\t"
  "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
  "%rax", "%rbx", "%rcx", "%rdx");
  for(long i = 1; i<=iterations; i++){
    for(int word = 1; word<=8; word++){
      array[0] = 1; array[8]  = 1; array[16]  = 1; array[24]  = 1; array[32]  = 1; array[40]
      = 1; array[48]  = 1; array[56]  = 1; array[64]
      = 1; array[72]  = 1; array[80]  = 1; array[88]  = 1; array[96]  = 1; array[104]  = 1; array[112]  = 1; array[120]  = 1; array[128]
      = 1; array[136]  = 1; array[144]  = 1; array[152]  = 1; array[160]  = 1; array[168]  = 1; array[176]  = 1; array[184]  = 1; array[192]
      = 1; array[200]  = 1; array[208]  = 1; array[216]  = 1; array[224]  = 1; array[232]  = 1; array[240]  = 1; array[248]  = 1; array[256]
      = 1; array[264]  = 1; array[272]  = 1; array[280]  = 1; array[288]  = 1; array[296]  = 1; array[304]  = 1; array[312]  = 1; array[320]
      = 1; array[328]  = 1; array[336]  = 1; array[344]  = 1; array[352]  = 1; array[360]  = 1; array[368]  = 1; array[376]  = 1; array[384]
      = 1; array[392]  = 1; array[400]  = 1; array[408]  = 1; array[416]  = 1; array[424]  = 1; array[432]  = 1; array[440]  = 1; array[448]
      = 1; array[456]  = 1; array[464]  = 1; array[472]  = 1; array[480]  = 1; array[488]  = 1; array[496]  = 1; array[504]  = 1; array[512];
      array+=1; //To cover every byte
    }
    array-=8;
  }
  asm volatile("RDTSCP\n\t"
  "mov %%edx, %0\n\t"
  "mov %%eax, %1\n\t"
  "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
  "%rax", "%rbx", "%rcx", "%rdx");

  //Calculate and store value
  start = ( ((uint64_t)cycles_high << 32) | cycles_low );
  end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
  number_of_cycles = end - start;
  number_of_cycles/=iterations;

  printf("%lu cycles for writing 4KB\n", number_of_cycles);
}
