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
            node->printEquation();
        }
        VariableNode::printEquationMap();
        VariableNode::printSolutionMap();
    }
    else
    {
        cout << "unable to open file: " << filename << endl;
    }
}
