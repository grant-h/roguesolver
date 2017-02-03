#include <stdio.h>
#include "Maze.h"
#include "MazeSolver.h"

extern int getTotalKeys();
extern int getCurrentKeys();
extern int getNumSteps();

extern void getCurrentPosition(int * arr);

extern void readMaze();

extern void printMaze();

extern int isComplete();

extern int move(int newRow, int newCol);

extern int look(int newRow, int newCol);

int main(int argc, char *argv[]){
  //error handling
  if(argc != 2){
    printf("This program requires the input filename as an argument.\n");
    return -1;
  }

  readMaze(argv[1]);

  solveMaze();
  printMaze();
  
  if(isComplete()){
    printf("You solved the maze in %d steps!\n", getNumSteps());
  }

  return 0;
}
