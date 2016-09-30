#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>

static unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;

//Sets the CPU number on which to execute the next instrutions
void setCPUNumber(int cpu){
  struct sched_param param;
  param.sched_priority = 99;
  if (sched_setscheduler(getpid(), SCHED_FIFO, &param) != 0) {
    perror("sched_setscheduler");
    exit(EXIT_FAILURE);
  }
  cpu_set_t mask;//variable to hold CPU number
  CPU_ZERO(&mask);
  CPU_SET(cpu,&mask);//use cpu number for running current process
  int result = sched_setaffinity(0,sizeof(mask),&mask);
}

static char byte[1] = "a";
static char byte_buffer[1];
static int fd[2], input_byte;

static void *kernel_thread_function(void *args){
  input_byte = read(fd[0], byte_buffer, sizeof(byte_buffer));
  //Get End Cycle count
  asm volatile("RDTSCP\n\t"
  "mov %%edx, %0\n\t"
  "mov %%eax, %1\n\t"
  "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
  "%rax", "%rbx", "%rcx", "%rdx");
}

void main(void){

  uint64_t start, end, number_of_cycles;
  long iterations = 2000000;
  long take_iterations = iterations/2;
  pipe(fd);
  //Setting up CPU
  setCPUNumber(1);

  //Preparing OUTPUT file - results.out-----------------------------------------
  FILE *output_file = fopen("results.out", "w");
  fprintf(output_file, "%s", "");
  fclose(output_file);
  output_file = fopen("results.out", "a");
  for(long i = 1; i<=iterations; i++){

    pthread_t t_ref;
    pthread_create(&t_ref, NULL, &kernel_thread_function, NULL);
    //Get Start Cycles
    asm volatile ("CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");
    write(fd[1], byte, 1);

    pthread_join(t_ref, NULL);
    //Take the last 50% of results
    //So that the cache effects are reduced and the CPU is "warmed up"
    if(i>take_iterations){
      //Calculating number of Cycles
      start = ( ((uint64_t)cycles_high << 32) | cycles_low );
      end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
      number_of_cycles = end - start;

      if(number_of_cycles < 100000){
        fprintf(output_file, "%lu\n", number_of_cycles);
      }
    }
  }
  //Closing output file
  fclose(output_file);
  return;
}
