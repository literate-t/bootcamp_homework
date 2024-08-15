#pragma once
#include <memory>

class IDataStructure;
class TestMenu {
public:
	explicit TestMenu(std::shared_ptr<IDataStructure> data) : data_(data) {

	}

private:
	std::weak_ptr<IDataStructure> data_;
};
