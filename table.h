#pragma once
#include<vector>
#include<string>
class table
{
private:
	int tableSize; // the size of the board
	std::vector<std::vector<int>> matrix; // the numbers on the board in a certain order
	int sumManhattan;
	int movesDone;
	std::pair<int,int> emptySpaceIndex; // the current place of the empty cell
	std::string state; // can be checked,unchecked,checking.Will be used in IDA*
public:
	table(const int&); // construct an empty board
	table(const int&, const int&); // constructs the board randomly
	table(std::vector<std::vector<int>>); // for testing purposes
	table(const table&);	//copy constructor
	table& operator = (const table&); // assign a board
	bool operator ==(const table&);	  //check if equal
	bool operator !=(const table&);
	void manhattan(); // calculates the sum of the distance needed for each element to get to his place via manhattan 
	bool isSolvable()const; // checks whether a puzzle is solvable
	bool isGoal(); // check whether the board is the desired one
	void setState(const std::string&); // set a new state for the table
	void setEmptySpace(const int&,const int&);
	void setMoves(const size_t&); //now being tested
	int getSize()const;
	std::string getState()const;
	std::vector<std::vector<int>> getMatrix() const;
	int getManhattan()const;
	int getMoves()const;
	std::pair<int, int> getEmptySpaceIndex() const;
	void outputMatrix()const;
	~table();

};

