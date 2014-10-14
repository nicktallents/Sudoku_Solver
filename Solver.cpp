#include "Solver.h"
using namespace std;

Solver::Solver(Sudoku* s)
{
	sudoku = s;
	elapsedTime = -1;
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
	int startTime = clock();
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

		for(int i=0;i<9;i++) {
			for(int j=0;j<9;j++) {
				if(sudoku->getGrid()[i+1][j+1] != 0) {
					possibleValues[i][j].clear();
					possibleValues[i][j].push_back(sudoku->getGrid()[i+1][j+1]);
				}
			}
		}
		for(int i=0;i<possibleValues.size(); i++) {
			for(int j=0;j<possibleValues[i].size();j++) {
				if(possibleValues[i][j].size() == 1) {
					sudoku->changeEntry(i+1,j+1,possibleValues[i][j][0]);
				}
			}
		}
	}
	

	//Force all initial constraints to assign values
	/*for(int i=0;i<possibleValues.size(); i++) {
		for(int j=0;j<possibleValues[i].size();j++) {
			if(possibleValues[i][j].size() == 1) {
				sudoku->changeEntry(i+1,j+1,possibleValues[i][j][0]);
			}
		}
	}*/
	sudoku->print(cout);
	getchar();
	
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
	elapsedTime = (clock() - startTime)/1000;
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
		int selected = sudoku->getGrid()[row][i+1];
		//If the element is non-zero
		if (selected != 0) {
			//For each other element in the row
			for (int k=0; k<9; k++) {
				if(k == i) continue;
				//Remove the value of the selected element from the domain of every other
				//element in the row.
					auto it = find(possibleValues[row-1][k].begin(), possibleValues[row-1][k].end(), selected);
					if(it != possibleValues[row-1][k].end()) {
						possibleValues[row-1][k].erase(it);
						change = true;
					}
					/*for(int j=0;j<possibleValues[row-1][k].size();j++) {
						if(possibleValues[row-1][k][j] == selected) {
							swap(possibleValues[row-1][k][j],possibleValues[row-1][k][possibleValues[row-1][k].size()-1]);
							possibleValues[row-1][k].pop_back();
							change = true;
							loop = true;
							break;
						}

					}*/
				}
	/*			for (auto j=possibleValues[row-1][k].begin(); j!=possibleValues[row-1][k].end(); j++) {
					if (*j == selected) {
						possibleValues[row-1][k].erase(j);
						change = true;
						break;
					}
				}*/
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
		int selected = sudoku->getGrid()[i+1][col];
		if (selected != 0) {
			//For each other element
			for (int k=0; k<9; k++) {
				if(k == i) continue;
				//Remove the value of the selected element from the domain of
				//each element in the column
				auto it = find(possibleValues[k][col-1].begin(), possibleValues[k][col-1].end(), selected);
				if(it != possibleValues[k][col-1].end()) {
					possibleValues[k][col-1].erase(it);
					change = true;
				}
			/*	loop = true;
				while(loop) {
					loop = false;
					for (int j=0; j<possibleValues[k][col-1].size(); j++) {
						if (possibleValues[k][col-1][j] == selected) {
							swap(possibleValues[k][col-1][j],possibleValues[k][col-1][possibleValues[k][col-1].size()-1]);
							possibleValues[k][col-1].pop_back();
							change = true;
							loop = true;
							break;
						}
					}
				}*/
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
		int selected = sudoku->getGrid()[squares[sq][i]][squares[sq][i+1]];
		if (selected != 0) {
			//If the slot has a value already
			for (int j=0; j<18; j+=2) {
				//For each slot in the square
				int tempRow = squares[sq][j]-1;
				int tempCol = squares[sq][j+1]-1;
				if(selected == sudoku->getGrid()[tempRow+1][tempCol+1]) continue;
					auto it = find(possibleValues[tempRow][tempCol].begin(), possibleValues[tempRow][tempCol].end(), selected);
					if( it != possibleValues[tempRow][tempCol].end()) {
						possibleValues[tempRow][tempCol].erase(it);
						change = true;
					}
					//}
					//for (int k=0; k< possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].size(); k++) {
					//	//For each possible value in the slot's domain
					//	if (selected == possibleValues[squares[sq][j]-1][squares[sq][j+1]-1][k]) {
					//		//If the value at the current non-zero slot equals a value in the domain of the slot in question
					//		swap(possibleValues[squares[sq][j]-1][squares[sq][j+1]-1][k], possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].at(possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].size()-1));
					//		//Move the value in the domain to the end of the vector
					//		possibleValues[squares[sq][j]-1][squares[sq][j+1]-1].pop_back();
					//		//Pop the value
					//		change = true;
					//	}
					//}
			}
		}
	}
	return change;
}