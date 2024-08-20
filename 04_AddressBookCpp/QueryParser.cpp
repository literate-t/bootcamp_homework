#include "QueryParser.h"

vector<string> QueryParser::SplitString(const string& input, char delim) {
	vector<string> result;
	string current;
	bool is_single_quote = false;

	size_t input_size = input.size();
	for (int i = 0; i < input_size; ++i) {
		char current_char = input[i];
		if ('\'' == current_char) {
			is_single_quote = !is_single_quote;
		}
		else if (!is_single_quote && delim == current_char && delim != input[i + 1]) {
			result.push_back(current);
			current.clear();			
		}
		else {
			current += current_char;
		}
	}

	result.push_back(current);

	return result;
}

string QueryParser::ParseStringValue(string& token) {
	vector<string> result = SplitString(token, '=');
	if (result.empty()) {
		return "";
	}

	string last = result.back();
	TrimSingleQuote(last);
	TrimLineFeed(last);

	return last;
}

int QueryParser::ParseIntValue(string& token) {
	vector<string> result = SplitString(token, '=');
	if (result.empty()) {
		return -1;
	}

	string last = result.back();
	TrimSingleQuote(last);
	TrimLineFeed(last);

	return stoi(last);
}

void QueryParser::ToLowerCase(string& input) {
	for (char& ch : input) {
		if ('A' <= ch && ch <= 'Z')
			ch += 32;
	}
}

bool QueryParser::TrimAroundEqual(string& input) {
	string result;
	result.reserve(input.size());

	for (int i = 0; i < input.length(); ++i) {
		if (!(' ' == input[i] && '=' == input[i + 1] || ' ' == input[i] && '=' == input[i - 1])) {
			result += input[i];
		}
	}

	if (input == result) {
		return true;
	}

	input = move(result);

	return false;
}

void QueryParser::TrimSingleQuote(string& input) {
	string result;
	for (int i = 0; i < input.size(); ++i) {
		if ('\'' != input[i]) {
			result += input[i];
		}
	}

	input = move(result);
}

void QueryParser::TrimLineFeed(string& input) {
	string result;
	for (int i = 0; i < input.size(); ++i) {
		if ('\n' != input[i]) {
			result += input[i];
		}
	}

	input = move(result);
}

bool QueryParser::Includes(string str, string sub) {
	return string::npos != str.find(sub);
}

bool QueryParser::Includes(const char* str, const char* sub)
{
	return strstr(str, sub);
}

vector<string> QueryParser::GetSetString(vector<string>& strings) {
	vector<string> result;
	result.reserve(strings.size());

	// update, set 문자열을 건너뛴다
	const int start = 2;
	result.reserve(strings.size() - start);

	for (int i = start; i < strings.size(); ++i) {
		string& token = strings[i];
		if ("where" == token) break;
		else if ("and" == token) continue;

		result.push_back(token);
	}

	return result;
}

vector<string> QueryParser::GetWhereString(vector<string>& strings) {
	vector<string> result;
	// update, set 문자열을 건너뛴다
	const int start = 2;
	result.reserve(strings.size() - start);

	for (int i = start; i < strings.size(); ++i) {
		string& token = strings[i];
		if ("and" == token) continue;

		TrimLineFeed(token);
		result.push_back(token);
	}

	return result;
}