#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "parser.h"
#include "equationNode.h"
using namespace std;


unordered_map<string, vector<EquationNode*>*> EquationNode::_equation_map;
unordered_map<string, double> EquationNode::_solution_map;

int main(int argc, char *argv[]) {

    char* filename = argv[1];
    cout << "File: " << filename << "\n\n";
    if (!Parser::checkFileName(filename))
    {
        cout << "invalid file name\n";
        return 0;
    }

    Parser::printFileContents(filename);

    vector<vector<string>> parsed_file = Parser::parseFile(filename);
    for (vector<string> line : parsed_file)
        EquationNode* node = new EquationNode(line);
    EquationNode::printSolutionMap();
    EquationNode::releaseDynamicMemory();
}
