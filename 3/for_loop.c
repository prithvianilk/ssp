#include<stdio.h>
#define N 1000000
int main()
{
	long long int a = 0;
	for(int i=0;i<N;i++)
	{
		a += i;
	}
	printf("%lld\n",a);
}