#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;

namespace Parser
{
    vector<string>* parseLine(const string &line);
    bool isDouble(const string&);
    void printFileContents(const string&);
}

#endif
