#ifndef PARSER_H
#define PARSER_H
#include <string>
#include <vector>
using namespace std;

namespace Parser
{
    // returns true if valid file, false otherwise
    bool checkFileName(const string & file_name);

    // takes a file name as input and breaks it up by line and then by word,
    // returning a vector of vectors of words.
    vector<vector<string>> parseFile(const string &line);

    // takes a string and returns a vector of the words in the string
    vector<string> parseLine(const string &line);

    // returns true if the string is a double
    bool isDouble(const string&);

    // print out the contents of a text file line for line
    void printFileContents(const string&);
}

#endif
