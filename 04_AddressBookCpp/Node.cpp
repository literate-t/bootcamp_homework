#include "Node.h"

Node::Node(std::unique_ptr<UserDataManager>&& user_data)
{
	user_manager_ = make_unique<UserDataManager>(user_data->GetUserData());
}

UserData* Node::GetUserData() const
{
	return user_manager_->GetUserData();
}

void Node::MarkNewAs(bool is_new)
{
	is_new_ = is_new;
}

void Node::MarkDeleteAs(bool is_deleted)
{
	is_deleted_ = is_deleted;
}

void Node::MarkUpdateAs(bool is_updated)
{
	is_updated_ = is_updated;
}

unsigned long long Node::GetId() const
{
	return id_;
}

int Node::GetOffset() const
{
	return offset_;
}

void Node::SetOffset(int offset)
{
	offset_ = offset;
}

string Node::GetName() const
{
	return user_manager_->GetUserName();
}

string Node::GetAddress() const
{
	return user_manager_->GetUserAddress();
}

string Node::GetPhone() const
{
	return user_manager_->GetUserPhone();
}

int Node::GetAge() const
{
	return user_manager_->GetUserAge();
}

bool Node::GetIsNew() const
{
	return is_new_;
}

bool Node::GetIsUpdated() const
{
	return is_updated_;
}

bool Node::GetIsDeleted() const
{
	return is_deleted_;
}
