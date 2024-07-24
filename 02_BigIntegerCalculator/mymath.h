#pragma once
#pragma once

extern char operators[];
extern const int operators_count;
extern const int kOneByte;
extern char answer[60];
extern int answer_index;
enum {SAME, FIRST_BIG, SECOND_BIG};

// Arithmatic
int IsSameSign(int x, int y);
int IsSameSignCh(char x, char y);
int IsSameNumber(char* x, int x_len, char* y, int y_len);
int IsMinus(int number);
int IsMinusCh(char ch);
int MyAbs(int number);
void Plus(char* x, char* y);
void Minus(char* x, char* y);

// Arithmatic Logic
int IsNumber(char ch);
int IsSign(char ch);
int IsNotNumberNotOperator(char ch);
int IsValidInput(char* input, const size_t string_size, const size_t total_size);
int NotOperatorIncludes(char ch);
int NotNumberIncludes(char ch);

int MySquare(int number, int exponent);
int MyAtoI(char ch);
char MyItoA(int i);
int MakeZeroToTen(int i);

int MakeCarry(int number);
int MakeBeforeCarry(int number);

void Push(char ch);
void MakeReverse(char* result, int dest_size);

void Split(char* input, size_t input_size, char* op, char* first, char* second);
void Swap(char* a, char* b);
void RemoveChar(char* data, int position);
void AddMinusCh(char* data, int position);
void RemoveZeroAndPlusFromHead(char* data);

char* SetSign(char* data);
void CopyIncludingSign(char* dest, size_t dest_size, char* source);
size_t GetBitSize(int data);
void ClearInputBuffer();