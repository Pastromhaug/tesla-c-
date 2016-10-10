#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include "parser.h"
#include "variableNode.h"
using namespace std;

int main(int argc, char *argv[]) {

    char* filename = argv[1];
    cout << "Filename: " << filename << "\n\n";
    Parser::printFileContents(filename);

    unordered_map<string, VariableNode*> equations_map;

    string line;
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            // cout << line << "\n";
            vector<string>* line_words = Parser::parseLine(line);
            VariableNode* node = new VariableNode(*line_words);
            for (string word: (*line_words))
            {
                if (Parser::isDouble(word)) continue;
                if (word == "+" || word == "=") continue;
                else
                    equations_map[word] = node;
            }
            delete line_words;
            node->print();
            // cout << node1.name << "\n";
            // cout << node1.value << "\n";
            // node1.print();
            // cout << "\n";
        }
    }
    else
    {
        cout << "unable to open file: " << filename << endl;
    }
}
