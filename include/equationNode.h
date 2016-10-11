#ifndef EQUATIONNODE_H
#define EQUATIONNODE_H

#include <vector>
#include <string>
#include <unordered_map>
using namespace std;


class EquationNode {

    //-------- variables ---------

    // holds the known integer value of this variable so far
    // on each side of the equation
    double _right_side_value;
    double _left_side_value;

    // map of the variables on each side of the equation.
    // map points from variable name to count
    unordered_map<string, int> _right_side_variables;
    unordered_map<string, int> _left_side_variables;

    // map from each variable to it's known value. Only contains the variables
    // that we've solved for
    static unordered_map<string, double> _solution_map;

    // maps from each variable to all the equations that contain it
    static unordered_map<string, vector<EquationNode* >* > _equation_map;

    // vector of pointers to all dynamically allocated instances of this class
    // used to clean up dynamic memory and prevent leaks
    static vector<EquationNode*> _dynamic_objects;

    //-------- functions ---------

    // helper function for initLeftSide and initRightSide
    void _setValAndVars(const vector<string>&, const int&, const int&, double&, double&, unordered_map<string, int>&, unordered_map<string, int>&);

    // initializes the left side of the equation by initializing
    // left_side_value and left_side_variables .
    // takes the vector of variables, and the index of the '='
    void _initLeftSide(const vector<string>&, const int&);
    void _initRightSide(const vector<string>&, const int&);

    // check if this function can be solved for some variable.
    // will only return true if the internal function representation has form
    // "a = 3" or "3 = a".
    bool _canEvaluate();

    // evaluate the function, return the variable, and the double representing
    // the number it is equal to
    pair<string, double> _evaluate();

    // updates the equation in the node with new info of a certain
    // variable's value. For example, if you have equation
    // "a = b + c + 5", and you now know that b = 3, this function updates
    // the equation to be "a = c + 8"
    void _updateWithValue(const string&, const double);

    // checks if the function can be evaluated. It some variable 'a' in the function
    // can be solved for, update all the other equations in the system with
    // the new information about the value of 'a'
    void _evaluateAndUpdate();

    // Adds the equation to the static map 'equations_map'
    void _addToEquationMap();

    // helper for addToEquationsMap()
    void _addToEquationMapHelper(unordered_map<string, int>&);

    // prints the contents of the static member equations_map
    static void _printEquationMap();

    // prints the contents of the static member solution_map
    static void _printSolutionMap();

    // print out the internal representation of the left side variables and values
    // or right side variables and values
    void _printMap(const unordered_map<string, int>&);

    // prints out the whole internal representation of the equation stored in
    // this node.
    void _printEquation();


    public:
        // constructor. takes a vector of strings representing all the
        // words/terms in an equation separated by space
        // e.x. "a = b + c + 5" should be input as ["a", "=", "b", "+", "c","+","5"]
        EquationNode(vector<string>&);

        // takes all the words in a file representing a system of equations, solves the system of equations,
        // prints the solution to the console
        static void solveSystemOfEquations(vector<vector<string>>& system);


};

#endif
