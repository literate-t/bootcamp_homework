#pragma once
#include "data_struct.h"
#include <stdbool.h>

// 영원히 nullptr이 되지 않은 더미 헤드 노드를 이용해서 구조를 최적화할 수 있다
//extern USERDATA g_head_node;
//extern USERDATA g_head_node;
extern NODE g_head_node;
extern NODE g_tail_node;

extern int data_count;

// for index
extern NODE** g_age_index_ptr;

void InitList();
void InitData();
void Push(USERDATA*);
void Enqueue(USERDATA*);
void AddNewNodeAtTail(int, const char*, const char*);
NODE* Pop();
NODE* Dequeue();

void IncreaseCount();
void DecreaseCount();
void InitCount();
int GetDataCount();

void ReleaseNodeList();

NODE* SearchByName(const char*);
NODE* FindNode(const char*);
int GetCountBetween(NODE* start, NODE* end);
NODE* SearchByFirstOfAge(int age);
NODE* SearchByLastOfAge(int age);
NODE** SearchRangeByAge(int minimum_age, int maximum_age, int* array_size);

void DeleteByName(const char*);
void RemoveNode(NODE*);

void EventLoopRun();
bool IsEmpty();

// sort
void Sort(NODE**);
NODE** MemoryCopy(NODE** dest, int dest_size, NODE* start);

// for index
void MakeIndexByAge();
void UpdateIndexByAge();
void InitIndices();
void ReleaseIndex();
void ListToArray(NODE* start, NODE** array, int list_size);
USERDATA** SearchRangeByAgeIndex(int min, int max, int* array_size);
int SearchByFirstOfAgeIndex(int age, int data_size);
int SearchByLastOfAgeIndex(int age, int data_size);

//=======================================================
// Merge sort
void Split(NODE* source, NODE** front, NODE** back);

// 두 연결 리스트를 정렬하면서 병합하는 함수
NODE* Merge(NODE* first, NODE* second);
void AppendRestNode(NODE* node, NODE* rest);

// 병합 정렬을 이용한 연결 리스트 정렬 함수
void MergeSort(NODE** headRef);

// Quick sort
void Swap(NODE** first, NODE** second);
void QuickSortByAge(NODE** start_node, int low_index, int high_index);

int Query(char* sql);
char** SplitString(char* string, int* size, char delim);
void FreeSplitString(char** string, int size);