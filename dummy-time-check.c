#include<stdio.h>
#include<time.h>
int main()
{
	double cpu_time;
	clock_t start=clock();
	printf("Hello world");
	clock_t end=clock();
	cpu_time = ((double)(end - start))/CLOCKS_PER_SEC;

	printf("Time: %f",cpu_time);
	return 0;
}

