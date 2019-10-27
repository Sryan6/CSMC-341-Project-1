// CMSC 341 - Fall 2019
// Project 1, Test 01

/* Test of the following 4x4 maze

      +--+--+--+--+
      |0 |4  8  12|
      +  +--+  +  +
      |1 |5  9 |13|
      +  +  +--+  +
      |2  6 |10|14|
      +  +--+  +  +
      |3   7 11|15|
      +--+--+--+--+

   The solution is 0 1 2 6 5 9 8 12 13 14 15
*/

// Compile with: g++ test01.cpp maze.cpp -o test01

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
  mz.insert(MazeCell(7, 3, 11));

  mz.insert(MazeCell(8, 4, 9, 12));
  mz.insert(MazeCell(9, 5, 8));
  mz.insert(MazeCell(10, 11));
  mz.insert(MazeCell(11, 7, 10));

  mz.insert(MazeCell(12, 8, 13));
  mz.insert(MazeCell(13, 12, 14));

  mz.insert(MazeCell(14, 13, 15));
  mz.insert(MazeCell(15, 14));

  mz.dump();

  auto path = mz.solve();

  for (auto x : path) {
    std::cout << x << std::endl;
  }
  
  return 0;
}
