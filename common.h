#include <stdlib.h>
#include <stdio.h>

typedef struct
{
	int *array;
	size_t size;
} Bucket;

Bucket *allocate_buckets(int n_arrays);
int get_bucket_index(int value, int max, int n_buckets);
Bucket add_bucket_value(Bucket array, int value);
void bubble_sort(void *array);
void combine_buckets(int *sort_array, Bucket *buckets, int n_buckets);
void free_buckets(Bucket *buckets, int n_buckets);

Bucket *allocate_buckets(int n_arrays)
{
	Bucket *arrays;

	arrays = (Bucket *)malloc(n_arrays * sizeof(Bucket));
	for (int i = 0; i < n_arrays; i++)
	{
		arrays[i].array = NULL;
		arrays[i].size = 0;
	}

	return arrays;
}

int get_bucket_index(int value, int max, int n_buckets)
{
	return (int)(value) / (max / n_buckets);
}

Bucket add_bucket_value(Bucket array, int value)
{
	array.array = realloc(array.array, (array.size + 1) * sizeof(int));
	array.array[array.size] = value;
	array.size++;
	return array;
}

void bubble_sort(void *array)
{
	Bucket *sort = (Bucket *)array;
	char flag;
	int temp;

	for (size_t i = 0; i < sort->size; ++i)
	{
		flag = 1;

		for (size_t j = 0; j < sort->size - 1 - i; ++j)
		{

			// If the current element is greater than the next element, swap them
			if (sort->array[j] > sort->array[j + 1])
			{
				flag = 0;
				temp = sort->array[j];
				sort->array[j] = sort->array[j + 1];
				sort->array[j + 1] = temp;
			}
		}

		// If no swaps were made, the array is sorted
		if (flag)
			return;
	}
	return;
}

void combine_buckets(int *sort_array, Bucket *buckets, int n_buckets)
{
	int cont = 0;
	for (int i = 0; i < n_buckets; i++)
	{
		for (size_t j = 0; j < buckets[i].size; j++)
			sort_array[cont++] = buckets[i].array[j];
	}
}

void free_buckets(Bucket *buckets, int n_buckets)
{
	for (int i = 0; i < n_buckets; i++)
		free(buckets[i].array);
	free(buckets);
}
