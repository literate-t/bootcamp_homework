#include "DataContainer.h"

void DataContainer::Init()
{
}

void DataContainer::AddNewNode(std::unique_ptr<UserData> new_data)
{
	std::unique_ptr<Node<UserData>> new_node = std::make_unique<Node<UserData>>(std::move(new_data));
	user_nodes_.push_back(new_node);
}

void DataContainer::RemoveNode(Node<UserData>* remove_node)
{

}
