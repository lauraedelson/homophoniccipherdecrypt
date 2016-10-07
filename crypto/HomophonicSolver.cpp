#include <fstream>

#include "HomophonicSolver.h"



HomophonicSolver::HomophonicSolver(vector<string> dictionaryFile, vector<string> messageArray)
{
	getDigrams(dictionaryFile, englishDigrams);
}


HomophonicSolver::~HomophonicSolver()
{
}

string HomophonicSolver::analyse(string cipherText)
{
	size_t cipherDigrams[103][103] = { 0 };

	//build the cipherDigrams matrix
	for (size_t i = 1; i < cipherText.length(); i++) {
		cipherDigrams[line[i - 1]][line[i]]++;	
	}

	//spacing analysis - might it one of our reference strings?


	return string();
}

vector<string> HomophonicSolver::outerHillClimb(vector<string> cipherText)
{
	return vector<string>();
}

string HomophonicSolver::randomInitialKey(size_t cipherDigrams[103][103])
{
	size_t bestScore = INT_MAX;
	char bestKey[103] = {' '};
	for (size_t i = 0; i < restarts; i++) {
		char key[103] = {' '};
		//make our random key
		for (size_t i = 0; i < 26; i++) {
			char letter = 'a' + i;
			for (size_t number = 0; i < frequencyDistribution[i]; i++) {
				bool success = false;
				while (success == false){
					int attempt = rand() % 103;
					if (key[attempt] == ' ') {
						key[attempt] = letter;
						success = true;
					}
				}
			}
		}
		//calculate a dict based on that key
		size_t putativeDict[26][26] = { 0 };
		for (int i = 0; i < 103; i++) {
			for (int j = 0; j < 103; j++) {
				char keyFirst = key[i];
				char keySecond = key[j];
				size_t new_i = keyFirst - 'a';
				size_t new_j = keySecond - 'a';
				putativeDict[new_i][new_j] += cipherDigrams[new_i][new_j];
			}
		}
		size_t initScore = innerHillClimb(putativeDict, key);
		if (initScore < bestScore) {
			bestScore = initScore;
			bestKey = key;
		}
	}
	return bestKey;
}

size_t HomophonicSolver::innerHillClimb(size_t putativeDict[26][26], char key[103])
{
	size_t score = 0;
	score = diffDictionaries(englishDigrams, putativeDict);
	for (int i = 0; i < 102; i++) { //note, the 102 here is intentional
		for (int j = 0; j < 103 - i; j++) {
			if (key[j] != key[j + i]) {//if the cipher chars map to the same letter, there's no point
				char testKey[103] = key;
				char keyFirst = testKey[i];
				char keySecond = testKey[j];
				size_t new_i = keyFirst - 'a';
				size_t new_j = keySecond - 'a';
				char temp = testKey[j];
				testKey[j] = testKey[j + i]; // test for char equality
				testKey[j + i] = temp;
				//get digrams for swaped columns
				testDict = putativeDict;
				char tempRow = testDict[new_j];
				testDict[j] = testDict[new_j + new_i];
				testDict[new_j + new_i] = tempRow;
				size_t testScore = diffDictionaries(englishDigrams, testDict);
			}
		}
	}
	return score;
}

void HomophonicSolver::getDigrams(vector<string>& dictionary, size_t digramMap[26][26])
{
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			dictionary[i][j] = 0;
		}
	}
	for (string line : dictionary) {
		for (size_t i = 1; i < line.length(); i++) {
			int first = line[i - 1] - 'a';
			int second = line[i] - 'a';
			digramMap[first][second]++;
		}
	}
}

size_t HomophonicSolver::diffDictionaries(size_t firstDictionary[26][26], size_t secondDictionary[26][26]) {
	size_t score = 0;
	for (int i = 0; i < 26; i++) {
		for (int j = 0; j < 26; j++) {
			score += abs((firstDictionary - secondDictionary));
		}
	}
	return score;
}

string & HomophonicSolver::decrypt(string & cipherText, char key[103])
{
	string message = "";
	while
	return message;
}
