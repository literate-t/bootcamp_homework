#include "UserData.h"

UserData::UserData(std::string name, 
	std::string address, 
	std::string phone, 
	int age) : name_{ name }, address_{ address }, phone_{ phone }, age_{ age } {}

std::string UserData::GetName() const {
	return name_;
}

std::string UserData::GetAddress() const {
	return address_;
}

std::string UserData::GetPhone() const {
	return phone_;
}

int UserData::GetAge() const {
	return age_;
}