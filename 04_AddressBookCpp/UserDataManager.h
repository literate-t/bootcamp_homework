#pragma once
#include "UserData.h"
#include <string>
using namespace std;

class UserDataManager
{
public:
	UserDataManager() = default;
	explicit UserDataManager(const UserData& rhs);
	explicit UserDataManager(const UserData* rhs);
	explicit UserDataManager(UserData&& rhs);
	explicit UserDataManager(const char* name, int age, const char* address, const char* phone);
	UserDataManager(string& name, int age, string& address, string& phone);
	~UserDataManager() = default;

	UserData* GetUserData();
	const string GetUserName() const;
	const string GetUserAddress() const;
	const string GetUserPhone() const;
	const int GetUserAge() const;

private:
	UserData user_data_ = {};
};
