#include <fstream>
#include <iostream>

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
	vector<string> tokens;
	string delim = ",";
	auto i = 0;
	auto pos = cipherText.find(delim);
	while (pos != string::npos) {
		tokens.push_back(cipherText.substr(i, pos - i));
		i = ++pos;
		pos = cipherText.find(delim, pos);

		if (pos == string::npos)
			tokens.push_back(cipherText.substr(i, cipherText.length()));
	}

	size_t cipherDigrams[KEY_SIZE][KEY_SIZE] = { 0 };

	//build the cipherDigrams matrix
	for (size_t i = 0; i < (tokens.size() - 1); i++) {
		if (tokens[i] != "," && tokens[i + 1] != "," && tokens[i] == " " && tokens[i + 1] == " ") {
			int first = atoi(tokens[i].c_str());
			int second = atoi(tokens[i + 1].c_str());
			cipherDigrams[first][second]++;
		}	
	}

	//spacing analysis - might it one of our reference strings?

	string key = getKey(cipherDigrams);

	return decrypt(tokens, key);
}

string HomophonicSolver::getKey(size_t cipherDigrams[KEY_SIZE][KEY_SIZE])
{
	size_t bestScore = INT_MAX;
	string bestKey;
	for (size_t i = 0; i < RESTARTS; i++) {
		char key[KEY_SIZE] = {' '};
		//make our random key
		for (size_t i = 0; i < ALPHABET_SIZE; i++) {
			char letter = 'a' + i;
			for (size_t number = 0; i < frequencyDistribution[i]; i++) {
				bool success = false;
				while (success == false){
					int attempt = rand() % KEY_SIZE;
					if (key[attempt] == ' ') {
						key[attempt] = letter;
						success = true;
					}
				}
			}
		}
		//calculate a dict based on that key
		vector<vector<size_t>> putativeDict;
		for (int i = 0; i < KEY_SIZE; i++) {
			for (int j = 0; j < KEY_SIZE; j++) {
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

size_t HomophonicSolver::innerHillClimb(vector<vector<size_t>> putativeDict, string key)
{
	size_t score = 0;
	score = diffDictionaries(englishDigrams, putativeDict);
	for (int i = 0; i < 102; i++) { //note, the 102 here is intentional
		for (int j = 0; j < KEY_SIZE - i; j++) {
			if (key[j] != key[j + i]) {//if the cipher chars map to the same letter, there's no point
				string testKey = key;
				char keyFirst = testKey[i];
				char keySecond = testKey[j];
				size_t new_i = keyFirst - 'a';
				size_t new_j = keySecond - 'a';
				char temp = testKey[j];
				testKey[j] = testKey[j + i]; // test for char equality
				testKey[j + i] = temp;
				//get digrams for swaped columns
				vector<vector<size_t>> testDict = putativeDict;
				vector<size_t> tempRow = testDict[new_j];
				testDict[j] = testDict[new_j + new_i];
				testDict[new_j + new_i] = tempRow;
				size_t testScore = diffDictionaries(englishDigrams, testDict);
			}
		}
	}
	return score;
}

void HomophonicSolver::getDigrams(vector<string>& dictionary, vector<vector<size_t>> digramMap)
{
	digramMap.resize(ALPHABET_SIZE, vector<size_t>(ALPHABET_SIZE, 0));
	digramMap[0].resize(ALPHABET_SIZE, 0);
	for (string line : dictionary) {
		for (size_t i = 1; i < line.length(); i++) {
			size_t first = line[i - 1] - 'a';
			size_t second = line[i] - 'a';
			//cout << "first:" << to_string(first) << " second:" << to_string(second) << endl;
			if (first < ALPHABET_SIZE && second < ALPHABET_SIZE) {
				digramMap[first][second]++;
			}
		}
	}
}

size_t HomophonicSolver::diffDictionaries(vector<vector<size_t>> firstDictionary, vector<vector<size_t>> secondDictionary) {
	size_t score = 0;
	for (int i = 0; i < ALPHABET_SIZE; i++) {
		for (int j = 0; j < ALPHABET_SIZE; j++) {
			int diff = firstDictionary[i][j] - secondDictionary[i][j];
			score += abs(diff);
		}
	}
	return score;
}

string HomophonicSolver::decrypt(vector<string>& cipherText, string key)
{
	string message = "";
	for (string token : cipherText) {
		if (token == " ") {
			message.append(" ");
		}
		else {
			char val = key[atoi(token.c_str())];
			message.push_back(val);
		}
	}
	return message;
}
