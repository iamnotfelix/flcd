#pragma once

#include <string>
#include <vector>

using namespace std;

class Item
{
private:
public:
	string lhs;
	vector<string> rhs;
	int dotPosition;

	Item(string lhs, vector<string> rhs, int dotPosition) : lhs{ lhs }, rhs{ rhs }, dotPosition{ dotPosition } {}

	bool operator==(const Item& otherItem) const;

	string toString();
};
