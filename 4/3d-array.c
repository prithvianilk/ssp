#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int traverse(int N,int* arr ){
	int s=0;
	for(int i=0;i<N;i++){
		for(int j=0;j<N;j++){
			for(int k=0;k<N;k++){
				s+=arr[i*N + j*N + k];
			}
		}
	}
}

int main(int argc, char *argv[])
{
	int N = atoi(argv[1]);
	int* arr = (int*)malloc(N*N*N*sizeof(int));
	
	for(int i=0;i<N*N*N;i++)
		arr[i] = i+1;

	clock_t t;
	t=clock();
	traverse(N,arr);
	t=clock()-t;

	double time_taken = ((double)t)/CLOCKS_PER_SEC;
	printf("Traversal time for 3d-array N=%d: %f\n",N,time_taken);
	free(arr);

	return 0;
}