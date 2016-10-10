#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "parser.h"
#include "variableNode.h"
using namespace std;


unordered_map<string, vector<VariableNode* >* > VariableNode::equations_map;
unordered_map<string, double> VariableNode::solution_map;

int main(int argc, char *argv[]) {

    char* filename = argv[1];
    cout << "Filename: " << filename << "\n\n";
    Parser::printFileContents(filename);

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
                if (VariableNode::equations_map.count(word) == 0)
                    VariableNode::equations_map[word] = new vector<VariableNode* >();
                auto begin = VariableNode::equations_map[word]->begin();
                auto end = VariableNode::equations_map[word]->end();
                if (count(begin, end, node) == 0)
                    VariableNode::equations_map[word]->push_back(node);
            }
            delete line_words;
            node->printEquation();
        }
        VariableNode::printEquationMap();

    }
    else
    {
        cout << "unable to open file: " << filename << endl;
    }
}
