#pragma once
#include <memory>

// T: data type U: node type
template<typename T, typename U>
class AbstractDataContainer abstract
{
protected:
	AbstractDataContainer() = default;
	virtual ~AbstractDataContainer() = default;

protected:
	virtual void Init() = 0;
	virtual void Push(T*) = 0;
	virtual void AddNewNode(std::unique_ptr<T> new_data) = 0;
	virtual void RemoveNode(U* remove_node) = 0;
};
