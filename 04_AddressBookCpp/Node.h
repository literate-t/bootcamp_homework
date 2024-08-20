#pragma once
#include <memory>
#include <string>
using namespace std;
#include "UserDataManager.h"

class Node {
public:
	Node(unique_ptr<UserDataManager>&& user_data);

	UserData* GetUserData() const;
	void MarkNewAs(bool is_new);
	void MarkDeleteAs(bool is_deleted);
	void MarkUpdateAs(bool is_updated);

	unsigned long long GetId() const;
	int GetOffset() const;
	void SetOffset(int offset);

	string GetName() const;
	string GetAddress() const;
	string GetPhone() const;
	int GetAge() const;

	bool GetIsNew() const;
	bool GetIsUpdated() const;
	bool GetIsDeleted() const;

private:
	bool is_new_ = false;
	bool is_updated_ = false;
	bool is_deleted_ = false;
	int offset_ = 0;

	// pk
	unsigned long long id_ = 0;

private:
	std::unique_ptr<UserDataManager> user_manager_;
};
