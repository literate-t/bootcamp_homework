#pragma once
#include <string>

class UserData {
public:
	UserData() = delete;	
	explicit UserData(std::string name, std::string address, std::string phone, int age);
	~UserData() = default;

	std::string GetName() const;
	std::string GetAddress() const;
	std::string GetPhone() const;
	int GetAge() const;

private:
	std::string name_;
	std::string address_;
	std::string phone_;
	int age_ = -1;
};