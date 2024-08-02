#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <conio.h>

#include "ui.h"
#include "list.h"

const char* const message[] = {"나이를 입력하세요: ", "이름을 입력하세요: ", "전화 번호를 입력하세요: " };

void PrintList(bool reverse) {
	NODE* start_ptr = reverse ? &g_tail_node : &g_head_node;
	if (nullptr == start_ptr) {
		puts("데이터가 없습니다.\n");
		return;
	}

	while (start_ptr != nullptr) {
		PrintItem(start_ptr);
		start_ptr = reverse ? start_ptr->prev_ptr : start_ptr->next_ptr;
	}
	puts("End of list");
}

void PrintItem(NODE* data_node) {
	if (nullptr == data_node) {
		return;
	}

	USERDATA* data = reinterpret_cast<USERDATA*>(data_node->data_cache);
	printf("[prev: %p]\t[me: %p] %d, %s\t\t[next: %p]\n",
		data_node->prev_ptr, data, data->age, data->name, data_node->next_ptr);
}

MENU PrintMenu() {
	MENU input = END;

	do {
		system("cls");
		puts("[1]New [2]Sort [3]Search [4]Search range [5]Print [6]Print reverse [7]Remove [0]Exit");
		printf("Input the number: ");
		scanf_s("%d", &input);
		ClearBuffer();
	} while (END <= input);


	return input;
}

void Pause() {
	_getch();
}

void ClearBuffer() {
	while (getchar() != '\n') {}
}

void JoinMember() {
	int age;
	char name[32];
	char phone[32];

	printf("%s", message[0]);
	scanf_s("%d%*c", &age);
	printf("%s", message[1]);
	scanf_s("%s", name, (unsigned)_countof(name));
	printf("%s", message[2]);
	scanf_s("%s", phone, (unsigned)_countof(phone));
	
	USERDATA new_user = {};
	new_user.age = age;
	memcpy_s(new_user.name, sizeof(new_user.name), name, sizeof(name));
	memcpy_s(new_user.phone, sizeof(new_user.phone), phone, sizeof(phone));

	Enqueue(&new_user);
}

void InputName(char* name, unsigned int size) {
	printf("name: ");
	scanf_s("%s", name, size);
}

void Search() {
	char name[32] = {};
	InputName(name, sizeof(name));
	NODE* find_node = SearchByName(name);
	printf("found: ");
	PrintItem(find_node);
}

void SearchRange() {
	int minimum_age, maximum_age;
	printf("Input age range> ");
	scanf_s("%d%d", &minimum_age, &maximum_age);

	int size = 0;
	USERDATA** result = SearchRangeByAgeIndex(minimum_age, maximum_age, &size);
	if (nullptr == result) {
		puts("No result");
	} 
	else {
		for (int i = 0; i < size; ++i) {
			printf("name: %s\tage: %d\n", result[i]->name, result[i]->age);
		}

		free(result);
	}
}

void Remove() {
	char name[32] = {};
	InputName(name, sizeof(name));
	DeleteByName(name);
}

void EventLoopRun() {
	MENU menu;

	while ((menu = PrintMenu()) < END) {
		switch (menu) {
		case EXIT:
			puts("Bye!");
			return;
		case NEW:
			JoinMember();
			break;
		case SORT:
			Sort(&g_head_node.next_ptr);
			break;
		case SEARCH:
			Search();
			break;
		case SEARCH_RANGE:
			// sort first, search later			
			SearchRange();
			break;
		case PRINT:
			PrintList(false);
			break;
		case REVERSE_PRINT:
			PrintList(true);
			break;
		case REMOVE:
			Remove();
			break;
		default:			
			break;
		}

		Pause();
	}
}