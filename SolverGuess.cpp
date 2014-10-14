#include "Solver.h"
using namespace std;

bool Solver::inconsistent() {
	for(int i=0;i<possibleValues.size(); i++) {
		for(int j=0;j<possibleValues[i].size();j++) {
			if(possibleValues[i][j].size() == 0) {
				return true;
			}
		}
	}
	return false;
}

void Solver::guess() {
	int smallest = 10;
	int guessRow, guessCol;

	//if(changes.size() == 0) {
	//	bool skip = false;
	//	for(int i=0;i<possibleValues.size(); i++) {
	//		for(int j=0;j<possibleValues[i].size(); j++) {
	//			if(sudoku->getGrid()[i+1][j+1]==0) {
	//				for(int k=0;k<chosenRoots.size();k++) {
	//					if(chosenRoots[k].row == i && chosenRoots[k].column == j) {
	//						skip = true;
	//					}
	//				}
	//				if(possibleValues[i][j].size() < smallest && possibleValues[i][j].size() > 1 && !skip) {
	//					smallest = possibleValues[i][j].size();
	//					guessRow = i;
	//					guessCol = j;
	//				}
	//			}
	//		}
	//	}
	//	if(smallest == 10) {
	//		cout << "No solution" << endl;
	//		exit(1);
	//	}
	//}
	//else {
		//For each unchosen node, find the smallest domain
		for(int i=0;i<possibleValues.size();i++) {
			for(int j=0;j<possibleValues[i].size();j++) {
				if(sudoku->getGrid()[i+1][j+1] == 0)
					if(possibleValues[i][j].size() < smallest && possibleValues[i][j].size() > 1) {
						smallest = possibleValues[i][j].size();
						guessRow = i;
						guessCol = j;
					}
			}
		}
//}

	Change ch;
	ch.row = guessRow;
	ch.column = guessCol;
	ch.value = possibleValues[guessRow][guessCol][possibleValues[guessRow][guessCol].size()-1];
	ch.domain = possibleValues[guessRow][guessCol];

	//Push on the new change object that stores the location, value, and current domain stack
	changes.push_back(ch);

	//Reassign the domain to only be the current value of the node
	possibleValues[guessRow][guessCol].clear();
	possibleValues[guessRow][guessCol].push_back(ch.value);
	sudoku->changeEntry(ch.row+1, ch.column+1, ch.value);

}


void Solver::backTrack(bool& root) {
	if(changes.size() == 0) {
		cout << "No solution" << endl;
		exit(1);
	}

	//if(changes.size() == 0) {
	//	root = true;
	//	return;
	//}
	Change ch = changes[changes.size()-1];
	changes.pop_back();

	//Revert the change
	restoreDomains(ch);

}

void Solver::restoreDomains(Change ch) {

	//Replace the domain in each node of the column
	for(int i=0;i<possibleValues[ch.row].size();i++) {
		possibleValues[ch.row][i].push_back(ch.value);
	}

	//Replace the domain in each node of the row
	for(int i=0;i<possibleValues.size(); i++) {
		possibleValues[i][ch.column].push_back(ch.value);
	}
	                 
	//Replace the domain in each node of the square, skipping those in the row/column
	vector<vector<int>> squares = sudoku->getSquares();
	int sq = sudoku->getSqNumber(ch.row+1, ch.column+1);
	for(int i=0;i<18; i+=2) {
		int tempCol = squares[sq][i];
		int tempRow = squares[sq][i+1];
		bool found = false;
		for(int j=0;j<possibleValues[tempRow-1][tempCol-1].size();j++) {
			if(possibleValues[tempRow-1][tempCol-1][j] == ch.value) {
				found = true;
			}
		}
		if(!found) {
			possibleValues[tempRow-1][tempCol-1].push_back(ch.value);
		}
	}

	//Restore the domain stack of the changed node, and remove the changed value from the domain
	possibleValues[ch.row][ch.column].clear();
	possibleValues[ch.row][ch.column] = ch.domain;
	for(int i = 0; i < possibleValues[ch.row][ch.column].size(); i++) {
		if(possibleValues[ch.row][ch.column][i] == ch.value) {
			swap(possibleValues[ch.row][ch.column][i], possibleValues[ch.row][ch.column][possibleValues[ch.row][ch.column].size()-1]);
			possibleValues[ch.row][ch.column].pop_back();
		}
	}
}