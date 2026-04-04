#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define THRESHOLD 50

typedef struct {
    int* arr;
    int left;
    int right;
    int max_threads;
} thread_arg;

void swap(int* arr, int i, int j)
{
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    
} 

int partition(int* arr, int low, int high)
{
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; ++j)
    {
        if (arr[j] <= pivot)
        {
            ++i;
            swap(arr, i, j);
        }
    }

    swap(arr, i+1, high);
    return i + 1;
}

void quickSort(int* arr, int low, int high)
{
    if (low < high)
    {
        int pivotIndex = partition(arr, low, high);

        quickSort(arr, low, pivotIndex - 1);
        quickSort(arr, pivotIndex + 1, high);
    }
}

void* parallelQuickSort(void* args) // take args from thread
{ 
    thread_arg* data = (thread_arg*)args;
    int* arr = data->arr;
    int left = data->left;
    int right = data->right;
    int max_threads = data->max_threads;

    free(data);

    if (left >= right) 
    {
        return NULL;
    }

    if (max_threads <= 1 || (right - left + 1) < THRESHOLD) 
    {
        quickSort(arr, left, right);
        return NULL;
    }

    int pivotIndex = partition(arr, left, right);

    thread_arg* left_arg = malloc(sizeof(thread_arg));
    left_arg->arr = arr;
    left_arg->left = left;
    left_arg->right = pivotIndex - 1;
    left_arg->max_threads = max_threads / 2;

    thread_arg* right_arg = malloc(sizeof(thread_arg));
    right_arg->arr = arr;
    right_arg->left = pivotIndex + 1;
    right_arg->right = right;
    right_arg->max_threads = max_threads - max_threads / 2;

    pthread_t left_thread, right_thread;
    pthread_create(&left_thread, NULL, parallelQuickSort, left_arg);
    pthread_create(&right_thread, NULL, parallelQuickSort, right_arg);

    pthread_join(left_thread, NULL);
    pthread_join(right_thread, NULL);

    return NULL;
}

void DivideAndRule(int* arr, int size, int count_of_threads) 
{
    thread_arg* main_arg = malloc(sizeof(thread_arg));
    main_arg->arr = arr;
    main_arg->left = 0;
    main_arg->right = size - 1;
    main_arg->max_threads = count_of_threads;
    parallelQuickSort(main_arg);
}

int main(int argc, char** argv)
{

    if (argc < 2)
    {
        perror("incorrect count of arguements. must be 2\n");
        exit(1);
    }


    int count_of_threads = 0;
    

    count_of_threads = atoi(argv[1]);
    if (count_of_threads < 0) 
    {
        perror("incorrect count of threads!\n");
        exit(1);
    }

    int capacity = 10;
    int size = 0;
    int* arr = malloc(capacity * sizeof(int));

    int num;

        while (scanf("%d", &num) == 1) {
        if (size >= capacity) {
            capacity *= 2;
            arr = realloc(arr, capacity * sizeof(int));
        }
        arr[size++] = num;
    }

    if (size > 0) {
        DivideAndRule(arr, size, count_of_threads);

        for (int i = 0; i < size; ++i) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    free(arr);

    return 0;
}
