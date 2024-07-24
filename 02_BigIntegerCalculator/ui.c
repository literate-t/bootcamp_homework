#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "mymath.h"

void EventLoop() {
	char input[56] = "";
	size_t input_size = sizeof input;

	while (1) {
		memset(answer, 0, sizeof answer);
		answer_index = 0;

		// 0으로 시작하는 입력값 검사
		if (!Input(input, input_size)) {
			return;
		}

		size_t size = strlen(input);

		char op = 0;
		char first[56] = "", second[56] = "";
		Split(input, size + 1, &op, first, second);

		char result[60] = "";
		switch (op) {
		case '+': {
			Plus(first, second);
			MakeReverse(result, (int)sizeof result);
			break;
		}
		case '-': {
			char result[52] = "";
			Minus(first, second);
			MakeReverse(result, (int)sizeof result);
			break;
		}
		default:
			puts("Wrong input");
		}

		MakeReverse(result, (int)sizeof result);
		printf("> %s\n", result);
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