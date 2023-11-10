#pragma once
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <vector>
#include <utility>
#include <sstream>

using namespace std;

class FiniteAutomata {
private:
	string initialState;
	unordered_set<string> finalStates;
	unordered_set<string> states;
	unordered_set<char> alphabet;
	unordered_map <string, unordered_map<char, string>> transitions; // vector<pair<string, char>>

	vector<string> tokenize(string s, char delim);
public:
	FiniteAutomata(string file);

	bool check(string s);

	void displayInitialState();
	void displayFinalStates();
	void displayAlphabet();
	void displayStates();
	void displayTransitions();
	void displayFA();
};

