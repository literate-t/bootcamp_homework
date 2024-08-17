#pragma once
#include <vector>

#include "AbstractDataContainer.h"
#include "UserData.h"
#include "Node.h"

class DataContainer : public AbstractDataContainer<UserData, Node<UserData>> {
public:
	DataContainer() = default;
	virtual ~DataContainer() = default;

	void Init() override;
	void AddNewNode(std::unique_ptr<UserData> new_data) override;
	void RemoveNode(Node<UserData>* remove_node) override;

private:
	std::vector <std::unique_ptr<Node<UserData>>> user_nodes_;
};

