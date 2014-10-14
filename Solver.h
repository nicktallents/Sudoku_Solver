#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include "Sudoku.h"

struct Change
{
	int row;
	int column;
	int value;
};

class Solver
{
public:
	Solver(Sudoku* s);
	void solve();
	bool solved();
	void constrainRowDomains(int row);
	void constrainColumnDomains(int col);
	void constrainLocalBlock(int row, int col);
private:
	Sudoku* sudoku;
	std::vector<Change> changes;
	std::vector<std::vector<std::vector<int>>> possibleValues;
};

#endif