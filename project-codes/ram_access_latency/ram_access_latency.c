#define _GNU_SOURCE
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>
#include <stdlib.h>
#include <pthread.h>

struct node {
  int number;
  struct node *next;
};

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

static struct node * head;

long getCount(long size_in_bytes){
  long count = size_in_bytes / sizeof(struct node);
  return count;
}

void main(){
  setCPUNumber(1);
  unsigned cycles_low, cycles_high, cycles_low1, cycles_high1;
  uint64_t start, end, number_of_cycles;
  FILE *output_file = fopen("results.out", "w");
  fprintf(output_file, "%s", "");
  fclose(output_file);
  output_file = fopen("results.out", "a");

  int MAX_E10 = 1000000; //Maximum bytes to be checked - 9MB
  for(int e = 1; e<=MAX_E10; e*=10){
    for(int f = 1; f<=9; f++){
      long bytes = f * e;
      //Create a Circular Linked List-------------------------------------------
      head = (struct node *) malloc(sizeof(struct node));
      head->number = 0;
      long count = getCount(bytes);
      struct node *ptr = head;
      for(long index = 1; index<count; index++){
        struct node *new_node = (struct node *) malloc(sizeof(struct node));
        new_node->number = rand();
        ptr->next = new_node;
        ptr = new_node;
      }
      ptr->next = head;
      //------------------------------------------------------------------------

      //Start Pointer redirections--------------------------------------------------
      ptr = head;
      asm volatile ("CPUID\n\t"
      "RDTSC\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t": "=r" (cycles_high), "=r" (cycles_low)::
      "%rax", "%rbx", "%rcx", "%rdx");
      for(long index = 1; index<=1000000; index++){
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
        ptr=ptr->next;
      }
      asm volatile("RDTSCP\n\t"
      "mov %%edx, %0\n\t"
      "mov %%eax, %1\n\t"
      "CPUID\n\t": "=r" (cycles_high1), "=r" (cycles_low1)::
      "%rax", "%rbx", "%rcx", "%rdx");
      //------------------------------------------------------------------------

      //Calculate and store value
      start = ( ((uint64_t)cycles_high << 32) | cycles_low );
      end = ( ((uint64_t)cycles_high1 << 32) | cycles_low1 );
      number_of_cycles = end - start;
      number_of_cycles/=100000; //Number of cycles per HUNDRED
      number_of_cycles-=2; //Number of Cycles per HUNDRED - 2 for loop overhead
      number_of_cycles/=100; //Number of Cycles per reference
      fprintf(output_file, "%lu - %lu\n", number_of_cycles, bytes);
      //------------------------------------------------------------------------

      //FREE MEMORY-------------------------------------------------------------
      head = ptr->next;
      ptr->next = NULL;
      struct node *temp;
      while(head != NULL){
        temp = head;
        head = head->next;
        free(temp);
      }
      //------------------------------------------------------------------------
    }
  }
  fclose(output_file);
  return;
}
