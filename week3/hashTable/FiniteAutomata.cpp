#include "FiniteAutomata.h"
#include <fstream>
#include <sstream>
#include <iostream>


FiniteAutomata::FiniteAutomata(string file)
{
	ifstream f(file);

	string initial, final, states, alphabet;

	getline(f, initial, '\n');
	getline(f, final, '\n');
	getline(f, alphabet, '\n');
	getline(f, states, '\n');

	this->initialState = initial;

	auto finalStatesTokens = this->tokenize(final, ' ');
	for (string s : finalStatesTokens) this->finalStates.insert(s);

	auto statesTokens = this->tokenize(states, ' ');
	for (string s : statesTokens) this->states.insert(s);

	auto alphabetTokens= this->tokenize(alphabet, ' ');
	for (string s : alphabetTokens) this->alphabet.insert(s[0]);

	string transition;
	while (getline(f, transition, '\n'))
	{
		auto transitionTokens = this->tokenize(transition, ' ');
		this->transitions[transitionTokens[0]].insert({ transitionTokens[2][0], transitionTokens[1]});
	}
}

vector<string> FiniteAutomata::tokenize(string s, char delim)
{
	stringstream ss(s);
	vector<string> result;
	string tmp;
	while (getline(ss, tmp, delim))
		result.push_back(tmp);
	return result;
}

bool FiniteAutomata::check(string s)
{
	string currentState = this->initialState;
	for (char c : s)
	{
		auto found = this->transitions[currentState].find(c);
		if (found == this->transitions[currentState].end()) return false;
		else currentState = found->second;
	}

	if (this->finalStates.find(currentState) != this->finalStates.end()) return true;
	return false;
}

void FiniteAutomata::displayInitialState()
{
	cout << "Initial states: " << this->initialState << '\n';
}

void FiniteAutomata::displayFinalStates()
{
	cout << "Final states: ";
	for (string s : this->finalStates) cout << s << " ";
	cout << '\n';
}

void FiniteAutomata::displayAlphabet()
{
	cout << "Alphabet states: ";
	for (char s : this->alphabet) cout << s << " ";
	cout << '\n';
}

void FiniteAutomata::displayStates()
{
	cout << "All states: ";
	for (string s : this->states) cout << s << " ";
	cout << '\n';
}

void FiniteAutomata::displayTransitions()
{
	cout << "Transitions: " << '\n';
	for (auto [currentState, states] : this->transitions)
		for (auto [value, nextState] : states)
		{
			cout << currentState << " " << nextState << " " << value << '\n';
		}
	cout << '\n';
}

void FiniteAutomata::displayFA() 
{
	this->displayInitialState();
	this->displayFinalStates();
	this->displayAlphabet();
	this->displayStates();
	this->displayTransitions();
}