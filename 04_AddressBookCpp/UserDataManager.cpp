#include "UserDataManager.h"
#include <cstring>

UserDataManager::UserDataManager(const UserData& rhs)
{
	memcpy_s(user_data_.name_, sizeof user_data_.name_, rhs.name_, sizeof rhs.name_);
	memcpy_s(user_data_.address_, sizeof user_data_.address_, rhs.address_, sizeof rhs.address_);
	memcpy_s(user_data_.phone_, sizeof user_data_.phone_, rhs.phone_, sizeof rhs.phone_);
	user_data_.age_ = rhs.age_;
}

UserDataManager::UserDataManager(const UserData* rhs)
{
	memcpy_s(user_data_.name_, sizeof user_data_.name_, rhs->name_, sizeof rhs->name_);
	memcpy_s(user_data_.address_, sizeof user_data_.address_, rhs->address_, sizeof rhs->address_);
	memcpy_s(user_data_.phone_, sizeof user_data_.phone_, rhs->phone_, sizeof rhs->phone_);
	user_data_.age_ = rhs->age_;
}

UserDataManager::UserDataManager(UserData&& rhs)
{
	memcpy_s(user_data_.name_, sizeof user_data_.name_, rhs.name_, sizeof rhs.name_);
	memcpy_s(user_data_.address_, sizeof user_data_.address_, rhs.address_, sizeof rhs.address_);
	memcpy_s(user_data_.phone_, sizeof user_data_.phone_, rhs.phone_, sizeof rhs.phone_);
	user_data_.age_ = rhs.age_;
}

UserDataManager::UserDataManager(const char* name, int age, const char* address, const char* phone)
{
	memcpy_s(user_data_.name_, sizeof user_data_.name_, name, strlen(name) + 1);
	memcpy_s(user_data_.address_, sizeof user_data_.address_, address, strlen(address) + 1);
	memcpy_s(user_data_.phone_, sizeof user_data_.phone_, phone, strlen(phone) + 1);
	user_data_.age_ = age;
}

UserDataManager::UserDataManager(string& name, int age, string& address, string& phone)
{
	memcpy_s(user_data_.name_, sizeof user_data_.name_, name.c_str(), name.size());
	memcpy_s(user_data_.address_, sizeof user_data_.address_, address.c_str(), address.size());
	memcpy_s(user_data_.phone_, sizeof user_data_.phone_, phone.c_str(), phone.size());
	user_data_.age_ = age;
}

UserData* UserDataManager::GetUserData()
{
	return &user_data_;
}

const string UserDataManager::GetUserName() const
{
	return string(user_data_.name_);
}

const string UserDataManager::GetUserAddress() const
{
	return string(user_data_.address_);
}

const string UserDataManager::GetUserPhone() const
{
	return string(user_data_.phone_);
}

const int UserDataManager::GetUserAge() const
{
	return user_data_.age_;
}
