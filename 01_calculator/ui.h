#pragma once

enum { SUCCESS, WRONG_SIZE, WRONG_INPUT, END };
extern const char* message[END];

void EventLoop();
int Input(char* input, size_t size);