#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

struct AVLNode;
typedef struct AVLNode *Position;
typedef struct AVLNode *AVLTree;
typedef int ElementType;

typedef struct AVLNode{
	ElementType element;
	AVLTree left, right;
	int height;
}AVLNode;

/*** Utils ***/

int	Max(int a, int b)
{
	return (a > b) ? a : b;
}

int	Height(Position T)
{
	if (T == NULL)
		return -1;
	else
		return T->height;
}

/*
Rotation functions for AVLTree
*/
Position SingleRotateWithLeft(Position node) // 왼쪽 노드가 루트가 되게끔 오른쪽 방향으로 회전하는 함수
{
	Position tmp = node->left; // 새로운 루트노드가 루트의 왼쪽

	node->left = tmp->right;
	tmp->right = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
	tmp->height = Max(Height(tmp->left), node->height) + 1;
	return tmp;
}

Position SingleRotateWithRight(Position node)
{
	Position tmp = node->right;

	node->right = tmp->left;
	tmp->left = node;
	node->height = Max(Height(node->left), Height(node->right)) + 1;
	tmp->height = Max(Height(tmp->right), node->height) + 1;
	return tmp;
}

Position DoubleRotateWithLeft(Position node)
{
	node->left = SingleRotateWithRight(node->left); // node->left = node->left->right 가 된다. (node->left가 루트고, 루트의 오른쪽 노드가 루트가 되게끔 왼쪽 회전)
	return SingleRotateWithLeft(node); // 루트의 왼쪽 노드가 루트가 되게끔 오른쪽 회전 
}

Position DoubleRotateWithRight(Position node)
{
	node->right = SingleRotateWithLeft(node->right);
	return SingleRotateWithRight(node);
}

/************************************/


/*
Insert the value X in AVLTree T
return:
	pointer of root
print out:
	"insertion error : X is already in the tree!\n" , X is already in T
*/
AVLTree Insert(ElementType X, AVLTree T)
{
	if (T == NULL)
	{
		T = malloc(sizeof(AVLNode));
		T->element = X;
		T->left = NULL;
		T->right = NULL;
		T->height = 0;
	}
	else if (X < T->element)
	{
		T->left = Insert(X, T->left);
		if (Height(T->left) - Height(T->right) == 2)
		{
			if (X < T->left->element)
				T = SingleRotateWithLeft(T);
			else
				T = DoubleRotateWithLeft(T);
		}
	}
	else if (X > T->element)
	{
		T->right = Insert(X, T->right);
		if (Height(T->right) - Height(T->left) == 2)
		{
			if (X > T->right->element)
				T = SingleRotateWithRight(T);
			else
				T = DoubleRotateWithRight(T);
		}
	}
	else
		fprintf(fout, "insertion error : %d is already in the tree!\n", X);
	T->height = Max(Height(T->left), Height(T->right)) + 1;
	return T;
}

Position Findmin(AVLTree T)
{
	if (T == NULL)
		return NULL;
	else if (T->left == NULL)
		return T;
	else
		return Findmin(T->left);
}

// 현재 노드를 반환하기전, 현재 노드 기준으로 트리 불균형이 있는지 검사
// delete함수가 재귀적으로 호출되기에 삭제한 노드의 부모노드부터 루트노드까지 전부 검사하게 됨
Position avl_rebalancing(Position T)
{
	if (T == NULL)
		return (T);
	if (Height(T->left) - Height(T->right) == 2)
	{
		if (Height(T->left->left) - Height(T->left->right) == 1)
			T = SingleRotateWithLeft(T);
		else
			T = DoubleRotateWithLeft(T);
	}
	else if (Height(T->right) - Height(T->left) == 2)
	{
		if (Height(T->right->right) - Height(T->right->left) == 1)
			T = SingleRotateWithRight(T);
		else
			T = DoubleRotateWithRight(T);
	}
	return (T);
}

/*
Delete the value X in AVLTree T
return:
	pointer of root
print out:
	"deletion error : X is not in the tree!\n", X is not in T
*/
AVLTree Delete(ElementType X, AVLTree T)
{
	Position	tmp;

	if (T == NULL)
	{
		fprintf(fout, "deletion error : %d is not in the tree!\n", X);
		return (T);
	}
	if (X < T->element)
		T->left = Delete(X, T->left);
	else if (X > T->element)
		T->right = Delete(X, T->right);
	else if (T->right && T->left)
	{
		tmp = Findmin(T->right);
		T->element = tmp->element;
		T->right = Delete(tmp->element, T->right);
	}
	else
	{
		tmp = T;
		if (T->left == NULL)
			T = T->right;
		else if (T->right == NULL)
			T = T->left;
		free(tmp);
	}
	if (T == NULL)
		return (T);
	T->height = Max(Height(T->left), Height(T->right)) + 1;
	return (avl_rebalancing(T));
}

/*
Pre order Traversal
*/
void PrintPreorder(AVLTree T)
{
	if (T == NULL)
		return;
	fprintf(fout, "%d(%d) ", T->element, T->height);
	PrintPreorder(T->left);
	PrintPreorder(T->right);
}

/*
Delete Tree
free allocated memory
*/
void DeleteTree(AVLTree T)
{
	if (T == NULL)
		return;
	DeleteTree(T->left);
	DeleteTree(T->right);
	free(T);
}

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	AVLTree Tree = NULL;
	char cv;
	int key;

	int i=0;

	while(!feof(fin)){
		printf("%d: ", ++i);
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d\n", &key);
				Tree = Insert(key, Tree);
				break;
			case 'd':
				fscanf(fin, "%d\n", &key);
				Tree = Delete(key, Tree);
				break;
		}
		PrintPreorder(Tree);
		fprintf(fout, "\n");
		printf("\n");
	}

	DeleteTree(Tree);
	fclose(fin);
	fclose(fout);
	return 0;
}

// void do_test1()
// {
// 	AVLTree Tree = NULL;
// 	char	str[10];
// 	int		arr[10] = {30, 50, 40, 20, 60, 10, 80, 70, 90, 100};

// 	memset(str, 'i', sizeof(str));
// 	for (int i = 0; i < 10; i++)
// 	{
// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }

// void do_test2()
// {
// 	AVLTree Tree = NULL;
// 	char	str[12];
// 	int		arr[12] = {40, 20, 60, 10, 30, 50, 70, 35, 65, 65, 50, 20};

// 	memset(str, 'i', sizeof(str));
// 	str[9] = 'd';
// 	str[10] = 'd';
// 	str[11] = 'd';
// 	for (int i = 0; i < 12; i++)
// 	{
// 		// printf("str[%d]: %c\n", i, str[i]);

// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }

// void do_test3()
// {
// 	AVLTree Tree = NULL;
// 	char	str[13];
// 	int		arr[13] = {60, 40, 80, 20, 50, 70, 90, 10, 30, 100, 20, 100, 60};

// 	memset(str, 'i', sizeof(str));
// 	str[10] = 'd';
// 	str[11] = 'd';
// 	str[12] = 'd';
// 	for (int i = 0; i < 13; i++)
// 	{
// 		// printf("str[%d]: %c\n", i, str[i]);

// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }


// void do_test4()
// {
// 	AVLTree Tree = NULL;
// 	char	str[17];
// 	int		arr[17] = {25, 15, 50, 10, 22, 35, 70, 4, 12, 18, 24, 31, 44, 66, 90, 30, 46};

// 	memset(str, 'i', sizeof(str));
// 	for (int i = 0; i < 17; i++)
// 	{
// 		// printf("str[%d]: %c\n", i, str[i]);

// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }

// void do_test5()
// {
// 	AVLTree Tree = NULL;
// 	char	str[19];
// 	int		arr[19] = {50, 25, 75, 15, 35, 60, 90, 10, 30, 55, 70, 5, 80, 95, 40, 45, 5, 90, 25}; 

// 	memset(str, 'i', sizeof(str));
// 	str[16] = 'd';
// 	str[17] = 'd';
// 	str[18] = 'd';
// 	for (int i = 0; i < 19; i++)
// 	{
// 		// printf("str[%d]: %c\n", i, str[i]);

// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }

// void do_test6()
// {
// 	AVLTree Tree = NULL;
// 	char	str[16];
// 	int		arr[16] = {60, 30, 90, 20, 40, 70, 100, 10, 50, 80, 110, 120, 10, 30, 50, 110};

// 	memset(str, 'i', sizeof(str));
// 	str[12] = 'd';
// 	str[13] = 'd';
// 	str[14] = 'd';
// 	str[15] = 'd';
// 	for (int i = 0; i < 16; i++)
// 	{
// 		// printf("str[%d]: %c\n", i, str[i]);

// 		if (str[i] == 'i')
// 			Tree = Insert(arr[i], Tree);
// 		else if (str[i] == 'd')
// 			Tree = Delete(arr[i], Tree);
// 		PrintPreorder(Tree);
// 		printf("\n");
// 	}
// 	printf("==================\n\n\n");
// }


// int	main(void)
// {
// 	fout = stdout;
// 	char cv;
// 	int	i;
// 	do_test1();
// 	do_test2();
// 	do_test3();
// 	do_test4();
// 	do_test5();
// 	do_test6();
// 	return (0);
// }
