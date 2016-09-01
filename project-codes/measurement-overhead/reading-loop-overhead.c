/*implementation understood from www.stackoverflow.com*/

#define _GNU_SOURCE  //required for setting CPU affinity
#include<stdio.h>
#include<unistd.h>
#include<sched.h>

static __inline__ unsigned long long rdtsc(void)
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A"(x)); //number is opcode of rdtsc
  return x;
}

int main(){
  unsigned long long cycles,sum=0;
  int x,i;
  cpu_set_t mask;//variable to hold CPU number
  CPU_ZERO(&mask);
  CPU_SET(7,&mask);//use cpu number 7 for running current process
  int result = sched_setaffinity(0,sizeof(mask),&mask);
  for(i=0;i<100000;i++)
    {
      cycles = rdtsc();
      for(x=0;x<100;x++){}
      cycles = rdtsc()-cycles;
      sum = cycles + sum;
    }
  printf("Loop overhead is %ld cycles\n",(unsigned)sum/i);
  return 0;
}
/*output
  Average loop overhead is 2650 cycles*/
