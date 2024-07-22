#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mymath.h"
#include "ui.h"
#include "typedef.h"

char operators[] = { '+', 'x', '*', '/', '-' };
const int operators_count = sizeof(operators);
const int kOneByte = 8;

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

int PlusWithBitwise(int x, int y) {
	if (0 == y) return x;
	return PlusWithBitwise(x ^ y, (x & y) << 1);
}

int MinusWithBitwise(int x, int y) {
	return x + (~y + 1);
}

int MultiplyWithBitwise(int x, int y) {

	// 011	 3
	// 010	 2
	// ---
	//   0
	//0110 -> 011 * 10은 0110 * 1과 동일
	// 011은 << 1 연산, 10은 >> 1 연산

	int first = MyAbs(x);
	int second = MyAbs(y);

	int result = 0;
	while (0!= first && 0 != second) {
		// 끝자리 1 확인
		if (second & 1) {
			result += first;
		}
		// 끝자리가 0이면 곱셈할 게 없다
		// 다음 연산을 위해 x는 비트는 << 1, y는 1 >>
		first <<= 1;
		second >>= 1;
	}

	return IsSameSign(x, y) ? result : ~result + 1;
}

void DivideWithBitwise(int dividend, int dividor, size_t bit_size, int* quotient, int* remainder) {
	// 몫을 피제수로 초기화
	// 피제수 MSB부터 하나씩 r로 옮기고
	// LSB부터는 몫을 추가하게 된다
	// 절대값으로 연산하고 마지막에 부호를 조정한다
	int q = MyAbs(dividend);
	int d = MyAbs(dividor);
	int r = 0;

	// 자료형 크기만큼 비트를 순회한다
	for (int i = 0; i < bit_size; ++i) {
		// 피제수의 msb를 r로 이동
		r <<= 1;
		r |= (q >> 31) & 1;
		q <<= 1;

		// r에서 divisor를 뺄셈할 때(r - d)
		// 부호가 바뀌면 해당 비트는 연산하지 않는다
		// r과 d가 부호가 다르면(r + d)
		// 둘의 부호가 다르면 xor 연산은 음수일 수밖에 없다
		int temp_r = 0;
		if ((r ^ d) < 0) {
			temp_r = r + d;
		}
		else {
			temp_r = r - d;
		}


		if ((temp_r ^ r) < 0) {
			q |= 0;
		} // temp_r이 r과 부호가 같을 때만 몫을 계산한다
		else {
			r = temp_r;
			q |= 1;
		}
	}

	// 마지막으로 제수와 피제수의 부호에 따른 결과 조정
	// 다르면 q를 보수로 변경
	if ((dividor ^ dividend) < 0) {
		q = ~q + 1;
	}
	if (IsMinus(dividend)) {
		r = ~r + 1;
	}

	*quotient = q;
	*remainder = r;
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

int MySquare(int number, int exponent) {
	int result = 1;
	for (int i = 0; i < exponent; ++i) {
		result = MultiplyWithBitwise(result, number);
	}

	return result;
}

int MyAtoI(const char* number_str) {
	int size = (int)strlen(number_str);
	int result = 0;
	char sign = *number_str;
	const char* number = NULL;

	if (IsSign(sign)) {
		number = number_str + 1;
		size -= 1;
	}
	else {
		number = number_str;
	}

	for (int i = 0; i < size; ++i) {
		result += MySquare(10, size - 1 - i) * (number[i] - '0');
	}

	if ('-' == sign) {
		result = ~result + 1;
	}

	return result;
}

void Split(char* input, size_t input_size, char* op, int* first, int* second) {
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

	*first = MyAtoI(lhs);
	*second = MyAtoI(rhs);

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
