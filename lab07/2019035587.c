#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>

enum	state
{
	BROKEN = 0,
	UNBROKEN = 1,
};

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_maze;
	int *ptr_arr;
}DisjointSets;

void init(DisjointSets *sets, DisjointSets *maze_print, int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets, DisjointSets *maze_print);


int main(int argc, char* agrv[])
{
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	DisjointSets *sets, *maze_print;
	fscanf(fin, "%d", &num);

	sets = (DisjointSets*)malloc(sizeof(DisjointSets));
	maze_print = (DisjointSets*)malloc(sizeof(DisjointSets));

	init(sets, maze_print, num);
	createMaze(sets, maze_print, num);
	printMaze(maze_print, num);

	freeMaze(sets, maze_print);

	fclose(fin);
	fclose(fout);

	return 0;
}

/*
Allocate and Initialize Disjoint sets
    "sets": have num*num disjoint sets
    "maze_print": have num*num*2 values (two directions: right, down)   
        the values are either 0(no wall) or 1 (wall)
*/
void init(DisjointSets *sets, DisjointSets *maze_print, int num)
{
	int	i;

	sets->size_maze = num * num;
	sets->ptr_arr = malloc(sizeof(int) * (sets->size_maze + 1));
	// n개의 disjoint set 생성
	memset(sets->ptr_arr, 0, sizeof(int) * (sets->size_maze + 1));
	maze_print->size_maze = num * num;
	maze_print->ptr_arr = malloc(sizeof(int) * (2 * maze_print->size_maze + 1));
	for (i = 0; i < 2 * maze_print->size_maze + 1; i++)
		maze_print->ptr_arr[i] = UNBROKEN;
	maze_print->ptr_arr[num * num] = BROKEN;
}

/*
Merge two disjoint sets including i and j respectively
*/
void Union(DisjointSets *sets, int i, int j)
{
	int	root1;
	int root2;

	root1 = find(sets, i);
	root2 = find(sets, j);

	if (root1 == root2)
	{
		printf("Error: i: %d and j: %d are already in same set\n", i, j);
		return ;
	}
	// path compression 탓에 rank를 기록할 방법이 없기에 랜덤한 방향으로 합친다
	if (rand() % 2 == 0)
		sets->ptr_arr[root1] = root2;
	else
		sets->ptr_arr[root2] = root1;
}

/*
Find the set including given element "i" and return the representative element  
*/
int find(DisjointSets *sets, int i)
{
	if (sets->ptr_arr[i] <= 0)
		return (i);
	sets->ptr_arr[i] = find(sets, sets->ptr_arr[i]);
	return (sets->ptr_arr[i]);
}

int	is_boundary(int edege, int num)
{
	return ((edege <= num * num && edege % num == 0)
			|| edege > 2 * num * num - num);
}

int	all_connected(DisjointSets *sets)
{
	int	root;

	root = find(sets, 1);
	for (int i = 1; i <= sets->size_maze; i++)
	{
		if (root != find(sets, i))
			return (0);
	}
	return (1);
}

/*
Create Maze without cycle
You should generate the maze by randomly choosing cell and direction 
*/
void createMaze(DisjointSets *sets, DisjointSets *maze_print, int num)
{
	int	edge;
	int	root1;
	int	root2;

	// while (find(sets, 1) != find(sets, sets->size_maze))
	while (1)
	{
		// 시간복잡도를 줄이기 위해 shortcut circuit evaluation 활용
		// 항상 앞의 조건식부터 평가하여 앞의 조건이 달성될 때까지 뒤 조건식을 평가하지 않음
		if (find(sets, 1) == find(sets, sets->size_maze)
			&& all_connected(sets))
			break ;
		edge = rand() % (2 * num * num + 1);
		if (is_boundary(edge, num) || maze_print->ptr_arr[edge] == BROKEN)
			continue ;
		if (edge <= num * num) // x축과 수직인 벽, 벽을 기준으로 양옆 두점 union
		{
			root1 = find(sets, edge);
			root2 = find(sets, edge + 1);
		}
		else // x축과 평행한 벽, 벽을 기준으로 위아래 두점 union
		{
			root1 = find(sets, edge - num * num);
			root2 = find(sets, edge - num * num + num);
		}
		if (root1 == root2)
			continue ;
		Union(sets, root1, root2);
		maze_print->ptr_arr[edge] = BROKEN;
	}
}

/*
Print Maze
Example> 
+---+---+---+---+---+
            |       |
+   +   +---+   +---+
|   |       |       |
+---+   +---+   +   +
|               |   |
+   +---+   +   +---+
|   |       |       |
+   +   +---+---+---+
|   |                
+---+---+---+---+---+
*/
void printMaze(DisjointSets *maze_print, int num)
{
	char	ch;
	char	*str;
	int		*edges;

	edges = maze_print->ptr_arr;
	
	// +---+---+ ... 맨 윗부분 테두리 그리기
	fprintf(fout, "+");
	for (int i = 1; i <= num; i++)
		fprintf(fout, "---+");
	fprintf(fout, "\n");

	// |와 ---로 구분되는 총 2n개의 행존재
	// n번 반복하는 동안 수직벽과 수평벽에 해당하는 행 각각 그리기
	for (int i = 0; i < num; i++)
	{
		ch = (i == 0) ? ' ' : '|';
		fprintf(fout, "%c   ", ch); // 가장왼쪽 가장자리는 맨 첫번째 칸만 뚫려있다
		for (int j = 1; j <= num; j++)
		{
			ch = edges[i  * num + j] == BROKEN ? ' ' : '|';
			j == 1 ? fprintf(fout, "%c", ch) : fprintf(fout, "   %c", ch);
		}
		fprintf(fout, "\n+");
		for (int j = 1; j <= num; j++)
		{
			str = edges[num * num + i * num + j] == BROKEN ? "   " : "---";
			fprintf(fout, "%s+", str);
		}
		fprintf(fout, "\n");
	}
}

/*
Deallocate sets
*/
void freeMaze(DisjointSets *sets, DisjointSets * maze_print)
{
	free(sets->ptr_arr);
	free(sets);
	free(maze_print->ptr_arr);
	free(maze_print);
}
