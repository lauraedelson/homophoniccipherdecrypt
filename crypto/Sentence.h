#pragma once

#include <string>
#include <vector>

using namespace std;

class Sentence
{
public:
	Sentence(const string& text);
	~Sentence();

	vector<string>& getWords() { return words; };
	const string& getText() { return text; };

	//evaluate whether the ciphertext specified by the tokens could map to the sentence
	bool isCandidate(const vector<string>& tokens);

private:
	const string text;
	vector<string> words;
	string lengths;
};

