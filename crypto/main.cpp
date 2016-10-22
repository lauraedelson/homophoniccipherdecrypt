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
    if (argc < 3) {
        cout << "usage: homophonicsolver englishDict.txt plaintexts.txt" << endl;
        exit(EXIT_FAILURE);
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
    else {
        cout << "Unable to open file" << argv[1] << endl;
        exit(EXIT_FAILURE);
    }

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
    else {
        cout << "Unable to open file" << argv[2] << endl;
        exit(EXIT_FAILURE);
    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Test // // Test // // Test // // Test // // Test // // Test // // Test // // Test // // Test // // Test //

//    int total = 566100620;
//
//    vector<vector<double>> digramMap{ {0} };
//    digramMap.resize(ALPHABET_SIZE, vector<double>(ALPHABET_SIZE, 0));
//    digramMap[0].resize(ALPHABET_SIZE, 0);
//
//
//
//    for(int x=0;x<ALPHABET_SIZE;x++)
//    {
//        for(int y=0;y<ALPHABET_SIZE;y++)
//        {
//            digramMap[x][y] = x+y;
//        }
//    }
//
//
//    for(int x=0;x<ALPHABET_SIZE;x++)
//    {
//        for(int y=0;y<ALPHABET_SIZE;y++)
//        {
//            digramMap[x][y] = (double)digramMap[x][y] / total;
//        }
//    }
//
//
//    for(int x=0;x<ALPHABET_SIZE;x++)
//    {
//        for(int y=0;y<ALPHABET_SIZE;y++)
//        {
//            cout.precision(17);
//            cout << "Pi: " << fixed << digramMap[x][y] << endl;
//        }
//    }


    /////////////////////////////////////////////////////////////////////////////////////////////////////////////




    HomophonicSolver solver = HomophonicSolver(englishDict, messageArray);
    cout << "Please enter ciphertext" << endl;
    string ciphertext;
    cin >> noskipws;
    getline(cin, ciphertext);
    cout << solver.analyse(ciphertext);
    exit(EXIT_SUCCESS);
}
