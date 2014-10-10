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
	return 0;
}

