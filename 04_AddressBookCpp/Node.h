#pragma once
#include <memory>
#include <string>

template<typename T>
class Node {
public:
	const T* GetUserData();
	void MarkAsNew();
	void MarkAsDelete();
	void MarkAsUpdate();

	unsigned long long GetId() const;
	int GetOffset() const;

private:
	bool is_new = false;
	bool is_updated = false;
	bool is_deleted = false;

	// pk
	unsigned long long id = 0;
	unsigned int user_data_size = 0;
	int offset = 0;

private:
	std::unique_ptr<T> user_data_;
};