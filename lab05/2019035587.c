#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct BST* Tree;
typedef struct BST{
	int value;
	struct BST* left;
	struct BST* right;
}BST;

Tree insertNode(Tree root, int key);
Tree deleteNode(Tree root, int key);
int findNode(Tree root, int key);
void printInorder(Tree root);
void deleteTree(Tree root);

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	Tree root = NULL;

	// TODO: 우분투에선 feof 동작 다른지 검사 (맥에서는 pi가 마지막 줄이면 insertion을 한번 더함)
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				if(!findNode(root, key))
					root = insertNode(root, key);
				else
					fprintf(fout, "insertion error: %d is already in the tree\n", key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key))
					fprintf(fout, "%d is in the tree\n", key);
				else
					fprintf(fout, "finding error: %d is not in the tree\n", key);
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
					fprintf(fout, "delete %d\n", key);
				}
				else{
					fprintf(fout, "deletion error: %d is not in the tree\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL)
						fprintf(fout, "tree is empty");
					else
						printInorder(root);
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
	//system("leaks a.out");
}

/*
Insert the value "key" in Tree "root"
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to insert.
return:
	pointer of root
print out:
	"insert key\n"
*/
Tree insertNode(Tree root, int key)
{
	if (root == NULL)
	{
		root = malloc(sizeof(BST));
		root->value = key;
		root->left = NULL;
		root->right = NULL;
		fprintf(fout, "insertion %d\n", key);
	}
	else if (key < root->value)
		root->left = insertNode(root->left, key); // 삽입하는 노드를 제외하고는 자기자신을 반환 후 할당한다
	else if (key > root->value)
		root->right = insertNode(root->right, key);
	return (root);
}

// delete를 위한 보조 함수, 오른쪽 서브트리의 최솟값을 이용한다.
Tree FindMin(Tree root)
{
	if (root == NULL)
		return (NULL);
	else if (root->left == NULL)
		return (root);
	else
		return (FindMin(root->left));
}

/*
Delete the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the new node to be deleted.
return :
	pointer of root
*/
Tree deleteNode(Tree root, int key){
	BST	*tmp;

	if (key < root->value)
		root->left = deleteNode(root->left, key);
	else if (key > root->value)
		root->right = deleteNode(root->right, key);
	else if (root->left && root->right) // 자식 노드가 두개인 경우
	{
		tmp = FindMin(root->right);
		root->value = tmp->value; // 루트 노드값을 오른쪽 서브트리의 최솟값으로 덮어쓰고
		root->right = deleteNode(root->right, tmp->value); // 오른쪽 서브트리에서 해당값 삭제를 재귀적으로 호출, 반환값은 오른쪽 서브트리의 루트
	}
	else // 
	{
		tmp = root;
		if (root->left == NULL)
			root = root->right; // 만약 오른쪽 자식이 널이면 그냥 널 리턴
		else if (root->right == NULL) // 왼쪽 자식이 널이 아니면 반드시 오른쪽 자식은 널임
			root = root->left; //왼쪽 서브트리의 루트를 반환, 이것이 삭제하는 노드를 새로 대체하게 됨
		free(tmp);
	}
	return (root);
}

/*
Find the value key in Tree root
Tree root : A pointer to the root node of the binary search tree.
int key : The value of the node trying to find.
return :
	1, key is found
	0, key is not found
*/
int findNode(Tree root, int key){
	if (root == NULL)
		return (0);
	else if (key < root->value)
		return (findNode(root->left, key));
	else if (key > root->value)
		return (findNode(root->right, key));
	else
		return (1); // root가 널도 아니고 대소구분이 안되면 키를 찾음
}

/*
In Order Traversal
Tree root : A pointer to the root node of the binary search tree.
print out:
	"root->value "
*/
void printInorder(Tree root)
{
	if (root != NULL)
	{
		printInorder(root->left);
		fprintf(fout, "%d ", root->value);
		printInorder(root->right);
	}
}

/*
Delete Tree
free allocated memory
Tree root : A pointer to the root node of the binary search tree.
*/
void deleteTree(Tree root)
{
	if (root != NULL)
	{
		deleteTree(root->left);
		deleteTree(root->right);
		free(root);
	}
}

/* 
명세 출력 포맷 정정 전, 맞추기 위해 만들었던 레거시
void printInorderFormater(Tree root, int *is_first)
{
    if (root != NULL)
	{
        printInorderFormater(root->left, is_first);
        if (*is_first)
		{
            fprintf(fout, "%d", root->value);
            *is_first = 0;
        }
		else
            fprintf(fout, " %d", root->value);
        printInorderFormater(root->right, is_first);
    }
}
/*
In Order Traversal
Tree root : A pointer to the root node of the binary search tree.
print out:
	"root->value "
void printInorder(Tree root)
{
    static int is_first = 1;
    printInorderFormater(root, &is_first);
    is_first = 1;
}
*/
