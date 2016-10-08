#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "parser.h"
using namespace std;

int main(int argc, char *argv[]) {

    char* filename = argv[1];
    cout << "Filename: " << filename << endl;

    string line;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            cout << line << "\n";
            string test = "hi my name is per andre ";
            Parser::parseLine(test);
            vector<string>* line_words= Parser::parseLine(line);
            for (int i = 0; i < line_words->size(); i++)
            {
                string word = (*line_words)[i];
                cout << word << ",";
            }
            cout << "\n";
        }
    }
    else
    {
        cout << "unable to open file: " << filename << endl;
    }
}
