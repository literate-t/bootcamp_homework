#include "ui.h"
#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <stdbool.h>
#include <time.h>

const char* const message[] = {"나이를 입력하세요: ", "이름을 입력하세요: ", "전화 번호를 입력하세요: " };

void PrintItem(NODE* data_node) 
{
	if (NULL == data_node) {
		return;
	}

	USERDATA* data = (USERDATA*)(data_node->data_cache);
	printf("[prev: %p]\t[me: %p] %d, %s\t\t[next: %p]\n",
		data_node->prev_ptr, data, data->age, data->name, data_node->next_ptr);
}

MENU PrintMenu() 
{
	MENU input = END;

	do {
		system("cls");
		puts("[1]Input Query [2]Commit [3]Rollback [4]Exit");
		printf("Input the number: ");
		scanf_s("%d", &input);
		ClearBuffer();
	} while (!(0 < input && input <= END));


	return input - 1;
}

void Pause() 
{
	_getch();
}

void ClearBuffer() 
{
	while (getchar() != '\n') {}
}

void InputQuery()
{
	char buffer[256];
	int buffer_size = sizeof buffer;
	printf("Query> ");
	fgets(buffer, buffer_size, stdin);

	struct timespec start, end;
	double elapsed_ms;

	int size = 0;
	timespec_get(&start, TIME_UTC);
	USERDATA** query_result = Query(buffer, &size);
	timespec_get(&end, TIME_UTC);
	elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0;
	elapsed_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
	printf("Elapsed time: %f ms\n", elapsed_ms);

	if (NULL == query_result)
	{
		puts("No data");
	}
	else
	{
		printf("data size: %d\n", size);
		for (int i = 0; i < size; ++i)
		{
			PrintUser(query_result[i]);
		}
	}

	FreeQueryResult(query_result);
}

void InputCommit()
{
	puts("Commit!");
	Commit();
}

void InputRollback()
{
	puts("Rolback!");
	ReleaseNodeList();
}

void (*Inputs[END])() = { InputQuery, InputCommit, InputRollback };

void PrintUser(USERDATA* user_data)
{
	printf("%s %d세 %s %s\n", user_data->name, user_data->age, user_data->address,  user_data->phone);
}

void EventLoopRun() 
{
	MENU menu;

	while ((menu = PrintMenu()) < END && 0 <= menu) {
		
		if (EXIT == menu)
		{
			puts("Bye!");
			return;
		}

		Inputs[menu]();
		Pause();
	}
}