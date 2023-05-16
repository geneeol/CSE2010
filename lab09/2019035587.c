#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct BNode* BNodePtr;

struct BNode{
	int order;
	int n_child;           /* number of children */
	BNodePtr *child;    /* children pointers */
	int *key;           /* keys */
	int	n_key;			// 추가한 맴버
	int is_leaf;
}BNode;

BNodePtr CreateTree(int order);

void Insert(BNodePtr *root, int key);

int Find(BNodePtr root, int key);

void PrintTree(BNodePtr root);

void DeleteTree(BNodePtr root);

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	int order;
	fscanf(fin, "%d", &order);
	BNodePtr root = CreateTree(order);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(Find(root, key))
					fprintf(fout, "insert error : key %d is already in the tree!\n", key);
				else
					Insert(&root, key);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(root, key))
					fprintf(fout, "key %d found\n", key);
				else
					fprintf(fout, "finding error : key %d is not in the tree!\n", key);
				break;
			case 'p':
				if (root->n_key == 0) // size(n_child) 대신 n_key로 수정
					fprintf(fout, "print error : tree is empty!");
				else
					PrintTree(root);
				fprintf(fout, "\n");
				break;
		}
	}
   
	DeleteTree(root);
	fclose(fin);
	fclose(fout);

	// 추가
	// root = 0;
	// system("leaks a.out");

	return 0;
}

/*
Create new BTree with given order
order: order of BTree (order >= 2)
return: 
		the pointer of new BTree
 */
BNodePtr CreateTree(int order)
{
	BNodePtr	root;

	root = malloc(sizeof(BNode));
	root->order = order;
	root->child = malloc(sizeof(BNodePtr) * order); // 0번 인덱스부터 사용
	for (int i = 0; i < order; i++)
		root->child[i] = NULL;
	root->key = malloc(sizeof(int) * order); // 0번 인덱스부터 사용, 한칸 크게 할당
	memset(root->key, 0, sizeof(int) * order);
	root->is_leaf = 1;
	root->n_child = -1; // 추후 0으로 변경해도 될듯
	root->n_key = 0;
	return (root);
}

// 추가한 함수
BNodePtr CreateNode(int order, int is_leaf)
{
	BNodePtr	node;

	node = malloc(sizeof(BNode));
	node->order = order;
	node->child = malloc(sizeof(BNodePtr) * order);
	for (int i = 0; i < order; i++)
		node->child[i] = NULL;
	node->key = malloc(sizeof(int) * order);
	memset(node->key, 0, sizeof(int) * order);
	node->is_leaf = is_leaf;
	node->n_child = -1;
	node->n_key = 0;

	return node;
}

// split parent->child[idx] into two nodes
// 현재 child1 key가 가득차있을 때 이걸 쪼개는 것임
// order가 3일 때, child1이 empty가 됨. (child1 empty, child2 1개, mid key 위로 올라감)
// 위 경우에 대한 별도 처리가 추가되면 좋을듯
void SplitChild(BNodePtr parent, int idx)
{
	BNodePtr	child1, child2;
	int			mid;

	child1 = parent->child[idx];
	child2 = CreateNode(parent->order, child1->is_leaf);
	// 여기 child2 키 개수를 랜덤하게 하는게 바람직
	// 현재는 order가 4, 5 일 때 child2->n_key  각각 1, 2
	child2->n_key = (child1->order + 1) / 2 - 1;
	// 짝수 차수일 경우 mid는 항상 정중앙, 홀수 차수일경우 mid는 정중앙의 왼쪽
	mid = child1->order / 2 - 1; // order가 3일 때 mid는 0 
	for (int i = 0; i < child2->n_key; i++)
		child2->key[i] = child1->key[i + mid + 1]; // child2 키에 child1 정중앙 오른쪽에 있는 키 옮기기
	// child1이 leaf가 아닐 경우 child1의 child를 child2의 child로 옮기기
	if (!child1->is_leaf)
	{
		for (int i = 0; i < child2->n_key + 1; i++)
			child2->child[i] = child1->child[i + mid + 1];
	}
	child1->n_key = mid; // child1키 개수는 mid개수 0 1 mid(2) 3 4 (5)
	// 부모에 child2 추가하기 위해 child배열 한칸씩 shift
	for (int i = parent->n_key; i > idx; i--)
		parent->child[i + 1] = parent->child[i];
	parent->child[idx + 1] = child2;
	// 부모에 mid key 추가하기 위해 key배열 한칸씩 shift
	for (int i = parent->n_key - 1; i >= idx; i--)
		parent->key[i + 1] = parent->key[i];
	parent->key[idx] = child1->key[mid];
	parent->n_key++;
}

void insert_nonfull(BNodePtr node, int key)
{
	int i;

	i = node->n_key - 1; // 제일 오른쪽 원소
	if (node->is_leaf)
	{
		while (i >= 0 && key < node->key[i])
		{
			node->key[i + 1] = node->key[i];
			i--;
		}
		node->key[i + 1] = key;
		node->n_key++;
	}
	else
	{
		while (i >= 0 && key < node->key[i])
			i--;
		i++;
		if (node->child[i]->n_key == node->order - 1)
		{
			SplitChild(node, i);
			if (key > node->key[i])
				i++;
		}
		insert_nonfull(node->child[i], key);
	}
}

/*
Insert the key value into BTree 
key: the key value in BTree node 
*/

// preemptive split을 적용함.
// 짝수일 때는 항상 최적, 홀수일 때는 랜덤하게 split하는게 바람직하나, 구현의 편의성을 위해
// 항상 오른쪽 자식 키개수가 더 많도록 split함
void Insert(BNodePtr *root, int key)
{
	BNodePtr	new_root;

	// top-down으로 내려가며 현재 노드의 key배열이 꽉 차있으면 split을 진행한다.
	// 그외는 insert_nonfull을 통해 삽입한다.
	if ((*root)->n_key == (*root)->order - 1)
	{
		// 새 노드 동적할당후 기존 root를 split
		new_root = CreateNode((*root)->order, 0);
		new_root->child[0] = *root;
		new_root->n_child = 1; 
		*root = new_root;
		SplitChild(new_root, 0);
		insert_nonfull(new_root, key);
	}
	else
		insert_nonfull(*root, key);
}

/*
Find node that has key in BTree
key: the key value in BTree node 
*/
int Find(BNodePtr root, int key)
{
	int	i;

	i = 0;
	while (i < root->n_key && key > root->key[i])
		i++;
	if (i < root->n_key && key == root->key[i])
		return 1;
	if (root->is_leaf)
		return 0;
	else
		return Find(root->child[i], key);
}

/* 
Print Tree, inorder traversal 
*/
void PrintTree(BNodePtr root)
{
	int 		i;

	for (i = 0; i < root->n_key; i++)
	{
		if (!root->is_leaf)
			PrintTree(root->child[i]);
		fprintf(fout, "%d ", root->key[i]);
	}
	if (!root->is_leaf)
		PrintTree(root->child[i]);
}

/*
Free memory, delete a BTree completely 
*/
void DeleteTree(BNodePtr root)
{
	for (int i = 0; i < root->n_key + 1; i++)
	{
		if (!root->is_leaf)
			DeleteTree(root->child[i]);
	}
	free(root->child);
	free(root->key);
	free(root);
}
