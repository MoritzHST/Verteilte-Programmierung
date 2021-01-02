// MergeSort.cpp : Diese Datei enthält die Funktion "main". Hier beginnt und endet die Ausführung des Programms.
//

#include <iostream>
#include <vector>
using namespace std;
const int cNUMBER_ELEMENTS = 1000;

//* Utility-Functions *//
int randomNumber() {
    int lowerLimit = 0;
    int upperLimit = 1000;

    return rand() % (upperLimit - lowerLimit) + lowerLimit;
}

 int* generateNumbers() {
    vector<int> arr(cNUMBER_ELEMENTS);
    
    for (int i = 0; i < cNUMBER_ELEMENTS; i++) {
        arr[i] = randomNumber();
    }

    int* retPtr = arr.data();
    return retPtr;
}

//* Merge Sort-Functions *//
void merge( int* arr,  int leftLimit,  int middleLimit,  int rightLimit) {
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

void mergeSort( int* arr,  int leftIndex,  int rightIndex, int hirachy) {
    if (leftIndex >= rightIndex) {
        return;
    }
    const int splitIndex = (leftIndex + rightIndex) / 2;
    mergeSort(arr, leftIndex, splitIndex, hirachy + 1);
    mergeSort(arr, splitIndex + 1, rightIndex, hirachy + 1);
    merge(arr, leftIndex, splitIndex, rightIndex);
}

int main()
{   
     int* numbersPtr = generateNumbers();
    cout << "Generated "  << cNUMBER_ELEMENTS << " Numbers" << endl;

    mergeSort(numbersPtr, 0, cNUMBER_ELEMENTS - 1, 0);

    int sorted = 1;
    for (int i = 0; i < cNUMBER_ELEMENTS - 1; ++i) {
        sorted &= (numbersPtr[i] <= numbersPtr[i + 1]);
    }
    printf("\nSorting Successful: %d\n", sorted); //1 -> true, 0 -> false
}
