/*measuring overhead for procedure call = actual overhead of call + overhead of reading time*/

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

void procedure0(){
  //has nothing
}

void procedure1(int a){
  //has nothing
}

void procedure2(int a,int b){
  //has nothing
}

void procedure3(int a,int b,int c){
  //has nothing
}

void procedure4(int a,int b,int c,int d){
  //has nothing
}

void procedure5(int a,int b,int c,int d, int e){
  //has nothing
}

void procedure6(int a,int b,int c,int d,int e,int f){
  //has nothing
}

void procedure7(int a,int b,int c,int d,int e,int f,int g){
  //has nothing
}


int main(){
  unsigned long long cycles,sum=0;
  int x,n=7;
  cpu_set_t mask;//variable to hold CPU number
  CPU_ZERO(&mask);
  CPU_SET(7,&mask);//use cpu number 7 for running current process
  int result = sched_setaffinity(0,sizeof(mask),&mask);
  for(x=0;x<100;x++)
    {
      cycles = rdtsc();
      procedure7(0,0,0,0,0,0,0);
      cycles = rdtsc()-cycles;
      sum=cycles+sum; 
    }
  printf("Average overhead for procedure call with %d argument(s) is %ld cycles\n",n,(unsigned)sum/x);
  return 0;
}
/*output
  Average overhead for procedure call with 0 argument(s) is 76 cycles
 Average overhead for procedure call with 1 argument(s) is 76 cycles
 Average overhead for procedure call with 2 argument(s) is 76 cycles
 Average overhead for procedure call with 7 argument(s) is 76 cycles*/
