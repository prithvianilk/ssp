#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define N 1000000
int main()
{
	int* arr = (int*)malloc(N*sizeof(int));
	memset(arr, 1, sizeof(arr));
	int a = 0;
	for(int i=0;i<N;i++)
	{
		a += arr[i];
	}
}