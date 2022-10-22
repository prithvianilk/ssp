#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 10000000
int main()
{
	int* arr = (int*)malloc(N*sizeof(int));
	
	for(int i=0;i<N;i++)
		arr[i] = 10;

	long long int a = 0;

	for(int i=0;i<N;i++)
		a += *(arr+i);

	printf("%lld\n",a);
}