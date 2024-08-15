#pragma once
#include <string>

class IDataStructure abstract {
protected:
	virtual IDataStructure* Next() = 0;
	virtual IDataStructure* Prev() = 0;

	virtual void Init(bool is_new, bool is_updated_, bool is_deleted,
		std::string key, void* data, int data_size, int offset_file) = 0;
	virtual char* GetKey() = 0;

	bool is_new_;
	bool is_updated_;
	bool is_deleted_;

	std::string key_;
	void* data_;
	unsigned int size_data_;
	int offset_file_;
};
