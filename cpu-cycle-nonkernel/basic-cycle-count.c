/*implementation understood from www.stackoverflow.com*/

#include<stdio.h>
#include<unistd.h>

static __inline__ unsigned long long rdtsc(void)
{
  unsigned long long int x;
  __asm__ volatile (".byte 0x0f, 0x31" : "=A"(x)); //number is opcode of rdtsc
  return x;
}

int main(){
   unsigned long long cycles;
  
    cycles = rdtsc();
    /*any code change add it here*/
    sleep(1);
    /*---------------------------*/
    cycles = rdtsc()-cycles;
    printf("Time is %ld cycles\n",(unsigned)cycles);
   return 0;
}
/*output
  Time is 2494576388 cycles*/
