#include<stdio.h>
#include<stdlib.h>
#include<string.h>

FILE *fin;
FILE *fout;

typedef int ElementType;
typedef ElementType List;
typedef struct HashTbl* HashTable;

typedef struct HashTbl{
	int TableSize;
	List *TheLists;
}HashTbl;

int deleted_slot[100000];

HashTable createTable(int TableSize);
void Insert(HashTable H, ElementType Key, int solution);
void Delete(HashTable H, ElementType Key, int solution);
int Find(HashTable H, ElementType Key, int solution);
void printTable(HashTable H);
void deleteTable(HashTable H);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");


	char solution_str[20];
	int solution, TableSize;

	fscanf(fin, "%s", solution_str);
	if(!strcmp(solution_str, "linear"))
		solution = 1;
	else if(!strcmp(solution_str, "quadratic"))
		solution = 2;
	else{
		fprintf(fout, "Error: Invalid hashing solution!");
		return 0;
	}
	
	fscanf(fin, "%d", &TableSize);

	HashTable H = createTable(TableSize);

	char cv;
	int key;
	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				Insert(H, key, solution);
				break;

			case 'f':
				fscanf(fin, "%d", &key);
				int result = Find(H, key, solution);
				if(result)
					fprintf(fout, "%d is in the table\n", key);
				else
					fprintf(fout, "%d is not in the table\n", key);
				break;

			case 'd':
				fscanf(fin, "%d", &key);
				Delete(H, key, solution);
				break;

			case 'p':
				printTable(H);
				break;
		}
	}

	deleteTable(H);

	return 0;
}

/*
Create new Hash Table with given TableSize
reture:
	the pointer of new hash table
*/
HashTable createTable(int TableSize)
{
	HashTable table = malloc(sizeof(HashTbl));
	table->TableSize = TableSize;
	table->TheLists = malloc(sizeof(List) * TableSize);
	for (int i = 0; i < TableSize; i++)
		table->TheLists[i] = 0;
	return (table);
}

/*
Insert the key in hash table with given solution (linear or quadratic).
print out : 
	Successful insertion : "insert %d into address %d\n"
	When inserting duplicate key value : "insertion error : %d already exists at address %d\n"
	When HashTable is full : "insertion error : table is full\n"
*/
void Insert(HashTable H, ElementType Key, int solution)
{
	int hval;
	int idx;
	int offset;

	hval = Key % H->TableSize;
	// 안전하게 곱하기 100 해줌
	// 테이블사이즈 100이하이므로 (100 * 100) ^ 2 해도 오버플로우 안남 
	// 이론상 테이블 사이즈 만큼만 반복하면 full인지 아닌지 판단 가능
	for (int i = 0; i < H->TableSize * 100; i++)
	{
		solution == 1 ? (offset = i) : (offset = i * i);
		idx = (hval + offset) % H->TableSize;
		if (H->TheLists[idx] == 0)
		{
			H->TheLists[idx] = Key;
			if (deleted_slot[idx])
				deleted_slot[idx] = 0;
			fprintf(fout, "insert %d into address %d\n", Key, idx);
			return;
		}
		if (H->TheLists[idx] == Key)
		{
			fprintf(fout, "insertion error: %d already exists at address %d\n", Key, idx);
			return;
		}
	}
	fprintf(fout, "insertion error: table is full\n");
}

// 찾으면 위치, 못찾으면 -1을 리턴하는 함수
int Find2(HashTable H, ElementType Key, int solution)
{
	int hval;
	int idx;
	int offset;

	hval = Key % H->TableSize;
	for (int i = 0; i < H->TableSize * 100; i++)
	{
		solution == 1 ? (offset = i) : (offset = i * i);
		idx = (hval + offset) % H->TableSize;
		if (deleted_slot[idx])
			continue ;
		if (H->TheLists[idx] == 0)
			return (-1);
		if (H->TheLists[idx] == Key)
			return (idx);
	}
	return (-1);
}

/*
Delete the key in hash table with given solution (linear or quadratic).
print out : 	
	when key value is found :  "delete %d\n"
	when key value does not exist : %d is not in the table\n"
*/
void Delete(HashTable H, ElementType Key, int solution)
{
	int idx;

	idx = Find2(H, Key, solution);
	if (idx == -1)
	{
		fprintf(fout, "deletion error: %d is not in the table\n", Key);
		return ;
	}
	H->TheLists[idx] = 0;
	deleted_slot[idx] = 1;
	fprintf(fout, "delete %d\n", Key);
}

/*
Find the Key from hash table with given solution (linear or quadratic).
return:
	0 : when the key is not found
	1 : when the key is found
*/
int Find(HashTable H, ElementType Key, int solution)
{
	int hval;
	int idx;
	int offset;

	hval = Key % H->TableSize;
	for (int i = 0; i < H->TableSize * 100; i++)
	{
		solution == 1 ? (offset = i) : (offset = i * i);
		idx = (hval + offset) % H->TableSize;
		if (deleted_slot[idx])
			continue ;
		if (H->TheLists[idx] == 0)
			return (0);
		if (H->TheLists[idx] == Key)
			return (1);
	}
	return (0);
}

/*
Print all values of Hash Table
print out the key values ​​in the Hash Table in the order of index in the Hash Table.
	empty Hash Table print : "0 "
	Non empty Hash Table print : "%d "
*/
void printTable(HashTable H)
{
	for (int i = 0; i < H->TableSize; i++)
		fprintf(fout, "%d ", H->TheLists[i]);
	fprintf(fout, "\n");
}

/*
delete Table 
*/
void deleteTable(HashTable H)
{
	free(H->TheLists);
	free(H);
}
