/*************************************
David Goehring, Nick Tallents
Comp 445 Artificial Intelligence
Sudoku Solver
*************************************/
#ifndef SOLVER_H
#define SOLVER_H

#include <vector>
#include <algorithm>
#include <ctime>
#include "Sudoku.h"

struct Change
{
	int row;
	int column;
	int value;
	vector<int> domain;
};

struct Pair
{
	int row;
	int column;
};

class Solver
{
public:
	Solver(Sudoku* s);
	void solve();
	bool solved();
	bool constrainRowDomains(int row);
	bool constrainColumnDomains(int col);
	bool constrainLocalBlock(int row, int col);

	void restoreDomains(Change ch);

	bool inconsistent();

	void backTrack(bool& root);
	void guess();

	float getElapsedTime() {return elapsedTime;}

private:
	Sudoku* sudoku;
	std::vector<Change> changes;
	std::vector<std::vector<std::vector<int>>> possibleValues;

	std::vector<Pair> chosenRoots;
	float elapsedTime;
};

#endif