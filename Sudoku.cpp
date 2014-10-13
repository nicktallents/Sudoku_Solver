#include "Sudoku.h"
using namespace std;

Sudoku::Sudoku(std::istream& in)
{
	grid = new int*[10];
	for (int i=0; i<10; i++)
		grid[i] = new int[10];
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			if (i == 0 || j == 0)
				grid[i][j] = 0;
			else
				in >> grid[i][j];
		}
	}
	grid[0][0] = 1;

	fillSquares();
}

void Sudoku::getGrid(int** val)
{
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++)
			val[i][j] = grid[i][j];
	}
}

void Sudoku::print(std::ostream& out)
{
	for (int i=PRINT_START; i<10; i++) {
		for (int j=PRINT_START; j<10; j++) {
			out << grid[i][j];
			if (j != 9)
				out << " ";
		}
		out << std::endl;
	}
}

void Sudoku::changeEntry(int row, int col, int val)
{
	if (row < 0 || row > 9)
		throw (new INVALID_ROW_PARAMETER());
	if (col < 0 || col > 9)
		throw (new INVALID_COL_PARAMETER());
	if (val < 0 || col > 9)
		throw (new INVALID_VAL_PARAMETER());
	grid[row][col] = val;
}

bool Sudoku::isRowFull(int row)
{
	if (row < 0 || row > 9)
		throw (new INVALID_ROW_PARAMETER());
	
	if (grid[row][0] == 1) return true;
	for (int i=1; i<10; i++)
		if (grid[row][i] == 0)
			return false;
	return true;
}

bool Sudoku::isColFull(int col)
{
	if (col < 0 || col > 9)
		throw (new INVALID_COL_PARAMETER());

	if (grid[col][0] == 1) return true;
	for (int i=1; i<10; i++)
		if (grid[i][col] == 0)
			return false;
	return true;
}

int Sudoku::getSqNumber(int row, int col)
{
	if (row < 1 || row > 9)
		throw (new INVALID_ROW_PARAMETER());
	if (col < 1 || col > 9)
		throw (new INVALID_COL_PARAMETER());

	int r, c; // Determine relative square coordinates
	if (row < 4) r = 1;
	else if (row > 6) r = 3;
	else r = 2;
	if (col < 4) c = 1;
	else if (col > 6) c = 3;
	else c = 2;

	switch (4*r + 3) {
	case 5:
		return 1;
	case 6:
		return 2;
	case 7:
		return 3;
	case 9:
		return 4;
	case 10:
		return 5;
	case 11:
		return 6;
	case 13:
		return 7;
	case 14:
		return 8;
	case 15:
		return 9;
	}
	return -1;
}

bool Sudoku::isValidEntry(int row, int col, int val)
{
	if (row < 0 || row > 9)
		throw (new INVALID_ROW_PARAMETER());
	if (col < 0 || col > 9)
		throw (new INVALID_COL_PARAMETER());
	if (val < 0 || col > 9)
		throw (new INVALID_VAL_PARAMETER());

	int square = getSqNumber(row,col);

	for (int i=1; i<10; i++) {
		if (grid[row][i] == val || grid[i][col] == val)
			return false;
		if (grid[squares[square][i-1]][squares[square][i]] == val)
			return false;
	}	

	return true;
}

void Sudoku::fillSquares()
{
	for (int i=0; i<9; i++)
		squares.push_back(vector<int>());
	for (int i=0; i<18; i++) {
		squares[0].push_back(SQ1[i]);
		squares[1].push_back(SQ2[i]);
		squares[2].push_back(SQ3[i]);
		squares[3].push_back(SQ4[i]);
		squares[4].push_back(SQ5[i]);
		squares[5].push_back(SQ6[i]);
		squares[6].push_back(SQ7[i]);
		squares[7].push_back(SQ8[i]);
		squares[8].push_back(SQ9[i]);
	}
}