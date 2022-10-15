#include <math.h>
#include <pthread.h>
#include <stdio.h>
#define NUM_RECT 10000000

int thread_count;
double gPi = 0.0;  //  global accumulator for areas computed

void *Area(void *pArg) {
  int myNum = *((int *)pArg);
  double h = 2.0 / NUM_RECT;
  double *partialSum, x;  // local to each thread
  partialSum = new double;
  *partialSum = 0;

  // use every NUMTHREADS-th ste
  for (int i = myNum; i < NUM_RECT; i += thread_count) {
    x = -1 + (i + 0.5f) * h;
    *partialSum += sqrt(1.0 - x * x) * h;
  }
  return (void *)partialSum;
}

int main(int argc, char **argv) {
  thread_count = atoi(argv[1]);
  pthread_t tHandles[thread_count];
  int tNum[thread_count], i;
  for (i = 0; i < thread_count; ++i) {
    tNum[i] = i;
    pthread_create(&tHandles[i],       // Returned thread handle
                   NULL,               // Thread Attributes
                   Area,               // Thread function
                   (void *)&tNum[i]);  // Data for Area()
  }

  for (i = 0; i < thread_count; ++i) {
    double *partialSum;
    pthread_join(tHandles[i], (void **)&partialSum);
    gPi += *partialSum;
    delete partialSum;
  }
  gPi *= 2.0;
  printf("Computed value of Pi:  %12.9f\n", gPi);
  return 0;
}
