#include "Maze.h"

static int mazeStructure[500][500] = { 0 };
static int numTotalKeys = 0;
static int numKeysHeld = 0;
static int maxRow = 0;
static int maxCol = 0;
static int playerRow = -1;
static int playerCol = -1;
static int exitRow = -1;
static int exitCol = -1;
static int readMazeRan = 0;
static int numSteps = 0;

int getCurrentKeys(){
  return numKeysHeld;
}

int getTotalKeys(){
  return numTotalKeys;
}

int getNumSteps(){
  return numSteps;
}

void getCurrentPosition(int * arr){
  arr[0] = playerRow;
  arr[1] = playerCol;
}

int abs(int x){
  if (x < 0) return 0-x;
  return x;
}

int dangerous(int x, int y){
  return x < 0 || y < 0 || x >= 500 || y >= 500;
}

//fills the maze based on the file given
void readMaze(char* fileName){
  if(readMazeRan){
    printf("ERROR: Multiple calls to readMaze\n");
    return;
  }

  readMazeRan = 1;

  //stuff too complicated to cover in class
  FILE *inputFile = fopen(fileName, "r");
  char currentChar;
  int currentRow = 0;
  int currentCol = 0;
  while((currentChar = fgetc(inputFile)) != EOF){
    if(currentChar == '\n'){
      if(currentCol > maxCol){
        maxCol = currentCol;
      }
      currentRow += 1;
      currentCol = 0;
    } else {
      if(currentChar == '\r')
        continue;
      //3 is a key
      if(currentChar == '3'){
        numTotalKeys++;

      //2 is a player, but only one is allowed
      }else if(currentChar == '2'){
        if(playerRow != -1){
          printf("ERROR: You can't have more than one player! The instance at (%d, %d) was ignored in favor of the instance at (%d, %d).\n", currentRow, currentCol, playerRow, playerCol);
          currentChar = '1';
        }else{
          playerRow = currentRow;
          playerCol = currentCol;
        }

      //4 is an exit, but only one is allowed
      }else if(currentChar == '4'){
        if(exitRow != -1){
          printf("ERROR: You can't have more than one exit! The instance at (%d, %d) was ignored in favor of the instance at (%d, %d).\n", currentRow, currentCol, exitRow, exitCol);
          currentChar = '1';
        }else{
          exitRow = currentRow;
          exitCol = currentCol;
        }
      }

      if(currentChar >= '0' && currentChar <= '4'){
        mazeStructure[currentRow][currentCol] = currentChar - 48;
      }else{
        printf("ERROR: Unrecognized character at (%d, %d)\n", currentRow, currentCol);
      }

      currentCol+=1;
    }

  }
  if(playerRow == -1){
    printf("ERROR: No player object was found\n");
  }

  if(exitRow == -1){
    printf("ERROR: No exit object was found\n");
  }

  maxRow = currentRow;
}

void printMaze(){
  int i,j;
  for(i=0; i<maxRow; i++){
    for(j=0; j<maxCol; j++){
      printf("%d", mazeStructure[i][j]);
    }
    printf("\n");
  }
  for(i=0;i<maxCol;i++){
    printf("-");
  }
  printf("\n");
}

int isComplete(){
  return playerRow == exitRow && playerCol == exitCol;
}

int move(int newRow, int newCol){
  if(dangerous(newRow, newCol)){
    //printf("Out of bounds\n");
    return 0;
  }
  if(playerRow == newRow && playerCol == newCol){
    printf("You are already at (%d, %d).\n", newRow, newCol);
    return 0;
  }

  if(abs(playerRow - newRow) == 1 && abs(playerCol - newCol) == 1){
    printf("You tried to move to (%d, %d), but that requires moving diagonally, which is not allowed!\n", newRow, newCol);
   return 0;
  }

  if(abs(playerRow - newRow) > 1 || abs(playerCol - newCol) > 1){
    printf("You tried to move to (%d, %d), but that is more than one square away!\n", newRow, newCol);
    return 0;
  }

  if(mazeStructure[newRow][newCol] == 0){
    printf("You tried to move to (%d, %d), but there is a wall in the way!\n", newRow, newCol);
    return 0;
  }

  if(mazeStructure[newRow][newCol] == 4 && numKeysHeld < numTotalKeys){
    printf("The exit is at (%d, %d), but you only have %d/%d keys!\n", newRow, newCol, numKeysHeld, numTotalKeys);
    return 0;
  }

  if(mazeStructure[newRow][newCol] == 3){
    printf("Congratulations! You have found a key! You now have %d/%d keys!\n", ++numKeysHeld, numTotalKeys);
    mazeStructure[newRow][newCol] = 1;
  }

  mazeStructure[playerRow][playerCol] = 1;
  mazeStructure[newRow][newCol] = 2;
  numSteps++;
  playerRow = newRow;
  playerCol = newCol;
  return 1;
}

int look(int newRow, int newCol){
  if(dangerous(newRow, newCol)){
    //printf("Out of bounds\n");
    return 0;
  }
  if(abs(playerRow - newRow) == 1 && abs(playerCol - newCol) == 1){
    printf("You tried to look at (%d, %d), but that requires looking diagonally, which is not allowed!\n", newRow, newCol);
    return -1;
  }

  if(abs(playerRow - newRow) > 1 || abs(playerCol - newCol) > 1){
    printf("You tried to look at (%d, %d), but that is more than one square away!\n", newRow, newCol);
    return -1;
  }

  switch(mazeStructure[newRow][newCol]){
    case 0:
      //printf("That's a wall.\n");
      break;
    case 1:
      //printf("There's nothing there.\n");
      break;
    case 2:
      //printf("You take a long hard look at yourself.\n");
      break;
    case 3:
      //printf("That's a key! Pick it up!\n");
      break;
    case 4:
      //printf("That's the exit! Do you have enough keys?\n");
      break;
    default:
      //printf("Not sure what that is.\n");
      break;
  }

  return mazeStructure[newRow][newCol];

}
