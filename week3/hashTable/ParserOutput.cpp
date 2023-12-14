#include "ParserOutput.h"

void ParserOutput::createParsingTree()
{
	int position = 0;
	// add first node, which has as starting atom the original start, not the enriched one
	this->parsingTree.push_back({ position++, this->grammar.productions[this->grammar.start][0][0], -1, -1 });

	// go over all the items in the output
	for (Item item : this->output)
	{
		for (Node node : this->parsingTree)
		{
			auto hasChildren = [&](const int& id) {
				for (Node n : this->parsingTree)
					if (n.father == id) return true;
				return false;
				};

			// if node has no children yet and node is the same as lhs of a production from output
			if (!hasChildren(node.id) && node.atom == item.lhs)
			{
				bool first = true;
				for (string atom : item.rhs)
				{
					if (first)
					{
						// add first atom from production
						this->parsingTree.push_back({ position++, atom, node.id, -1 });
						first = false;
					}
					else {
						// set sibling when it is not the first one in production
						this->parsingTree[position - 1].sibling = position;

						// add node
						this->parsingTree.push_back({ position++, atom, node.id, -1 });
					}
				}
				break;
			}
		}
	}
}

void ParserOutput::display()
{
	for (Node node : this->parsingTree)
		cout << node.toString() << endl;
}

void ParserOutput::saveToFile(string file)
{
	ofstream out(file);

	for (Node node : this->parsingTree)
		out << node.toString() << "\n";
}