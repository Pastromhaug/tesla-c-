#include "equationNode.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

EquationNode::EquationNode(vector<string>& line_words) {
    _right_side_value = 0;
    _left_side_value = 0;
    vector<string>::iterator equal_ptr = find(line_words.begin(), line_words.end(), "=");
    int equal_idx = distance(line_words.begin(), equal_ptr);

    _initLeftSide(line_words, equal_idx);
    equal_idx++;
    _initRightSide(line_words, equal_idx);
    _addToEquationMap();
    _evaluateAndUpdate();
}

void EquationNode::_evaluateAndUpdate() {
    if (_canEvaluate())
    {
        pair<string,double> pair = _evaluate();
        string var_name = pair.first;
        double var_val = pair.second;
        _solution_map[var_name] = var_val;

        vector<EquationNode* > node_vector= *(_equation_map[var_name]);
        for (auto node : node_vector)
            node->_updateWithValue(var_name, var_val);
    }
}

void EquationNode::_updateWithValue(const string& var_name, const double var_val){
    if (_left_side_variables.count(var_name) != 0)
    {
        int count = _left_side_variables[var_name];
        _left_side_value += (count*var_val - _right_side_value);
        _left_side_variables.erase(var_name);
    }
    if (_right_side_variables.count(var_name) != 0)
    {
        int count = _right_side_variables[var_name];
        _right_side_value += (count*var_val - _left_side_value);
        _right_side_variables.erase(var_name);
    }
    _evaluateAndUpdate();
}

void EquationNode::_addToEquationMap() {
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

    for (int i = start_idx; i < end_idx; i++)
    {
        string word = line_words[i];
        if (word == "+") continue;
        if (Parser::isDouble(word))
        {
            value += stod(word) - other_value;
            other_value = 0;
        }
        else if (_solution_map.count(word) == 1)
        {
            value += _solution_map[word] - other_value;
            other_value = 0;
        }
        else
        {
            int other_map_count = other_map.count(word);
            // if the other side has this variable, subtract it from other side
            if (other_map_count != 0)
            {
                if (other_map_count == 1)
                    other_map.erase(word);
                else
                {
                    int old_count = other_map[word];
                    other_map[word] == --old_count;
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
    int end_idx = (int)line_words.size();
    _setValAndVars(line_words, start_idx, end_idx, _right_side_value, _left_side_value,
        _right_side_variables, _left_side_variables);
}

void EquationNode::_initLeftSide(const vector<string>& line_words, const int& end_idx) {
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
    int left_count = _left_side_variables.size();
    int right_count = _right_side_variables.size();
    if (left_count == 1)
    {
        string var_name = _left_side_variables.begin()->first;
        double var_val = _right_side_value - _left_side_value;
        return make_pair(var_name, var_val);
    }
    else
    {
        string var_name = _right_side_variables.begin()->first;
        double var_val = _left_side_value - _right_side_value;
        return make_pair(var_name, var_val);
    }
}

void EquationNode::printMap(const unordered_map<string, int>& map) {
    for (auto it: map)
        cout << it.second << "*" << it.first << " + ";
}

void EquationNode::printEquationMap() {
    cout << "\t _equation_map \n";
    for (auto it: EquationNode::_equation_map)
    {
        cout << it.first << ":";
        for (auto ptr: *(it.second))
            cout << " " << ptr;
        cout << "\n";
    }
}

void EquationNode::printSolutionMap() {
    cout << "\t solution \n";
    vector<string> var_names;
    for (auto it: EquationNode::_solution_map)
        var_names.push_back(it.first);

    sort(var_names.begin(), var_names.end());
    for (string var_name : var_names)
    {
        cout << var_name << ": " << _solution_map[var_name] << "\n";
    }
}

void EquationNode::printEquation() {
    printMap(_left_side_variables);
    cout  << _left_side_value;
    cout << " = ";
    printMap(_right_side_variables);
    cout  << _right_side_value;
    cout << "\n";
}

void EquationNode::releaseDynamicMemory() {
    for (auto it : _equation_map)
    {
//        cout << 1 << endl;
        vector<EquationNode*>* vec = it.second;
//        cout << 2 << endl;
        delete vec;
//        cout << 4 << endl;
    }
}
