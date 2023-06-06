#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct Node {
	int vertex;
	int dist;	//distance
	int prev;
}Node;

typedef struct Graph {
	int size;
	int** vertices;
	Node* nodes;
}Graph;

typedef struct Heap {
	int Capacity;
	int Size;
	Node* Element;
}Heap;

Graph* createGraph(int size);
void deleteGraph(Graph* g);
void dijkstra(Graph* g);
int* shortestPath(Graph* g, int dest);
Heap* createMinHeap(int heapSize);
void deleteMinHeap(Heap* minHeap);
void insertToMinHeap(Heap* minHeap, int vertex, int distance);
Node deleteMin(Heap* minHeap);

void swap(Node* node1, Node* node2) {
	Node temp = *node1;
	*node1 = *node2;
	*node2 = temp;
}

void main(int argc, char *argv[]) {
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	Graph* g;
	int size;
	fscanf(fin, "%d\n", &size);
	g = createGraph(size + 1);

	char tmp = 0;
	while (tmp != '\n' && tmp != EOF) {
		int node1, node2, weight;
		fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
		// node1 to node2으로의 간선
		g->vertices[node1][node2] = weight;
		tmp = fgetc(fin);
	}

	dijkstra(g);

	int j;
	for(int i = 2; i <= g->size; i++){
		int* path = shortestPath(g, i);
		if(path == NULL){
			fprintf(fout, "can not reach to node %d\n", i);
			continue;
		}
		for (j = g->size - 1; j > 0; j--){
			if(path[j] == 0) continue;
			fprintf(fout, "%d->", path[j]);
		} 
		fprintf(fout, "%d (cost : %d)\n", i, g->nodes[i].dist);
		free(path);
	}
	deleteGraph(g);

	// fclose(fin);
	// fclose(fout);
	// g = 0;
	// system("leaks a.out");
}


// 할당할 때부터 사이즈 + 1로 인자로 넘김
/*
Allocate Graph Matrix

Initial distance: INF except 1st node (source)
Initial prev: -1
*/
Graph* createGraph(int size) 
{
	Graph *g = malloc(sizeof(Graph));

	g->size = size - 1;
	g->vertices = malloc(sizeof(int *) * size) ;
	g->nodes = malloc(sizeof(Node) * size);
	// n + 1 만큼 할당
	for (int i = 1; i < size; i++) {
		g->vertices[i] = malloc(sizeof(int) * size);
		g->nodes[i].vertex = i;
		g->nodes[i].dist = INF;
		g->nodes[i].prev = -1;
	}
	// 인접행렬 초기화
	for (int i = 1; i < size; i++)
		for (int j = 1; j < size; j++)
			g->vertices[i][j] = 0;
	g->nodes[1].dist = 0;
	return g;
}
// deallocate graph
void deleteGraph(Graph* g)
{
	free(g->nodes);
	for (int i = 1; i <= g->size; i++)
		free(g->vertices[i]);
	free(g->vertices);
	free(g);
}

/*
Allocate min heap
*/ 
Heap* createMinHeap(int heapSize)
{
	Heap *ret;

	ret = malloc(sizeof(Heap));
	ret->Capacity = heapSize;
	ret->Size = 0;
	// 한 칸 크게 할당, 1번 인덱스부터 사용
	ret->Element = malloc(sizeof(Node) * (heapSize + 1));
	ret->Element[0].dist = -1;
	return (ret);

}
/*
Deallocate min heap
*/
void deleteMinHeap(Heap* minHeap)
{
	free(minHeap->Element);
	free(minHeap);
}

/*
Dijkstra Algorithm
*/
void dijkstra(Graph* g)
{
	Heap *minHeap;
	Node cur;

	minHeap = createMinHeap(g->size);
	insertToMinHeap(minHeap, 1, 0);
	while (minHeap->Size != 0)
	{
		cur = deleteMin(minHeap);
		// 1번 노드부터 n번 노드까지 검사
		for (int i = 1; i <= g->size; i++)
		{
			// 간선이 없으면 넘어감
			if (g->vertices[cur.vertex][i] == 0)
				continue;
			// 거리를 업데이트 해야하는 경우
			// 거리를 업데이트 한 후 힙에 push
			if (g->nodes[i].dist > cur.dist + g->vertices[cur.vertex][i])
			{
				g->nodes[i].dist = cur.dist + g->vertices[cur.vertex][i];
				g->nodes[i].prev = cur.vertex;
				insertToMinHeap(minHeap, i, g->nodes[i].dist);
			}
		}
	}
	deleteMinHeap(minHeap);
}

/*
Return shortest path from source(1) to dest(i)
Return type is array and the size is the number of node.
You should save the path reversely. 
For example, If shortest path is 1->2->3 
and the number of node(g->size) is 5,
you must return [3, 2, 1 , 0, 0]
If there is no path, return NULL 
*/
int* shortestPath(Graph* g, int dest)
{
	int *path;
	int prev;

	if (g->nodes[dest].dist == INF)
		return (NULL);
	path = malloc(sizeof(int) * g->size);
	prev = dest;
	for (int i = 0; i < g->size; i++)
	{
		if (prev != -1)
		{
			path[i] = prev;
			prev = g->nodes[prev].prev;
		}
		else
			path[i] = 0;
	}
	return (path);
}

/*
Insert Node with vertex and distance into minHeap
*/
void insertToMinHeap(Heap* minHeap, int vertex, int distance)
{
	int i;

	if (minHeap->Size == minHeap->Capacity)
	{
		// 디버깅용으로 추가
		printf("minHeap is full\n");
		return ;
	}
	for (i = ++minHeap->Size; minHeap->Element[i / 2].dist > distance; i /= 2)
		minHeap->Element[i] = minHeap->Element[i / 2];
	// prev값 아무렇게나 초기화해도 상관 없음.
	minHeap->Element[i] = (Node){vertex, distance, -10};
}

/*
pop the Node with minimum distance from minHeap
return:
	Node with minimum distance
*/
Node deleteMin(Heap *minHeap)
{
	Node min_elem;
	Node last_elem;
	int i;
	int child;

	if (minHeap->Size == 0)
	{
		// 디버깅용
		printf("minHeap is empty\n");
		return ((Node){0, 0, 0});
	}
	min_elem = minHeap->Element[1];
	last_elem = minHeap->Element[minHeap->Size--];

	for (i = 1; i * 2 <= minHeap->Size; i = child)
	{
		child = i * 2;
		if (child < minHeap->Size
			&& minHeap->Element[child + 1].dist < minHeap->Element[child].dist)
			child++;
		if (last_elem.dist > minHeap->Element[child].dist)
			minHeap->Element[i] = minHeap->Element[child];
		else
			break;
	}
	minHeap->Element[i] = last_elem;
	return (min_elem);
}
