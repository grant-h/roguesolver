#include <stdio.h>

int getTotalKeys();
int getCurrentKeys();
int getNumSteps(); // returns the number of steps which you have made so far. Might be useful for debugging purposes.

void getCurrentPosition(int * arr); // this puts the player's row into arr[0], and the player's column into arr[1].

void readMaze(); // points will be taken off if you use this.

void printMaze(); // prints the current maze. Might be useful for debugging.

int isComplete(); //returns 1 if the maze is complete, and 0 otherwise. Useful for loop conditions

int move(int newRow, int newCol); // if possible, it moves the player to the position specified. If not possible, it prints a handy error message explaining what happened. You are definitely allowed to comment out the print statements if you get tired of seeing the error messages.

int look(int newRow, int newCol); // if possible, it tells you what's in the square you are looking at. It also returns the integer value of what is in the square (see maze file explanation). If not possible, it prints a handy error message explaining what happened. You are definitely allowed to comment out the print statements if you get tired of seeing the error messages.
