#include <string>
#include <vector>
#include "Sentence.h"
using namespace std;

#define ALPHABET_SIZE 26
#define KEY_SIZE 103
#define RESTARTS 20

#pragma once
class HomophonicSolver
{
public:
    HomophonicSolver(const vector<string>& dictionaryFile, const vector<string>& messageArray);
    ~HomophonicSolver();

	//outputs a best guess decryption
    string analyse(string cipherText);

private:
	vector<vector<double>> englishDigrams{ {0} };
	vector<Sentence> decryptedSentences;
	vector<string> dictionaryFile;
	size_t frequencyDistribution[ALPHABET_SIZE] = { 8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2,1,6,6,9,3,1,2,1,2,1 };

	string getKey(double cipherDigrams[KEY_SIZE][KEY_SIZE]);
	double innerHillClimb(vector<vector<double>>& putativeDict, string& key);
	void getDigrams(const vector<string>& dictionary, vector<vector<double>>& digramMap);
	double diffDictionaries(const vector<vector<double>>& firstMap, const vector<vector<double>>& secondMap);
	string deriveKey(const vector<string>& tokens, Sentence& message);
	string decrypt(const vector<string>& cipherText, const string& key);
};

