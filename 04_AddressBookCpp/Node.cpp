#include "Node.h"

template<typename T>
unsigned int Node<T>::user_data_size_ = 0;

template<typename T>
Node<T>::Node(std::unique_ptr<T>&& user_data)
{
}

template<typename T>
const T* Node<T>::GetUserData() {
	return user_data_.get();
}

template<typename T>
void Node<T>::MarkAsNew() {
	is_new = true;
}

template<typename T>
void Node<T>::MarkAsDelete() {
	is_deleted = true;
}

template<typename T>
void Node<T>::MarkAsUpdate() {
	is_updated = true;
}

template<typename T>
unsigned long long Node<T>::GetId() const {
	return id;
}

template<typename T>
int Node<T>::GetOffset() const {
	return offset;
}
