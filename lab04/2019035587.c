
#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

#define FROMPARENT 0
#define FROMTHREAD 1

typedef struct ThreadedTree* ThreadedPtr;
typedef int ElementType;

struct ThreadedTree {
	int left_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
	ThreadedPtr left_child;
	ElementType data;
	ThreadedPtr right_child;
	int right_thread; // flag if ptr is thread, 1, if it is a thread or 0, if it points to a child node
}ThreadedTree;

/*
Create a root of new Threaded Tree
Root data should be -1
return:
	the pointer of new Threaded Tree
*/
// 루트노드가 더미노드, 0번 인덱스
// left_thread와 right_thread값 초기화 
ThreadedPtr CreateTree()
{
	ThreadedPtr	root;

	root = malloc(sizeof(ThreadedTree));
	root->left_child = root;
	root->left_thread = 1;
	root->right_child = root;
	root->right_thread = 1;
	root->data = -1;
	return (root);
}

/*
Insert the key value "data" in Threaded Tree
root : Pointer to the root node of Threaded Tree
root_idx : Index of the root node of Threaded Tree, means the first node to find the insertion position
data : data key value of the node to be newly inserted,
idx : The index of the node to be newly inserted, 0 means root index
return :
	1, the insertion succeeds
	0, the insertion fails.
*/
// 현재 메인 문에서는 실패하지 않음. 만약 삽입해야 하는 idx위치가 잘못들어오면 에러 발생 가능
// 인덱스 0, 1 반드시 예외 체크
// 충분히 테스트 거칠 것 (적은 수, 큰 수, 0과 1, 서로 다른 트리 형태)
int Insert(ThreadedPtr root, int root_idx, ElementType data, int idx)
{
	ThreadedPtr	node;
	ThreadedPtr	parent_of_target;
	int			divisor;	

	node = malloc(sizeof(ThreadedTree));
	if (!node)
		return (0);
	node->data = data;
	divisor = 1;
	while (idx / divisor != 1)
		divisor *= 2;
	parent_of_target = root;
	while (divisor != 1)
	{
		// if (divisor == 2)
		// 	prev_of_parent = parent_of_target;
		if ((idx / divisor) % 2 == 1)
			parent_of_target = parent_of_target->right_child;
		else
			parent_of_target = parent_of_target->left_child;
		divisor /= 2;
	}
	if (idx % 2 == 1) // 오른쪽 자식 여부 확인
	{
		node->left_thread = 1;
		node->left_child = parent_of_target;
		node->right_thread = 1;
		node->right_child = parent_of_target->right_child;
		parent_of_target->right_thread = 0;
		parent_of_target->right_child = node;
		/* 레거시
		if ((idx & (idx + 1)) == 0) // 가장 오른쪽 위치에 삽입한 건지 판별
			node->right_child = root;
		else
			node->right_child = prev_of_parent;
		*/
	}
	else
	{
		node->right_thread = 1;
		node->right_child = parent_of_target;
		node->left_thread = 1;
		node->left_child = parent_of_target->left_child;
		parent_of_target->left_thread = 0;
		parent_of_target->left_child = node;
		/* 레거시
		if ((idx & (idx - 1)) == 0) // 가장 왼쪽 위치에 삽입한 건지 판별
			node->left_child = root;
		else
			node->left_child = prev_of_parent;
		*/
	}
	return (1);
}

// Function I added.
// 왼쪽 끝으로 이동한 후 right_child 따라서 이동 (중위 순회)
ThreadedPtr	InSucc(ThreadedPtr node)
{
	ThreadedPtr	ret;

	ret = node->right_child;
	if (!node->right_thread)
	{
		while (!ret->left_thread)
			ret = ret->left_child;
	}
	return (ret);
}

/*
In Order Traversal
Implementing in a recursive form is prohibited.
When printing out, the interval between each key is one space
print out:
	"inorder traversal : data1 data2 data3" (When there are 3 data, except root node)

*/
// 함수 하나 추가
void printInorder(ThreadedPtr root)
{
	fprintf(fout, "inorder traversal :");
	ThreadedPtr	tmp = root;
	while (1)
	{
		tmp = InSucc(tmp);
		if (tmp == root)
			break ;
		fprintf(fout, " %d", tmp->data);
	}
}

/*
Delete the Tree
*/
// TODO 이거 구현 빼먹을 뻔..
void DeleteTree(ThreadedPtr root)
{
	ThreadedPtr	tmp;
	ThreadedPtr	prev;

	tmp = root;
	while (1)
	{
		prev = tmp;
		tmp = InSucc(tmp);
		free(prev);
		if (tmp == root)
			return ;
	}
}

// void	my_inorder(ThreadedPtr node)
// {
// 	if (!node->left_thread && !node->right_thread)
// 	{
// 		my_inorder(node->left_child);
// 		printf("%d ", node->data);
// 		my_inorder(node->right_child);
// 	}
// }

int main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	ThreadedPtr root = CreateTree();

	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);

	int root_idx=0, idx=0;

	while(++idx <= NUM_NODES){
		ElementType data;
		fscanf(fin, "%d", &data);
		if(Insert(root, root_idx, data, idx) == 0){
			fprintf(fout, "Insertion failed!\n");
			return 0;
		}
	}
	printInorder(root);
	DeleteTree(root);
	
	fclose(fin);
	fclose(fout);

	return 0;
}
