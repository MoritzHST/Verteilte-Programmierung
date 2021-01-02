/*
* @author: Alina Pohl
*
* Parallel Merge Sort
*/

#include <omp.h>
#include <ctime>
#include <algorithm>

void mergeSort(int arr[], int arr_size, int l, int r);
void sequentialMergeSort(int arr[], int l, int r);
void parallelMergeSort(int arr[], int l, int r, int threshold);
void merge(int arr[], int l, int m, int r);

int* temp;

int main(int argc, char** argv)
{
    int arr_size;
    if (argc > 1) {
        arr_size = atoi(argv[1]);
    }
    else {
        printf("Using default array size = 100.000.000\n");
        arr_size = 100000000;
    }

    //set size of arrays
    int* arr = (int*)malloc(arr_size * sizeof(int));
    temp = (int*)malloc(arr_size * sizeof(int));

    //fill array with random numbers
    srand((unsigned)time(0));
    for (int i = 0; i < arr_size; i++) {
        arr[i] = rand() % 1000; //range from 0 to 1000
    }

    mergeSort(arr, arr_size, 0, arr_size - 1);

#pragma omp parallel
    {
#pragma omp master
        {
            printf("Threads: %d\nProcs: %d\n", omp_get_num_threads(), omp_get_num_procs());
        }
    }

    //check if array is sorted correctly
    int sorted = 1;
    for (int i = 0; i < arr_size - 1; ++i) {
        sorted &= (arr[i] <= arr[i + 1]);
    }
    printf("\nSorting Successful: %d\n", sorted); //1 -> true, 0 -> false

    return 0;
}

void mergeSort(int arr[], int arr_size, int l, int r) {
    int numThreads = 2;
    double clock_start, clock_stop;
    int threshold = arr_size / numThreads;

    omp_set_num_threads(numThreads);
    clock_start = omp_get_wtime();
    parallelMergeSort(arr, l, r, threshold);
    clock_stop = omp_get_wtime();
    printf("Time %f\n", clock_stop - clock_start);
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k, startIndexRight;
    j = l;
    startIndexRight = m + 1;

    for (i = l; j <= m && startIndexRight <= r; i++)
    {
        if (arr[j] <= arr[startIndexRight])
        {
            temp[i] = arr[j];
            j++;
        }
        else
        {
            temp[i] = arr[startIndexRight];
            startIndexRight++;
        }
    }
    if (j > m)
    {
        for (k = startIndexRight; k <= r; k++)
        {
            temp[i] = arr[k];
            i++;
        }
    }
    else
    {
        for (k = j; k <= m; k++)
        {
            temp[i] = arr[k];
            i++;
        }
    }
    for (k = l; k <= r; k++)
        arr[k] = temp[k];
}

//l is for left index and r is right index of the sub-array of arr to be sorted
void sequentialMergeSort(int arr[], int l, int r)
{
    if (r - l == 1)
    {
        if (arr[l] <= arr[r]) {
            return;
        }
        else {
            std::swap(arr[l], arr[r]);
            return;
        }
    }

    if (l < r) {
        int m = (l + r) / 2; //avoids overflow for large l and r
        sequentialMergeSort(arr, l, m); //first half
        sequentialMergeSort(arr, m + 1, r); //second half
        merge(arr, l, m, r);
    }
}

//l is for left index and r is right index of the sub-array of arr to be sorted
void parallelMergeSort(int arr[], int l, int r, int threshold)
{
    if (r - l == 1)
    {
        if (arr[l] <= arr[r]) {
            return;
        }
        else {
            std::swap(arr[l], arr[r]);
            return;
        }
    }

    if (l < r)
    {
        int m = l + (r - l) / 2; //avoids overflow for large l and r

        if ((r - l + 1) <= threshold) {
            sequentialMergeSort(arr, l, m);
            sequentialMergeSort(arr, m + 1, r);
        }
        else {
#pragma omp parallel
            {
#pragma omp sections
                {
#pragma omp section
                    parallelMergeSort(arr, l, m, threshold); //first half
#pragma omp section
                    parallelMergeSort(arr, m + 1, r, threshold); //second half
                }
            }
        }
        merge(arr, l, m, r);
    }
}