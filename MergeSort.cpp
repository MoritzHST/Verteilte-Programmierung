#include <iostream>
#include <vector>
#include <omp.h>

using namespace std;
const int cNUMBER_ELEMENTS = 10000000;
const int cNUM_THREADS = 2;
const int cTHRESHOLD_PARALLEL_HIRACHY = 1;
bool parallel = true;

//* Utility-Functions *//
int randomNumber() {
    int lowerLimit = 0;
    int upperLimit = 1000;

    return rand() % (upperLimit - lowerLimit) + lowerLimit;
}

int* generateNumbers() {
    int* retPtr = (int*)malloc(cNUMBER_ELEMENTS * sizeof(int));
    for (int i = 0; i < cNUMBER_ELEMENTS; i++) {
        retPtr[i] = randomNumber();
    }
    return retPtr;
}

void logArr(int* arr, int elem) {
    printf("\nFirst %i elements", elem);
    for (int i = 0; i < elem; i++) {
        printf("\n%i. Element: %i", i, arr[i]);
    }

    printf("\nLast %i elements", elem);
    for (int i = cNUMBER_ELEMENTS - 1; i > cNUMBER_ELEMENTS - elem - 1; i--) {
        printf("\n%i. Element: %i", i, arr[i]);
    }
}

//* Merge Sort-Functions *//
void merge(int* arr,  int leftLimit,  int middleLimit,  int rightLimit) {
    const int cntLeft = middleLimit - leftLimit + 1;
    const int cntRight = rightLimit - middleLimit;
    vector<int> arrLeft(cntLeft);
    vector<int> arrRight(cntRight);

    for (int i = 0; i < cntLeft; i++)
        arrLeft[i] = arr[leftLimit + i];
    for (int j = 0; j < cntRight; j++)
        arrRight[j] = arr[middleLimit + 1 + j];

     int i = 0;
     int j = 0;
     int k = leftLimit;

    while (i < cntLeft && j < cntRight) {
        if (arrLeft[i] <= arrRight[j]) {
            arr[k] = arrLeft[i];
            i++;
        }
        else {
            arr[k] = arrRight[j];
            j++;
        }
        k++;
    }


    while (i < cntLeft) {
        arr[k] = arrLeft[i];
        i++;
        k++;
    }


    while (j < cntRight) {
        arr[k] = arrRight[j];
        j++;
        k++;
    }
}

void mergeSort(int* arr,  int leftIndex,  int rightIndex, int hirachy) {
    if (leftIndex >= rightIndex) {
        return;
    }
    const int splitIndex = (leftIndex + rightIndex) / 2;
    if (parallel && hirachy == cTHRESHOLD_PARALLEL_HIRACHY) {
        #pragma omp parallel sections num_threads(cNUM_THREADS)
        {
            #pragma omp section
            {
                mergeSort(arr, leftIndex, splitIndex, hirachy + 1);
            }

            #pragma omp section 
            {
                mergeSort(arr, splitIndex + 1, rightIndex, hirachy + 1);
            }
            #pragma omp section 
            {
                merge(arr, leftIndex, splitIndex, rightIndex);
            }
        }
    }
    else {
        mergeSort(arr, leftIndex, splitIndex, hirachy + 1);
        mergeSort(arr, splitIndex + 1, rightIndex, hirachy + 1);
        merge(arr, leftIndex, splitIndex, rightIndex);
    }
}


int main()
{   
    int* numbers = generateNumbers();
    cout << "Generated "  << cNUMBER_ELEMENTS << " Numbers" << endl;

    logArr(numbers, 5);

    double clock_start, clock_stop;
    clock_start = omp_get_wtime();
    mergeSort(numbers, 0, cNUMBER_ELEMENTS - 1, 0);
    clock_stop = omp_get_wtime();
    printf("\nTime %f\n", clock_stop - clock_start);

    logArr(numbers, 5);
    
    int sorted = 1;
    for (int i = 0; i < cNUMBER_ELEMENTS - 1; ++i) {
        sorted &= (numbers[i] <= numbers[i + 1]);
    }
    printf("\nSorting Successful: %d\n", sorted); 
}
