#pragma once
#include <string>
#include <vector>
#include <memory>
using namespace std;

class QueryParser
{
public:
	vector<string> SplitString(const string& input, char delim);

	void ToLowerCase(string& input);
	bool TrimAroundEqual(string& input);
	void TrimSingleQuote(string& input);
	void TrimLineFeed(string& input);
	bool Includes(string str, string sub);
	bool Includes(const char* str, const char* sub);

	string ParseStringValue(string& token);
	int ParseIntValue(string& token);
	vector<string> GetSetString(vector<string>& strings);
	vector<string> GetWhereString(vector<string>& strings);
};