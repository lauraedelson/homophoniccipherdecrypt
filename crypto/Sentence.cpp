#include "Sentence.h"
#include <sstream>
#include <fstream>
#include <string>
#include <iostream>
#include <iterator>

using namespace std;

//given a space delimited string, return a vector of the parts
vector<string> tokenize(string input) {
	stringstream ss(input);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> vstrings(begin, end);
	return vstrings;
}

Sentence::Sentence(string inText): text(inText)
{
	words = tokenize(inText);
	for (auto word : words) {
		lengths.append(to_string(word.size()));
	}


}


Sentence::~Sentence()
{
}

void Sentence::setDigrams(vector<vector<size_t>>& inDigrams)
{
	digrams = inDigrams;
}

bool Sentence::isCandidate(vector<string>& tokens)
{
	string candidateLengths;
	size_t wordCount = 0;
	size_t i = 1;
	for (auto token : tokens) {
		if (token == " ") {
			wordCount++;
			candidateLengths.append(to_string(i-1));
			i = 0;
		}
		i++;
	}
	wordCount++;
	candidateLengths.append(to_string(i - 1));

	if (candidateLengths == lengths && wordCount == words.size()) {
		return true;
	}
	return false;
}
