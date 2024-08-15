#pragma once

#include "data_struct.h"

typedef enum MENU { QUERY, COMMIT, ROLLBACK, EXIT, END } MENU;

extern const char* const message[3];
extern void (*Inputs[END])();

void PrintUser(USERDATA* user_data);
void PrintItem(NODE*);
MENU PrintMenu();
void ClearBuffer();
void Pause();

void InputQuery();
void InputCommit();
void InputRollback();