#include<stdio.h>
#include<stdlib.h>
#define INF 1e9


FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
int Insert(Heap *heap, int value);
int Find(Heap *heap, int value);
int DeleteMax(Heap* heap);
int* GetElements(Heap* heap);
int IsFull(Heap *heap);
int IsEmpty(Heap *heap);
int Size(Heap *heap);

void swap(int* a, int* b){
	int t = *a;
	*a = *b;
	*b = t;
}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;
	int ret;
	int* elements;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
				ret = Insert(maxHeap, key);
				if(ret == 0) fprintf(fout, "insert %d\n", key);
				else if(ret == 1) fprintf(fout, "insert error : heap is full\n");
				else if(ret == 2) fprintf(fout, "insert error : %d is already in the heap\n", key);
				else {
					fprintf(fout, "error: unknown return type\n");
					return -1;
				} 

				break;
			case 'd':
				max_element = DeleteMax(maxHeap);
				if(max_element == 0){
					fprintf(fout, "delete error : heap is empty\n");

				}else if(max_element > 0){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
				break;
			case 'p':
				if(IsEmpty(maxHeap)){
					fprintf(fout, "print error : heap is empty\n");
				}else{
					elements = GetElements(maxHeap);
					int size = Size(maxHeap);
					for(int i = 0; elements[i] > -1 && i < size; i++){
						fprintf(fout, "%d ", elements[i]);
					}
					fprintf(fout, "\n");
					free(elements);
				}
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "finding error : %d is not in the heap\n", key);
				break;
		}
	}

	return 0;
}

/*
Create new heap with given heapSize
Element[0] should save INF
heapSize: positive integer
return:
	the pointer of new heap  
*/
Heap* CreateHeap(int heapSize)
{
	Heap	*ret;

	ret = malloc(sizeof(Heap));
	ret->Elements = malloc(sizeof(int) * (heapSize + 1));
	ret->Elements[0] = INF;
	ret->Size = 0;
	ret->Capacity = heapSize;
	return (ret);
}

/*
Insert the value into Heap
value: positive integer
return:
	0 , success
	1 , heap is full
	2 , duplicated
*/
int Insert(Heap *heap, int value)
{
	int	i;

	if (IsFull(heap))
		return (1);
	if (Find(heap, value))
		return (2);
	// 만약 i가 2면 a[2] = a[1], i가 1이면 a[0] < value a[1] = a[0] 오류! 
	// for (int i = ++heap->Size; heap->Elements[i / 2] < value; i /= 2)
	i = ++heap->Size;
	while ((i != 1) && value > heap->Elements[i / 2])
	{
		// 부모를 자식에 할당, 즉 값을 한칸씩 아래로 내려서 삽입할 공간을 찾음
		heap->Elements[i] = heap->Elements[i / 2];
		i /= 2;
	}
	heap->Elements[i] = value;
	return (0);
}

/*
Find the value in Heap
return:
	1, success
	0, fail  
*/
int Find(Heap* heap, int value)
{
	for (int i = 1; i <= heap->Size; i++)
	{
		if (heap->Elements[i] == value)
			return (1);
	}
	return (0);
}

/*
Delete the maximum value in Heap
return:
	Max value, success
	0, heap is empty  
*/
int DeleteMax(Heap* heap)
{
	int	max_elem;
	int	last_elem;
	int	parent;
	int	child;

	if (IsEmpty(heap))
		return (0);
	max_elem = heap->Elements[1];
	last_elem = heap->Elements[heap->Size--];
	parent = 1;
	while (parent * 2 <= heap->Size)
	{
		child = parent * 2;
		// 만약 오른쪽 자식 값이 더 크면 오른쪽 자식과 부모를 교환한다
		if (child < heap->Size
			&& heap->Elements[child + 1] > heap->Elements[child])
			child++;
		if (last_elem > heap->Elements[child])
			break ;
		heap->Elements[parent] = heap->Elements[child];
		parent = child;
	}
	heap->Elements[parent] = last_elem;
	return (max_elem);
}

/*
Return an array of copied elements from the heap
The length of the returned array should be the same as the size of the heap
The order of the returned array should be the same as the order of heap->Elements  
If heap is empty, return NULL
***WARNING***
Do not just return heap->Elements
You must copy the elements to another allocated array
*/
int* GetElements(Heap* heap)
{
	int	*arr;

	arr = malloc(sizeof(int) * (heap->Size));
	for (int i = 0; i < heap->Size; i++)
		arr[i] = heap->Elements[i + 1];
	return (arr);
}

/*
check heap is full
return:
	1, heap is full
	0, heap is not full
*/
int IsFull(Heap* heap)
{
	return (heap->Capacity == heap->Size);
}

/*
check heap is empty
return:
	1, heap is empty
	0, heap is not empty
*/
int IsEmpty(Heap *heap)
{
	return (heap->Size == 0);
}

/*
get size of heap
return:
	size of heap
*/
int Size(Heap *heap)
{
	return (heap->Size);
}
