// CMSC 341 - Fall 2019
// Project 1, Test 00

// Test of the sample maze from the project description.
// The maze is encoded in the file data5x5.txt

// Compile with: g++ test00.cpp maze.cpp -o test00

#include <iostream>
#include "maze.h"

using namespace std;

int main() {

  Maze mz;

  mz.readFile("data5x5.txt");
  
  mz.dump();

  auto path = mz.solve();

  for (auto x : path) {
    std::cout << x << std::endl;
  }
  
  return 0;
}
