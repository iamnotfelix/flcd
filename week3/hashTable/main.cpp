#include "HashTable.h"
#include "Scanner.h"

#include <iostream>

int main() 
{
	/*HashTable<string> table(30);

	
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

	tableInt.displayHash();*/

	Scanner scanner(
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\token.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\separator.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\operator.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\reservedWord.in"
	);
	try
	{
		scanner.scan("C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week1\\p1err.ili");
		scanner.displayPif();
	}
	catch (runtime_error e)
	{
		cout << e.what();
	}

	return 0;
}