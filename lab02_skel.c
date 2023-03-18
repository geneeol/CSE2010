#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct Node *PtrToNode;
typedef PtrToNode List;
typedef PtrToNode Position;
typedef int ElementType;

struct Node{
	ElementType element;
	Position next;
};

List MakeEmpty(List L);
int IsEmpty(List L);
int IsLast(Position P, List L);
void Insert(ElementType X, List L, Position P);
Position FindPrevious(ElementType X, List L);
Position Find(ElementType X, List L);
void Delete(ElementType X, List L);
void DeleteList(List L);
int* GetElements(List L);
int ListSize=0;

int main(int argc, char **argv){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");
	char x;
	int* elements;

	Position header=NULL, tmp=NULL;
	header = MakeEmpty(header);
	while(fscanf(fin,"%c",&x)!=EOF){
		if(x=='i'){
			int a,b;	fscanf(fin,"%d%d",&a,&b);
			tmp = Find(a, header);
			if(tmp!=NULL){
				fprintf(fout, "insertion(%d) failed : the key already exists in the list\n", a);
				continue;
			}
			tmp = Find(b, header);
			if(tmp==NULL){
				fprintf(fout, "insertion(%d) failed : can not find the location to be inserted\n", a);
				continue;
			}
			Insert(a, header, tmp);
		}
		else if(x=='d'){
			int a;	fscanf(fin,"%d",&a);
			tmp = Find(a, header);
			if(tmp==NULL){
				fprintf(fout, "deletion(%d) failed : element %d is not in the list\n", a, a);
				continue;
			}
			Delete(a, header);
		}
		else if(x=='f'){
			int a;	fscanf(fin,"%d",&a);
			tmp = FindPrevious(a, header);
			if(IsLast(tmp, header)) fprintf(fout, "finding(%d) failed : element %d is not in the list\n",a,a);
			else{
				if(tmp->element>0) fprintf(fout, "key of the previous node of %d is %d\n", a, tmp->element);
				else fprintf(fout, "key of the previous node of %d is head\n",a);
			}
		}
		else if(x=='p') {
			if(IsEmpty(header))
				fprintf(fout, "empty list!\n");
			else
			{
				elements = GetElements(header);
				for(int i = 0; i < ListSize; i++){
						fprintf(fout, "key:%d ", elements[i]);
					}
					fprintf(fout, "\n");
				free(elements);
			}
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);

	return 0;
}

List MakeEmpty(List L){
/*
Make new header
element should be -1
return:
	the pointer of new list
*/
}

int IsEmpty(List L){
/*
Check if list L is empty or not
return:
	1, list is full
	0, list is not full
*/
}

int IsLast(Position P, List L){
/*
Check if Position P is last or not
return:
	1, list is full
	0, list is not full
*/
}

void Insert(ElementType X, List L, Position P){
/*
Insert X in position P of list L
*/
}

int* GetElements(List L){
/*
Return an array of copied elements from the List L
The length of the returned array should be the same as the size of the ListSize
The order of the returned array should be the same as the order of L->element  
If List is empty, return NULL
***WARNING***
You must copy the elements to another allocated array
*/
}

Position FindPrevious(ElementType X, List L){
/*
Find the previous node of the node with the given element X in List L
return:
	the pointer of previous node of node that has element X  
*/
}

Position Find(ElementType X, List L){
/*
Find the node with the given element X in List L
return:
	the pointer of node has element X
*/
}

void Delete(ElementType X, List L){
/*
Delete the node that has element X in List L
*/
}

void DeleteList(List L){
/*
Delete the List
*/
}