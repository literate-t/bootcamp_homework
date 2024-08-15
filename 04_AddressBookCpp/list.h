#pragma once
#include "data_struct.h"
#include <stdbool.h>

extern NODE g_head_node;
extern NODE g_tail_node;

extern int data_count;

void InitList();
void Push(bool is_new, USERDATA*);
USERDATA* AddNewNodeAtTail(bool is_new, const char* name, int age, const char* address, const char* phone, int offset);
NODE* Pop();
NODE* Dequeue();

bool IsAllDataEmpty(const char* name, int age, const char* address, const char* phone);

void IncreaseCount();
void DecreaseCount();
void InitCount();

void ReleaseNodeList();

void FreeQueryResult(USERDATA** result_list);

void RemoveNode(NODE*);

void EventLoopRun();
bool IsEmpty();

bool Includes(const char* whole, const char* part);
USERDATA** Query(char* sql, int *size);
char** SplitString(char* string, int* size, char delim);
void FreeSplitString(char** string, int size);

void ParseStringValue(char* token, char* out_value, int value_size);
void ParseIntValue(char* token, int* value, int value_size);
void ToLowerCase(char* str_ptr);
bool TrimAroundEqual(char* str_ptr);
void TrimSingleQuote(char* value);
void TrimLineFeed(char* value);
char* GetLastString(char** string, int size);

bool Equal(const char* first, const char* second);
bool Empty(const char* str);

bool IsDuplicatedUserFromList(USERDATA* user_data);
bool IsSameUserData(USERDATA* first, USERDATA* second);

// Query
USERDATA** ParseAndCommandInsert(char* sql, int* data_size);
USERDATA** ParseAndCommandSelect(char* sql, int* data_size );
USERDATA** ParseAndCommandUpdate(char* sql, int* data_size );
USERDATA** ParseAndCommandDelete(char* sql, int* data_size );

USERDATA** InsertByQuery(const char* name, const char* address, const char* phone, int age, int* size);
USERDATA** SelectByQuery(const char* name, const char* address, const char* phone, int age, int* size);
USERDATA** UpdateByQuery(const char* set_name, const char* set_address, const char* set_phone, int set_age,
	const char* where_name, const char* where_address, const char* where_phone, int where_age, int* data_size);
USERDATA** DeleteByQuery(const char* name, const char* address, const char* phone, int age, int* data_size);

char** GetSetString(char** string_array, int array_size, int* out_size);
char** GetWhereString(char** string_array, int array_size, int* out_size);

void UpdateNode(NODE* tobe_updated, USERDATA* updating);
void DeleteNode(NODE* tobe_deleted);

void SetUserData(USERDATA* data, const char* name, const char* address, const char* phone, int age);

bool IsValidData(USERDATA* where, USERDATA* find);

USERDATA** FindUsersFromListBySearchData(USERDATA* search, int size);
USERDATA** FindUsersFromListBySetData(USERDATA* search, int size);
USERDATA** FindUsersFromListByDeleteData(USERDATA* search, int size);

void Commit();