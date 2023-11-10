#include "HashTable.h"
#include "Scanner.h"
#include "FiniteAutomata.h"
#include "Menu.h"

#include <iostream>

void testScanner()
{
	FiniteAutomata faIntConst{ "C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week6\\fa_int_const.in" };
	FiniteAutomata faIdentifier{ "C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week6\\fa_identifier.in" };

	Scanner scanner(
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\token.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\separator.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\operator.in",
		"C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week2\\reservedWord.in",
		faIntConst,
		faIdentifier
	);
	try
	{
		scanner.scan("C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week1\\p1.ili");
		//scanner.displayPif();
		scanner.dumpToFile();
	}
	catch (runtime_error e)
	{
		cout << e.what();
	}
}

void testFA()
{
	FiniteAutomata faIntConst{ "C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week6\\fa_int_const.in" };
	cout << faIntConst.check("12") << endl;
	cout << faIntConst.check("-12") << endl;
	cout << faIntConst.check("++12") << endl;
	cout << faIntConst.check("0+12") << endl;
	cout << faIntConst.check("-0") << endl;
	cout << faIntConst.check("-a") << endl;
	cout << faIntConst.check("aasdf") << endl;
	cout << faIntConst.check("+aasdf") << endl;
	cout << faIntConst.check("0") << endl;
	cout << faIntConst.check("1231231231") << endl;
	//fa.displayFA();

	FiniteAutomata faIdentifier{ "C:\\Users\\andre\\Desktop\\folders\\facultate\\sem_5\\compilers\\week6\\fa_identifier.in" };
	cout << faIdentifier.check("___") << endl;
	cout << faIdentifier.check("12") << endl;
	cout << faIdentifier.check("_asdf3_") << endl;
	cout << faIdentifier.check("__aAdf3_") << endl;
	cout << faIdentifier.check("_333__") << endl;
	cout << faIdentifier.check("__a_") << endl;
	cout << faIdentifier.check("adf4___") << endl;
	cout << faIdentifier.check("ABFdf4_S__") << endl;
	cout << faIdentifier.check("4___") << endl;
	//faIdentifier.displayFA();

	Menu menu(faIntConst);
	menu.start();
}

int main() 
{
	testScanner();
	//testFA();

	return 0;
}