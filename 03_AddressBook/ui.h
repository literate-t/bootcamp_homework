#pragma once

struct USERDATA;
struct NODE;

typedef enum MENU { EXIT, NEW, SORT, SEARCH, SEARCH_RANGE, PRINT, REVERSE_PRINT, REMOVE, END } MENU;

void PrintList(bool);
void PrintItem(NODE*);
MENU PrintMenu();
void ClearBuffer();
void InputName(char* name, unsigned int size);
void JoinMember();
void SearchRange();
void Pause();
extern const char* const message[3];