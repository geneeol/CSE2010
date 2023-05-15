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
				if (root->n_key == 0) // -1로 수정(원래 1)
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
	root->child = malloc(sizeof(BNodePtr) * order); // 한칸 크게 할당
	for (int i = 0; i < order; i++)
		root->child[i] = NULL;
	root->key = malloc(sizeof(int) * order);
	memset(root->key, 0, sizeof(int) * order); // TODO: 키값 몇으로? 
	root->is_leaf = 1;
	root->n_child = -1; // TODO: 왜 사이즈가 1이지..?
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
// 현재 child1이 가득차있을 때 이걸 쪼개는 것임
// order가 3일 때, child1이 empty가 됨. 따라서 이 부분에 대한 처리 추가하면 좋을듯
// 위 경우 parent->child[idx]를 아예 없애는게 맞는듯 
void SplitChild(BNodePtr parent, int idx)
{
	BNodePtr	child1, child2;
	int			mid;

	child1 = parent->child[idx];
	child2 = CreateNode(parent->order, child1->is_leaf);
	// 여기 child2 키 개수를 랜덤하게 하는게 바람직
	// order가 4, 5 일 때 child2->n_key  각각 1, 2
	// child2->key: 
	// order = 2t -1 꼴일 때, z->n = t - 1
	child2->n_key = (child1->order + 1) / 2 - 1;
	mid = child1->order / 2 - 1; // order가 3일 때 mid는 0 
	for (int i = 0; i < child2->n_key; i++)
		child2->key[i] = child1->key[i + mid + 1];
	if (!child1->is_leaf)
	{
		for (int i = 0; i < child2->n_key + 1; i++)
			child2->child[i] = child1->child[i + mid + 1];
	}
	child1->n_key = mid; // child1키 개수는 mid개수 0 1 mid(2) 3 4 (5)
	for (int i = parent->n_key; i > idx; i--)
		parent->child[i + 1] = parent->child[i];
	parent->child[idx + 1] = child2;
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
void Insert(BNodePtr *root, int key)
{
	BNodePtr	new_root;

	if ((*root)->n_child == (*root)->order - 1)
	{
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
	static int	is_last = 0;

	for (i = 0; i < root->n_key; i++)
	{
		if (!root->is_leaf)
			PrintTree(root->child[i]);
		fprintf(fout, "%d ", root->key[i]);
	}
	if (!root->is_leaf)
	{
		is_last = 1;
		PrintTree(root->child[i]);
	}
	if (is_last)
	{
		fprintf(fout, "\n");
		is_last = 0;
	}
}

/*
Free memory, delete a BTree completely 
*/
void DeleteTree(BNodePtr root){

}
