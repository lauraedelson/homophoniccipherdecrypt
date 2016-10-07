#include <string>
#include <vector>

using namespace std;

#pragma once
class HomophonicSolver
{
public:
	HomophonicSolver(vector<string> dictionaryFile, vector<string> messageArray);
	~HomophonicSolver();

	string analyse(string cipherText);

	vector<string> outerHillClimb(vector<string> cipherText);

	string randomInitialKey(size_t cipherDigrams[103][103]);

	size_t innerHillClimb(size_t putativeDict[26][26], char key[103]);

	

private:
	size_t englishDigrams[26][26];
	size_t frequencyDistribution[26] = { 8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2,1,6,6,9,3,1,2,1,2,1 };
	size_t restarts = 40;
	void getDigrams(vector<string>& dictionary, size_t digramMap[26][26]); 
	size_t diffDictionaries(size_t firstMap[26][26], size_t secondMap[26][26]);
	string& decrypt(string& cipherText, char key[103]);
};

