#include <omp.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "bucket-sort.h"

#define RANDOM_MAX 256

/*
### Implement the Bucket Sort Algorithm
    - Consider Arrays of 3 Sizes:
        - __1,000__
        - __10,000__
        - __100,000__
    - Arrays of Positive Integers, randomly generated using the _rand()_ function.
    - The maximum value of the array is defined by __RAND_MAX__
    - Implement using _Pthreads_ and _OpenMP_
        - Execute with 1, 2, 4, and 8 threads
*/


int *random_array(size_t size);
void test_openmp(int *array, size_t size);
void test_pthreads(int *array, size_t size);
int is_sorted(int *sort, size_t size);
void print_array(int *array, size_t size);

int main(void){
  int *array1K = NULL;
  int *array10K = NULL;
  int *array100K = NULL;

  // init random seed
  srand( (unsigned) time(NULL) );

  // create random arrays
  array1K = random_array(1000);
  array10K = random_array(10000);
  array100K = random_array(100000);

  // perform tests using OpenMP
  printf("----------- Testing OPENMP ------------\n");
  printf("--------------------------------------\n");
  test_openmp(array1K, 1000);
  test_openmp(array10K, 10000);
  test_openmp(array100K, 100000);

  // perform tests using Pthreads
  printf("---------- Testing PTHREADS -----------\n");
  printf("--------------------------------------\n");
  test_pthreads(array1K, 1000);
  test_pthreads(array10K, 10000);
  test_pthreads(array100K, 100000);

  // free memory
  free(array1K);
  free(array10K);
  free(array100K);

  return 0;
}

int *random_array(size_t size){
  int *array = malloc(size * sizeof(int));

  for(size_t i = 0; i < size; i++)
    array[i] = (rand() % RANDOM_MAX);

  return array;
}

void test_openmp(int *array, size_t size){
  int *sort = NULL;

  printf("| Array Size %d:\n", (int) size);
  printf("| | Time: \n");

  sort = bucket_sort_openmp(array, size, RANDOM_MAX, 1);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_openmp(array, size, RANDOM_MAX, 2);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_openmp(array, size, RANDOM_MAX, 4);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_openmp(array, size, RANDOM_MAX, 8);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  printf("| \n");
}

void test_pthreads(int *array, size_t size){
  int *sort = NULL;

  printf("| Array size %d:\n", (int) size);

  sort = bucket_sort_pthreads(array, size, RANDOM_MAX, 1);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_pthreads(array, size, RANDOM_MAX, 2);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_pthreads(array, size, RANDOM_MAX, 4);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  sort = bucket_sort_pthreads(array, size, RANDOM_MAX, 8);
  if(!is_sorted(sort,size)){
    printf("Array not ordered!\n");
    exit(0);
  }
  free(sort);

  printf("| \n");
}

int is_sorted(int *sort, size_t size){
  for(size_t i = 1; i < size; i++){
    if (sort[i] < sort[i - 1])
      return 0;
  }
  return 1;
}