#pragma once

#include <string>
#include <vector>

using namespace std;

class Sentence
{
public:
	Sentence(string text);
	~Sentence();
	void setDigrams(vector<vector<size_t>>& inDigrams);
	bool isCandidate(vector<string>& tokens);
	vector<string>& getWords() { return words; };
	const string& getText() { return text; };

private:
	const string text;
	vector<string> words;
	vector<vector<size_t>> digrams{ { 0 } };
	string lengths;
};

