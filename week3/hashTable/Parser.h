#pragma once

#include "State.h"
#include "Item.h"
#include "Grammar.h"

#include <vector>
#include <algorithm>
#include <stack>

using namespace std;

class TableRow
{
public:
	Action action;
	unordered_map<string, int> gotos;	// other cases
	Item production;					// in the case of reduce 

	TableRow() : action{ Action::Accept }, production{ {}, {}, 0 } {}
	TableRow(Action action, unordered_map<string, int> gotos) : action{ action }, gotos{ gotos }, production{ {}, {}, 0 } {}
	TableRow(Action action, Item production) : action{ action }, production{ production } {}
};

class Parser
{
public:
	Grammar grammar;
	unordered_map<int, vector<pair<string, int>>> connections;
	vector<State> cannonicalCollection;
	vector<TableRow> parsingTable; 

	Parser(Grammar grammar) : grammar{ grammar } {}

	State closure(vector<Item> items);
	State goTo(State state, string atom);
	void createCanonicalCollection();
	void createParsingTable();
	vector<Item> parse(vector<string> atoms);

	int getStateIndex(const State& state) const;
	string toString();
};