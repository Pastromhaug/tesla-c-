#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include "parser.h"
#include "equationNode.h"
using namespace std;


int main(int argc, char *argv[]) {

    // grab the file name argument from command line
    char* filename = argv[1];
    cout << "File: " << filename << "\n\n";

    // make sure the file exists
    if (!Parser::checkFileName(filename))
    {
        cout << "invalid file name\n";
        return 0;
    }

    // print out the file contents
    Parser::printFileContents(filename);
    // parse the file into a vector of vectors of strings, where each vector is a line
    // and each string is a word separated by spaces.
    vector<vector<string>> parsed_file = Parser::parseFile(filename);
    // solve the system of equations and print out the solution
    EquationNode::solveSystemOfEquations(parsed_file);
}
