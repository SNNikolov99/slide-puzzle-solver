#include "table.h"
#include <random>
#include <algorithm>
#include <iostream>
#include <cassert>

struct coordComp {
	int num;
	int i;
	int j;
	coordComp() :num(), i(), j() {};
	coordComp(int _num, int _i, int _j) :num(_num), i(_i), j(_j) {};
};



table::table(const int& size)
{
	tableSize = size;
	sumManhattan = 0;
	state = "unchecked";
}

table::table(const int& size, const int& _index)
{
	tableSize = size;
	sumManhattan = 0;
	movesDone = 0;
	state = "unchecked";
	emptySpaceIndex.first = (_index / tableSize) % tableSize;
	emptySpaceIndex.second = _index % tableSize;
	int i,j;
	int number = 0;
	std::vector<int> listOfNumbers;

	for (i = 0; i < size * size; i++) {//put a sequence of numbers in a array
		listOfNumbers.push_back(number);
		number++;
	}

	std::srand(time(NULL));
	std::random_shuffle(listOfNumbers.begin(), listOfNumbers.end()); // shuffle the numbers

	for (i = 0; i < size * size; i++) {
		if (listOfNumbers[i] == 0) {
			if (_index != -1) {
				std::swap(listOfNumbers[i], listOfNumbers[_index]); // put the empty field 
			}
			else {
				std::swap(listOfNumbers[i], listOfNumbers[listOfNumbers.size() - size]); // put the empty field on the default place
			}
		}
		
	}
	number = 0;

	for (i = 0; i < size; i++) { // insert the numbers into the matrix
		std::vector<int> vec ;
		for (j = 0; j < size; j++) {
			vec.push_back(listOfNumbers[number]);
			number++;
		}
		matrix.push_back(vec);
	}
	/*
	for (i = 0; i < size; i++) { // insert the numbers into the matrix
		for (j = 0; j < size; j++) {
			matrix[i][j] = listOfNumbers[number];
		}
	}
	*/
	
	manhattan(); // calculate the sum of manhattan distances from each number to its desired location
}

table::table(std::vector<std::vector<int>> _matrix)
{
	this->matrix = _matrix;
	tableSize = _matrix.size();
	sumManhattan = 0;
	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			if (_matrix[i][j] == 0) {
				emptySpaceIndex.first = i;
				emptySpaceIndex.second = j;
			}
		}
	}
}

table::table(const table& other)
{
	int i,j;

	this->matrix = other.matrix;
	this->tableSize = other.tableSize;
	this->emptySpaceIndex = other.emptySpaceIndex;
	this->sumManhattan = other.sumManhattan;
	this->movesDone = other.movesDone;
}

table& table::operator=(const table& other)
{
	if (this != &other) {
		this->matrix = other.matrix;
		this->emptySpaceIndex = other.emptySpaceIndex;
		this->sumManhattan = other.sumManhattan;
		this->tableSize = other.tableSize;
		this->movesDone = other.movesDone;
	}
	return *this;
}

bool table::operator==(const table& other)
{
	return matrix == other.matrix && 
			emptySpaceIndex == other.emptySpaceIndex &&
			sumManhattan == other.sumManhattan &&
			tableSize == other.tableSize &&
			movesDone == other.movesDone;
}

bool table::operator!=(const table& other)
{
	return !this->operator==(other);
}

void table::manhattan()
{
	sumManhattan = 0;
	std::vector<coordComp> bufCoord; // right coords for numbers
	coordComp buffer;
	for (int i = 0; i < tableSize*tableSize - 1; i++) {
		buffer.num = i + 1 ;
		buffer.i = (i / tableSize) % tableSize;
		buffer.j = i % tableSize;
		bufCoord.push_back(buffer);
	}

	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			int num = matrix[i][j];
			if (num == 0) {
				//sumManhattan = sumManhattan + abs(i - (tableSize - 1) ) + abs(j - (tableSize - 1));
			}
			else {
				sumManhattan = sumManhattan + abs(i - bufCoord[num - 1].i) + abs(j - bufCoord[num - 1].j);
			}
		}
	}
}

bool table::isSolvable()const
{
	int sumInversionCounter = 0;
	int currentInversionCounter = 0;
	std::vector<int> copyArr;
	
	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			if (matrix[i][j] != 0) {
				copyArr.push_back(matrix[i][j]);
			}
		}
	}
	//calculate inversions 
	for (size_t i = 1; i < tableSize * tableSize; i++) {
		currentInversionCounter = count_if(find(copyArr.begin(), copyArr.end(), i), copyArr.end(), [i](int curr) {return curr < i; }); // how many numbers are are less than i after i
		sumInversionCounter += currentInversionCounter;
	}

	if (sumInversionCounter % 2 == 0 && tableSize % 2 == 1) {
		return true;
	}
	else if ((sumInversionCounter + emptySpaceIndex.first) % 2 == 1 && tableSize % 2 == 0) {
		return true;
	}
	else {
		return false;
	}

}

bool table::isGoal()
{
	//if the empty cell isn`t in the lower right angle return false
	if (matrix[tableSize - 1][tableSize - 1] != 0) {
		return false;
	}

	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			// check whether every number except 0 is on it`s place
			if (matrix[i][j] != i * tableSize + j + 1 && (i != tableSize - 1 && j != tableSize - 1)) {
				return false;
			}
		}
	}
	return true;
}

void table::setState(const std::string& newState)
{
	state = newState;
}

void table::setEmptySpace(const int& x,const int& y)
{
	int oldX, oldY;
	oldX = emptySpaceIndex.first;
	oldY = emptySpaceIndex.second;
	if (oldX + x >= 0 && oldX + x < tableSize && oldY + y >= 0 && oldY + y < tableSize) {
		emptySpaceIndex.first = oldX + x;
		emptySpaceIndex.second = oldY + y;
		std::swap(matrix[oldX][oldY], matrix[oldX + x][oldY + y]);
	}
	// reset the manhattan distanse and recalculate the sum again
	manhattan();
}

void table::setMoves(const size_t& moves)
{
	movesDone = moves;
}



int table::getSize() const
{
	return tableSize;
}

std::string table::getState() const
{
	return this->state;
}

std::vector<std::vector<int>> table::getMatrix() const
{
	return matrix;
}

int table::getManhattan() const
{
	return this->sumManhattan;
}

int table ::getMoves() const {
	return movesDone;
}

std::pair<int, int> table::getEmptySpaceIndex() const
{
	return emptySpaceIndex;
}

void table::outputMatrix() const
{
	for (int i = 0; i < tableSize; i++) {
		for (int j = 0; j < tableSize; j++) {
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
}

table::~table()
{
	
}
