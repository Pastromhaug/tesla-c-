#include "variableNode.h"
#include "parser.h"
#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

VariableNode::VariableNode(vector<string>& line_words) {
    right_side_value = 0;
    left_side_value = 0;
    vector<string>::iterator equal_ptr = find(line_words.begin(), line_words.end(), "=");
    int equal_idx = distance(line_words.begin(), equal_ptr);

    initLeftSide(line_words, equal_idx);
    equal_idx++;
    initRightSide(line_words, equal_idx);
    addToEquationMap(line_words);
    evaluateAndUpdate();

}

void VariableNode::evaluateAndUpdate() {
    if (canEvaluate())
    {
        pair<string,double> pair = evaluate();
        string var_name = pair.first;
        double var_val = pair.second;
        solution_map[var_name] = var_val;

        vector<VariableNode*> node_vector= *(equations_map[var_name]);
        for (auto node : node_vector)
            node->updateWithValue(var_name, var_val);
    }
}

void VariableNode::updateWithValue(const string& var_name, double var_val){
    // cout << 1 << endl;
    if (left_side_variables.count(var_name) != 0)
    {
        // cout << 3 << endl;
        int count = left_side_variables[var_name];
        // cout << 4 << endl;
        left_side_value += (count*var_val - right_side_value);
        left_side_variables.erase(var_name);
        // cout << 5 << endl;
    }
    // cout << 2 << endl;
    if (right_side_variables.count(var_name) != 0)
    {
        int count = right_side_variables[var_name];
        right_side_value += (count*var_val - left_side_value);
        right_side_variables.erase(var_name);
    }
    evaluateAndUpdate();
}

void VariableNode::addToEquationMap(vector<string>& line_words) {
    for (auto it : left_side_variables)
    {
        string var_name = it.first;
        if (equations_map.count(var_name) == 0)
            equations_map[var_name] = new vector<VariableNode* >();
        equations_map[var_name]->push_back(this);
    }
    for (auto it : right_side_variables)
    {
        string var_name = it.first;
        if (equations_map.count(var_name) == 0)
            equations_map[var_name] = new vector<VariableNode* >();
        equations_map[var_name]->push_back(this);
    }
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

void VariableNode::printEquationMap() {
    cout << "\n equations_map: \n";
    for (auto it: VariableNode::equations_map)
    {
        cout << it.first << ":";
        for (auto ptr: *(it.second))
            cout << " " << ptr;
        cout << "\n";
    }
}

void VariableNode::printSolutionMap() {
    cout << "\n solution_map: \n";
    vector<string> var_names;
    for (auto it: VariableNode::solution_map)
        var_names.push_back(it.first);

    sort(var_names.begin(), var_names.end());
    for (string var_name : var_names)
    {
        cout << var_name << ": " << solution_map[var_name] << "\n";
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
