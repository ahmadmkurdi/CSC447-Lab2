#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <pthread.h>
#include "common.h"

int *bucket_sort_sequential(int *array, size_t size, int max_value);
int *bucket_sort_openmp(int *array, size_t size, int max_value, int n_threads);
int *bucket_sort_pthreads(int *array, size_t size, int max_value, int n_threads);

int *bucket_sort_sequential(int *array, size_t size, int max_value){
    Bucket *buckets = NULL;
    int *sort_array = NULL;
    long double start, final;

    // initial time
    start = clock();

    // allocate the arrays to be sorted
    buckets = allocate_buckets(1);

    // allocate the sorted array
    sort_array = malloc(size * sizeof(int));

    // Add the values to the buckets
    for(size_t i = 0; i < size; i++){
        int index = get_bucket_index(array[i],max_value,1);
        buckets[index] = add_bucket_value(buckets[index], array[i]);
    }//for

    // Sort the bucket
    bubble_sort((void *) &buckets[0]);

    // combine all the values
    combine_buckets(sort_array, buckets, 1);

    // final time
    final = (clock() - start) / CLOCKS_PER_SEC;

    // print the final time
    printf("| | | using sequential method: %Lf s\n", final);

    // free the buckets from memory
    free_buckets(buckets, 1);

    // return the sorted array
    return sort_array;
}

int *bucket_sort_openmp(int *array, size_t size, int max_value, int n_threads){
	Bucket *buckets = NULL;
	int *sort_array = NULL;
	long double start, final;

	// initial time
	start = omp_get_wtime();

	// allocate the arrays to be sorted
	buckets = allocate_buckets(n_threads);

	// allocate the sorted array
	sort_array = malloc(size * sizeof(int));

	// Add the values to the buckets
	for(size_t i = 0; i < size; i++){
		int index = get_bucket_index(array[i],max_value,n_threads);
		buckets[index] = add_bucket_value(buckets[index], array[i]);
	}//for

	// Divide into N Threads
	#pragma omp parallel for schedule(dynamic) num_threads(n_threads)
	for(int i = 0; i < n_threads; i++)
		bubble_sort((void *) &buckets[i]);

	// combine all the values
	combine_buckets(sort_array, buckets, n_threads);

	// final time
	final = omp_get_wtime() - start;

	// print the final time
	printf("| | | using %d thread(s): %Lf s\n", n_threads, final);

	// free the buckets from memory
	free_buckets(buckets, n_threads);

	// return the sorted array
	return sort_array;
}

int *bucket_sort_pthreads(int *array, size_t size, int max_value, int n_threads)
{
	pthread_t threads[n_threads];
	Bucket *buckets = NULL;
	int *sort_array = NULL;
	long double start, final;

	// initial time
	start = omp_get_wtime();

	// allocates the arrays to be sorted
	buckets = allocate_buckets(n_threads);

	// allocates the sorted array
	sort_array = malloc(size * sizeof(int));

	// Add the values to the buckets
	for (size_t i = 0; i < size; i++)
	{
		int index = get_bucket_index(array[i], max_value, n_threads);
		buckets[index] = add_bucket_value(buckets[index], array[i]);
	}

	// each thread receives a bucket
	for (int i = 0; i < n_threads; i++)
		pthread_create(&threads[i], NULL, (void *)bubble_sort, (void *)&buckets[i]);

	// waits for the threads to finish
	for (int i = 0; i < n_threads; i++)
	{
		pthread_join(threads[i], NULL);
	}

	// combines all the values
	combine_buckets(sort_array, buckets, n_threads);

	// final time
	final = omp_get_wtime() - start;

	// prints the time
	printf("| | | using %d thread(s): %Lf\n", n_threads, final);

	// frees the buckets from memory
	free_buckets(buckets, n_threads);

	// returns the sorted array
	return sort_array;
}