#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right);
int Partition(Array* array, int left, int right);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

/*
Create new array 
input : size of the number to be sorted
output : the pointer of array
*/
Array* CreateArray(int size)
{
    Array *arr;

    arr = malloc(sizeof(Array));
    arr->size = size;
    arr->values = malloc(sizeof(int) * size);
    return (arr);
}

/*
Quick sort
*/
void QuickSort(Array* array, int left, int right)
{
    int pos;

    if (left < right)
    {
        pos = Partition(array, left, right);
        QuickSort(array, left, pos - 1);
        QuickSort(array, pos + 1, right);
    }
}

/*
Divide into 2 sub-lists based on pivot.
+ Select right-most data as pivot
+ Print left, right, pivot index at first
+ Print array values for every swap -> Printing array values should be implemented in a PrintArray function
Print format : 
    1. "left : %d, right : %d, pivot : %d\n"
    2. refered on PrintArray function
return:
    pivot index
*/
int Partition(Array* array, int left, int right)
{
    int pivot;
    int i;

    int j;

    pivot = array->values[right];
    i = left;
    j = right - 1;
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right);
    while (1)
    {
        while (array->values[j] > pivot && j > left)
            j--;
        while (array->values[i] <= pivot && i < right)
            i++;
        if (i < j)
        {
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }
        else
        {
            swap(&array->values[i], &array->values[right]);
            if (i != right)
                PrintArray(array, left, right);
            return (i);
        }
    }
    // 여기까지 닿지 않는다
    return (-1);
    // fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
}

/*
Print array values 
Print format :
    “%-3s” : out of bound and should be filled with "-" charater
    “%-3d” : array values to be printed
*/
void PrintArray(Array* array, int left, int right)
{
    for (int i = 0; i < array->size; i++)
    {
        if (i < left || i > right)
            fprintf(fout, "%-3s", "-");
        else
            fprintf(fout, "%-3d", array->values[i]);
    }
    fprintf(fout, "\n");

    // fprintf(fout, "%-3s", "-");
    // fprintf(fout, "%-3d", array->values[i]);
    // fprintf(fout, "\n");
}

/*
DeleteArray
*/
void DeleteArray(Array* array)
{
    free(array->values);
    free(array);
}

/*
Swap values
*/
void swap(int* a, int* b)
{
    int tmp;

    tmp = *a;
    *a = *b;
    *b = tmp;
}
