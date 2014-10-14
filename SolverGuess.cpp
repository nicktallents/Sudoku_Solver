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


void Solver::backTrack() {
	Change ch = changes[changes.size()];
	changes.pop_back();

	restoreDomains(ch);
}

void Solver::restoreDomains(Change ch) {
	for(int i=0;i<possibleValues[ch.row].size();i++) {
		possibleValues[ch.row][i].push_back(ch.value);
	}
	for(int i=0;i<possibleValues.size(); i++) {
		possibleValues[i][ch.column].push_back(ch.value);
	}
	                              
	for(int i=0;i<possibleValues.size(); i++) {

	}
}