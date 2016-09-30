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
  register long * sum = 0;
  array = (int *)valloc(size); //To reduce page faults
  long iterations = 100000000;
  bzero(array, size);

  asm volatile ("CPUID\n\t"
  "RDTSC\n\t"
  "mov %%edx, %0\n\t"
  "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
  "%rax", "%rbx", "%rcx", "%rdx");
  for(long i = 1; i<=iterations; i++){
    for(int word = 1; word<=8; word++){
    sum+= array[0] + array[8] + array[16] + array[24] + array[32] + array[40]
     + array[48] + array[56] + array[64]
 				+ array[72] + array[80] + array[88] + array[96] + array[104] + array[112] + array[120] + array[128]
 				+ array[136] + array[144] + array[152] + array[160] + array[168] + array[176] + array[184] + array[192]
 				+ array[200] + array[208] + array[216] + array[224] + array[232] + array[240] + array[248] + array[256]
 				+ array[264] + array[272] + array[280] + array[288] + array[296] + array[304] + array[312] + array[320]
 				+ array[328] + array[336] + array[344] + array[352] + array[360] + array[368] + array[376] + array[384]
 				+ array[392] + array[400] + array[408] + array[416] + array[424] + array[432] + array[440] + array[448]
 				+ array[456] + array[464] + array[472] + array[480] + array[488] + array[496] + array[504] + array[512];
        array+=1;
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
