#pragma once
#include "table.h"
#include "queue"

class solver
{
private:
	std::vector <std::pair<table, table >> pq; // vector will be used as a priority queue because the stl one isn`t abstract enough
	std::vector<table>previousTables; // vector containing all checked boards
	int movesCounter;
public:
	solver();
	void ida(const table&); // finds the optimal solution for the given board if it is solvable
	void output();//shows the way of the empty cell
	std::vector<table> getNeighbors(const table&)const; // get the neighbor boards of the given one
	void Astar (const table& );
	int getMovesCounter()const;
	~solver();
};

