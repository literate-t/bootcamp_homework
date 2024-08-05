#pragma once
#include <stdbool.h>

typedef struct USERDATA {
	int age;
	char name[32];
	char phone[32];
} USERDATA;

typedef struct NODE
{
	bool is_new;
	bool is_modified;
	bool is_deleted;

	char* key;
	void* data_cache;       // USERDATA*
	unsigned int data_size; // sizeof USERDATA
	unsigned int offset;    // 파일 상의 위치

	struct NODE* next_ptr;
	struct NODE* prev_ptr;
} NODE;