#include "HashTable.h"
#include <iostream>

int main() 
{
	HashTable<string> table(30);

	
	table.insert("some");
	table.insert("another");
	table.insert("another2");
	table.insert("another3");
	table.insert("another4");

	table.displayHash();


	HashTable<int> tableInt(3);
	tableInt.insert(2);
	tableInt.insert(4);
	tableInt.insert(12);
	tableInt.insert(123);
	tableInt.insert(125);

	tableInt.displayHash();

	return 0;
}