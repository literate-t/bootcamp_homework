#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>
#include <cstdbool>

#include "list.h"

USERDATA head_node = { -1, "{Head}", "{}" };
USERDATA tail_node = { -1, "{Tail}", "{}" };

NODE g_head_node = { false, nullptr, &head_node, 0, 0};
NODE g_tail_node = { false, nullptr, &tail_node, 0, 0 };
int data_count = 0;
NODE** g_age_index_ptr = NULL;

void DeleteByName(const char* name) {
	NODE* remove_node = FindNode(name);
	RemoveNode(remove_node);
}

NODE* FindNode(const char* name) {
	NODE* find_ptr = g_head_node.next_ptr;

	while (nullptr != find_ptr) {
		USERDATA* find = reinterpret_cast<USERDATA*>(find_ptr->data_cache);
		if (!strcmp(find->name, name)) {
			return find_ptr;
		}

		find_ptr = find_ptr->next_ptr;
	}

	return nullptr;
}

void RemoveNode(NODE* remove_node) {
	NODE* prev_node = remove_node->prev_ptr;
	NODE* next_node = remove_node->next_ptr;

	prev_node->next_ptr = next_node;
	next_node->prev_ptr = prev_node;

	free(remove_node->data_cache);
	free(remove_node);

	DecreaseCount();

	UpdateIndexByAge();
}

void InitList()
{
	g_head_node.next_ptr = &g_tail_node;
	g_tail_node.prev_ptr = &g_head_node;
	InitIndices();
	InitCount();
	InitData();
}

void InitIndices() {
	ReleaseIndex();
}

void InitData() {
	//for (int i = 0; i < 200000; ++i) {
		AddNewNodeAtTail(10, "Kim", "010-1111");
		AddNewNodeAtTail(12, "Cho", "010-5555");
		AddNewNodeAtTail(13, "Sung", "010-6666");
		AddNewNodeAtTail(14, "Jung", "010-9999");
		AddNewNodeAtTail(15, "Pi", "010-1010");
		AddNewNodeAtTail(13, "Yoon", "010-7777");
		AddNewNodeAtTail(14, "Kwak", "010-8888");
		AddNewNodeAtTail(10, "Lee", "010-2222");
		AddNewNodeAtTail(11, "Park", "010-3333");
		AddNewNodeAtTail(11, "Choi", "010-4444");
	//}
}

void IncreaseCount() {
	++data_count;
}

void DecreaseCount() {
	if (0 < data_count) {
		--data_count;
	}
	else if (data_count <= 0) {
		data_count = 0;
	}
}

void InitCount() {
	data_count = 0;
}

int GetDataCount() {
	return data_count;
}

void Push(USERDATA* new_data) {
	USERDATA* new_data_ptr = reinterpret_cast<USERDATA*>(malloc(sizeof(USERDATA)));
	NODE* new_node_ptr = reinterpret_cast<NODE*>(malloc(sizeof(NODE)));
	if (!(nullptr != new_data && nullptr != new_data_ptr && nullptr != new_node_ptr)) {
		return;
	}

	memcpy_s(new_data_ptr, sizeof(USERDATA), new_data, sizeof(USERDATA));

	NODE* original_first_node = g_head_node.next_ptr;


	new_node_ptr->prev_ptr = &g_head_node;
	new_node_ptr->next_ptr = original_first_node;
	original_first_node->prev_ptr = new_node_ptr;

	original_first_node = new_node_ptr;

	new_node_ptr->data_cache = new_data_ptr;

	IncreaseCount();

	UpdateIndexByAge();
}

void Enqueue(USERDATA* data) {
	if (nullptr == data) {
		return;
	}

	AddNewNodeAtTail(data->age, data->name, data->phone);
}

void AddNewNodeAtTail(int age, const char* name, const char* phone) {
	USERDATA* new_data_ptr = reinterpret_cast<USERDATA*>(malloc(sizeof(USERDATA)));
	NODE* new_node_ptr = reinterpret_cast<NODE*>(malloc(sizeof(NODE)));
	if (nullptr == new_data_ptr || nullptr == new_node_ptr) {
		return;
	}

	new_data_ptr->age = age;
	// 배열인 경우에는 C++ 템플릿을 이용해 컴파일 타임에 크기를 알 수 있다
	strcpy_s(new_data_ptr->name, name);
	strcpy_s(new_data_ptr->phone, phone);	

	new_node_ptr->next_ptr = nullptr;
	new_node_ptr->prev_ptr = nullptr;
	new_node_ptr->data_size = 0;
	new_node_ptr->offset = 0;
	new_node_ptr->is_new = true;

	NODE* original_last_node = g_tail_node.prev_ptr;
	new_node_ptr->prev_ptr = original_last_node;
	new_node_ptr->next_ptr = &g_tail_node;

	new_node_ptr->data_cache = new_data_ptr;

	original_last_node->next_ptr = new_node_ptr;
	g_tail_node.prev_ptr = new_node_ptr;

	IncreaseCount();

	UpdateIndexByAge();
}

NODE* Dequeue() {
	return Pop();
}

NODE* Pop()
{
	if (IsEmpty()) {
		return nullptr;
	}

	NODE* pop_data = g_head_node.next_ptr;

	g_head_node.next_ptr = pop_data->next_ptr;
	pop_data->next_ptr->prev_ptr = &g_head_node;

	DecreaseCount();

	UpdateIndexByAge();

	return pop_data;
}

bool IsEmpty() {
	return g_head_node.next_ptr == &g_tail_node;
}

NODE* SearchByName(const char* name) {
	return FindNode(name);
}

NODE* SearchByFirstOfAge(int age) {
	NODE* find_node = g_head_node.next_ptr;
	while (nullptr != find_node) {
		USERDATA* find = reinterpret_cast<USERDATA*>(find_node->data_cache);
		if (age == find->age) {
			return find_node;
		}

		find_node = find_node->next_ptr;
	}

	return nullptr;
}

NODE* SearchByLastOfAge(int age) {
	NODE* find_node = g_head_node.next_ptr;
	bool is_same_age = false;

	while (nullptr != find_node) {
		USERDATA* find = reinterpret_cast<USERDATA*>(find_node->data_cache);
		if (!is_same_age && age == find->age) {
			is_same_age = true;
		}
		else if (is_same_age && age != find->age) {
			is_same_age = false;
			return find_node->prev_ptr;
		}

		find_node = find_node->next_ptr;
	}

	return nullptr;
}

int GetCountBetween(NODE* start, NODE* end) {
	int count = 1;
	while (nullptr != start) {
		if (start == end) {
			return count;
		}

		++count;
		start = start->next_ptr;
	}

	return count;
}

NODE** MemoryCopy(NODE** dest, int dest_size, NODE* start) {
	for (int i = 0; i < dest_size; ++i) {
		if (nullptr == start) {
			free(dest);
			return nullptr;
		}

		dest[i] = start;
		start = start->next_ptr;
	}

	return dest;
}

NODE** SearchRangeByAge(int minimum_age, int maximum_age, int* array_size) {

	NODE* first = SearchByFirstOfAge(minimum_age);
	NODE* last = SearchByLastOfAge(maximum_age);

	while (first == nullptr) first = SearchByFirstOfAge(++minimum_age);
	while (last == nullptr) last = SearchByLastOfAge(--maximum_age);

	int size = GetCountBetween(first, last);
	NODE** result = 0 != size ? (NODE**)malloc(size * sizeof (NODE*)) : nullptr;
	if (nullptr == result) return nullptr;

	*array_size = size;

	result = MemoryCopy(result, size, first);
	if (nullptr == result) {
		*array_size = 0;
	}

	return result;
}

void Sort(NODE** start_node_ptr) {
	MergeSort(start_node_ptr);
}

void ReleaseNodeList() {
	NODE* node = g_head_node.next_ptr;

	while (nullptr != node && &g_tail_node != node) {
		NODE* next = node->next_ptr;
		free(node);

		node = next;
	}

	InitList();
}

void ReleaseIndex() {
	free(g_age_index_ptr);
	g_age_index_ptr = nullptr;
}

//===========================================================
// Merge sort
void Split(NODE* source, NODE** front, NODE** back) {
	NODE* slow = source, * fast = source->next_ptr;
	while (fast && fast->next_ptr) {
		slow = slow->next_ptr;
		fast = fast->next_ptr->next_ptr;
	}

	*front = source; // 앞 부분 시작
	*back = slow->next_ptr; // 뒷 부분 시작
	slow->next_ptr = nullptr; // 연결 끊기
}

NODE* Merge(NODE* first_node, NODE* second_node) {
	NODE dummy = {};
	NODE* result = &dummy;
	int count = 0;

	while (first_node && second_node) {
		USERDATA* first = reinterpret_cast<USERDATA*>(first_node->data_cache);
		USERDATA* second = reinterpret_cast<USERDATA*>(second_node->data_cache);

		if (first->age <= second->age) {
			result->next_ptr = first_node;
			first_node = first_node->next_ptr;
		}
		else {
			result->next_ptr = second_node;			
			second_node = second_node->next_ptr;
		}

		if (&g_tail_node == result->next_ptr) break;

		result->next_ptr->prev_ptr = result;
		result = result->next_ptr;
	}

	// prev_ptr이 없을 때 남은 리스트 이어붙이기
	//result->next_ptr = first ? first : second;

	// prev_ptr이 있는 경우 남은 리스트 이어붙이기
	NODE* rest = first_node ? first_node : second_node;
	AppendRestNode(result, rest);	

	dummy.next_ptr->prev_ptr = &g_head_node;

	return dummy.next_ptr;
}

void AppendRestNode(NODE* node, NODE* rest)
{
	while (rest) {
		node->next_ptr = rest;
		node->next_ptr->prev_ptr = node;

		node = node->next_ptr;
		rest = rest->next_ptr;
	}

	if (node != &g_tail_node)
	{
		node->next_ptr = &g_tail_node;
		g_tail_node.prev_ptr = node;
	}
}

void MergeSort(NODE** headRef) {
	NODE* head = *headRef;
	if (!head ||!head->next_ptr) return;
	if (&g_tail_node == head) return;

	NODE* first, * second;
	Split(head, &first, &second);

	MergeSort(&first);
	MergeSort(&second);

	*headRef = Merge(first, second);
}

// Quick sort
void Swap(NODE** first, NODE** second) {
	NODE* temp = *first;
	*first = *second;
	*second = temp;
}

void QuickSortByAge(NODE** data_array, int low_index, int high_index) {
	if (high_index <= low_index) return;

	int i = low_index, j = low_index;
	NODE** pivot_ptr = &data_array[high_index];

	// j인덱스는 마치 첨병처럼 pivot보다 작은 수를 찾고
	// i인덱스는 swap할 위치
	for (; j < high_index; ++j) {
		USERDATA* data = reinterpret_cast<USERDATA*>(data_array[i]->data_cache);
		USERDATA* pivot = reinterpret_cast<USERDATA*>((*pivot_ptr)->data_cache);

		if (data->age < pivot->age) {
			Swap(&data_array[i++], &data_array[j]);
		}
	}
	Swap(&data_array[i], pivot_ptr);

	QuickSortByAge(data_array, low_index, i - 1);
	QuickSortByAge(data_array, i + 1, high_index);
}

// index
void MakeIndexByAge() {
	int data_size = GetDataCount();
	g_age_index_ptr = (NODE**)malloc(sizeof(NODE*) * data_size);

	if (nullptr == g_age_index_ptr) return;

	ListToArray(g_head_node.next_ptr, g_age_index_ptr, data_size);

	QuickSortByAge(g_age_index_ptr, 0, data_size - 1);
}

void UpdateIndexByAge() {
	free(g_age_index_ptr);

	int data_size = GetDataCount();
	g_age_index_ptr = (NODE**)malloc(sizeof(NODE*) * data_size);

	if (nullptr == g_age_index_ptr) return;

	ListToArray(g_head_node.next_ptr, g_age_index_ptr, data_size);

	QuickSortByAge(g_age_index_ptr, 0, data_size - 1);
}

void ListToArray(NODE* start, NODE** array, int list_size) {
	for (int i = 0; i < list_size; ++i) {
		array[i] = start;
		start = start->next_ptr;
	}
}

USERDATA** SearchRangeByAgeIndex(int min, int max, int* array_size) {

	UpdateIndexByAge();

	int min_index = -1, max_index = -1;
	int data_size = GetDataCount();
	while (-1 == min_index) min_index = SearchByFirstOfAgeIndex(min++, data_size);
	while (-1 == max_index) max_index = SearchByLastOfAgeIndex(max--, data_size);

	int size = max_index - min_index + 1;
	USERDATA** result = 0 != size ? (USERDATA**)malloc(size * sizeof(USERDATA*)) : nullptr;
	if (nullptr == result) return nullptr;

	*array_size = size;

	size_t copy_size = size * sizeof(USERDATA*);
	memcpy_s(result, copy_size, g_age_index_ptr + min_index, copy_size);
	if (nullptr == result) {
		*array_size = 0;
	}

	return result;
}

int SearchByFirstOfAgeIndex(int age, int data_size) {
	NODE** node_ptr = g_age_index_ptr;
	int i = 0;
	for (; i < data_size; ++i) {
		USERDATA* data = reinterpret_cast<USERDATA*>(node_ptr[i]->data_cache);
		if (age == data->age) {
			return i;
		}
	}

	return -1;
}

int SearchByLastOfAgeIndex(int age, int data_size) {
	NODE** node_ptr = g_age_index_ptr;

	int i = data_size - 1;
	for (; 0 <= i; --i) {
		USERDATA* data = reinterpret_cast<USERDATA*>(node_ptr[i]->data_cache);
		if (age == data->age) {
			return i;
		}
	}

	return -1;
}
