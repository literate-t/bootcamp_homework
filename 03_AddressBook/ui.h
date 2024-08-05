#pragma once

#include "data_struct.h"

//typedef enum MENU { EXIT, NEW, SORT, SEARCH, SEARCH_RANGE, PRINT, REVERSE_PRINT, REMOVE, END } MENU;
typedef enum MENU { QUERY, COMMIT, ROLLBACK, EXIT, END } MENU;

void PrintList(bool);
void PrintItem(NODE*);
MENU PrintMenu();
void ClearBuffer();
void InputName(char* name, unsigned int size);
void JoinMember();
void SearchRange();
void Pause();
extern const char* const message[3];

void InputQuery();
void InputCommit();
void InputRollback();

void (*Inputs[END])();