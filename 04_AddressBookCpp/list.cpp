#include "list.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>

constexpr int AGE_INIT_VALUE = -1;

#define USERDATA_INIT {"", AGE_INIT_VALUE, "", ""}

USERDATA head_node = { "{Head}", AGE_INIT_VALUE, "", ""};
USERDATA tail_node = { "{Tail}", 999, "", ""};

NODE g_head_node = { false, false, false, nullptr, &head_node, 0, 0, nullptr, nullptr};
NODE g_tail_node = { false, false, false, nullptr, &tail_node, 0, 0, nullptr, nullptr};
int data_count = 0;

void RemoveNode(NODE* remove_node) 
{
	NODE* prev_node = remove_node->prev_ptr;
	NODE* next_node = remove_node->next_ptr;

	prev_node->next_ptr = next_node;
	next_node->prev_ptr = prev_node;

	delete remove_node->data_cache;
	delete remove_node;

	DecreaseCount();
}

void InitList()
{
	g_head_node.next_ptr = &g_tail_node;
	g_tail_node.prev_ptr = &g_head_node;
	InitCount();
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

void Push(bool is_new, USERDATA* new_data)
{
	USERDATA* new_data_ptr = new USERDATA;
	NODE* new_node_ptr = new NODE;
	if (!(nullptr != new_data && nullptr != new_data_ptr && nullptr != new_node_ptr)) {
		return;
	}

	memcpy_s(new_data_ptr, sizeof(USERDATA), new_data, sizeof(USERDATA));

	new_node_ptr->next_ptr = nullptr;
	new_node_ptr->prev_ptr = nullptr;
	new_node_ptr->data_size = sizeof(USERDATA);
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
}

bool IsAllDataEmpty(const char* name, int age, const char* address, const char* phone)
{
	if (Empty(name) && Empty(address) && Empty(phone) && AGE_INIT_VALUE == age)
		return true;
	return false;
}

USERDATA* AddNewNodeAtTail(bool is_new, const char* name, int age, const char* address, const char* phone, int offset)
{
	if (IsAllDataEmpty(name, age, address, phone))
	{
		return nullptr;
	}

	USERDATA* new_data_ptr = new USERDATA;
	NODE* new_node_ptr = new NODE;
	if (nullptr == new_data_ptr || nullptr == new_node_ptr) {
		return nullptr;
	}

	new_data_ptr->age = age;
	// 배열인 경우에는 C++ 템플릿을 이용해 컴파일 타임에 크기를 알 수 있다
	strcpy_s(new_data_ptr->name, sizeof new_data_ptr->name, name);
	strcpy_s(new_data_ptr->address, sizeof new_data_ptr->address, address);
	strcpy_s(new_data_ptr->phone, sizeof new_data_ptr->phone, phone);

	new_node_ptr->next_ptr = nullptr;
	new_node_ptr->prev_ptr = nullptr;
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

	return new_data_ptr;
}

NODE* Dequeue()
{
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

	return pop_data;
}

bool IsEmpty()
{
	return g_head_node.next_ptr == &g_tail_node;
}

void FreeQueryResult(USERDATA** result_list)
{
	// 연결 리스트 데이터는 이벤트 루프 끝나고 별도로 해지하는 로직이 있고
	// 여기에서는 연결 리스트에 존재하는 데이터 주소를 동적 배열 요소에 단순 복사한 거기 때문에
	// 동적 배열로 생성한 부분만 해제한다
	delete[] result_list;
}

void ReleaseNodeList()
{
	NODE* node = g_head_node.next_ptr;

	while (nullptr != node && &g_tail_node != node) {
		NODE* next = node->next_ptr;
		delete node;

		node = next;
	}

	InitList();
}

char* Parser(char* sql)
{
	return nullptr;
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
	int size = 0;
	char** split_result = SplitString(token, &size, '=');
	char* last_string = GetLastString(split_result, size);
	TrimSingleQuote(last_string);
	TrimLineFeed(last_string);

	int result = atoi(last_string);
	*value = result;

	FreeSplitString(split_result, size);
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
		USERDATA* find_user = static_cast<USERDATA*>(find->data_cache);

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

	return nullptr;
}

USERDATA** ParseAndCommandInsert(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = AGE_INIT_VALUE;

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

	if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && AGE_INIT_VALUE == age) return nullptr;

	return InsertByQuery(name, address, phone, age, data_size);
}

USERDATA** ParseAndCommandSelect(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = AGE_INIT_VALUE;

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

	//if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && -1 == age) return nullptr;

	return SelectByQuery(name, address, phone, age, data_size);
}

USERDATA** ParseAndCommandDelete(char* sql, int* data_size)
{
	char name[32] = { 0 };
	char address[32] = { 0 };
	char phone[32] = { 0 };
	int age = AGE_INIT_VALUE;

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

	if (!strcmp(name, "") && !strcmp(address, "") && !strcmp(phone, "") && AGE_INIT_VALUE == age) return nullptr;

	USERDATA** search_result = SelectByQuery(name, address, phone, age, data_size);
	if (nullptr == search_result) return nullptr;

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
	int set_age = AGE_INIT_VALUE;
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
	int where_age = AGE_INIT_VALUE;
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

	if (!strcmp(where_name, "") && !strcmp(where_address, "") && !strcmp(where_phone, "") && AGE_INIT_VALUE == where_age) return nullptr;

	USERDATA** search_result = SelectByQuery(where_name, where_address, where_phone, where_age, data_size);
	if (nullptr == search_result) return nullptr;

	if (!strcmp(set_name, "") && !strcmp(set_address, "") && !strcmp(set_phone, "") && AGE_INIT_VALUE == set_age
		&& !strcmp(where_name, "") && !strcmp(where_address, "") && !strcmp(where_phone, "") && AGE_INIT_VALUE == where_age) return nullptr;

	FreeSplitString(where_string, where_size);
	FreeSplitString(set_string, set_size);
	FreeSplitString(split_result, split_size);

	return UpdateByQuery(set_name, set_address, set_phone, set_age, where_name, where_address, where_phone, where_age, data_size);
}

USERDATA** InsertByQuery(const char* name, const char* address, const char* phone, int age, int* out_size)
{
	USERDATA insert_data = USERDATA_INIT;
	SetUserData(&insert_data, name, address, phone, age);

	USERDATA* new_user = AddNewNodeAtTail(true, insert_data.name, insert_data.age, insert_data.address, insert_data.phone, AGE_INIT_VALUE);
	USERDATA** result_list = new USERDATA*[1];

	if (nullptr == result_list) return nullptr;

	result_list[0] = new_user;
	*out_size = 1;

	return result_list;
}

USERDATA** SelectByQuery(const char* name, const char* address, const char* phone, int age, int* out_size)
{
	FILE* fp = nullptr;
	fopen_s(&fp, "user_address.dat", "rb");
	if (nullptr == fp)
	{
		puts("file open failure");
		return nullptr;
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
	*out_size = data_count;
	if (0 == data_count) return nullptr;

	USERDATA** result_list = FindUsersFromListBySearchData(&search_data, data_count);
	if (nullptr == result_list) return nullptr;

	return result_list;
}

USERDATA** UpdateByQuery(const char* set_name, const char* set_address, const char* set_phone, int set_age,
	const char* where_name, const char* where_address, const char* where_phone, int where_age, int* out_data_size)
{
	USERDATA set_data = USERDATA_INIT, where_data = USERDATA_INIT;
	SetUserData(&set_data, set_name, set_address, set_phone, set_age);
	SetUserData(&where_data, where_name, where_address, where_phone, where_age);

	int data_count = 0;
	NODE* find_node = g_head_node.next_ptr;
	while (find_node)
	{
		USERDATA* find_data = static_cast<USERDATA*>(find_node->data_cache);
		if (IsValidData(&where_data, find_data))
		{
			UpdateNode(find_node, &set_data);
			++data_count;
		}

		find_node = find_node->next_ptr;
	}

	*out_data_size = data_count;
	if (0 == data_count) return nullptr;

	USERDATA** result_list = FindUsersFromListBySetData(&set_data, data_count);

	if (nullptr == result_list) return nullptr;

	return result_list;
}

USERDATA** DeleteByQuery(const char* name, const char* address, const char* phone, int age, int* out_data_size)
{
	USERDATA delete_data = USERDATA_INIT;
	SetUserData(&delete_data, name, address, phone, age);

	int data_count = 0;
	NODE* find_node = g_head_node.next_ptr;
	while (find_node)
	{
		USERDATA* find_data = static_cast<USERDATA*>(find_node->data_cache);
		if (IsValidData(&delete_data, find_data))
		{
			DeleteNode(find_node);
			++data_count;
		}

		find_node = find_node->next_ptr;
	}

	*out_data_size = data_count;
	if (0 == data_count) return nullptr;

	USERDATA** result_list = FindUsersFromListByDeleteData(&delete_data, data_count);

	if (nullptr == result_list) return nullptr;

	return result_list;
}

void DeleteNode(NODE* tobe_deleted)
{
	tobe_deleted->is_deleted = true;
}

USERDATA** FindUsersFromListByDeleteData(USERDATA* delete_data, int size)
{
	if (nullptr == delete_data || 0 == size) return nullptr;

	USERDATA** result = new USERDATA * [size];

	if (nullptr == result) return nullptr;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = static_cast<USERDATA*>(find_node->data_cache);
		if (true == find_node->is_deleted && IsValidData(delete_data, find))
		{
			result[index++] = find;
		}

		find_node = find_node->next_ptr;
	}

	return result;
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
	if (AGE_INIT_VALUE != age)
	{
		data->age = age;
	}
}

bool IsValidData(USERDATA* where, USERDATA* find)
{
	if (0 == (Empty(where->name) ? 0 : strcmp(find->name, where->name))
		&& 0 == (Empty(where->phone) ? 0 : strcmp(find->phone, where->phone))
		&& find->age == (AGE_INIT_VALUE < where->age ? where->age : find->age)
		&& Includes(find->address, where->address))
		return true;

	return false;
}

USERDATA** FindUsersFromListBySearchData(USERDATA* search, int size)
{
	if (nullptr == search || 0 == size) return nullptr;

	USERDATA** result = new USERDATA * [size];

	if (nullptr == result) return nullptr;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = static_cast<USERDATA*>(find_node->data_cache);
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
	if (nullptr == search || 0 == size) return nullptr;

	USERDATA** result = new USERDATA * [size];

	if (nullptr == result) return nullptr;

	NODE* find_node = g_head_node.next_ptr;

	int index = 0;
	while (index != size && find_node)
	{
		USERDATA* find = static_cast<USERDATA*>(find_node->data_cache);
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
	SetUserData(static_cast<USERDATA*>(tobe_updated->data_cache), updating->name,
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

	char** set_string = new char * [size];
	if (nullptr == set_string) return nullptr;

	int index = 0;
	for (int i = start; i < array_size; ++i)
	{
		if (Equal(string_array[i], "where")) break;
		else if (Equal(string_array[i], "and")) continue;

		int token_size = (int)strlen(string_array[i]) + 1;
		char* token = new char[token_size];
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
	if (where_next_index == 0) return nullptr;

	++where_next_index;

	// get 'and' count
	int size = 0;
	for (int i = where_next_index; i < array_size; ++i)
	{ 
		if (Equal(string_array[i], "and")) ++size;
	}
	++size;
	*out_size = size;

	char** set_string = new char * [size];
	if (nullptr == set_string) return nullptr;

	int index = 0;
	for (int i = where_next_index; i < array_size; ++i)
	{
		if (Equal(string_array[i], "and")) continue;

		TrimLineFeed(string_array[i]);
		int token_size = (int)strlen(string_array[i]) + 1;
		char* token = new char[token_size];
		memcpy_s(token, token_size, string_array[i], token_size);

		set_string[index++] = token;
	}

	return set_string;
}

char** SplitString(char* string, int* out_size, char delim)
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

	*out_size = delim_count + 1;
	char** string_array = new char * [delim_count + 1];
	if (nullptr == string_array) return nullptr;	

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

			char* token = new char[dist + 1];
			if (nullptr == token) return nullptr;

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
		delete string[i];
	delete[] string;
}

void Commit()
{
	NODE* node = g_head_node.next_ptr;
	while (node)
	{
		if (node->is_deleted)
		{
			FILE* fp = nullptr;
			fopen_s(&fp, "user_address.dat", "rb+");
			if (nullptr == fp) break;

			USERDATA* user = static_cast<USERDATA*>(node->data_cache);
			fseek(fp, node->offset * sizeof(USERDATA), SEEK_SET);
			USERDATA init = USERDATA_INIT;
			if (0 < fwrite(&init, sizeof(USERDATA), 1, fp))
			{
				printf("delete: %s %s %s\n", user->name, user->address, user->phone);

				FILE* fp_deleted = nullptr;
				fopen_s(&fp_deleted, "user_address_deleted.dat", "ab+");
				if (nullptr == fp_deleted) break;

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
			FILE* fp = nullptr;
			fopen_s(&fp, "user_address.dat", "ab+");
			if (nullptr == fp) break;

			USERDATA* user = static_cast<USERDATA*>(node->data_cache);

			// find deleted offset
			FILE* fp_deleted = nullptr;
			fopen_s(&fp_deleted, "user_address_deleted.dat", "rb+");
			if (nullptr == fp_deleted)
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
			FILE* fp = nullptr;
			fopen_s(&fp, "user_address.dat", "rb+");
			if (nullptr == fp) break;

			USERDATA* user = static_cast<USERDATA*>(node->data_cache);
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