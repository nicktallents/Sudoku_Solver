#include <iostream>
#include <vector>
using std::vector;

#ifndef SUDOKU_H
#define SUDOKU_H

const int PRINT_START = 1; // Index to start printing, 0 to include indices that show full rows or columns
const int SQ0[18] = {1,1, 1,2, 1,3, 2,1, 2,2, 2,3, 3,1, 3,2, 3,3};	// Sets of (row,column) pairs
const int SQ1[18] = {1,4, 1,5, 1,6, 2,4, 2,5, 2,6, 3,4, 3,5, 3,6};	// ___________________
const int SQ2[18] = {1,7, 1,8, 1,9, 2,7, 2,8, 2,9, 3,7, 3,8, 3,9};	// | SQ0 | SQ1 | SQ2 |
const int SQ3[18] = {4,1, 4,2, 4,3, 5,1, 5,2, 5,3, 6,1, 6,2, 6,3};	// |-----|-----|-----|
const int SQ4[18] = {4,4, 4,5, 4,6, 5,4, 5,5, 5,6, 6,4, 6,5, 6,6};	// | SQ3 | SQ4 | SQ5 |
const int SQ5[18] = {4,7, 4,8, 4,9, 5,7, 5,8, 5,9, 6,7, 6,8, 6,9};	// |-----|-----|-----|
const int SQ6[18] = {7,1, 7,2, 7,3, 8,1, 8,2, 8,3, 9,1, 9,2, 9,3};	// | SQ6 | SQ7 | SQ8 |
const int SQ7[18] = {7,4, 7,5, 7,6, 8,4, 8,5, 8,6, 9,4, 9,5, 9,6};	// |_____|_____|_____|
const int SQ8[18] = {7,7, 7,8, 7,9, 8,7, 8,8, 8,9, 9,7, 9,8, 9,9};	//

class Sudoku
{
public:
	Sudoku(std::istream& in);

	int** getGrid(); // Shallow copy of grid
	void getGrid(int** val); // Accessor function for grid. val must be initialized as a double bounded array [10][10]
	void print(std::ostream& out);
	void changeEntry(int row, int col, int val); // row, col, and val must be 0-9
	bool isRowFull(int row); // row must be 0-9
	bool isColFull(int col); // col must be 0-9
	std::vector<std::vector<int> > getSquares() {return squares;}
	
	int  getSqNumber(int row, int col); // returns the index of the 3x3 square you're in given the coordinates
	bool isValidEntry(int row, int col, int val); // row, col, and val must be 0-9

	class INVALID_ROW_PARAMETER {};
	class INVALID_COL_PARAMETER {};
	class INVALID_VAL_PARAMETER {};
private:
	int** grid; // Double bounded array. Index 0 determines whether or not a row/column is filled.


	void fillSquares(); // populates squares vector
	std::vector<std::vector<int> >squares;
};

#endif