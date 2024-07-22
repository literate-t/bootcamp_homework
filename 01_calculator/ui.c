#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "ui.h"
#include "mymath.h"

const char* message[END] = { "", "24 bytes exceeded", "Wrong input" };

void EventLoop() {
	char input[24] = "";
	size_t input_size = sizeof input;

	while (1) {
		if (!Input(input, input_size)) {
			return;
		}

		size_t size = strlen(input);

		char op = 0;
		int first = 0, second = 0;
		Split(input, size + 1, &op, &first, &second);

		switch (op) {
		case 'x':
		case '*':
			printf("> %d\n", MultiplyWithBitwise(first, second));
			break;
		case '+':
			printf("> %d\n", PlusWithBitwise(first, second));
			break;
		case '/': {
			size_t size = GetBitSize(first);
			int q, r;
			if (0 == second) {
				puts("0으로 나눌 수 없습니다");
				break;
			}

			DivideWithBitwise(first, second, size, &q, &r);
			printf("> 몫:%d, 나머지:%d\n", q, r);
			break;
		}
		case '-':
			printf("> %d\n", MinusWithBitwise(first, second));
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
		SUCCESS != flag && printf("%s\n", message[flag]);
		ClearInputBuffer();
	}

	return 1;
}