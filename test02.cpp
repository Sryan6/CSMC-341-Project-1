// CMSC 341 - Fall 2019
// Project 1, Test 01

/* Test of the following MODIFIED 4x4 maze

      +--+--+--+--+
      |0 |4  8  12|
      +  +--+  +  +
      |1 |5  9 |13|
      +  +  +--+  +
      |2  6 |10 14|
      +  +--+--+--+
      |3   7|11 15|
      +--+--+--+--+

   There is no solution.
*/

// Compile with: g++ test02.cpp maze.cpp -o test02

#include <iostream>
#include "maze.h"

using namespace std;

int main() {

  Maze mz(16);
  
  mz.insert(MazeCell(0, 1));
  mz.insert(MazeCell(1, 0, 2));
  mz.insert(MazeCell(2, 1, 3, 6));
  mz.insert(MazeCell(3, 2, 7));

  mz.insert(MazeCell(4, 8));
  mz.insert(MazeCell(5, 6, 9));
  mz.insert(MazeCell(6, 2, 5));
  mz.insert(MazeCell(7, 3));

  mz.insert(MazeCell(8, 4, 9, 12));
  mz.insert(MazeCell(9, 5, 8));
  mz.insert(MazeCell(10, 14));
  mz.insert(MazeCell(11, 15));

  mz.insert(MazeCell(12, 8, 13));
  mz.insert(MazeCell(13, 12, 14));

  mz.insert(MazeCell(14, 13, 10));
  mz.insert(MazeCell(15, 11));

  mz.dump();

  auto path = mz.solve();

  for (auto x : path) {
    std::cout << x << std::endl;
  }
  
  return 0;
}
