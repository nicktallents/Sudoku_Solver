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

	int r = 1;
	int c = 1;
	int v = 1;

	while (v != -1) {
		if (puzzle.isValidEntry(r,c,v))
			cout << v << " Is a valid entry at " << r << "," << c << endl;
		else
			cout << v << " Is not a valid entry at " << r << "," << c << endl;
		cout << "r: ";
		cin >> r;
		cout << "c: ";
		cin >> c;
		cout << "v: ";
		cin >> v;
	}

	cout << endl;
	return 0;
}

