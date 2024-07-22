#pragma once

extern char operators[];
extern const int operators_count;
extern const int kOneByte;

// Arithmatic
int IsSameSign(int x, int y);
int IsMinus(int number);
int MyAbs(int number);
int PlusWithBitwise(int x, int y);
int MinusWithBitwise(int x, int y);
int MultiplyWithBitwise(int x, int y);
void DivideWithBitwise(int dividend, int dividor, size_t bit_size, int* quotient, int* remainder);

// Arithmatic Logic
int IsNumber(char ch);
int IsSign(char ch);
int IsNotNumberNotOperator(char ch);
int IsValidInput(char* input, const size_t string_size, const size_t total_size);
int NotOperatorIncludes(char ch);
int NotNumberIncludes(char ch);

int MySquare(int number, int exponent);
int MyAtoI(const char* number_str);
void Split(char* input, size_t input_size, char* op, int* first, int* second);

char* SetSign(char* data);
void CopyIncludingSign(char* dest, size_t dest_size, char* source);
size_t GetBitSize(int data);
void ClearInputBuffer();