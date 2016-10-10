#include "variableNode.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

VariableNode::VariableNode(vector<string>& line_words, unordered_map<string,double>& solution_map) {
    right_side_value = 0;
    left_side_value = 0;
    vector<string>::iterator equal_ptr = find(line_words.begin(), line_words.end(), "=");
    int equal_idx = distance(line_words.begin(), equal_ptr);

    initLeftSide(line_words, equal_idx);
    equal_idx++;
    initRightSide(line_words, equal_idx);
}

void VariableNode::setValAndVars(vector<string>& line_words, int &start_idx,
    int &end_idx, double& value, double& other_value, unordered_map<string, int> & map, unordered_map<string, int> & other_map) {

    for (int i = start_idx; i < end_idx; i++)
    {
        string word = line_words[i];
        if (word == "+") continue;
        if (Parser::isDouble(word))
        {
            value += stod(word) - other_value;
            other_value = 0;
        }
        else if (solution_map.count(word) == 1)
        {
            value += solution_map[word] - other_value;
            other_value = 0;
        }
        else
        {   // this block is copied from the c++ reference k
            // http://www.cplusplus.com/reference/unordered_map/unordered_map/find/
            int other_map_count = other_map.count(word);
            // if the other side has this variable, subtract it from that side
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

void VariableNode::initRightSide(vector<string>& line_words, int& start_idx) {
    int end_idx = (int)line_words.size();
    setValAndVars(line_words, start_idx, end_idx, right_side_value, left_side_value,
        right_side_variables, left_side_variables);
}

void VariableNode::initLeftSide(vector<string>& line_words, int& end_idx) {
    int start_idx = 0;
    setValAndVars(line_words, start_idx , end_idx, left_side_value, right_side_value,
        left_side_variables, right_side_variables);
}

bool VariableNode::canEvaluate() {
    int left_count = left_side_variables.size();
    int right_count = right_side_variables.size();

    // here we have 'var=5' or '5=var';
    if (left_count + right_count == 1) return true;
    return false;
}

pair<string, double> VariableNode::evaluate() {
    int left_count = left_side_variables.size();
    int right_count = right_side_variables.size();
    if (left_count == 1)
    {
        string var_name = left_side_variables.begin()->first;
        double var_val = right_side_value - left_side_value;
        return make_pair(var_name, var_val);
    }
    else
    {
        string var_name = right_side_variables.begin()->first;
        double var_val = left_side_value - right_side_value;
        return make_pair(var_name, var_val);
    }
}

void VariableNode::printMap(unordered_map<string, int>& map) {
    for (auto it: map)
        cout << it.second << "*" << it.first << " + ";
}

static void VariableNode::printEquationMap() {
    cout << "\n equations_map: \n";
    for (auto it: VariableNode::equations_map)
    {
        cout << it.first << ":";
        for (auto ptr: *(it.second))
            cout << " " << ptr;
        cout << "\n";
    }
}

void VariableNode::printEquation() {
    printMap(left_side_variables);
    cout  << left_side_value;
    cout << " = ";
    printMap(right_side_variables);
    cout  << right_side_value;
    cout << "\n";
}
