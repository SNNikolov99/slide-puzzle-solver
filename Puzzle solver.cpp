// Puzzle solver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include<vector>
#include "table.h"
#include "solver.h"

int main()
{
    int size, index;
    std::cout << "Enter the size of the table: ";
    std::cin >> size;
    std::cout << "Enter the starting index of the empty space: ";
    std::cin >> index;

    table a(size,index);
    a.outputMatrix();
    std::cout<<std::endl;
    if (a.isSolvable() == true) {
        std::cout << "Solvable";
    }
    else {
        std::cout << "Not solvable";
    }
    std::cout << "\n";
    std::cout<<a.getManhattan();
    std::cout << std::endl;
    solver s;
    std::vector<table>neigh = s.getNeighbors(a);


  s.Astar(a);
  s.getMovesCounter();
  s.output();
}

