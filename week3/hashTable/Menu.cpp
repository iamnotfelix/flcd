#include <iostream>

#include "Menu.h"


void Menu::displayMenu() 
{
	cout << "help\tDisplay this menu." << '\n';
	cout << "init\tDisplay initial state." << '\n';
	cout << "final\tDisplay final states." << '\n';
	cout << "state\tDisplay all states." << '\n';
	cout << "alpha\tDisplay the alphabet." << '\n';
	cout << "trans\tDisplay all the transitions." << '\n';
	cout << "check\tCheck if sequence is accepted by the FA." << '\n';
	cout << "exit\tExit the application." << '\n';
	cout << '\n';
}

void Menu::parseCommand(string command) 
{
	auto c = this->toCommand.find(command);
	if (c == this->toCommand.end()) throw invalid_argument("Invalid command!\n");
	switch (c->second)
	{
	case Command::Help:
		this->displayMenu();
		break;
	case Command::Init:
		this->fa.displayInitialState();
		break;
	case Command::Final:
		this->fa.displayFinalStates();
		break;
	case Command::State:
		this->fa.displayStates();
		break;
	case Command::Alpha:
		this->fa.displayAlphabet();
		break;
	case Command::Trans:
		this->fa.displayTransitions();
		break;
	case Command::Check:
		this->checkSequence();
		break;
	case Command::Exit:
		throw runtime_error("exit");
		break;
	default:
		break;
	}
}

void Menu::checkSequence()
{
	cout << "Enter sequence: ";
	string sequence = "";
	cin >> sequence;

	if (this->fa.check(sequence)) cout << "true" << '\n';
	else cout << "false" << '\n';
}

void Menu::start() {
	this->displayMenu();

	while (true)
	{
		string command = "";
		cout << ">>";
		cin >> command;

		if (command == "exit")
			return;

		try
		{
			this->parseCommand(command);
		}
		catch (const invalid_argument& e)
		{
			cout << e.what();
		}
		catch (const runtime_error& e)
		{
			return;
		}
	}
}