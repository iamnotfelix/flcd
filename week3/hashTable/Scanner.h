#pragma once

#include "HashTable.h"

#include <string>
#include <vector>
#include <string>
#include <unordered_set>
#include <unordered_map>

using namespace std;

class Scanner {
private:
	HashTable<string> symbolTable;
	vector<pair<int, int>> pif;

	unordered_map<string, int> atomCodes;
	vector<string> codesToAtom;

	unordered_set<string> separators;
	unordered_set<string> operators;
	unordered_set<string> reservedWords;

	void readAtoms(string filename, unordered_set<string>& container);

	bool isReservedWord(string atom);
	bool isIdentifier(string atom);
	bool isConstant(string atom);

	void classifyAtom(string atom, int lineNr);

public:
	Scanner(
		string tokensFilename,
		string separatorsFilename,
		string operatorsFilename,
		string reservedWordsFilename
	);
	void scan(string outputFile);
	void displayPif();
};