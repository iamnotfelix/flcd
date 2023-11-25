#include "Grammar.h"

#include <fstream>
#include <sstream>
#include <iostream>


#include <algorithm> 
#include <cctype>
#include <locale>

// trim from start (in place)
static inline void ltrim(string& s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(string& s) {
	s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(string& s) {
	rtrim(s);
	ltrim(s);
}

vector<string> Grammar::parseTerminalsAndNonTerminals(string input) 
{
	stringstream ss(input);
	vector<string> tokens;
	string token;

	while (getline(ss, token, ' ')) tokens.push_back(token);

	return tokens;
}

pair<string, vector<string>> Grammar::parseProduction(string production) 
{
	string nonTerminal = production.substr(0, production.find('='));
	trim(nonTerminal);
	if (this->nonterminals.find(nonTerminal) == this->nonterminals.end())
		throw invalid_argument("Grammar is not CFG or non-terminal does not exist.\n");

	production = production.substr(production.find('=') + 1);
	trim(production);

	vector<string> tokens;
	stringstream ss(production);
	string token;

	while (getline(ss, token, '|'))
	{
		trim(token);
		tokens.push_back(token);
	}

	return { nonTerminal, tokens };
}

Grammar::Grammar(string file)
{
	ifstream f(file);

	string nonterminals, terminals;
	getline(f, nonterminals, '\n');
	getline(f, terminals, '\n');
	getline(f, this->start, '\n');

	vector<string> nonterminalsVector = this->parseTerminalsAndNonTerminals(nonterminals);
	vector<string> terminalsVector = this->parseTerminalsAndNonTerminals(terminals);

	this->nonterminals = unordered_set<string>(nonterminalsVector.begin(), nonterminalsVector.end());
	this->terminals = unordered_set<string>(terminalsVector.begin(), terminalsVector.end());

	string production;
	while (getline(f, production, '\n'))
		this->productions.insert(this->parseProduction(production));
}

void Grammar::displayTerminals()
{
	for (string terminal : this->terminals)
	{
		cout << terminal << " ";
	}

	cout << '\n';
}

void Grammar::displayNonTerminals()
{
	for (string nonterminal : this->nonterminals)
	{
		cout << nonterminal << " ";
	}

	cout << '\n';
}

void Grammar::displayProductions()
{
	for (auto [key, production] : this->productions)
	{
		cout << key << " = ";
		bool first = true;
		for (string prod : production)
		{
			if (!first) cout << " | ";
			else first = false;
			cout << prod;
		}
		cout << '\n';
	}
}

void  Grammar::displayProduction(string nonterminal)
{
	auto [key, production] = *this->productions.find(nonterminal);

	cout << key << " = ";
	bool first = true;
	for (string prod : production)
	{
		if (!first) cout << " | ";
		else first = false;
		cout << prod;
	}
	cout << '\n';
}