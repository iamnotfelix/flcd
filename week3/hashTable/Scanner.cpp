#pragma once

#include "Scanner.h"
#include "HashTable.h"

#include <fstream>
#include <sstream>
#include <string>
#include <regex>

Scanner::Scanner(
	string tokensFilename,
	string separatorsFilename,
	string operatorsFilename,
	string reservedWordsFilename,
	FiniteAutomata faIntConst,
	FiniteAutomata faIdentifier
) : faIntConst{ faIntConst }, faIdentifier{ faIdentifier } {
	this->symbolTable = HashTable<string>(1000);

	ifstream file(tokensFilename);
	string atom;
	int i = 0;
	this->codesToAtom.push_back("");
	while (getline(file, atom, '\n'))
	{
		this->codesToAtom.push_back(atom);
		this->atomCodes.insert({ atom, ++i });
	}

	this->readAtoms(separatorsFilename, this->separators);
	this->readAtoms(operatorsFilename, this->operators);
	this->readAtoms(reservedWordsFilename, this->reservedWords);
}

void Scanner::readAtoms(string filename, unordered_set<string>& container)
{
	ifstream file(filename);
	string atom;
	while (getline(file, atom, '\n')) container.insert(atom);
}

bool Scanner::isReservedWord(string atom) 
{
	if (atom.empty()) return false;
	if (this->reservedWords.find(atom) != this->reservedWords.end()) return true;
	return false;
}

bool Scanner::isIdentifier(string atom) 
{
	if (atom.empty()) return false;
	return this->faIdentifier.check(atom);
	//if (regex_match(atom, regex("^[a-zA-Z_][a-zA-Z0-9_]*$"))) return true;
	return false;
}

bool Scanner::isConstant(string atom) 
{
	if (atom.empty()) return false;

	// Integer or boolean in number representation
	//if (regex_match(atom, regex("^[+-]?[0-9]*$"))) return true;
	if (this->faIntConst.check(atom)) return true;
	
	// String
	if (regex_match(atom, regex("^\".*\"$"))) return true;

	// Bool
	if (regex_match(atom, regex("^(?:true|false)^"))) return true;


	return false;
}

void Scanner::classifyAtom(string atom, int lineNr) 
{
	if (this->isReservedWord(atom))
	{
		pif.push_back({ this->atomCodes[atom], 0 });
	}
	else if (this->isIdentifier(atom))
	{
		pair<bool, int> found = this->symbolTable.search(atom);
		int code = -1;
		if (!found.first) code = this->symbolTable.insert(atom);
		else code = found.second;

		pif.push_back({ this->atomCodes["identifier"], code });
	}
	else if (this->isConstant(atom))
	{
		pair<bool, int> found = this->symbolTable.search(atom);
		int code = -1;
		if (!found.first) code = this->symbolTable.insert(atom);
		else code = found.second;

		pif.push_back({ this->atomCodes["constant"], code });
	}
	else if (!atom.empty())
	{
		string ex = "Lexical error on line " + to_string(lineNr) + ", at atom '" + atom + "'";
		throw runtime_error(ex);
	}
}

void Scanner::scan(string source) 
{
	ifstream f(source);

	string line;
	int lineNr = 0;

	bool startedString = false;
	while (getline(f, line, '\n')) 
	{
		++lineNr;

		string atom = "";
		for (int i = 0; i < line.length(); ++i)
		{
			string currentChar = string(line.begin() + i, line.begin() + i + 1);
			//cout << currentChar;
			if (startedString)
			{
				atom += currentChar;
				if (currentChar == "\"")
				{
					this->classifyAtom(atom, lineNr);
					startedString = false;
					atom = "";
				}
			}
			else if (this->separators.find(currentChar) != separators.end()) 
			{
				bool isQuote = currentChar == "\"";
				this->classifyAtom(atom, lineNr);
				
				if (isQuote) startedString = true;

				if (currentChar != " " && !isQuote)
				{
					pif.push_back({ this->atomCodes[currentChar], 0 });
				}

				atom = (isQuote ? "\"" : "");
			}
			else if (this->operators.find(currentChar) != operators.end())
			{
				string altOperator1 = (i - 1 >= 0
					? line[i - 1] + currentChar
					: currentChar);
				string altOperator2 = (i + 1 < line.length()
					? currentChar + line[i + 1]
					: currentChar);

				if (this->operators.find(altOperator1) != operators.end())
				{
					atom.pop_back();
					this->classifyAtom(atom, lineNr);
					pif.push_back({ this->atomCodes[altOperator1], 0 });
				}
				else if (this->operators.find(altOperator2) != operators.end())
				{
					this->classifyAtom(atom, lineNr);
					pif.push_back({ this->atomCodes[altOperator2], 0 });
					++i;
				}
				else 
				{
					this->classifyAtom(atom, lineNr);
					pif.push_back({ this->atomCodes[currentChar], 0 });
				}
				atom = "";
			}
			else 
			{
				atom += line[i];
			}
		}
		//cout << "\n";
	}
}

void Scanner::displayPif() 
{
	for (auto [atomCode, index] : this->pif)
	{
		cout << this->codesToAtom[atomCode] << " " << index << endl;
	}
}

void Scanner::dumpToFile()
{
	this->symbolTable.dumpToFile();

	ofstream g("pif.out");
	for (auto [atomCode, index] : this->pif)
	{
		g << atomCode << " " << index << endl;
	}
}