#include "Solver.h"
using namespace std;

Solver::Solver(Sudoku* s)
{
	sudoku = s;
	vector<int> domain;
	vector<vector<int>> tempCols;
	for (int i=1; i<10; i++)
		domain.push_back(i);
	for (int j=0; j<9; j++)
		tempCols.push_back(domain);
	for (int k=0; k<9; k++)
		possibleValues.push_back(tempCols);
}

void Solver::solve()
{

	//Set domains of initial values to only the inital value
	for(int i=0;i<9;i++) {
		for(int j=0;j<9;j++) {
			if(sudoku->getGrid()[i+1][j+1] != 0) {
				possibleValues[i][j].clear();
				possibleValues[i][j].push_back(sudoku->getGrid()[i+1][j+1]);
			}
		}
	}

	//Constraint cascade
	bool change = true;
	while (change) {
		bool a,b,c;
		a = b = c = false;
		for (int i=1; i<10; i++)
			a = constrainRowDomains(i);
		for (int i=1; i<10; i++)
			b = constrainColumnDomains(i);
		for (int i=1; i<10; i+=3) {
			for (int j=1; j<10; j+=3) {
				if (constrainLocalBlock(i,j))
					c = true;
			}
		}
		change = (a || b || c);
	}
	
	while (!solved()) {

		//Check for inconsistencies in guesses
		bool root = false;
		while(inconsistent() && !root) {
			backTrack(root);
		}

		//Guess a new value
		guess();

		//Constraint cascade in response to guess
		change = true;
		while (change) {
			bool a,b,c;
			a = b = c = false;
			for (int i=1; i<10; i++)
				a = constrainRowDomains(i);
			for (int i=1; i<10; i++)
				b = constrainColumnDomains(i);
			for (int i=1; i<10; i+=3) {
				for (int j=1; j<10; j+=3) {
					if (constrainLocalBlock(i,j))
						c = true;
				}
			}
			change = (a || b || c);
		}
	}
	cout << "SOLVED" << endl;
	sudoku->print(cout);
}

bool Solver::solved()
{
	int count = 0;
	for (int i=1; i<10; i++) {
		if (sudoku->isColFull(i))
			count++;
	}
	if (count == 9)
		return true;
	return false;
}

bool Solver::constrainRowDomains(int row)
{
	bool change = false;
	//For each element in the row
	for (int i=0; i<9; i++) {
		int temp = sudoku->getGrid()[row][i+1];
		//If the element is non-zero
		if (sudoku->getGrid()[row][i+1] != 0) {
			//For each other element in the row
			for (int k=0; k<9; k++) {
				if(k == i) continue;
				//Remove the value of the selected element from the domain of every other
				//element in the row. 
				for (auto j=possibleValues[row-1][k].begin(); j!=possibleValues[row-1][k].end(); j++) {
					if (*j == sudoku->getGrid()[row][i+1]) {
						possibleValues[row-1][k].erase(j);
						change = true;
						break;
					}
				}
			}
		}
	}
	return change;
}

bool Solver::constrainColumnDomains(int col)
{
	bool change = false;
	//For each element in the column
	for (int i=0; i<9; i++) {
		//If the element is nonzero
		if (sudoku->getGrid()[i+1][col] != 0) {
			//For each other element
			for (int k=0; k<9; k++) {
				if(k == i) continue;
				//Remove the value of the selected element from teh domain of
				//each element in the column
				for (int j=0; j<possibleValues[k][col-1].size(); j++) {
					if (possibleValues[k][col-1][j] == sudoku->getGrid()[i+1][col]) {
						swap(possibleValues[k][col-1][j],possibleValues[k][col-1][possibleValues[k][col-1].size()-1]);
						possibleValues[k][col-1].pop_back();
						change = true;
						break;
					}
				}
			}
		}
	}
	return change;
}

bool Solver::constrainLocalBlock(int row, int col)
{
	bool change = false;
	int sq = sudoku->getSqNumber(row,col);
	vector<vector<int>>squares = sudoku->getSquares();
	//Square Indices
	for (int i=0; i<18; i+=2) {
		//For each slot in the square
		if (sudoku->getGrid()[squares[sq][i]][squares[sq][i+1]] != 0) {
			//If the slot has a value already
			for (int j=0; j<18; j+=2) {
				//For each slot in the square
				if(sudoku->getGrid()[squares[sq][i]][squares[sq][i+1]] == sudoku->getGrid()[squares[sq][j]][squares[sq][j+1]]) continue;
				for (int k=0; k< possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].size(); k++) {
					//For each possible value in the slot's domain
					if (sudoku->getGrid()[squares[sq][i]][squares[sq][i+1]] == possibleValues[squares[sq][j]-1][squares[sq][j+1]-1][k]) {
						//If the value at the current non-zero slot equals a value in the domain of the slot in question
						swap(possibleValues[squares[sq][j]-1][squares[sq][j+1]-1][k], possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].at(possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].size()-1));
						//Move the value in the domain to the end of the vector
						possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].pop_back();
						//Pop the value
						change = true;
						break;
					}
				}
			}
		}
	}
	return change;
}