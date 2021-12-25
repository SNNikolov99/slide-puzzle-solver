#include "solver.h"
#include <algorithm>
#include <iostream>
#include <cassert>

solver::solver()
{
	movesCounter = 0;
}

std::vector<table> solver::getNeighbors(const table& curr)const
{
	std::vector<table> neighbors;
	// all the neighbors have the same board as the current one with the exemption of the place of the empty cell
	table left(curr);
	table right(curr);
	table up(curr);
	table down(curr);

	left.setEmptySpace(0, -1);
	right.setEmptySpace(0, +1);
	up.setEmptySpace(-1, 0);
	down.setEmptySpace(+1, 0);

	//how neighbor boards will be inserted in the vector depending on the place of the empty cell
	if (curr.getEmptySpaceIndex().first == 0 && curr.getEmptySpaceIndex().second == 0) { //upper left angle
		neighbors.push_back(right);
		neighbors.push_back(down);
	}
	else if (curr.getEmptySpaceIndex().first == 0 && curr.getEmptySpaceIndex().second == curr.getSize() - 1) { //upper right angle
		neighbors.push_back(left);
		neighbors.push_back(down);
	}
	else if (curr.getEmptySpaceIndex().first == curr.getSize() - 1 && curr.getEmptySpaceIndex().second == 0) { //lower left angle
		neighbors.push_back(right);
		neighbors.push_back(up);
	}
	else if (curr.getEmptySpaceIndex().first == curr.getSize() - 1 && curr.getEmptySpaceIndex().second == curr.getSize() - 1) { //lower right angle
		neighbors.push_back(left);
		neighbors.push_back(up);
	}
	else if (curr.getEmptySpaceIndex().second == 0) { // left foremost column
		neighbors.push_back(right);
		neighbors.push_back(down);
		neighbors.push_back(up);
	}
	else if (curr.getEmptySpaceIndex().second == curr.getSize() - 1) { //right foremost column
		neighbors.push_back(left);
		neighbors.push_back(down);
		neighbors.push_back(up);
	}
	else if (curr.getEmptySpaceIndex().first == 0) { //upper foremost row
		neighbors.push_back(left);
		neighbors.push_back(down);
		neighbors.push_back(right);
	}
	else if (curr.getEmptySpaceIndex().first == curr.getSize() - 1) { // lower foremost row
		neighbors.push_back(left);
		neighbors.push_back(up);
		neighbors.push_back(right);
	}
	else { // inside the borders of the board
		neighbors.push_back(left);
		neighbors.push_back(up);
		neighbors.push_back(right);
		neighbors.push_back(down);
	}

	return neighbors;
}


void solver::Astar(const table& startBoard)
{
	if (startBoard.isSolvable() != false) {
		std::vector<table> neighbors; // easier to read than  current.getNeighbors().Also,saves some time
		std::pair<table, table> currPrev = *new std::pair<table, table>(startBoard, *new table(0));
		pq.push_back(currPrev);
		table current(startBoard);


		while (!current.isGoal()) { 
			current = pq.front().first;

			neighbors = getNeighbors(current);
			pq.erase(pq.begin());
			movesCounter++;

			//push all neighbors in the PQ
			if (previousTables.size() == 0) {
				for (size_t i = 0; i < neighbors.size(); i++) {
					neighbors[i].setMoves(movesCounter);
					currPrev.first = neighbors[i];
					currPrev.second = current;
					pq.push_back(currPrev);
				}
			}
			else{
				for (size_t i = 0; i < neighbors.size(); i++) {
						if (neighbors[i].getMatrix()!= currPrev.second.getMatrix()) { // anti loop condition
							neighbors[i].setMoves(movesCounter);
							currPrev.first = neighbors[i];
							currPrev.second = current;
							pq.push_back(currPrev);
						}
				}
			}
			previousTables.push_back(current);
			
			//sorting by manhattan distance
			std::sort(pq.begin(), pq.end(), [](std::pair<table,table> a, std::pair<table, table> b) {
				return a.first.getManhattan() + a.first.getMoves() < b.first.getManhattan() + b.first.getMoves();
				});
			
			//delete all duplicates
			/*			std::unique(pq.begin(), pq.end(), [](std::pair<table, table> a, std::pair<table, table> b) {
				return a.first.getMatrix() == b.first.getMatrix();
				});
			*/

		}
	}
	else std::cerr << "The board is not solvable so ida* does not start";
}



//depth limited search with a heuristic threshold
void solver::ida(const table& startTable) {

}

void solver::output() {
	int diffX, diffY;
	
	if (previousTables.size() == 0) {
		std::cerr << std::endl << "There is no  decision path";
	}
	else if(previousTables.size() == 1){
		diffX = previousTables[0].getEmptySpaceIndex().first - previousTables[0].getSize() + 1;
		diffY = previousTables[0].getEmptySpaceIndex().second - previousTables[0].getSize() + 1;
		if (diffX == 0 && diffY == 1) {
			std::cout << "right\n";
		}
		if (diffX == 0 && diffY == -1) {
			std::cout << "left\n";
		}
		if (diffX == 1 && diffY == 0) {
			std::cout << "down\n";
		}
		if (diffX == -1 && diffY == 0) {
			std::cout << "up\n";
		}
	}
	else{
		table previous(previousTables[0].getSize());
		previous = previousTables[0];
		table current(previousTables[0].getSize());
		current = previousTables[1];

		for (int i = 1; i < previousTables.size(); i++) {

			current = previousTables[i];
			diffX = current.getEmptySpaceIndex().first - previous.getEmptySpaceIndex().first;
			diffY = current.getEmptySpaceIndex().second - previous.getEmptySpaceIndex().second;

			if (diffX == 0 && diffY == 1) {
				std::cout << "right\n";
			}
			if (diffX == 0 && diffY == -1) {
				std::cout << "left\n";
			}
			if (diffX == 1 && diffY == 0) {
				std::cout << "down\n";
			}
			if (diffX == -1 && diffY == 0) {
				std::cout << "up\n";
			}
			previous = current;

		}
	}
}

int solver::getMovesCounter() const
{
	return movesCounter;
}

solver::~solver()
{
}
