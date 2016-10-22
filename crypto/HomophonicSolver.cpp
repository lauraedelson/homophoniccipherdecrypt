#include <fstream>
#include <iostream>
#include <map>
#include "HomophonicSolver.h"



HomophonicSolver::HomophonicSolver(const vector<string>& dictionaryFile, const vector<string>& messageArray)
{
	getDigrams(dictionaryFile, englishDigrams);
	for (string message : messageArray) {
		if (!message.empty())
		decryptedSentences.push_back(Sentence(message));
	}

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

	//spacing analysis - might it one of our reference strings?
	vector<Sentence> candidates;
	for (Sentence sentence : decryptedSentences) {
		if (sentence.isCandidate(tokens)) {
			candidates.push_back(sentence);
			string result = deriveKey(tokens, sentence);
			if (!result.empty()) {
				return result;
			}
		}
	}
	
    double cipherDigrams[KEY_SIZE][KEY_SIZE] = { 0 };
    //build the cipherDigrams matrix
    for (size_t i = 0; i < (tokens.size() - 1); i++) {
        if (tokens[i] != "," && tokens[i + 1] != "," && tokens[i] != " " && tokens[i + 1] != " ") {
            int first = atoi(tokens[i].c_str());
            int second = atoi(tokens[i + 1].c_str());
            cipherDigrams[first][second]++;
        }
    }

    string key = getKey(cipherDigrams);

    return decrypt(tokens, key);
}

string HomophonicSolver::getKey(double cipherDigrams[KEY_SIZE][KEY_SIZE])
{
    double bestScore = INT_MAX;
    string bestKey;
    for (size_t i = 0; i < RESTARTS; i++) {
        char key[KEY_SIZE] = {' '};
        key[0] = '\0';
        //make our random key
        for (size_t i = 0; i < ALPHABET_SIZE; i++) {
            char letter = 'a' + i;
            for (size_t number = 0; number < frequencyDistribution[i]; number++) {
                bool success = false;
                int attempt = rand() % KEY_SIZE;
                while (success == false){
                    size_t place = attempt % KEY_SIZE;
                    if (key[place] == '\0') {
                        key[place] = letter;
                        success = true;
                    }
                    else {
                        attempt++;
                    }
                }
            }
        }		//calculate a dict based on that key
        vector<vector<double>> putativeDict;
        putativeDict.resize(ALPHABET_SIZE, vector<double>(ALPHABET_SIZE, 0));
        for (int i = 0; i < KEY_SIZE; i++) {
            for (int j = 0; j < KEY_SIZE; j++) {
                char keyFirst = key[i];
                char keySecond = key[j];
                size_t new_i = keyFirst - 'a';
                size_t new_j = keySecond - 'a';
                putativeDict[new_i][new_j] += cipherDigrams[new_i][new_j];
            }
        }
        string strkey = key;
        double initScore = innerHillClimb(putativeDict, strkey);
        if (initScore < bestScore) {
            bestScore = initScore;
            bestKey = key;
        }
    }
    return bestKey;
}

double HomophonicSolver::innerHillClimb(vector<vector<double>>& putativeDict, string& key)
{
    double score = 0;
    score = diffDictionaries(englishDigrams, putativeDict);
    for (int i = 1; i < (KEY_SIZE-1); i++) { //note, the 102 here is intentional
        for (int j = 0; j < KEY_SIZE - i; j++) {
            if (key[j] != key[j + i]) {//if the cipher chars map to the same letter, there's no point
                string testKey = key;
                char keyFirst = testKey[j];
                char keySecond = testKey[j + i];
                size_t new_i = keyFirst - 'a';
                size_t new_j = keySecond - 'a';
                char temp = testKey[j];
                testKey[j] = testKey[j + i]; // test for char equality
                testKey[j + i] = temp;
                //get digrams for swaped columns
                vector<vector<double>> testDict = putativeDict;
                vector<double> tempRow = testDict[new_j];
                testDict[new_j] = testDict[new_i];
                testDict[new_i] = tempRow;
                double testScore = diffDictionaries(englishDigrams, testDict);
                if (testScore < score) {
                    putativeDict = testDict;
                    key = testKey;
                    return innerHillClimb(putativeDict, testKey);
                }
            }
        }
    }
    cout << "score:" << endl;
    cout << score << endl;
    return score;
}




void HomophonicSolver::getDigrams(const vector<string>& dictionary, vector<vector<double>>& digramMap)
{
    double total = 0;
    digramMap.resize(ALPHABET_SIZE, vector<double>(ALPHABET_SIZE, 0));
    digramMap[0].resize(ALPHABET_SIZE, 0);

    for (string line : dictionary) {
        for (size_t i = 1; i < line.length(); i++) {
            size_t first = line[i - 1] - 'a';
            size_t second = line[i] - 'a';

            if (first < ALPHABET_SIZE && second < ALPHABET_SIZE) {
                digramMap[first][second]++;
            }

        }
    }
    // Finding the total frequencies for the whole matrix
    for(int x=0;x<ALPHABET_SIZE;x++)
    {
        for(int y=0;y<ALPHABET_SIZE;y++){
            total = total + digramMap[x][y];
        }
    }

    cout<<"sum of frequencies : ";
    cout<<total<<endl;


    for(int x=0;x<ALPHABET_SIZE;x++)
    {
        for(int y=0;y<ALPHABET_SIZE;y++)
        {
            // Dividing the individual frequencies to the sum_of_frequencies to find the relative value
            digramMap[x][y] = (double)digramMap[x][y] / total;
        }
    }

    //Printing out for test purposes
    for(int x=0;x<ALPHABET_SIZE;x++)
    {
        for(int y=0;y<ALPHABET_SIZE;y++)
        {
            cout.precision(17);
            cout << " x: " << x <<" y: " <<y << " map value: "<< fixed << digramMap[x][y] << endl;
        }
    }

}


double HomophonicSolver::diffDictionaries(const vector<vector<double>>& firstDictionary, const vector<vector<double>>& secondDictionary) {
    double score = 0;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        for (int j = 0; j < ALPHABET_SIZE; j++) {
            double diff = firstDictionary[i][j] - secondDictionary[i][j];
            score += abs(diff);
        }
    }
    return score;
}

string HomophonicSolver::deriveKey(const vector<string>& tokens, Sentence& message)
{
	map<string, char> translation;
	translation[" "] = ' ';
	if (tokens.size() == message.getText().size()) {
		string text = message.getText();

		for (size_t i = 0; i < tokens.size(); i++) {
			if (translation.find(tokens[i]) == translation.end()) {
				translation[tokens[i]] = text[i];
			}
			else if (translation[tokens[i]] == text[i]) {
				continue;
			}
			else {
				return string();
			}
		}
	}
	string decryption;
	for (auto token : tokens) {
		decryption += translation[token];
	}
	return decryption;
}



string HomophonicSolver::decrypt(const vector<string>& cipherText, const string& key)
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
