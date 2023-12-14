#include "Parser.h"

State Parser::closure(vector<Item> items)
{
    // create current closure (we need 'items' for the return)
    vector<Item> currentClosure = items;

    // closure till you can no more
    bool done = false;
    while (!done)
    {
        // save a copy of the closure for comparison (so you know when to stop)
        vector<Item> oldClosure = currentClosure;
        for (int i = 0; i < currentClosure.size(); ++i)
        {
            auto closureItem = currentClosure[i];
            // if item in closure has dot before non-terminal, expand closure
            if (closureItem.dotPosition < closureItem.rhs.size()
                && this->grammar.nonterminals.find(closureItem.rhs[closureItem.dotPosition])
                != this->grammar.nonterminals.end())
            {
                // go over all the productions of that non-terminal
                string afterDot = closureItem.rhs[closureItem.dotPosition];
                for (auto production : this->grammar.productions[afterDot])
                {
                    // check if item is already in closure
                    Item newItem{ afterDot, production, 0 };
                    bool isItemInCurrentClosure = false;
                    for (Item itemInCurrentClosure : currentClosure)
                        if (itemInCurrentClosure == newItem)
                        {
                            isItemInCurrentClosure = true;
                            break;
                        }
                    
                    // add it if not already in
                    if (!isItemInCurrentClosure) currentClosure.push_back(newItem);
                }
            }
        }
        // stop if closure doesn't modify anymore
        if (currentClosure == oldClosure) done = true;
    }

    return State{items, currentClosure};
}


State Parser::goTo(State state, string atom)
{
    vector<Item> goToItems;

    // go over all items from the states closure
    for (auto item : state.closure)
        // pick the ones that have the atom after the dot
        if (item.dotPosition < item.rhs.size() && item.rhs[item.dotPosition] == atom)
            goToItems.push_back(Item{ item.lhs, item.rhs, item.dotPosition + 1 });

    // check if state already exists, return it if it does
    for (auto otherState : this->cannonicalCollection)
        if (otherState.hasSameClosure(goToItems)) return otherState;

    // find closure if not, and return that new state
    return this->closure(goToItems);
}

void Parser::createCanonicalCollection()
{
    // get closure of first state (which needs to be augmented)
    State firstState = this->closure({ 
        Item{ this->grammar.start, this->grammar.productions[this->grammar.start][0], 0 } 
    });

    // add it to the cannonical collection
    this->cannonicalCollection.push_back(firstState);

    int index = 0;

    // go over all the states
    while (index < this->cannonicalCollection.size())
    {
        // try finding new states using goTo
        auto state = this->cannonicalCollection[index];
        vector<string> atomsAfterDot = state.getAtomAfterDot();
        for (string atom : atomsAfterDot)
        {
            State newState = this->goTo(state, atom);
            // check that state is not already existent and add it
            if (find(this->cannonicalCollection.begin(), this->cannonicalCollection.end(), newState)
                == this->cannonicalCollection.end())
                this->cannonicalCollection.push_back(newState);
            
            // create connection between current state and next state through atom
            this->connections[this->getStateIndex(state)].push_back({ atom, this->getStateIndex(newState) });
        }
        ++index;
    }
}

void Parser::createParsingTable()
{
    // parsing table has as many rows as states in cannonical collection
    this->parsingTable = vector<TableRow>(this->cannonicalCollection.size());
    string startingAtom = this->grammar.start;

    // go over all states in the cannonical collection
    for (int i = 0; i < this->cannonicalCollection.size(); ++i)
    {
        // currenct state
        State s = this->cannonicalCollection[i];

        // action of the current state
        Action action = s.getAction(startingAtom);

        // all connections of the current state
        auto stateConnections = this->connections[i];
        if (stateConnections.size() == 0)
        {
            if (action == Action::Accept)
            {
                // mark the state as accept in the parsing table
                this->parsingTable[i] = { Action::Accept, unordered_map<string, int> {} };
            }
            else if (action == Action::Reduce)
            {
                // add the production corresponding to the reduce action
                this->parsingTable[i] = { Action::Reduce, s.closure[0] };
            }
        }
        else if (action == Action::Shift)
        {
            unordered_map<string, int> gotos;
            // add all the connections of the current state in the table (only on shift action)
            for (auto [atom, nextState] : stateConnections)
                gotos[atom] = nextState;

            this->parsingTable[i] = { action, gotos };
        }
        else if (action == Action::ShiftReduceConflict)
        {
            throw string("Shift-reduce conflict for state: " + s.toString(i));
        }
        else if (action == Action::ReduceReduceConflict)
        {
            throw string("Reduce-reduce conflict for state: " + s.toString(i));
        }
    }
}

vector<Item> Parser::parse(vector<string> atoms)
{
    string endSign = "$";

    // preparing stacks
    stack<string> work;
    stack<string> input;
    list<Item> output;

    // adding endSign and starting state in work stack
    work.push(endSign);
    work.push("0");

    // adding endSign and all the atoms in reverse order(so first atom is on top) in input stack
    input.push(endSign);
    for (auto it = atoms.rbegin(); it != atoms.rend(); ++it) input.push(*it);

    // we are done when accept action is reached (finished == true)
    bool finished = false;
    while (!finished && (work.top() != endSign || input.top() != endSign))
    {
        // get top state, which is a string that represents the index of the state
        int topState = stoi(work.top());

        // accept case, if reached we are done
        if (this->parsingTable[topState].action == Action::Accept)
        {
            finished = true;
        }
        // shift case
        else if (this->parsingTable[topState].action == Action::Shift)
        {
            string atom = input.top();
            input.pop();

            // push the top of input stack
            work.push(atom);

            // check that the parsing table at the current state has something at the atom(top of input stack) 
            if (this->parsingTable[topState].gotos.find(atom) == this->parsingTable[topState].gotos.end())
                throw string("Invalid goto for state " + to_string(topState) + " through atom " + atom + ".");

            // push the state connected by the atom
            work.push(to_string(this->parsingTable[topState].gotos[atom]));
        }
        // reduce case
        else if (this->parsingTable[topState].action == Action::Reduce)
        {
            // add the production corresponding to the reduce action in the output
            output.push_front(this->parsingTable[topState].production);
            
            // pop the work stack 2 * prodLength times (the size of the rhs of the corresponding reduce production)
            int prodLength = this->parsingTable[topState].production.rhs.size();
            while (prodLength)
            {
                // two pops because we need 2 * prodLength
                // two pops because atom is always followed by a state in work stack
                work.pop();
                work.pop();
                --prodLength;
            }

            int currentTopState = stoi(work.top());
            string newTopState = this->parsingTable[topState].production.lhs;

            // push lhs of the production corresponding to reduce state
            work.push(newTopState);

            // check that the parsing table at the current state has something at the newTopState(top of input stack) 
            if (this->parsingTable[currentTopState].gotos.find(newTopState) == this->parsingTable[currentTopState].gotos.end())
                throw string("Invalid goto for state " + to_string(topState) + " through atom " + newTopState + ".");

            // push the state connected by newTopState(= which is not actually a state, it is an atom)
            work.push(to_string(this->parsingTable[currentTopState].gotos[newTopState]));
        }
    }

    return vector<Item>(output.begin(), output.end());
}

int Parser::getStateIndex(const State& state) const
{
    for (int i = 0; i < this->cannonicalCollection.size(); ++i)
        if (this->cannonicalCollection[i] == state) return i;
    return -1;
}

string Parser::toString()
{
    string cannonicalString = "";
    for (int i = 0; i < this->cannonicalCollection.size(); ++i)
    {
        cannonicalString += this->cannonicalCollection[i].toString(i) + "\n";
    }

    return cannonicalString;
}


// logic for shift reduce ???
/*else if (this->parsingTable[topState].action == Action::ShiftReduceConflict)
{
    string atom = input.top();
    input.pop();

    if (atom == endSign
        || this->parsingTable[topState].gotos.find(atom) == this->parsingTable[topState].gotos.end())
    {
        output.push_front(this->parsingTable[topState].production);

        int prodLength = this->parsingTable[topState].production.rhs.size();
        while (prodLength)
        {
            work.pop();
            work.pop();
            --prodLength;
        }

        int currentTopState = stoi(work.top());
        string newTopState = this->parsingTable[topState].production.lhs;
        work.push(newTopState);
        work.push(to_string(this->parsingTable[currentTopState].gotos[newTopState]));
    }
    else
    {
        string atom = input.top();
        work.push(atom);

        if (this->parsingTable[topState].gotos.find(atom) == this->parsingTable[topState].gotos.end())
        {
            string msg = "Invalid goto for state " + to_string(topState) + " through atom " + atom + ".";
            throw msg;
        }

        work.push(to_string(this->parsingTable[topState].gotos[atom]));
    }
}*/
