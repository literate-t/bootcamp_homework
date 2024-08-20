#pragma once
#include <memory>
#include <mutex>
#include <string>
#include <vector>
using namespace std;

#include "Node.h"
#include "QueryParser.h"
#include "UserData.h"
#include "UserDataManager.h"

class DefaultDataContainer
{
private:
	DefaultDataContainer() = default;
	DefaultDataContainer(const DefaultDataContainer&) = delete;
	DefaultDataContainer(DefaultDataContainer&&) = delete;
	DefaultDataContainer& operator=(const DefaultDataContainer&) = delete;
	DefaultDataContainer& operator=(DefaultDataContainer&&) = delete;

public:
	virtual ~DefaultDataContainer() = default;

	inline static DefaultDataContainer* GetInstance()
	{
		call_once(init_instance_flag_, []() {
			instance_ = new DefaultDataContainer();
			atexit([]() {
				delete instance_;
				});
			});

		return instance_;
	}

	vector<unique_ptr<UserDataManager>> Query(string& sql);
	void Commit();
	void Clear();

protected:
	virtual void Init() {};
	virtual void AddNewNode(bool is_new, int offset, unique_ptr<UserDataManager> new_data);
	virtual void RemoveNode(Node* remove_node);

private:
	vector<unique_ptr<UserDataManager>> CommandInsert(string& sql);
	vector<unique_ptr<UserDataManager>> CommandSelect(string& sql);
	vector<unique_ptr<UserDataManager>> CommandUpdate(string& sql);
	vector<unique_ptr<UserDataManager>> CommandDelete(string& sql);

	vector<unique_ptr<UserDataManager>> InsertByQuery(string& name, string& address, string& phone, int age);
	vector<unique_ptr<UserDataManager>> SelectByQuery(string& name, string& address, string& phone, int age);
	vector<unique_ptr<UserDataManager>> UpdateByQuery(string& set_name, string& set_address, string& set_phone, int set_age,
		string& where_name, string& where_address, string& where_phone, int where_age);
	vector<unique_ptr<UserDataManager>> DeleteByQuery(string& name, string& address, string& phone, int age);

	void UpdateNode(Node* tobe_updated, const UserData* updating);
	void DeleteNode(Node* tobe_deleted);
	void SetUserData(UserData* data, const char* name, int age, const char* address, const char* phone);
	bool Empty(const char* str);	

	bool IsValidData(const UserData* where, const UserData* find);
	bool IsDuplicatedUserFromList(UserData* user_data);

	vector<unique_ptr<UserDataManager>> FindUsersFromListBySearchData(const UserData* search);

protected:
	QueryParser parser_;

private:
	vector <unique_ptr<Node>> user_nodes_;
	static DefaultDataContainer* instance_;
	static once_flag init_instance_flag_;
};