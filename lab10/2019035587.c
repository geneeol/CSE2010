#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef struct _Queue {
	int* key;
	int first, rear, qsize, max_queue_size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	int** matrix;
}Graph;

int	node_to_idx[100];

Graph* CreateGraph(int* nodes, int n);
void InsertEdge(Graph* G, int a, int b);
void DeleteGraph(Graph* G);
int* Topsort(Graph* G);
Queue* MakeNewQueue(int X);
int IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);
void countInput(int* n, char* str);
void parseInput(int* arr, char* str, int n);

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	char numbers[100], inputs[150];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers); // 정점 개수 n에 반환
	int* nodes = (int*)malloc(sizeof(int)*n);
	int* sorted_arr = NULL;
	parseInput(nodes, numbers, n);

	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	// PrintGraph(G);
	fprintf(fout, "%-3s", " ");
	for (i = 0; i < G->size; i++) fprintf(fout, "%-3d", G->node[i]);
	fprintf(fout, "\n");
	for (i = 0; i < G->size; i++) {
		fprintf(fout, "%-3d", G->node[i]);
		for (j = 0; j < G->size; j++) fprintf(fout, "%-3d", G->matrix[i][j]);
		fprintf(fout, "\n");
	}
	fprintf(fout, "\n");

	sorted_arr = Topsort(G);
	if(sorted_arr == NULL){
		fprintf(fout, "sorting error : cycle!");
	}else{
		for(i = 0; i < G->size; i ++)
			fprintf(fout, "%d ", sorted_arr[i]);
		fprintf(fout, "\n");
	}
	free(sorted_arr);
	// 추가한 줄
	free(nodes);
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);
	// system("leaks a.out");

	return 0;
}

/*
Create new graph with given nodes and the count of nodes.
saved nodes should be sorted.
return: 
        the pointer of new graph
 */
Graph* CreateGraph(int* nodes, int n)
{
	Graph* g = malloc(sizeof(Graph));
	int	tmp;

	g->node = malloc(sizeof(int) * n);
	for (int i = 0; i < n; i++)
		g->node[i] = nodes[i];
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n - i - 1; j++)
		{
			if (g->node[j] > g->node[j + 1])
			{
				tmp = g->node[j];
				g->node[j] = g->node[j + 1];
				g->node[j + 1] = tmp;
			}
		}
	}
	for (int i = 0; i < n; i++)
		node_to_idx[g->node[i]] = i;

	g->size = n;
	g->matrix = malloc(sizeof(int*) * n);
	for (int i = 0; i < n; i++)
		g->matrix[i] = malloc(sizeof(int) * n);
	for (int y = 0; y < n; y++)
		for (int x = 0; x < n; x++)
			g->matrix[y][x] = 0;
	return (g);
}

/*
Insert edge from a to b in the graph.
*/
void InsertEdge(Graph* G, int a, int b)
{
	G->matrix[node_to_idx[a]][node_to_idx[b]] = 1;
}
/*
Deallocate the memory of the graph G.
*/
void DeleteGraph(Graph* G)
{
	free(G->node);
	for (int i = 0; i < G->size; i++)
		free(G->matrix[i]);
	free(G->matrix);
	free(G);
}

void mark_in_degree(Graph *G, int *in_degree)
{
	int i, j;
	for (i = 0; i < G->size; i++)
	{
		// i번째 노드의 in_degree 계산
		in_degree[i] = 0;
		for (j = 0; j < G->size; j++)
			if (G->matrix[j][i] == 1)
				in_degree[i]++;
	}
}

/*
Topological sort
If two or more nodes have same priority,
    the node with the smaller number takes precedence.
return:
    the array of sorted
    NULL if the graph has a cycle
*/
int* Topsort(Graph* G)
{
	// 넉넉하게 두배 크기로
	Queue	*q;
	int	*sorted_arr;
	int	in_degree[100];
	int	i;
	
	q = MakeNewQueue(100);
	sorted_arr = malloc(sizeof(int) * G->size);
	mark_in_degree(G, in_degree);
	for (int i = 0; i < G->size; i++)
	{
		if (in_degree[i] == 0)
			Enqueue(q, G->node[i]);
	}
	i = 0;
	while (!IsEmpty(q))
	{
		int		node = Dequeue(q);
		sorted_arr[i++] = node;
		int		idx = node_to_idx[node];
		for (int i = 0; i < G->size; i++)
		{
			if (G->matrix[idx][i] == 1)
			{
				in_degree[i]--;
				if (in_degree[i] == 0)
					Enqueue(q, G->node[i]);
			}
		}
	}
	DeleteQueue(q);
	if (i == G->size)
		return (sorted_arr);
	else
	{
		free(sorted_arr);
		return (NULL);
	}
}

/*
Create new Queue with maximum size X
return:
        the pointer of new queue
*/
Queue* MakeNewQueue(int X)
{
	Queue* q = malloc(sizeof(Queue));
	q->key = malloc(sizeof(int) * X);
	q->first = 0;
	q->rear = 0;
	q->qsize = 0;
	q->max_queue_size = X;
	return (q);
}
/*
Check the queue either is empty or not.
return:
        1, empty
        0, non-empty
*/
int IsEmpty(Queue* Q)
{
	return (Q->qsize == 0);
}
/*
pop the front key in queue
return:
        the front key in queue
*/
int Dequeue(Queue* Q)
{
	Q->qsize--;
	return (Q->key[Q->first++]);
}
/*
push the key in queue
*/
void Enqueue(Queue* Q, int X)
{
	Q->key[Q->rear++] = X;
	Q->qsize++;
}

/*
Deallocate the memory of the queue
*/
void DeleteQueue(Queue* Q)
{
	free(Q->key);
	free(Q);
}

// 추가된 함수 
void countInput(int* n, char* str)
{
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10)
			(*n)++;
}

void parseInput(int* arr, char* str, int n)
{
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10)
			arr[cnt++] = str[i] - '0';
}
