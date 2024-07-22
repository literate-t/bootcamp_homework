#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "mymath.h"

void EventLoop() {
	char input[48] = "";
	size_t input_size = sizeof input;

	while (1) {
		if (!Input(input, input_size)) {
			return;
		}

		size_t size = strlen(input);

		char op = 0;
		char first[24] = "", second[24] = "";
		Split(input, size + 1, &op, first, second);

		switch (op) {
		case '+':
			Plus(first, second);
			char result[52] = "";
			MakeReverse(result, (int)sizeof result);
			printf("> %s\n", result);
			break;
		case '-':
			//printf("> %d\n", Minus(first, second));
			break;
		default:
			puts("Wrong input");
		}
	}
}

int Input(char* input, size_t size) {
	int flag = 1;

	while (flag) {
		printf("> ");
		scanf_s("%s", input, (unsigned)size);
		if (!strcmp("exit", input)) {
			puts("계산기를 종료합니다");
			return 0;
		}

		int str_size = (int)strlen(input) + 1;
		flag = IsValidInput(input, str_size, size);
		ClearInputBuffer();
	}

	return 1;
}