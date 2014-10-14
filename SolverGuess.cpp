#include "Solver.h"
using namespace std;

bool Solver::inconsistent() {
	for(int i=0;i<possibleValues.size(); i++) {
		for(int j=0;j<possibleValues[i].size();j++) {
			if(possibleValues[i][j].size() == 0) {
				return false;
			}
		}
	}
	return true;
}

void Solver::guess() {
	int smallest = 10;
	int guessRow, guessCol;
	for(int i=0;i<possibleValues.size();i++) {
		for(int j=0;j<possibleValues[i].size();j++) {
			if(possibleValues[i][j].size() < smallest) {
				smallest = possibleValues[i][j].size();
				guessRow = i;
				guessCol = j;
			}
		}
	}

	Change ch;
	ch.row = guessRow;
	ch.column = guessCol;
	ch.value = possibleValues[guessRow][guessCol][possibleValues[guessRow][guessCol].size()-1];
	changes.push_back(ch);
	possibleValues[guessRow][guessCol].pop_back();
	sudoku->changeEntry(ch.row, ch.column, ch.value);

}


void Solver::backTrack() {
	Change ch = changes[changes.size()];
	changes.pop_back();

	restoreDomains(ch);

	//Pop 3 times because the domain is restored 3 times. 
	possibleValues[ch.row][ch.column].pop_back();
	possibleValues[ch.row][ch.column].pop_back();
	possibleValues[ch.row][ch.column].pop_back();
}

void Solver::restoreDomains(Change ch) {
	for(int i=0;i<possibleValues[ch.row].size();i++) {
		possibleValues[ch.row][i].push_back(ch.value);
	}
	for(int i=0;i<possibleValues.size(); i++) {
		possibleValues[i][ch.column].push_back(ch.value);
	}
	                 
	vector<vector<int>> squares = sudoku->getSquares();
	int sq = sudoku->getSqNumber(ch.row, ch.column);
	for(int i=0;i<18; i+=2) {
		int tempCol = squares[sq][i];
		int tempRow = squares[sq][i+1];
		possibleValues[tempRow][tempCol].push_back(ch.value);
	}
}