#pragma once

#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <string>
#include <utility>

using namespace std;

class Grammar
{
private:
	unordered_set<string> terminals;
	unordered_set<string> nonterminals;
	string start;
	unordered_map<string, vector<string>> productions;
	unordered_map<string, string> specialTerminals{
		{"SPACE", " " }, 
		{ "EQUAL", "=" }, 
		{ "OPEN_CURLY", "{" }, 
		{ "CLOSE_CURLY", "}" }, 
		{ "OPEN_ROUNDED", "(" }, 
		{ "CLOSE_ROUNDED", ")" }, 
		{ "OPEN_SQUARE", "[" }, 
		{ "CLOSE_SQUARE", "]" }, 
		{ "ANY_CHARACTER", "" },
	};

	vector<string> parseTerminalsAndNonTerminals(string input);
	pair<string, vector<string>> parseProduction(string production);

public:
	Grammar(string file);
	void displayTerminals();
	void displayNonTerminals();
	void displayProductions();
	void displayProduction(string nonterminal);
};