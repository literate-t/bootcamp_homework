#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mymath.h"
#include "ui.h"
#include "typedef.h"

char operators[] = { '+', '-' };
const int operators_count = sizeof(operators);
const int kOneByte = 8;
char answer[52] = "";

// Arithmatic
int IsSameSign(int x, int y) {
	return 0 <= (x ^ y);
}

int IsMinus(int number) {
	return 1 & (number >> 31);
}

int MyAbs(int number) {
	return IsMinus(number) ? -number : number;
}

int MyAtoI(char ch) {
	return ch - '0';
}

char MyItoA(int i) {
	return i + '0';
}

int MakeCarry(int number) {
	return 9 < number ? 1 : 0;
}

int MakeBeforeCarry(int number) {
	return 9 < number ? number - 10 : number;
}

void Push(char ch) {
	static int index = 0;
	answer[index++] = ch;
}

void Swap(char* a, char* b) {
	char temp = *a;
	*a = *b;
	*b = temp;
}

void MakeReverse(char* result, int dest_size) {
	strcpy_s(result, dest_size, answer);
	const int size = (int)strlen(answer);

	for (int i = 0, j = size - 1; i < j; ++i, --j) {
		Swap(&result[i], &result[j]);
	}	
}

void Plus(char* x, char* y) {
	if (NULL == x || NULL == y) return;

	int x_len = (int)strlen(x);
	int y_len = (int)strlen(y);

	int size = x_len < y_len ? y_len : x_len;
	int less_size = x_len < y_len ? x_len : y_len;
	int carry = 0, j = less_size - 1;

	for (int i = size - 1; 0 <= i; --i) {
		int first = MyAtoI(x[i]);
		int second = 0;
		if (0 <= j) {
			second = MyAtoI(y[j--]);
		}

		int result = first + second + carry;
		carry = MakeCarry(result);
		result = MakeBeforeCarry(result);

		Push(MyItoA(result));
	}

	// 둘이 부호가 다르면 사실상 마이너스
	// 둘이 음수이면 양수 덧셈 이후에 마이너스 부호를 붙인다
}

int Minus(char* x, char* y) {
	return 0;
}

// Arithmatic Logic
int IsNumber(char ch) {
	return '0' <= ch && ch <= '9' ? 1 : 0;
}

int IsSign(char ch) {
	return '+' == ch || '-' == ch ? 1 : 0;
}

int IsNotNumberNotOperator(char ch) {
	if (NotNumberIncludes(ch)) {
		return 1;
	}

	return 0;
}

int IsValidInput(char* input, const size_t string_size, const size_t total_size) {
	if (string_size <= 1) {
		return WRONG_SIZE;
	}

	// 형식 검사
	// 숫자와 연산자 이외의 문자 존재 확인
	for (int i = 0; i < string_size; ++i) {
		char ch = input[i];
		if ('\0' == ch) continue;
		else if (IsNotNumberNotOperator(ch)) {
			return 2;
		}
	}

	return 0;
}

int NotOperatorIncludes(char ch) {
	for (int i = 0; i < operators_count; ++i) {
		if (operators[i] == ch) {
			return 0;
		}
	}

	return 1;
}

int NotNumberIncludes(char ch) {
	if (!IsNumber(ch) && NotOperatorIncludes(ch)) {
		return 1;
	}

	return 0;
}

void Split(char* input, size_t input_size, char* op, char* first, char* second) {
	char* lhs = (char*)malloc(input_size);
	char* rhs = (char*)malloc(input_size);
	if (!lhs || !rhs) return;

	char* original_input = input;
	while (!IsNumber(*input)) {
		++input;
	}

	// 숫자 앞에 여러 개의 부호가 있을 경우 숫자 바로 앞 부호만 인정
	char* start = input;
	if (0 < input - original_input) {
		start = SetSign(input);
	}

	for (int i = 0; i < input_size; ++i) {
		if (!IsNumber(input[i])) {
			*op = input[i];
			input[i] = '\0';

			CopyIncludingSign(lhs, input_size, start);

			start = &input[i] + 1;
			original_input = start;
			while (!IsNumber(*start)) {
				++start;
			}
			if (0 < start - original_input) {
				start = SetSign(start);
			}

			CopyIncludingSign(rhs, input_size, start);

			break;
		}
	}

	strcpy_s(first, input_size, lhs);
	strcpy_s(second, input_size, rhs);

	free(rhs);
	free(lhs);
}

char* SetSign(char* data) {
	return data - 1;
}

void CopyIncludingSign(char* dest, size_t dest_size, char* source) {
	strcpy_s(dest, dest_size, source);
}

size_t GetBitSize(int data) {
	return sizeof data * kOneByte;
}

void ClearInputBuffer() {
	while ('\n' != getchar()) {}
}
