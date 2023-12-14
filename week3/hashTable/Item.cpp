#include "Item.h"

bool Item::operator==(const Item& otherItem) const
{
	if (this->lhs != otherItem.lhs) return false;
	if (this->dotPosition != otherItem.dotPosition) return false;
	if (this->rhs.size() != otherItem.rhs.size()) return false;

	for (int i = 0; i < this->rhs.size(); ++i)
		if (this->rhs[i] != otherItem.rhs[i]) return false;

	return true;
}

string Item::toString()
{
	string itemString =  "[ " + this->lhs + "->";
	for (int i = 0; i < this->rhs.size(); ++i)
	{
		if (i == this->dotPosition) itemString += ".";
		string value = this->rhs[i];
		itemString += value;
	}
	if (this->dotPosition == this->rhs.size()) itemString += ".";
	itemString += " ]";

	return itemString;
}