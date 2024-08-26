#pragma once
#include <cstdio>
#include <string>
using namespace std;

class Node;
class FileManager {
public:
	FileManager(string file_name, string mode);
	~FileManager();

	size_t read(void* data, size_t data_size, size_t element_count);
	void ProcessDeleteNode(string file_name, Node* node);
	void ProcessNewNode(string file_name, Node* node);
	void ProcessUpdateNode(string file_name, Node* node);

private:
	FILE* fp_;
};

