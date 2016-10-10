#include <fstream>
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

/**
 Takes a string and ouputs a vector of all the words in the string
*/
vector<string> Parser::parseLine(const string &line) {
    vector<string> line_words;
    int i = 0;
    string word;
    while (i < line.length())
    {
        while (line[i] == ' ') i++;
        int next_space = line.find(' ',i);
        if (next_space == -1) next_space = line.length();
        if (next_space == i) break;
        word = line.substr(i, next_space-i);
        i = next_space + 1;
        line_words.push_back(word);
    }
    return line_words;
}

bool Parser::isDouble(const string &word) {
    bool is_num = true;
    string digits = "1234567890.";
    for (int j = 0; j < word.length(); j++)
    {
        if (digits.find(word[j]) == -1)
        {
            is_num = false;
            break;
        }
    }
    return is_num;
}

void Parser::printFileContents(const string& fileName) {
    string line;
    ifstream myfile(fileName);
    if (myfile.is_open())
        while (getline(myfile, line))
            cout << line << "\n";
    cout << "\n";
}
