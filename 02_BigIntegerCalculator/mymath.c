#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mymath.h"
#include "ui.h"
#include "typedef.h"

#define IN
#define OUT
#define INOUT

char operators[] = { '+', '-' };
const int operators_count = sizeof(operators);
const int kOneByte = 8;
char answer[60] = "";
int answer_index = 0;

// Arithmatic
int IsSameSign(int x, int y) {
	return 0 <= (x ^ y);
}

int IsSameSignCh(char x, char y) {
	if ('-' == x && '-' == y) return 1;
	if ('-' != x && '-' != y) return 1;
	if ('+' == x && '+' == y) return 1;
	return 0;
}

int IsMinus(int number) {
	return 1 & (number >> 31);
}

int IsMinusCh(char ch) {
	return '-' == ch;
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
	answer[answer_index++] = ch;
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

void RemoveZeroAndPlusFromHead(char* data) {
	if ('-' == *data) ++data;
	char* start = data;
	while ('0' == *start || '+'== *start) {
		++start;
	}

	memcpy_s(data, 56, start, 56 - (start - data));
}

void Plus(char* x, char* y) {
	if (NULL == x || NULL == y) return;
	if (!IsSameSignCh(*x, *y)) {
		if (!IsMinusCh(*y)) {
			char* temp = x;
			x = y;
			y = temp;
		}

		RemoveChar(y, 0);
		Minus(x, y);
		return;
	}

	char* first_ptr = x;
	char* second_ptr = y;

	RemoveZeroAndPlusFromHead(first_ptr);
	RemoveZeroAndPlusFromHead(second_ptr);

	int first_length = (int)strlen(first_ptr);
	int second_length = (int)strlen(second_ptr);

	int bigger = IsSameNumber(first_ptr, first_length, second_ptr, second_length);
	if (SAME == bigger) {
		Push('0');
		return;
	}

	int size = first_length < second_length ? second_length : first_length;
	int less_size = first_length < second_length ? first_length : second_length;
	int carry = 0, j = less_size - 1;

	char* first_ptr_original = size == first_length ? first_ptr : second_ptr;
	char* second_ptr_original = size == first_length ? second_ptr : first_ptr;
	first_ptr = first_ptr_original;
	second_ptr = second_ptr_original;

	int is_minus = IsMinusCh(*first_ptr);
	if (is_minus) {
		++first_ptr;
		++second_ptr;
		--size;
		--j;
	}

	for (int i = size - 1; 0 <= i; --i) {
		int first = MyAtoI(first_ptr[i]);
		int second = 0;
		if (0 <= j) {
			second = MyAtoI(second_ptr[j--]);
		}

		int result = first + second + carry;
		carry = MakeCarry(result);
		result = MakeBeforeCarry(result);

		Push(MyItoA(result));
	}

	if (IsMinusCh(*x)) {
		Push('-');
	}
}
/*
* 1. 절대값 계산
* 2. (큰 길이의 값) - (작은 길이의 값)
* 3. 마이너스 부호 위치 판별해 결과 값에 붙이기
*/
void Minus(char* x, char* y) {
	if (NULL == x || NULL == y) return;
	if (!IsSameSignCh(*x, *y)) {
		if (IsMinusCh(*y)) {
			RemoveChar(y, 0);
		}
		else {			
			AddMinusCh(y, 0);
		}
		Plus(x, y);
		return;
	}

	char* first_ptr = x;
	char* second_ptr = y;

	RemoveZeroAndPlusFromHead(first_ptr);
	RemoveZeroAndPlusFromHead(second_ptr);

	int first_length = (int)strlen(first_ptr);
	int second_length = (int)strlen(second_ptr);

	int bigger = IsSameNumber(first_ptr, first_length, second_ptr, second_length);
	if (SAME == bigger) {
		Push('0');
		return;
	}

	int size = first_length < second_length ? second_length : first_length;
	int less_size = first_length < second_length ? first_length : second_length;
	int borrow = 0, j = less_size - 1;

	char* first_ptr_original = size == first_length ? first_ptr : second_ptr;
	char* second_ptr_original = size == first_length ? second_ptr : first_ptr;
	first_ptr = first_ptr_original;
	second_ptr = second_ptr_original;

	int is_minus = IsMinusCh(*first_ptr);
	if (is_minus) {
		++first_ptr;
		++second_ptr;
		--size;
		--j;
	}

	for (int i = size - 1; 0 <= i; --i) {
		int first = MyAtoI(first_ptr[i]);
		int second = 0;

		if (0 <= j) {
			second = MyAtoI(second_ptr[j--]);
		}

		if (first < second) {
			borrow = 1;
			char* start = &first_ptr[i - 1];

			while (1) {
				if ('0' == *start) {
					*start = '9';
					--start;
				}
				else {
					int new_number = MyAtoI(*start) - 1;
					*start = MyItoA(new_number);
					break;
				}
			}
		}

		int result = first + (borrow * 10) - second;
		borrow = 0;

		Push(MyItoA(result));
	}

	if (IsMinusCh(*first_ptr_original) && FIRST_BIG == bigger) {
		Push('-');
	}
	else if (!IsMinusCh(*first_ptr_original) && SECOND_BIG == bigger) {
		Push('-');
	}
}

int IsSameNumber(char* x, int x_len, char* y, int y_len) {
	if (x_len == y_len) {
		char* first = x;
		char* second = y;
		int is_first_bigger = 0;
		int is_second_bigger = 0;
		int count = 0;

		while ('\0' != *first) {
			++count;
			if (*second < *first) {
				is_first_bigger = 1;
				return FIRST_BIG;
			}
			else if (*first < *second) {
				is_second_bigger = 1;
				return SECOND_BIG;
			}

			++first;
			++second;
		}

		if (x_len == count) {
			return SAME;
		}
	}
	else if (y_len < x_len) {
		return FIRST_BIG;
	}
	
	return SECOND_BIG;
}

void RemoveChar(char* data, int position) {
	int size = (int)strlen(data);
	for (int i = position; i < size; ++i) {
		data[i] = data[i + 1];
	}
}

void AddMinusCh(char* data, int position) {
	if ('+' == *data) {
		*data = '-';
		return;
	}

	int size = (int)strlen(data);
	for (int i = size; position <= i ; --i) {
		data[i + 1] = data[i];
	}

	data[position] = '-';
}

int MakeZeroToTen(int i) {
	return 0 == i ? 10 : i;
}

void MakeBothSize(INOUT int* size, INOUT int* less_size) {

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
