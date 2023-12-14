#pragma once

#include <unordered_set>
#include <string>
#include "Item.h"

using namespace std;

enum Action {
	Accept,
	Reduce,
	Shift,
	ShiftReduceConflict,
	ReduceReduceConflict,
};


class State
{
private:
	bool allDotsNotAtEnd();
	bool allDotsAtEnd();

public:
	vector<Item> closureItems;
	vector<Item> closure;

	State() {}
	State(vector<Item> closureItems, vector<Item> closure) : closureItems{ closureItems }, closure{ closure } {}

	bool operator==(const State& otherState) const;

	bool hasSameClosure(const vector<Item>& otherClosure);
	vector<string> getAtomAfterDot();

	Action getAction(string startingAtom);

	string toString(int index) const;
};

template<>
struct hash<State>
{
	size_t operator()(const State& state) const
	{
		return hash<string>()(state.toString(0));
	}
};