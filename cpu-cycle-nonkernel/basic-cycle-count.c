/*implementation understood from www.stackoverflow.com*/

#include<stdio.h>

static __inline__ unsigned long long rdtsc(void)
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A"(x)); //number is opcode of rdtsc
  return x;
}

int main(){
  int i=0;
  unsigned long long cycles = rdtsc();
  /*any code change add it here*/
  /*---------------------------*/
  cycles = rdtsc()-cycles;
  printf("Time is %d cycles\n",(unsigned)cycles);
  return 0;
}
