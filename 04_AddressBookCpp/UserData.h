#pragma once
#include "IDataStructure.h"

class UserData : public IDataStructure {
public:
	UserData* dummy_head_node_;
	UserData* dummy_tail_node_;



private:
	std::string name;
	std::string address;
	std::string phone;
	int age;
};