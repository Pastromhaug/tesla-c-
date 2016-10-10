#ifndef VARIABLENODE_H
#define VARIABLENODE_H

#include <vector>
#include <string>
#include <unordered_map>
using namespace std;

// Node keeps track of a variable and it's connections to other
// variables.
class VariableNode {
    // takes a vector of strings, start index, stop index, pointer to value
    // accumulator, and pointer to a map, and iterates through the vector
    // initializing the value and the map
    void setValAndVars(vector<string>&, int&, int&, double&, unordered_map<string, int>&);

    // initializes the left side of the equation. takes the vector
    // of variables, and the index of the '='
    void initLeftSide(vector<string>&, int&);
    void initRightSide(vector<string>&, int&);

    public:
        // holds the known integer value of this variable so far
        // on each side of the equation
        double right_side_value;
        double left_side_value;

        // map of the variables on each side of the equation.
        // map points from variable name to count
        unordered_map<string, int> right_side_variables;
        unordered_map<string, int> left_side_variables;
        VariableNode(vector<string>&);
        void printMap(unordered_map<string, int>&);
        void print();
};

#endif
