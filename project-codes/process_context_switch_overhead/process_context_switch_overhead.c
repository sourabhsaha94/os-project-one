#define _GNU_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/mman.h>

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

int main(void){
  unsigned cycles_low = 0, cycles_high = 0, cycles_low1 = 0, cycles_high1 = 0;
  uint64_t start, end, number_of_cycles;
  char byte[1] = "a";
  char byte_buffer[1];
  int fd[2], input_byte;
  setCPUNumber(1);
  FILE *output_start_file = fopen("start_results.out", "a");
  FILE *output_end_file = fopen("end_results.out", "a");
  //Creating pipe with file descriptor
  pipe(fd);
  //Creating new process
  pid_t pid;
  if((pid = fork()) == -1){
    perror("COULD NOT FORK");
    exit(1);
  }
  if(pid == 0){
    asm volatile ("CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");
    write(fd[1], byte, 1);
  //  fprintf(output_start_file, "%lu\n",( ((uint64_t)cycles_high << 32) | cycles_low ));
    //exit(0);
  }else{
    input_byte = read(fd[0], byte_buffer, sizeof(byte_buffer));
    asm volatile("RDTSCP\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t"
    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
    "%rax", "%rbx", "%rcx", "%rdx");
//    fprintf(output_end_file, "%lu\n",( ((uint64_t)cycles_high1 << 32) | cycles_low1 ));
    int status;
    if(waitpid(pid, &status, 0) == -1){
      perror("COULD NOT WAIT FOR CHILD PROCESS");
    }
  }
  if(cycles_high1 > 0){
    //printf("%lu\n",( ((uint64_t)cycles_high1 << 32) | cycles_low1 ));
    fprintf(output_end_file, "%lu\n",( ((uint64_t)cycles_high1 << 32) | cycles_low1 ));
  }
  if(cycles_high > 0){
    //printf("%lu\n",( ((uint64_t)cycles_high << 32) | cycles_low ));
    fprintf(output_start_file, "%lu\n",( ((uint64_t)cycles_high << 32) | cycles_low ));
  }


  fclose(output_end_file);
  fclose(output_start_file);
  return(0);
}
