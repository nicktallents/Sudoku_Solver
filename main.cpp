#include <iostream>
#include <fstream>
#include "Sudoku.h"
using namespace std;

int main(int argc, char **argv)
{
	ifstream fin("input.txt");
	Sudoku puzzle(fin);

	puzzle.print(cout);
	cout << endl;

	if (puzzle.isValidEntry(1,1,1))
		cout << "Is a valid entry" << endl;
	else
		cout << "Is not a valid entry" << endl;

	cout << endl;
	return 0;
}

