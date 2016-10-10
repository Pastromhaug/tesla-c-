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
}

void VariableNode::setValAndVars(vector<string>& line_words, int &start_idx,
    int &end_idx, double& value, unordered_map<string, int> & map) {

    for (int i = start_idx; i < end_idx; i++)
    {
        string word = line_words[i];
        if (word == "+") continue;
        if (Parser::isDouble(word)) value += stod(word);
        else
        {   // this block is copied from the c++ reference k
            // http://www.cplusplus.com/reference/unordered_map/unordered_map/find/
            if ( map.count(word) == 0 )
            {
                map[word] = 1;
            }
            else
            {
                int old_count = map[word];
                map[word] = ++old_count;
            }
        }
    }
}

void VariableNode::initRightSide(vector<string>& line_words, int& start_idx) {
    int end_idx = (int)line_words.size();
    setValAndVars(line_words, start_idx, end_idx, right_side_value, right_side_variables);
}

void VariableNode::initLeftSide(vector<string>& line_words, int& end_idx) {
    int start_idx = 0;
    setValAndVars(line_words, start_idx , end_idx, left_side_value, left_side_variables);
}

void VariableNode::printMap(unordered_map<string, int>& map) {
    for (auto it: map)
        cout << it.first << ": " << it.second << ", ";
}

void VariableNode::print() {
    cout << "[left] ";
    printMap(left_side_variables);
    cout << " [value] " << left_side_value;
    cout << " [right] ";
    printMap(right_side_variables);
    cout << " [value] " << right_side_value;
    cout << "\n";
}
