#include <stdexcept>
#include "maze.h"

Maze::Maze(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::Maze(): ncell must be >= 1");
  }

  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

Maze::~Maze() {
  // IMPLEMENT DESTRUCTOR
  delete [] _maze;
}

void Maze::readFile(std::string fileName) {
  int numCells;
  int cell, n1, n2, n3, n4; 
  std::ifstream dataFile;
  
  dataFile.open(fileName);

  if (dataFile.is_open()) {
    dataFile >> numCells;
    this->reset(numCells);

    for (int i = 0; i < numCells; i++) {
      dataFile >> cell >> n1 >> n2 >> n3 >> n4;
      this->insert(cell_t(cell, n1, n2, n3, n4));
    }
  }
}

int Maze::getNcell() const {
  return _ncell;
}

void Maze::reset(int ncell) {
  if (ncell < 1) {
    throw std::invalid_argument("Maze::reset(): ncell must be >= 1");
  }

  if (_maze != nullptr) {
    delete [] _maze;
  }
  
  _ncell = ncell;
  _maze = new cell_t[_ncell];
}

void Maze::insert(cell_t cell) {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::insert(): attempt to insert into uninitialized Maze object");
  }
  if (cell.cellNum < 0 || cell.cellNum >= _ncell) {
    throw std::invalid_argument("Maze:insert(): invalid cell number");
  }
  _maze[cell.cellNum] = cell;
}
  
cell_t Maze::retrieve(int nc) const {
  if (_maze == nullptr) {
    throw std::domain_error("Maze::retrieve(): attempt to retrieve from an uninitialized Maze object");
  }
  if (nc < 0 || nc >= _ncell) {
    throw std::invalid_argument("Maze:retrieve(): invalid cell index");
  }
  
  return _maze[nc];
}

void Maze::dump() const {

  std::cout << "[*] Dumping the maze cells..." << std::endl;
  
  for (int i = 0; i < _ncell; i++) {
    int nc = _maze[i].cellNum;
    if (nc != i) {
      std::cerr << "Warning: maze cell at index " << i
		<<" has cellNum of " << nc << std::endl;
    }
    std::cout << "Cell " << i << " has neighbor cells: ";
    neighbor_t nbs = _maze[i].neighbors;
    int n0, n1, n2, n3;
    if ( nbs[0] >= 0 ) std::cout << nbs[0] << " ";
    if ( nbs[1] >= 0 ) std::cout << nbs[1] << " ";
    if ( nbs[2] >= 0 ) std::cout << nbs[2] << " ";
    if ( nbs[3] >= 0 ) std::cout << nbs[3] << " ";
    std::cout << std::endl;
  }
}

std::vector<int> Maze::solve() const {
  // IMPLEMENT THE SOLVE METHOD
  std::cout << "[*] Start of call to solve()..." << std::endl;
  SolveStack solveStack;
  int deadEnd = 0;
  std::vector <int> route;
  int placeholder;
  int counter = 0;

  //If the maze is empty, throw a domain error
  try{
    if(!_maze){
      throw(std::domain_error(""));
    }
  }
  catch(std::domain_error){
    std::vector<int> nullVector;
    std::cerr << "[-]Empty Maze. Cannot Solve" << std::endl;
    return nullVector;
  }
  
  //Deep copy the maze array
  cell_t *deepCopy = new cell_t[getNcell()];
  //Fill copy with cells from the maze
  for(int i = 0; i < getNcell(); i++){
    deepCopy[i] = _maze[i];
  }

  //places first entry on the stack
  if(solveStack.empty()){    
    entry_t start(VACANT, deepCopy[0].cellNum);
    solveStack.push(start.src_dest);
  }
  
  //while the maze is not solved
  while(solveStack.read().second != getNcell() - 1){

    //for each neighbor
    for(int i = 0; i < deepCopy[solveStack.read().second].neighbors.size(); i++){

      //if that neighbor is vacant, increment dead end counter
      if(deepCopy[solveStack.read().second].neighbors[i] == VACANT){
	deadEnd += 1;
	
      }
      //if the neighbor is not vacant, note the number
      else{
	  route.push_back(deepCopy[solveStack.read().second].neighbors[i]);
      }
    }
    
    //If there is no solution
    if(deadEnd == deepCopy[solveStack.read().second].neighbors.size()){
      std::cerr << "[-] There is no solution" << std::endl;
      std::vector<int> nullVector;
      delete []deepCopy;
      return nullVector;
    }
    
    //If the neighbors indicate a dead end, pop entry off the stack
    if(deadEnd == deepCopy[solveStack.read().second].neighbors.size() - 1){
      
      placeholder = deepCopy[solveStack.read().second].cellNum;
      solveStack.pop();
      for(int i = 0; i < deepCopy[solveStack.read().second].neighbors.size(); i++){
	if(deepCopy[solveStack.read().second].neighbors[i] != placeholder){
	  //empty because only the != operator was overloaded instead of the =
	}
	else{
	  deepCopy[solveStack.read().second].neighbors[i] = VACANT;
	  solveStack.pop();
	}
      }
    }
    //resets the dead end counter
    deadEnd = 0;
    
    //if the way ahead has a viable option, place the entry on the stack    
    for(int i = 0; i < route.size(); i++){
      if(route[i] != solveStack.read().first){
	entry_t next(solveStack.read().second, route[i]);
	solveStack.push(next.src_dest);
	break;
      }
    }
    //resets list of viable routes
    route.clear();
  }
  
  std::vector<int> path;
  std::vector<int> stackOutput;
  
  //Insert the stack into a vector
  while(solveStack.read().first != VACANT){
    stackOutput.push_back(solveStack.read().second);
    solveStack.pop();
  }
  stackOutput.push_back(solveStack.read().second);

  //Reverses the stack output in order to get a readable solution
  for(int i = stackOutput.size() - 1; i >= 0; i--){
    path.push_back(stackOutput[i]);
  }
  std::cout << "[+] Solution found!" << std::endl;
  delete [] deepCopy;
  return path;
}

Maze::SolveStack::SolveStack() {
  _stack = nullptr;
}

Maze::SolveStack::~SolveStack() {
  // IMPLEMENT DESTRUCTOR
  entry_t *pointer = _stack;
  entry_t *holder;
  while(pointer != nullptr){
    holder = pointer->next;
    delete pointer;
    pointer = holder; 
  }
}

bool Maze::SolveStack::empty() const {
  return _stack == nullptr;
}
				   
void Maze::SolveStack::push(src_dest_t src_dest) {
  // IMPLEMENT PUSH METHOD
  entry_t *newEntry = new entry_t (src_dest);
  newEntry->next = _stack;
  _stack = newEntry;
}

src_dest_t Maze::SolveStack::pop() {
  // IMPLEMENT POP METHOD
  src_dest_t returnValue = _stack->src_dest;
  entry_t * placeholder = _stack->next;
  try{
    if(_stack->next != nullptr){
      delete _stack;
      _stack = placeholder;
      return returnValue;
    }
    else{
      throw(std::domain_error(""));
    }
  }
  catch(std::domain_error){
    std::cerr << "Cannot pop from an empty stack" << std::endl;
    return returnValue;
  }
}

src_dest_t Maze::SolveStack::read() const {
  // IMPLEMENT READ METHOD
  try{
    if(_stack != nullptr){
      return _stack->src_dest;
    }
    else{
      throw(std::domain_error(""));
    }
  }
  catch(std::domain_error){
    std::cerr << "Cannot read from an empty stack" << std::endl;
    src_dest_t null_src_dest;
    return null_src_dest;
  }
}
