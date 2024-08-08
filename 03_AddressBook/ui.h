#pragma once

#include "data_struct.h"

//typedef enum MENU { EXIT, NEW, SORT, SEARCH, SEARCH_RANGE, PRINT, REVERSE_PRINT, REMOVE, END } MENU;
typedef enum MENU { QUERY, COMMIT, ROLLBACK, EXIT, END } MENU;

void PrintUser(USERDATA* user_data);
void PrintItem(NODE*);
MENU PrintMenu();
void ClearBuffer();
void Pause();
extern const char* const message[3];

void InputQuery();
void InputCommit();
void InputRollback();

void (*Inputs[END])();