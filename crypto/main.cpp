#include <fstream>
#include <string>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>

#include "HomophonicSolver.h"
#include "main.h"

using namespace std;

int main(int argc, char* argv[])
{
	if (sizeof(argv) < 2) {
		cout << "usage: homophonicsolver englishDict.txt plaintexts.txt" << endl;
	}

	ifstream dictFile(argv[1]);
	vector<string> englishDict = vector<string>();
	if (dictFile.is_open())
	{
		string line;
		while (getline(dictFile, line))
		{
			englishDict.push_back(line);
		}
		dictFile.close();
	}
	else cout << "Unable to open file" << endl;

	vector<string> messageArray = vector<string>();
	ifstream readFile(argv[2]);
	if (readFile.is_open())
	{
		string line;
		while (getline(readFile, line))
		{
			messageArray.push_back(line);
		}
		readFile.close();
	}
	else cout << "Unable to open file" << endl;

	HomophonicSolver solver = HomophonicSolver(englishDict, messageArray);
	cout << "Please enter ciphertext" << endl;
	string ciphertext;
	cin >> ciphertext;
	cout << solver.analyse(ciphertext);
} 