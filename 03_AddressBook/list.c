#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>

#define OUT
#define USERDATA_INIT {"", -1, "", ""}

USERDATA head_node = { "{Head}", -1, "", ""};
USERDATA tail_node = { "{Tail}", 999, "", ""};

NODE g_head_node = { false, false, false, NULL, &head_node, 0, 0, NULL, NULL};
NODE g_tail_node = { false, false, false, NULL, &tail_node, 0, 0, NULL, NULL};
int data_count = 0;
NODE** g_age_index_ptr = NULL;

void DeleteByName(const char* name) 
{
	//NODE* remove_node = FindUserFromFileByName(name);
	//RemoveNode(remove_node);
}

USERDATA** FindUserFromFileByName(const char* name)
{
	FILE* fp = NULL;
	fopen_s(&fp, "user_address.dat", "rb");
	if (NULL == fp)
	{
		puts("file open failure");
		return NULL;
	}

	USERDATA user_data;
	int offset = 0;
	int data_count = 0;
	while (0 < fread(&user_data, sizeof(USERDATA), 1, fp))
	{
		if (0 == strcmp(user_data.name, name))
		{
			AddNewNodeAtTail(false, user_data.name, user_data.age, user_data.address, user_data.phone, offset);
			++data_count;
		}

		++offset;
	}
	fclose(fp);

	if (0 == data_count) return NULL;

	USERDATA** result_list = FindUserFromListByName(name);

	return result_list;
}

int CountFromListByName(const char* name)
{
	int count = 0;
	NODE* find = g_head_node.next_ptr;

	while (find)
	{
		USERDATA* user = find->data_cache;
		if (0 == strcmp(user->name, name)) ++count;

		find = find->next_ptr;
	}

	return count;
}

void SetArrayFromListByName(USERDATA** user_data_ptr, const char* name)
{
	NODE* find = g_head_node.next_ptr;
	int index = 0;
	while (find)
	{
		USERDATA* user = find->data_cache;
		if (0 == strcmp(user->name, name))
			user_data_ptr[index++] = user;

		find = find->next_ptr;
	}
}

USERDATA** FindUserFromListByName(const char* name)
{
	int count = CountFromListByName(name);
	if (0 == count) return NULL;

	USERDATA** user_result_list = (USERDATA**)malloc(sizeof(USERDATA*) * count);
	if (NULL == user_result_list) return NULL;

	SetArrayFromListByName(user_result_list, name);

	return user_result_list;
}

void RemoveNode(NODE* remove_node) 
{
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
	//InitData();
}

void InitIndices() 
{
	ReleaseIndex();
}

void InitData() 
{
	//for (int i = 0; i < 200000; ++i) {
	//	AddNewNodeAtTail(10, "Kim", "010-1111");
	//	AddNewNodeAtTail(12, "Cho", "010-5555");
	//	AddNewNodeAtTail(13, "Sung", "010-6666");
	//	AddNewNodeAtTail(14, "Jung", "010-9999");
	//	AddNewNodeAtTail(15, "Pi", "010-1010");
	//	AddNewNodeAtTail(13, "Yoon", "010-7777");
	//	AddNewNodeAtTail(14, "Kwak", "010-8888");
	//	AddNewNodeAtTail(10, "Lee", "010-2222");
	//	AddNewNodeAtTail(11, "Park", "010-3333");
	//	AddNewNodeAtTail(11, "Choi", "010-4444");
	//}
}

void IncreaseCount() 
{
	++data_count;
}

void DecreaseCount()
{
	if (0 < data_count) {
		--data_count;
	}
	else if (data_count <= 0) {
		data_count = 0;
	}
}

void InitCount()
{
	data_count = 0;
}

int GetDataCount()
{
	return data_count;
}

void Push(bool is_new, USERDATA* new_data)
{
	USERDATA* new_data_ptr = (USERDATA*)(malloc(sizeof(USERDATA)));
	NODE* new_node_ptr = (NODE*)(malloc(sizeof(NODE)));
	if (!(NULL != new_data && NULL != new_data_ptr && NULL != new_node_ptr)) {
		return;
	}

	memcpy_s(new_data_ptr, sizeof(USERDATA), new_data, sizeof(USERDATA));

	new_node_ptr->next_ptr = NULL;
	new_node_ptr->prev_ptr = NULL;
	new_node_ptr->data_size = 0;
	new_node_ptr->offset = 0;
	new_node_ptr->is_new = is_new;
	new_node_ptr->is_updated = false;
	new_node_ptr->is_deleted = false;
	new_node_ptr->data_cache = new_data_ptr;

	NODE* original_first_node = g_head_node.next_ptr;

	new_node_ptr->prev_ptr = &g_head_node;
	new_node_ptr->next_ptr = original_first_node;
	original_first_node->prev_ptr = new_node_ptr;

	original_first_node = new_node_ptr;


	IncreaseCount();

	UpdateIndexByAge();
}

void Enqueue(USERDATA* data)
{
	//if (NULL == data) {
	//	return;
	//}

	//AddNewNodeAtTail(data->age, data->name, data->phone);
}

bool IsAllDataEmpty(const char* name, int age, const char* address, const char* phone)
{
	if (Empty(name) && Empty(address) && Empty(phone) && -1 == age)
		return true;
	return false;
}

USERDATA* AddNewNodeAtTail(bool is_new, const char* name, int age, const char* address, const char* phone, int offset)
{
	if (IsAllDataEmpty(name, age, address, phone))
	{
		return NULL;
	}

	USERDATA* new_data_ptr = (USERDATA*)(malloc(sizeof(USERDATA)));
	NODE* new_node_ptr = (NODE*)(malloc(sizeof(NODE)));
	if (NULL == new_data_ptr || NULL == new_node_ptr) {
		return NULL;
	}

	new_data_ptr->age = age;
	// 배열인 경우에는 C++ 템플릿을 이용해 컴파일 타임에 크기를 알 수 있다
	strcpy_s(new_data_ptr->name, sizeof new_data_ptr->name, name);
	strcpy_s(new_data_ptr->address, sizeof new_data_ptr->address, address);
	strcpy_s(new_data_ptr->phone, sizeof new_data_ptr->phone, phone);

	new_node_ptr->next_ptr = NULL;
	new_node_ptr->prev_ptr = NULL;
	new_node_ptr->data_size = sizeof(USERDATA);
	new_node_ptr->offset = offset;
	new_node_ptr->is_new = is_new;
	new_node_ptr->is_updated = false;
	new_node_ptr->is_deleted = false;

	NODE* original_last_node = g_tail_node.prev_ptr;
	new_node_ptr->prev_ptr = original_last_node;
	new_node_ptr->next_ptr = &g_tail_node;

	new_node_ptr->data_cache = new_data_ptr;

	original_last_node->next_ptr = new_node_ptr;
	g_tail_node.prev_ptr = new_node_ptr;

	IncreaseCount();

	UpdateIndexByAge();

	return new_data_ptr;
}

NODE* Dequeue()
{
	return Pop();
}

NODE* Pop()
{
	if (IsEmpty()) {
		return NULL;
	}

	NODE* pop_data = g_head_node.next_ptr;

	g_head_node.next_ptr = pop_data->next_ptr;
	pop_data->next_ptr->prev_ptr = &g_head_node;

	DecreaseCount();

	UpdateIndexByAge();

	return pop_data;
}

bool IsEmpty()
{
	return g_head_node.next_ptr == &g_tail_node;
}

USERDATA** SearchByName(const char* name, int* count)
{
	USERDATA** result_list = FindUserFromListByName(name);
	if (NULL == result_list)
	{
		result_list = FindUserFromFileByName(name);
	}

	*count = CountFromListByName(name);

	return result_list;
}

void FreeQueryResult(USERDATA** result_list)
{
	// 연결 리스트 데이터는 이벤트 루프 끝나고 별도로 해지하는 로직이 있고
	// 여기에서는 연결 리스트에 존재하는 데이터 주소를 동적 배열 요소에 단순 복사한 거기 때문에
	// 동적 배열로 생성한 부분만 해제한다
	free(result_list);
}

NODE* SearchByFirstOfAge(int age)
{
	NODE* find_node = g_head_node.next_ptr;
	while (NULL != find_node) {
		USERDATA* find = (USERDATA*)(find_node->data_cache);
		if (age == find->age) {
			return find_node;
		}

		find_node = find_node->next_ptr;
	}

	return NULL;
}

NODE* SearchByLastOfAge(int age)
{
	NODE* find_node = g_head_node.next_ptr;
	bool is_same_age = false;

	while (NULL != find_node) {
		USERDATA* find = (USERDATA*)(find_node->data_cache);
		if (!is_same_age && age == find->age) {
			is_same_age = true;
		}
		else if (is_same_age && age != find->age) {
			is_same_age = false;
			return find_node->prev_ptr;
		}

		find_node = find_node->next_ptr;
	}

	return NULL;
}

int GetCountBetween(NODE* start, NODE* end)
{
	int count = 1;
	while (NULL != start) {
		if (start == end) {
			return count;
		}

		++count;
		start = start->next_ptr;
	}

	return count;
}

NODE** MemoryCopy(NODE** dest, int dest_size, NODE* start)
{
	for (int i = 0; i < dest_size; ++i) {
		if (NULL == start) {
			free(dest);
			return NULL;
		}

		dest[i] = start;
		start = start->next_ptr;
	}

	return dest;
}

NODE** SearchRangeByAge(int minimum_age, int maximum_age, int* array_size)
{

	NODE* first = SearchByFirstOfAge(minimum_age);
	NODE* last = SearchByLastOfAge(maximum_age);

	while (first == NULL) first = SearchByFirstOfAge(++minimum_age);
	while (last == NULL) last = SearchByLastOfAge(--maximum_age);

	int size = GetCountBetween(first, last);
	NODE** result = 0 != size ? (NODE**)malloc(size * sizeof (NODE*)) : NULL;
	if (NULL == result) return NULL;

	*array_size = size;

	result = MemoryCopy(result, size, first);
	if (NULL == result) {
		*array_size = 0;
	}

	return result;
}

void Sort(NODE** start_node_ptr)
{
	MergeSort(start_node_ptr);
}

void ReleaseNodeList()
{
	NODE* node = g_head_node.next_ptr;

	while (NULL != node && &g_tail_node != node) {
		NODE* next = node->next_ptr;
		free(node);

		node = next;
	}

	InitList();
}

void ReleaseIndex()
{
	free(g_age_index_ptr);
	g_age_index_ptr = NULL;
}

//===========================================================
// Merge sort
void Split(NODE* source, NODE** front, NODE** back)
{
	NODE* slow = source, * fast = source->next_ptr;
	while (fast && fast->next_ptr) {
		slow = slow->next_ptr;
		fast = fast->next_ptr->next_ptr;
	}

	*front = source; // 앞 부분 시작
	*back = slow->next_ptr; // 뒷 부분 시작
	slow->next_ptr = NULL; // 연결 끊기
}

NODE* Merge(NODE* first_node, NODE* second_node)
{
	NODE dummy = {0};
	NODE* result = &dummy;
	int count = 0;

	while (first_node && second_node) {
		USERDATA* first = (USERDATA*)(first_node->data_cache);
		USERDATA* second = (USERDATA*)(second_node->data_cache);

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

void MergeSort(NODE** headRef)
{
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
void Swap(NODE** first, NODE** second)
{
	NODE* temp = *first;
	*first = *second;
	*second = temp;
}

void QuickSortByAge(NODE** data_array, int low_index, int high_index)
{
	if (high_index <= low_index) return;

	int i = low_index, j = low_index;
	NODE** pivot_ptr = &data_array[high_index];

	// j인덱스는 마치 첨병처럼 pivot보다 작은 수를 찾고
	// i인덱스는 swap할 위치
	for (; j < high_index; ++j) {
		USERDATA* data = (USERDATA*)(data_array[i]->data_cache);
		USERDATA* pivot = (USERDATA*)((*pivot_ptr)->data_cache);

		if (data->age < pivot->age) {
			Swap(&data_array[i++], &data_array[j]);
		}
	}
	Swap(&data_array[i], pivot_ptr);

	QuickSortByAge(data_array, low_index, i - 1);
	QuickSortByAge(data_array, i + 1, high_index);
}

// index
void MakeIndexByAge()
{
	int data_size = GetDataCount();
	g_age_index_ptr = (NODE**)malloc(sizeof(NODE*) * data_size);

	if (NULL == g_age_index_ptr) return;

	ListToArray(g_head_node.next_ptr, g_age_index_ptr, data_size);

	QuickSortByAge(g_age_index_ptr, 0, data_size - 1);
}

void UpdateIndexByAge()
{
	free(g_age_index_ptr);

	int data_size = GetDataCount();
	g_age_index_ptr = (NODE**)malloc(sizeof(NODE*) * data_size);

	if (NULL == g_age_index_ptr) return;

	ListToArray(g_head_node.next_ptr, g_age_index_ptr, data_size);

	QuickSortByAge(g_age_index_ptr, 0, data_size - 1);
}

void ListToArray(NODE* start, NODE** array, int list_size)
{
	for (int i = 0; i < list_size; ++i) {
		array[i] = start;
		start = start->next_ptr;
	}
}

USERDATA** SearchRangeByAgeIndex(int min, int max, int* array_size)
{

	UpdateIndexByAge();

	int min_index = -1, max_index = -1;
	int data_size = GetDataCount();
	while (-1 == min_index) min_index = SearchByFirstOfAgeIndex(min++, data_size);
	while (-1 == max_index) max_index = SearchByLastOfAgeIndex(max--, data_size);

	int size = max_index - min_index + 1;
	USERDATA** result = 0 != size ? (USERDATA**)malloc(size * sizeof(USERDATA*)) : NULL;
	if (NULL == result) return NULL;

	*array_size = size;

	size_t copy_size = size * sizeof(USERDATA*);
	memcpy_s(result, copy_size, g_age_index_ptr + min_index, copy_size);
	if (NULL == result) {
		*array_size = 0;
	}

	return result;
}

int SearchByFirstOfAgeIndex(int age, int data_size)
{
	NODE** node_ptr = g_age_index_ptr;
	int i = 0;
	for (; i < data_size; ++i) {
		USERDATA* data = (USERDATA*)(node_ptr[i]->data_cache);
		if (age == data->age) {
			return i;
		}
	}

	return -1;
}

int SearchByLastOfAgeIndex(int age, int data_size)
{
	NODE** node_ptr = g_age_index_ptr;

	int i = data_size - 1;
	for (; 0 <= i; --i) {
		USERDATA* data = (USERDATA*)(node_ptr[i]->data_cache);
		if (age == data->age) {
			return i;
		}
	}

	return -1;
}

char* Parser(char* sql)
{
	return NULL;
}

char* GetLastString(char** string, int size)
{
	return string[size - 1];
}

void TrimSingleQuote(char* value)
{
	char* dest = value;
	char* src = value;

	while ('\0' != *src)
	{
		if ('\'' != *src)
		{
			*dest = *src;
			++dest;
		}
		++src;
	}

	*dest = '\0';
}

bool Includes(const char* whole, const char* part)
{
	return strstr(whole, part);
}

bool TrimAroundEqual(char* str_ptr)
{
	char* dest = str_ptr;
	char* src = str_ptr;

	while ('\0' != *src)
	{
		if (!(' ' == *src && '=' == *(src + 1) || ' ' == *src && '=' == *(src - 1)))
		{
			*dest = *src;
			++dest;
		}
		++src;
	}
	if (src == dest)
		return true;

	*dest = '\0';

	return false;
}

void TrimLineFeed(char* value)
{
	char* dest = value;
	char* src = value;

	while ('\0' != *src)
	{
		if (('\n' != *src))
		{
			*dest = *src;
			++dest;
		}
		++src;
	}

	*dest = '\0';
}

void ToLowerCase(char* str_ptr)
{
	while ('\0' != *str_ptr)
	{
		if ('A' <= *str_ptr && *str_ptr <= 'Z')
			*str_ptr += 32;
		++str_ptr;
	}
}

void ParseIntValue(char* token, int* value, int value_size)
{
	// age = 12
	int size = 0;
	char** split_result = SplitString(token, &size, '=');
	char* last_string = GetLastString(split_result, size);
	TrimSingleQuote(last_string);
	TrimLineFeed(last_string);

	int result = atoi(last_string);
	*value = result;

	FreeSplitString(split_result, size);
}

int CountFromListByAllColumn(const char* name, const char* address, const char* phone, int age)
{
	int count = 0;
	NODE* find = g_head_node.next_ptr;

	while (find)
	{
		USERDATA* user = find->data_cache;
		if (0 == (name ? strcmp(user->name, name) : 0)
			&& 0 == (phone ? strcmp(user->phone, phone): 0)
			&& user->age == (-1 < age ? age : user->age)
			&& Includes(user->address, address)) 
			++count;

		find = find->next_ptr;
	}

	return count;
}

bool Empty(const char* str)
{
	return !strlen(str);
}

bool Equal(const char* first, const char* second)
{
	return 0 == strcmp(first, second);
}

bool IsDuplicatedUserFromList(USERDATA* user_data)
{
	NODE* find = g_head_node.next_ptr;

	while (find)
	{
		USERDATA* find_user = find->data_cache;

		if (IsSameUserData(find_user, user_data))
			return true;

		find = find->next_ptr;
	}

	return false;

}

bool IsSameUserData(USERDATA* first, USERDATA* second)
{
	if (0 == strcmp(first->address, second->address)
		&& 0 == strcmp(first->name, second->name)
		&& 0 == strcmp(first->phone, second->phone)
		&& first->age == second->age)
		return true;

	return false;
}

void ParseStringValue(char* token, char* out_value, int value_size)
{
	int size = 0;
	char** split_result = SplitString(token, &size, '=');
	char* last_string = GetLastString(split_result, size);
	TrimSingleQuote(last_string);
	TrimLineFeed(last_string);

	memcpy_s(out_value, value_size, last_string, strlen(last_string) + 1);

	FreeSplitString(split_result, size);
}

USERDATA** Query(char* sql, int* size)
{
	ToLowerCase(sql);
	bool is_end = false;
	while (!is_end)
		is_end = TrimAroundEqual(sql);

	if (Includes(sql, "insert"))
		return ParseAndCommandInsert(sql, size);
	else if (Includes(sql, "select"))
		return ParseAndCommandSelect(sql, size);
	else if (Includes(sql, "update set"))
		return ParseAndCommandUpdate(sql, size);
	else if (Includes(sql, "delete"))
		return ParseAndCommandDelete(sql, size);

	return NULL;
}

USERDATA** ParseAndCommandInsert(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = -1;

	int split_size = -1;
	char** split_result = SplitString(sql, &split_size, ' ');

	for (int i = 0; i < split_size; ++i)
	{
		char* token = split_result[i];
		if (Includes(token, "name"))
			ParseStringValue(token, name, sizeof name);
		else if (Includes(token, "address"))
			ParseStringValue(token, address, sizeof address);
		else if (Includes(token, "phone"))
			ParseStringValue(token, phone, sizeof phone);
		else if (Includes(token, "age"))
			ParseIntValue(token, &age, sizeof age);
	}

	FreeSplitString(split_result, split_size);

	if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && -1 == age) return NULL;

	return InsertByQuery(name, address, phone, age, data_size);
}

USERDATA** ParseAndCommandSelect(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = -1;

	int split_size = -1;
	char** split_result = SplitString(sql, &split_size, ' ');

	for (int i = 0; i < split_size; ++i)
	{
		char* token = split_result[i];
		if (Includes(token, "name"))
			ParseStringValue(token, name, sizeof name);
		else if (Includes(token, "address"))
			ParseStringValue(token, address, sizeof address);
		else if (Includes(token, "phone"))
			ParseStringValue(token, phone, sizeof phone);
		else if (Includes(token, "age"))
			ParseIntValue(token, &age, sizeof age);
	}

	FreeSplitString(split_result, split_size);

	//if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && -1 == age) return NULL;

	return SelectByQuery(name, address, phone, age, data_size);
}

USERDATA** ParseAndCommandDelete(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = -1;

	int split_size = -1;
	char** split_result = SplitString(sql, &split_size, ' ');

	for (int i = 0; i < split_size; ++i)
	{
		char* token = split_result[i];
		if (Includes(token, "name"))
			ParseStringValue(token, name, sizeof name);
		else if (Includes(token, "address"))
			ParseStringValue(token, address, sizeof address);
		else if (Includes(token, "phone"))
			ParseStringValue(token, phone, sizeof phone);
		else if (Includes(token, "age"))
			ParseIntValue(token, &age, sizeof age);
	}

	FreeSplitString(split_result, split_size);

	if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && -1 == age) return NULL;

	USERDATA** search_result = SelectByQuery(name, address, phone, age, data_size);
	if (NULL == search_result) return NULL;

	return DeleteByQuery(name, address, phone, age, data_size);
}

USERDATA** ParseAndCommandUpdate(char* sql, int* data_size)
{
	int split_size = -1;
	char** split_result = SplitString(sql, &split_size, ' ');

	int set_size = 0, where_size = 0;
	char** set_string = GetSetString(split_result, split_size, &set_size);
	char** where_string = GetWhereString(split_result, split_size, &where_size);

	char set_name[32] = "";
	char set_address[32] = "";
	char set_phone[32] = "";
	int set_age = -1;
	for (int i = 0; i < set_size; ++i)
	{
		char* token = set_string[i];
		if (Includes(token, "name"))
			ParseStringValue(token, set_name, sizeof set_name);
		else if (Includes(token, "address"))
			ParseStringValue(token, set_address, sizeof set_address);
		else if (Includes(token, "phone"))
			ParseStringValue(token, set_phone, sizeof set_phone);
		else if (Includes(token, "age"))
			ParseIntValue(token, &set_age, sizeof set_age);
	}

	char where_name[32] = "";
	char where_address[32] = "";
	char where_phone[32] = "";
	int where_age = -1;
	for (int i = 0; i < where_size; ++i)
	{
		char* token = where_string[i];
		if (Includes(token, "name"))
			ParseStringValue(token, where_name, sizeof where_name);
		else if (Includes(token, "address"))
			ParseStringValue(token, where_address, sizeof where_address);
		else if (Includes(token, "phone"))
			ParseStringValue(token, where_phone, sizeof where_phone);
		else if (Includes(token, "age"))
			ParseIntValue(token, &where_age, sizeof where_age);
	}

	if (!strcmp(where_name, "") && !strcmp(where_address, "") && !strcmp(where_phone, "") && -1 == where_age) return NULL;

	USERDATA** search_result = SelectByQuery(where_name, where_address, where_phone, where_age, data_size);
	if (NULL == search_result) return NULL;

	if (!strcmp(set_name, "") && !strcmp(set_address, "") && !strcmp(set_phone, "") && -1 == set_age
		&& !strcmp(where_name, "") && !strcmp(where_address, "") && !strcmp(where_phone, "") && -1 == where_age) return NULL;

	FreeSplitString(where_string, where_size);
	FreeSplitString(set_string, set_size);
	FreeSplitString(split_result, split_size);

	return UpdateByQuery(set_name, set_address, set_phone, set_age, where_name, where_address, where_phone, where_age, data_size);
}

USERDATA** InsertByQuery(const char* name, const char* address, const char* phone, int age, int* size)
{
	USERDATA insert_data = USERDATA_INIT;
	SetUserData(&insert_data, name, address, phone, age);

	USERDATA* new_user = AddNewNodeAtTail(true, insert_data.name, insert_data.age, insert_data.address, insert_data.phone, -1);
	USERDATA** result_list = malloc(sizeof(USERDATA));

	if (NULL == result_list) return NULL;

	result_list[0] = new_user;
	*size = 1;

	return result_list;
}

USERDATA** SelectByQuery(const char* name, const char* address, const char* phone, int age, int* size)
{
	FILE* fp = NULL;
	fopen_s(&fp, "user_address.dat", "rb");
	if (NULL == fp)
	{
		puts("file open failure");
		return NULL;
	}

	USERDATA user_data, search_data = USERDATA_INIT;
	SetUserData(&search_data, name, address, phone, age);

	int offset = 0;
	int data_count = 0;
	while (0 < fread(&user_data, sizeof(USERDATA), 1, fp))
	{
		// sql의 조건 유효성 확인
		if (IsValidData(&search_data, &user_data))
		{
			// O(N) -> 개선 필요
			// 중복 데이터는 리스트에 넣지 않는다
			if (!IsDuplicatedUserFromList(&user_data))
			{
				AddNewNodeAtTail(false, user_data.name, user_data.age, user_data.address, user_data.phone, offset);
			}

			++data_count;
		}

		++offset;
	}
	fclose(fp);

	// 메모리 상으로는 변경됐는데 파일에 반영 안 되어 있으면
	// 여기서 data_count가 0이 된다
	*size = data_count;
	if (0 == data_count) return NULL;

	USERDATA** result_list = FindUsersFromListBySearchData(&search_data, data_count);
	if (NULL == result_list) return NULL;

	return result_list;
}

USERDATA** UpdateByQuery(const char* set_name, const char* set_address, const char* set_phone, int set_age,
	const char* where_name, const char* where_address, const char* where_phone, int where_age, int* data_size)
{
	USERDATA set_data = USERDATA_INIT, where_data = USERDATA_INIT;
	SetUserData(&set_data, set_name, set_address, set_phone, set_age);
	SetUserData(&where_data, where_name, where_address, where_phone, where_age);

	int data_count = 0;
	NODE* find_node = g_head_node.next_ptr;
	while (find_node)
	{
		USERDATA* find_data = find_node->data_cache;
		if (IsValidData(&where_data, find_data))
		{
			UpdateNode(find_node, &set_data);
			++data_count;
		}

		find_node = find_node->next_ptr;
	}

	*data_size = data_count;
	if (0 == data_count) return NULL;

	USERDATA** result_list = FindUsersFromListBySetData(&set_data, data_count);

	if (NULL == result_list) return NULL;

	return result_list;
}

USERDATA** DeleteByQuery(const char* name, const char* address, const char* phone, int age, int* data_size)
{
	USERDATA delete_data = USERDATA_INIT;
	SetUserData(&delete_data, name, address, phone, age);

	int data_count = 0;
	NODE* find_node = g_head_node.next_ptr;
	while (find_node)
	{
		USERDATA* find_data = find_node->data_cache;
		if (IsValidData(&delete_data, find_data))
		{
			DeleteNode(find_node);
			++data_count;
		}

		find_node = find_node->next_ptr;
	}

	*data_size = data_count;
	if (0 == data_count) return NULL;

	USERDATA** result_list = FindUsersFromListByDeleteData(&delete_data, data_count);

	if (NULL == result_list) return NULL;

	return result_list;
}

void DeleteNode(NODE* tobe_deleted)
{
	tobe_deleted->is_deleted = true;
}

USERDATA** FindUsersFromListByDeleteData(USERDATA* delete_data, int size)
{
	if (NULL == delete_data || 0 == size) return NULL;

	USERDATA** result = malloc(sizeof(USERDATA*) * size);

	if (NULL == result) return NULL;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = find_node->data_cache;
		if (true == find_node->is_deleted && IsValidData(delete_data, find))
		{
			result[index++] = find;
		}

		find_node = find_node->next_ptr;
	}

	return result;
}

void InitUserData(USERDATA* data)
{
	
}

void SetUserData(USERDATA* data, const char* name, const char* address, const char* phone, int age)
{
	if (!Empty(name))
	{
		int name_size = (int)strlen(name) + 1;
		memcpy_s(data->name, sizeof data->name, name, name_size);
	}
	if (!Empty(address))
	{
		int address_size = (int)strlen(address) + 1;
		memcpy_s(data->address, sizeof data->address, address, address_size);
	}
	if (!Empty(phone))
	{
		int phone_size = (int)strlen(phone) + 1;
		memcpy_s(data->phone, sizeof data->phone, phone, phone_size);
	}
	if (-1 != age)
	{
		data->age = age;
	}
}

bool IsValidData(USERDATA* where, USERDATA* find)
{
	if (0 == (Empty(where->name) ? 0 : strcmp(find->name, where->name))
		&& 0 == (Empty(where->phone) ? 0 : strcmp(find->phone, where->phone))
		&& find->age == (-1 < where->age ? where->age : find->age)
		&& Includes(find->address, where->address))
		return true;

	return false;
}

USERDATA** FindUsersFromListBySearchData(USERDATA* search, int size)
{
	if (NULL == search || 0 == size) return NULL;

	USERDATA** result = malloc(sizeof(USERDATA*) * size);

	if (NULL == result) return NULL;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = find_node->data_cache;
		if (IsValidData(search, find))
		{
			result[index++] = find;
		}

		find_node = find_node->next_ptr;
	}

	return result;
}

USERDATA** FindUsersFromListBySetData(USERDATA* search, int size)
{
	if (NULL == search || 0 == size) return NULL;

	USERDATA** result = malloc(sizeof(USERDATA*) * size);

	if (NULL == result) return NULL;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = find_node->data_cache;
		if (true == find_node->is_updated && IsValidData(search, find))
		{
			result[index++] = find;
		}

		find_node = find_node->next_ptr;
	}

	return result;
}

void UpdateNode(NODE* tobe_updated, USERDATA* updating)
{
	tobe_updated->is_updated = true;
	SetUserData(tobe_updated->data_cache, updating->name, 
		updating->address, updating->phone, updating->age);
}

// 연결 리스트로 하면 더 편할 듯
char** GetSetString(char** string_array, int array_size, int* out_size)
{
	// get 'and' count
	const int start = 2; // update, set은 건너뛴다
	int size = 0;
	for (int i = start; i < array_size; ++i)
	{
		if (Equal(string_array[i], "where")) break;
		else if (Equal(string_array[i], "and")) ++size;
	}
	++size;
	*out_size = size;

	char** set_string = (char**)malloc(size * sizeof(char*));
	if (NULL == set_string) return NULL;

	int index = 0;
	for (int i = start; i < array_size; ++i)
	{
		if (Equal(string_array[i], "where")) break;
		else if (Equal(string_array[i], "and")) continue;

		int token_size = (int)strlen(string_array[i]) + 1;
		char* token = (char*)malloc(token_size);
		memcpy_s(token, token_size, string_array[i], token_size);

		set_string[index++] = token;
	}

	return set_string;
}

char** GetWhereString(char** string_array, int array_size, int* out_size)
{
	// find where index
	const int start = 2; // update, set은 건너뛴다
	int where_next_index = 0;
	for (int i = start; i < array_size; ++i)
	{
		if (Equal(string_array[i], "where"))
		{
			where_next_index = i;
			break;
		}
	}
	if (where_next_index == 0) return NULL;

	++where_next_index;

	// get 'and' count
	int size = 0;
	for (int i = where_next_index; i < array_size; ++i)
	{ 
		if (Equal(string_array[i], "and")) ++size;
	}
	++size;
	*out_size = size;

	char** set_string = (char**)malloc(size * sizeof(char*));
	if (NULL == set_string) return NULL;

	int index = 0;
	for (int i = where_next_index; i < array_size; ++i)
	{
		if (Equal(string_array[i], "and")) continue;

		TrimLineFeed(string_array[i]);
		int token_size = (int)strlen(string_array[i]) + 1;
		char* token = (char*)malloc(token_size);
		memcpy_s(token, token_size, string_array[i], token_size);

		set_string[index++] = token;
	}

	return set_string;
}

char** SplitString(char* string, OUT int* size, char delim)
{
	int delim_count = 0;
	char* end = string;
	bool is_single_quote = false;
	while ('\0' != *end)
	{
		if ('\'' == *end)
		{
			is_single_quote = !is_single_quote;
		}
		else if (!is_single_quote && delim == *end && delim != *(end + 1)) ++delim_count;

		++end;
	}

	*size = delim_count + 1;
	char** string_array = (char**)malloc(sizeof(char*) * (delim_count + 1));
	if (NULL == string_array) return NULL;	

	char* start = string;
	end = string;
	int index = 0;
	is_single_quote = false;
	while (true)
	{
		if ('\'' == *end)
		{
			is_single_quote = !is_single_quote;
		}
		else if (!is_single_quote && (delim == *end && delim != *(end + 1)) || '\0' == *end)
		{
			int dist = (int)(end - start);

			char* token = (char*)malloc(sizeof(char) * (dist + 1));
			if (NULL == token) return NULL;

			memcpy_s(token, dist, start, dist);
			string_array[index] = token;
			token[dist] = '\0';

			start = end + 1;
			++index;
		}

		if ('\0' == *end) break;

		++end;
	}

	return string_array;
}

void FreeSplitString(char** string, int size)
{
	for (int i = 0; i < size; ++i)
		free(string[i]);
	free(string);
}

void Commit()
{
	NODE* node = g_head_node.next_ptr;
	while (node)
	{
		if (node->is_deleted)
		{
			FILE* fp = NULL;
			fopen_s(&fp, "user_address.dat", "rb+");
			if (NULL == fp) break;

			USERDATA* user = node->data_cache;
			fseek(fp, node->offset * sizeof(USERDATA), SEEK_SET);
			USERDATA init = USERDATA_INIT;
			if (0 < fwrite(&init, sizeof(USERDATA), 1, fp))
			{
				printf("delete: %s %s %s\n", user->name, user->address, user->phone);

				FILE* fp_deleted = NULL;
				fopen_s(&fp_deleted, "user_address_deleted.dat", "ab+");
				if (NULL == fp_deleted) break;

				int offset = node->offset;
				if (0 < fwrite(&offset, sizeof(int), 1, fp_deleted))
				{
					node->is_deleted = false;
					puts("set deleted offset\n");
				}
				else
				{
					puts("set deleted offset failure\n");
				}

				fclose(fp_deleted);
			}
			else
			{
				puts("delete failure");
			}

			fclose(fp);
		}
		else if (node->is_new)
		{
			// open file db
			FILE* fp = NULL;
			fopen_s(&fp, "user_address.dat", "ab+");
			if (NULL == fp) break;

			USERDATA* user = node->data_cache;

			// find deleted offset
			FILE* fp_deleted = NULL;
			fopen_s(&fp_deleted, "user_address_deleted.dat", "rb+");
			if (NULL == fp_deleted)
			{
				puts("fopen_s error: user_address_deleted.dat");
				break;
			}
			// set a cursor to start
			fseek(fp_deleted, 0, SEEK_SET);

			int deleted_offset = -1;
			int offset = -1;
			while (0 < fread(&deleted_offset, sizeof(int), 1, fp_deleted))
			{
				++offset;
				if (-1 == deleted_offset) continue;

				// after found deleted_offset
				// and set that offset to file db
				fseek(fp, sizeof(USERDATA) * deleted_offset, SEEK_SET);
				node->offset = deleted_offset;

				// set deleted_offset to -1 when deleted_offset is used
				fseek(fp_deleted, offset * sizeof(int), SEEK_SET);
				int complete = -1;
				if (fwrite(&complete, sizeof(int), 1, fp_deleted) <= 0)
				{
					puts("failure: set deleted_offset to -1");
				}

				fclose(fp_deleted);
				break;
			}

			if (-1 == deleted_offset)
			{
				fseek(fp, 0, SEEK_END);
			}


			if (0 < fwrite(user, sizeof(USERDATA), 1, fp))
			{
				node->is_new = false;
				printf("insert: %s %s %s\n", user->name, user->address, user->phone);
			}
			else
			{
				puts("delete failure");
			}

			fclose(fp);
		}
		else if (node->is_updated)
		{
			FILE* fp = NULL;
			fopen_s(&fp, "user_address.dat", "rb+");
			if (NULL == fp) break;

			USERDATA* user = node->data_cache;
			fseek(fp, node->offset * sizeof(USERDATA), SEEK_SET);
			USERDATA new_user = USERDATA_INIT;

			if (0 < fwrite(user, sizeof(USERDATA), 1, fp))
			{
				node->is_updated = false;
				printf("update: %s %s %s\n", user->name, user->address, user->phone);
			}
			else
			{
				puts("update failure");
			}

			fclose(fp);
		}

		node = node->next_ptr;
	}
}