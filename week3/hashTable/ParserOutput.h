#pragma once

#include "Grammar.h"
#include "Item.h"

#include <iostream>
#include <fstream>


class ParserOutput
{
public:

	struct Node
	{
		int id;
		string atom;
		int father;
		int sibling;

		Node(int id, string atom, int father, int sibling) : id{ id }, atom{ atom }, father{ father }, sibling{ sibling } {}
		string toString() 
		{
			return to_string(this->id) + " " + this->atom + " " + to_string(this->father) + " " + to_string(this->sibling);
		}
	};

	Grammar grammar;
	vector<Item> output;
	vector<Node> parsingTree;

	ParserOutput(Grammar grammar, vector<Item> output) : grammar{ grammar }, output{ output } {}

	void createParsingTree();
	void display();
	void saveToFile(string file);
};