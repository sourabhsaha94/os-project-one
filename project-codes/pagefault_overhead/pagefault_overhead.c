#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>

//Sets the CPU number on which to execute the next instrutions
void setCPUNumber(int cpu){
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

#define FILEPATH "file"
#define FILESIZE (5*1024*1024)

int main(void)
{
  uint64_t start, end, number_of_cycles;
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  setCPUNumber(1);
  int i;
  int fd;
  char *map;  /* Initializing map address */
  int pagesize = getpagesize(); //page size
  fd = open(FILEPATH, O_RDONLY); //opening file of around 5mb

  //Preparing OUTPUT file - results.out
  FILE *output_file = fopen("results.out", "w");
  fprintf(output_file, "%s", "");
  fclose(output_file);
  output_file = fopen("results.out", "a");

  if (fd == -1) {
    perror("Error opening file for reading");
    exit(1);
  }

  map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0); //Creating memory map

  if (map == MAP_FAILED) {
    close(fd);
    perror("Error mmapping the file");
    exit(1);
  }

  //Flushing out all data from memory map
  if (msync(map, FILESIZE, MS_INVALIDATE) != 0) {
    perror("Error trying tp flush map");
    exit(1);
  }

  long a = 0;
  long page_count = FILESIZE/pagesize;
  long iterations = 100000, take = iterations/2;
  for(long c = 1; c<iterations; c++){
    asm volatile ("CPUID\n\t"
    "RDTSC\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
    "%rax", "%rbx", "%rcx", "%rdx");
    for (i = 0; i < page_count; i++) {
      a+= map[i*pagesize];
    }
    //Get End Cycle count
    asm volatile("RDTSCP\n\t"
    "mov %%edx, %0\n\t"
    "mov %%eax, %1\n\t"
    "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
    "%rax", "%rbx", "%rcx", "%rdx");
    a += 1;
    if(c>take){
      start = ( ((uint64_t)cycles_high << 32) | cycles_low );
      end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
      number_of_cycles = end - start;
      fprintf(output_file, "%lu\n",number_of_cycles / page_count);
    }
    if (msync(map, FILESIZE, MS_INVALIDATE) != 0) {
      perror("Error trying tp flush map");
      exit(1);
    }
    if (munmap(map, FILESIZE) == -1) {
      perror("Error un-mmapping the file");
    }
    map = mmap(0, FILESIZE, PROT_READ, MAP_SHARED, fd, 0); //Creating memory map
  }
  close(fd);
  fclose(output_file);
  return 0;
}
