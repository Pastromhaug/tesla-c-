#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
using namespace std;

vector<string>* Parser::parseLine(string &line) {
    vector<string>* line_words = new vector<string>();
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
        if (word != "=")
        {
            line_words->push_back(word);
        }
    }
    return line_words;
}
