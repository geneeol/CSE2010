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
	(void)argc;
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

/*
Make new header
element should be -1
return:
	the pointer of new list
*/
List MakeEmpty(List L)
{
	List	ret;

	(void)L;
	ret = malloc(sizeof(List));
	if (ret)
	{
		ret->next = NULL;
		ret->element = -1;
	}
	return (ret);
}

/*
Check if list L is empty or not
return:
	1, list is full
	0, list is not full
*/
int IsEmpty(List L)
{
	return (L->next == NULL);
}

/*
Check if Position P is last or not
return:
	1, list is full
	0, list is not full
*/
int IsLast(Position P, List L)
{
	(void)L;
	return (P->next == NULL);
}

/*
Insert X in position P of list L
*/
void Insert(ElementType X, List L, Position P)
{
	Position	node;

	(void)L;
	node = malloc(sizeof(PtrToNode));
	if (!node)
		return ;
	node->element = X;
	node->next = P->next;
	P->next = node;
	ListSize++;
}

/*
Return an array of copied elements from the List L
The length of the returned array should be the same as the size of the ListSize
The order of the returned array should be the same as the order of L->element  
If List is empty, return NULL
***WARNING***
You must copy the elements to another allocated array
*/
int* GetElements(List L)
{
	int			*arr;
	Position	cur;

	arr = malloc(sizeof(int) * ListSize);
	cur = L->next;
	for (int i = 0; i < ListSize; i++)
	{
		arr[i] = cur->element;
		cur = cur->next;
	}
	return (arr);
}

/*
Find the previous node of the node with the given element X in List L
return:
	the pointer of previous node of node that has element X  
*/
Position FindPrevious(ElementType X, List L)
{
	Position	cur;

	cur = L;
	while (cur->next != NULL && cur->next->element != X)
		cur = cur->next;
	return (cur);

}

/*
Find the node with the given element X in List L
return:
	the pointer of node has element X
*/
Position Find(ElementType X, List L)
{
	Position	cur;

	cur = L;
	while (cur != NULL && cur->element != X)
		cur = cur->next;
	return (cur);
}

/*
Delete the node that has element X in List L
*/
// FindPrevious를 통해 간단히 구현할 수 있다
void Delete(ElementType X, List L)
{
	Position	prev, target;

	prev = FindPrevious(X, L);
	if (!IsLast(prev, L))
	{
		target = prev->next;
		prev->next = target->next;
		free(target);
		ListSize--;
	}
}

/*
Delete the List
*/
// DeleteList는 모든 노드를 삭제해야하므로 헤더도 삭제하도록 처리 (마지막에만 호출하므로)
void DeleteList(List L)
{
	Position	cur, tmp;

	cur = L;
	while (cur != NULL)
	{
		tmp = cur;
		cur = cur->next;
		free(tmp);
	}
	ListSize = 0;
}