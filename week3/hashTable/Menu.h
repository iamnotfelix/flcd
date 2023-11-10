#pragma once
#include <string>
#include <unordered_map>

#include "FiniteAutomata.h"

using namespace std;

class Menu
{
private:
	FiniteAutomata fa;

	void displayMenu();
	void parseCommand(string command);
	void checkSequence();
public:
	typedef enum {
		Help, Init, Final, State, Alpha, Trans, Check, Exit
	} Command;

	unordered_map<string, Command> toCommand{
		{"help", Command::Help},
		{"init", Command::Init},
		{"final", Command::Final},
		{"state", Command::State},
		{"alpha", Command::Alpha},
		{"trans", Command::Trans},
		{"check", Command::Check},
		{"exit", Command::Exit},
	};

	Menu(FiniteAutomata f) : fa{ f } {}
	void start();
};
