#include "equationNode.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

// initializing static members of class EquationNode
unordered_map<string, vector<EquationNode*>*> EquationNode::_equation_map;
unordered_map<string, double> EquationNode::_solution_map;
vector<EquationNode*> EquationNode::_dynamic_objects;

EquationNode::EquationNode(vector<string>& line_words) {
    _right_side_value = 0;
    _left_side_value = 0;

    // find the index of the "=" in the list of terms in the equation
    vector<string>::iterator equal_ptr = find(line_words.begin(), line_words.end(), "=");
    int equal_idx = distance(line_words.begin(), equal_ptr);

    _initLeftSide(line_words, equal_idx);
    equal_idx++;
    _initRightSide(line_words, equal_idx);

    // add this EquationNode to the _equation_map
    _addToEquationMap();

    // check if this equation can be evaluated, and make the appropriate updates if so.
    _evaluateAndUpdate();
}

void EquationNode::solveSystemOfEquations(vector<vector<string>>& system) {
    // store created objects in vector to allow them to be retrieved and deleted later
    for (vector<string> line : system)
        _dynamic_objects.push_back(new EquationNode(line));

    // print out results
    _printSolutionMap();

    // delete all allocated dynamic memory
    for (EquationNode* node : _dynamic_objects)
        delete node;
    for (auto it : _equation_map)
        delete it.second;
}

void EquationNode::_evaluateAndUpdate() {
    // check if the equation can be evaluated for a variable
    if (_canEvaluate())
    {
        // if it can be evaluated, get a pair of ex <var, 3.0>
        pair<string,double> pair = _evaluate();
        string var_name = pair.first;
        double var_val = pair.second;

        // add mapping from variable to it's value to solutions_map
        _solution_map[var_name] = var_val;

        // get the vector of equations that contains the variable we solved for
        vector<EquationNode* > node_vector= *(_equation_map[var_name]);

        // iterate through the equations, updating them with the variable's value
        for (auto node : node_vector)
            node->_updateWithValue(var_name, var_val);
    }
}

void EquationNode::_updateWithValue(const string& var_name, const double var_val){

    // if variable var_name is on the left side of the equation
    if (_left_side_variables.count(var_name) != 0)
    {
        // get the number of times var_name appears on the left side
        int count = _left_side_variables[var_name];
        // update the left side value by adding the value of var_name
        // multiplied by how many times the variable occurs.
        _left_side_value += count*var_val;
        // remove the variable from the left side of the equation
        _left_side_variables.erase(var_name);
    }

    // else if variable var_name is on the right side
    if (_right_side_variables.count(var_name) != 0)
    {
        // get the number of times var_name appears on the right side
        int count = _right_side_variables[var_name];
        // update the right side value by adding the value of var_name
        // multiplied by how many times the variable occurs.
        _right_side_value += count*var_val;
        // remove the variable from right side of equation.
        _right_side_variables.erase(var_name);
    }

    // now that equation has been updated, check if it can be evaluated for some variable x
    // and if so, evaluate it and update all equations that have x
    _evaluateAndUpdate();
}

void EquationNode::_addToEquationMap() {
    // loop through all the variables on the left and right side
    // of the equation and add mapping from the variable to this equation in
    // equation_map
    _addToEquationMapHelper(_left_side_variables);
    _addToEquationMapHelper(_right_side_variables);
}

void EquationNode::_addToEquationMapHelper(unordered_map<string, int>& map) {
    for (auto it : map)
    {
        string var_name = it.first;
        if (_equation_map.count(var_name) == 0)
            _equation_map[var_name] = new vector<EquationNode* >();
        _equation_map[var_name]->push_back(this);
    }
}

void EquationNode::_setValAndVars(const vector<string>& line_words, const int &start_idx,
    const int &end_idx, double& value, double& other_value, unordered_map<string, int> & map, unordered_map<string, int> & other_map) {
    // this function does much of the hard work of initializing the EquationNode object

    // loop through the vector of terms eg ["a","+","b","=","5"]
    for (int i = start_idx; i < end_idx; i++)
    {

        string word = line_words[i];
        if (word == "+") continue; // can skip "+"

        // check if the variable is a number, if so update the appropriate value
        if (Parser::isDouble(word))
        {
            value += stod(word) - other_value;
            other_value = 0;
        }

        // check if the variable's value is already known, and if so update the appropriate value
        else if (_solution_map.count(word) == 1)
        {
            value += _solution_map[word] - other_value;
            other_value = 0;
        }

        // else,this is a term that is not a number, and is not solved for yet.
        else
        {
            int other_map_count = other_map.count(word);
            // if the other side has this variable, subtract it from other side
            if (other_map_count != 0)
            {
                if (other_map[word] == 1)
                    other_map.erase(word);
                else
                {
                    int old_count = other_map[word];
                    other_map[word] = --old_count;
                }
            }
            else // other side doesn't have the variable, so add it to this side
            {
                if ( map.count(word) == 0 )
                    map[word] = 1;
                else
                {
                    int old_count = map[word];
                    map[word] = ++old_count;
                }
            }
        }
    }
}


void EquationNode::_initRightSide(const vector<string>& line_words, const int& start_idx) {
    // presumably, parameter start_idx is the index in line_worlds following the "="
    int end_idx = (int)line_words.size();
    _setValAndVars(line_words, start_idx, end_idx, _right_side_value, _left_side_value,
        _right_side_variables, _left_side_variables);
}

void EquationNode::_initLeftSide(const vector<string>& line_words, const int& end_idx) {
    // presumably, parameter end_idx is the index of "=" in the equation
    int start_idx = 0;
    _setValAndVars(line_words, start_idx , end_idx, _left_side_value, _right_side_value,
        _left_side_variables, _right_side_variables);
}

bool EquationNode::_canEvaluate() {
    int left_count = _left_side_variables.size();
    int right_count = _right_side_variables.size();

    // here we have 'var=5' or '5=var';
    if (left_count + right_count == 1) return true;
    return false;
}


pair<string, double> EquationNode::_evaluate() {
    // this function assumes that the expression can be evaluated, and has form
    // "var = 6" or "6 = var"

    int left_count = _left_side_variables.size();
    int right_count = _right_side_variables.size();
    // check if variable is on left side, such as "2*var = 6"
    if (left_count == 1)
    {
        auto it = _left_side_variables.begin();
        string var_name = it->first;
        int var_count = it->second;
        double var_val = (_right_side_value - _left_side_value)/var_count;
        return make_pair(var_name, var_val);
    }
    // else we know the variable is on the right side "6 = 2*var"
    else
    {
        auto it = _right_side_variables.begin();
        string var_name = it->first;
        int var_count = it->second;
        double var_val = (_left_side_value - _right_side_value)/var_count;
        return make_pair(var_name, var_val);
    }
}

void EquationNode::_printMap(const unordered_map<string, int>& map) {
    // this function is presumably used to print out _left_side_variables
    // and _right_side_variables
    for (auto it: map)
        cout << it.second << "*" << it.first << " + ";
}

void EquationNode::_printEquationMap() {
    cout << "\t _equation_map \n";
    for (auto it: EquationNode::_equation_map)
    {
        cout << it.first << ":";
        for (auto ptr: *(it.second))
            cout << " " << ptr;
        cout << "\n";
    }
}

void EquationNode::_printSolutionMap() {
    vector<string> var_names;
    for (auto it: EquationNode::_solution_map)
        var_names.push_back(it.first);

    sort(var_names.begin(), var_names.end());
    for (string var_name : var_names)
    {
        cout << var_name << " = " << _solution_map[var_name] << "\n";
    }
}

void EquationNode::_printEquation() {
    _printMap(_left_side_variables);
    cout  << _left_side_value;
    cout << " = ";
    _printMap(_right_side_variables);
    cout  << _right_side_value;
    cout << "\n";
}
