#include <string>
#include <vector>

using namespace std;

#define ALPHABET_SIZE 26
#define KEY_SIZE 103
#define RESTARTS 40

#pragma once
class HomophonicSolver
{
public:
    HomophonicSolver(vector<string> dictionaryFile, vector<string> messageArray);
    ~HomophonicSolver();

    string analyse(string cipherText);

private:
    vector<vector<double>> englishDigrams{ {0} };
    vector<string> messageArray;
    vector<string> dictionaryFile;
    size_t frequencyDistribution[ALPHABET_SIZE] = { 8, 1, 3, 4, 13, 2, 2, 6, 7, 1, 1, 4, 2, 7, 8, 2,1,6,6,9,3,1,2,1,2,1 };

    string getKey(size_t cipherDigrams[KEY_SIZE][KEY_SIZE]);
    double innerHillClimb(vector<vector<double>>& putativeDict, string& key);
    void getDigrams(vector<string>& dictionary, vector<vector<double>>& digramMap);
    size_t diffDictionaries(vector<vector<double>> firstMap, vector<vector<double>> secondMap);
    string decrypt(vector<string>& cipherText, string key);
};
