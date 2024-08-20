#include "DefaultDataContainer.h"
#include "FileManager.h"

constexpr int AGE_INIT_VALUE = -1;

DefaultDataContainer* DefaultDataContainer::instance_;
once_flag DefaultDataContainer::init_instance_flag_;

void DefaultDataContainer::AddNewNode(bool is_new, int offset, unique_ptr<UserDataManager> new_data) {	
	unique_ptr<Node> new_node = make_unique<Node>(move(new_data));
	new_node->MarkNewAs(is_new);
	user_nodes_.push_back(std::move(new_node));
}

void DefaultDataContainer::RemoveNode(Node* remove_node) {
	auto it = std::remove_if(user_nodes_.begin(), user_nodes_.end(),
		[&remove_node](const std::unique_ptr<Node>& node) {
			return node.get() == remove_node;
		});

	user_nodes_.erase(it, user_nodes_.end());
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::Query(string& sql) {
	parser_.ToLowerCase(sql);
	bool is_end = false;
	while (!is_end) {
		is_end = parser_.TrimAroundEqual(sql);
	}

	if (parser_.Includes(sql, "insert")) {
		return CommandInsert(sql);
	}
	else if (parser_.Includes(sql, "select")) {
		return CommandSelect(sql);
	}
	else if (parser_.Includes(sql, "update set")) {
		return CommandUpdate(sql);
	}
	else if (parser_.Includes(sql, "delete")) {
		return CommandDelete(sql);
	}

	return vector<unique_ptr<UserDataManager>>();
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::CommandInsert(string& sql) {

	string name;
	string address;
	string phone;
	int age = -1;

	vector<string> split_result = parser_.SplitString(sql, ' ');
	for (int i = 0; i < split_result.size(); ++i) {
		string& token = split_result[i];

		if (parser_.Includes(token, "name")) {
			name = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "address")) {
			address = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "phone")) {
			phone = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "age")) {
			age = parser_.ParseIntValue(token);
		}
	}

	if (name.empty() && address.empty() && phone.empty() && -1 == age) {
		return vector<unique_ptr<UserDataManager>>();
	}

	return InsertByQuery(name, address, phone, age);
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::CommandSelect(string& sql) {
	string name;
	string address;
	string phone;
	int age = -1;

	vector<string> split_result = parser_.SplitString(sql, ' ');
	for (int i = 0; i < split_result.size(); ++i) {
		string& token = split_result[i];

		if (parser_.Includes(token, "name")) {
			name = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "address")) {
			address = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "phone")) {
			phone = parser_.ParseStringValue(token);
		}
		else if (parser_.Includes(token, "age")) {
			age = parser_.ParseIntValue(token);
		}
	}

	if (name.empty() && address.empty() && phone.empty() && -1 == age) {
		return vector<unique_ptr<UserDataManager>>();
	}

	return SelectByQuery(name, address, phone, age);
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::CommandUpdate(string& sql)
{
	vector<string> split_strings	= parser_.SplitString(sql, ' ');
	vector<string> set_strings		= parser_.GetSetString(split_strings);
	vector<string> where_strings	= parser_.GetWhereString(split_strings);

	string set_name;
	string set_address;
	string set_phone;
	int set_age = AGE_INIT_VALUE;
	for (auto& token : set_strings)
	{
		if (parser_.Includes(token, "name"))
			set_name = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "address"))
			set_address = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "phone"))
			set_phone = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "age"))
			set_age = parser_.ParseIntValue(token);
	}

	string where_name;
	string where_address;
	string where_phone;
	int where_age = AGE_INIT_VALUE;
	for (auto& token : where_strings)
	{
		if (parser_.Includes(token, "name"))
			where_name = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "address"))
			where_address = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "phone"))
			where_phone = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "age"))
			where_age = parser_.ParseIntValue(token);
	}

	if (where_name.empty() && where_address.empty() && where_phone.empty() && AGE_INIT_VALUE == where_age &&
		set_name.empty() && set_address.empty() && set_phone.empty() && AGE_INIT_VALUE == set_age)
	{
		return vector<unique_ptr<UserDataManager>>();
	}

	vector<unique_ptr<UserDataManager>> search_strings = SelectByQuery(where_name, where_address, where_phone, where_age);
	if (search_strings.empty())
	{
		return vector<unique_ptr<UserDataManager>>();
	}

	return UpdateByQuery(set_name, set_address, set_phone, set_age,
			where_name, where_address, where_phone, where_age);
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::CommandDelete(string& sql)
{
	string name;
	string address;
	string phone;
	int age = AGE_INIT_VALUE;

	vector<string> split_strings = parser_.SplitString(sql, ' ');
	for (auto& token : split_strings)
	{
		if (parser_.Includes(token, "name"))
			name = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "address"))
			address = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "phone"))
			phone = parser_.ParseStringValue(token);
		else if (parser_.Includes(token, "age"))
			age = parser_.ParseIntValue(token);
	}

	if (name.empty() && address.empty() && phone.empty() && AGE_INIT_VALUE == age)
	{
		return vector<unique_ptr<UserDataManager>>();
	}

	auto search_result = SelectByQuery(name, address, phone, age);
	if (search_result.empty())
	{
		return vector<unique_ptr<UserDataManager>>();
	}

	return DeleteByQuery(name, address, phone, age);
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::InsertByQuery(string& name, string& address, string& phone, int age) {
	unique_ptr<UserDataManager> new_data = 
		make_unique<UserDataManager>(name.c_str(), age, address.c_str(), phone.c_str());

	AddNewNode(true, 0, move(new_data));

	vector<unique_ptr<UserDataManager>> user_data_vector;
	new_data = make_unique<UserDataManager>(name.c_str(), age, address.c_str(), phone.c_str());

	user_data_vector.push_back(move(new_data));

	return user_data_vector;
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::SelectByQuery(string& name, string& address, string& phone, int age)
{
	int offset = 0;

	FileManager file_manager("user_address.dat", "rb");
	UserData user_data;
	UserDataManager search_data_manager(name.c_str(), age, address.c_str(), phone.c_str());

	vector<unique_ptr<UserDataManager>> result_vector;

	while (0 < file_manager.read(&user_data, sizeof UserData, 1)) {
		if (IsValidData(search_data_manager.GetUserData(), reinterpret_cast<UserData*>(&user_data))) {
			if (!IsDuplicatedUserFromList(&user_data)) {
				unique_ptr<UserDataManager> new_user = 
					make_unique<UserDataManager>(user_data.name_, user_data.age_, user_data.address_, user_data.phone_);
				AddNewNode(false, offset, move(new_user));

				new_user =
					make_unique<UserDataManager>(user_data.name_, user_data.age_, user_data.address_, user_data.phone_);
				result_vector.push_back(move(new_user));				
			}
		}
		++offset;
	}	

	return result_vector;
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::UpdateByQuery(string& set_name, string& set_address, string& set_phone, int set_age, string& where_name, string& where_address, string& where_phone, int where_age)
{
	UserDataManager set_data_manger(set_name, set_age, set_address, set_phone);
	UserDataManager where_data_manger(where_name, where_age, where_address, where_phone);
	vector<unique_ptr<UserDataManager>> results;

	for (auto& find_node : user_nodes_)
	{
		if (IsValidData(where_data_manger.GetUserData(), find_node->GetUserData()))
		{
			UpdateNode(find_node.get(), set_data_manger.GetUserData());
			results.push_back(make_unique<UserDataManager>(find_node->GetUserData()));
		}
	}

	return results;
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::DeleteByQuery(string& name, string& address, string& phone, int age)
{
	UserDataManager delete_data_manager(name, age, address, phone);
	vector<unique_ptr<UserDataManager>> results;

	for (auto& find_node : user_nodes_)
	{
		if (IsValidData(delete_data_manager.GetUserData(), find_node->GetUserData()))
		{
			DeleteNode(find_node.get());
			results.push_back(make_unique<UserDataManager>(find_node->GetUserData()));
		}
	}

	return results;
}

void DefaultDataContainer::UpdateNode(Node* tobe_updated, const UserData* updating)
{
	tobe_updated->MarkUpdateAs(true);
	UserData* updating_user = tobe_updated->GetUserData();
	SetUserData(updating_user, updating->name_, updating->age_, 
		updating->address_, updating->phone_);
}

void DefaultDataContainer::DeleteNode(Node* tobe_deleted)
{
	tobe_deleted->MarkDeleteAs(true);
}

void DefaultDataContainer::SetUserData(UserData* data, const char* name, int age, const char* address, const char* phone)
{
	if (!Empty(name))
	{
		int name_size = (int)strlen(name) + 1;
		memcpy_s(data->name_, sizeof data->name_, name, name_size);
	}
	if (!Empty(address))
	{
		int address_size = (int)strlen(address) + 1;
		memcpy_s(data->address_, sizeof data->address_, address, address_size);
	}
	if (!Empty(phone))
	{
		int phone_size = (int)strlen(phone) + 1;
		memcpy_s(data->phone_, sizeof data->phone_, phone, phone_size);
	}
	if (AGE_INIT_VALUE != age)
	{
		data->age_ = age;
	}
}

//void DefaultDataContainer::SetUserData(UserData* data, string&& name, int age, string&& address, string&& phone)
//{
//	if (!name.empty())
//	{
//		size_t name_size = name.size() + 1;
//		memcpy_s(data->name_, sizeof data->name_, name.c_str(), name_size);
//	}
//	if (!address.empty())
//	{
//		size_t address_size = address.size() + 1;
//		memcpy_s(data->address_, sizeof data->address_, address.c_str(), address_size);
//	}
//	if (!phone.empty())
//	{
//		size_t phone_size = phone.size() + 1;
//		memcpy_s(data->phone_, sizeof data->phone_, phone.c_str(), phone_size);
//	}
//	if (AGE_INIT_VALUE != age)
//	{
//		data->age_ = age;
//	}
//}

bool DefaultDataContainer::Empty(const char* str)
{
	return !strlen(str);
}

bool DefaultDataContainer::IsValidData(const UserData* search, const UserData* find) {

	if (0 == (Empty(search->name_) ? 0 : strcmp(find->name_, search->name_))
		&& 0 == (Empty(search->phone_) ? 0 : strcmp(find->phone_, search->phone_))
		&& find->age_ == (AGE_INIT_VALUE < search->age_ ? search->age_ : find->age_)
		&& parser_.Includes(find->address_, search->address_))
		return true;

	return false;
}

bool DefaultDataContainer::IsDuplicatedUserFromList(UserData* user_data)
{
	auto it = find_if(user_nodes_.begin(), user_nodes_.end(), [&user_data](const std::unique_ptr<Node>& node) {
		return 0 == strcmp(node->GetUserData()->name_, user_data->name_) &&
			0 == strcmp(node->GetUserData()->address_, user_data->address_) &&
			0 == strcmp(node->GetUserData()->phone_, user_data->phone_) &&
			0 == node->GetUserData()->age_ == user_data->age_;
		});

	if (user_nodes_.end() != it) {
		return true;
	}

	return false;
}

vector<unique_ptr<UserDataManager>> DefaultDataContainer::FindUsersFromListBySearchData(const UserData* search)
{
	vector<unique_ptr<UserDataManager>> result_list;
	for (const auto& node : user_nodes_) {
		if (IsValidData(search, node->GetUserData())) {
			unique_ptr<UserDataManager> data = make_unique<UserDataManager>(*(node->GetUserData()));
			result_list.push_back(move(data));
		}
	}
	return result_list;
}

void DefaultDataContainer::Commit()
{
	for (const auto& node : user_nodes_)
	{
		if (node->GetIsDeleted())
		{
			FileManager file_manager("user_address.dat", "rb+");
			file_manager.ProcessDeleteNode("user_address_deleted.dat", "ab+", node.get());
		}
		else if (node->GetIsNew())
		{
			FileManager file_manager("user_address.dat", "ab+");
			file_manager.ProcessNewNode("user_address_deleted.dat", "rb+", node.get());
		}
		else if (node->GetIsUpdated())
		{
			FileManager file_manager("user_address.dat", "rb+");
		}
	}
}

void DefaultDataContainer::Clear()
{
	user_nodes_.clear();
}
