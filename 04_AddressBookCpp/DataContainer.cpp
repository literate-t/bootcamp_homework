#include "DataContainer.h"

void DataContainer::Init()
{
}

void DataContainer::AddNewNode(std::unique_ptr<UserData> new_data)
{
	std::unique_ptr<Node<UserData>> new_node = std::make_unique<Node<UserData>>(std::move(new_data));
	//user_nodes_.push_back(std::move(new_node));
}

void DataContainer::RemoveNode(Node<UserData>* remove_node)
{
	auto it = std::remove_if(user_nodes_.begin(), user_nodes_.end(), 
		[&remove_node](const std::unique_ptr<Node<UserData>>& node) {
			return node.get() == remove_node;
		});
}
