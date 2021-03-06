# Rogue-Like Solver

Written for fun and for the love of ASCII.
Problem taken from a friend.

What's unique about this problem, is that you start out knowing *nothing* about the maze. Your avatar has to build a representation of the maze and progressively search for interesting tiles, such as exits and keys. Once you have found all the keys, you are able to finish the map. If you haven't found the exit, but have found all the keys, then you need to still search for the exit.

My solution for this uses a stack-based backtracking solution that will record every action (left, right, up, down) taken and then determine the last best known location to travel to when no other unexplored tiles are nearby. It also uses dijkstra's path finding to efficiently travel to locations in the maze. This is easily noticable when all keys have been found and the player travels directly back to the found exit.

## Demo!

Check out the sweet ASCII in this ASCIInema demo:
[![asciicast](https://asciinema.org/a/jWtX5IsB7bSIiIKippjUfXK6f.png)](https://asciinema.org/a/jWtX5IsB7bSIiIKippjUfXK6f)

## Problem Statement

All of your code must be created in you own file and you will call it "MazeSolver.h".
Editing the other files except main is CHEATING.

```
Background:
You are writing an AI for solving a maze. Don't worry, it's not as hard as it
sounds. The maze has 3 important objects, the player, the exit, and keys. The
player is the only object that can move. To solve the maze, the player must
move onto all of the keys, and then move onto the exit.

    You have been given 4 files: Maze.c - Which contains definitions of
    functions I've written.  Maze.h - Which contains headers for functions I've
    written.  Main.c - Which contains the main method I've written.
    MazeSolver.h - Which contains methods I've started for you. 

    You will only be submitting MazeSolver.h! Don't make edits to the other
    three, as I will be using my original copies, not your versions!

Compiling & Running: To compile your project with my files, you will need them
all in the same directory. You will also need to alert the linker to them
somehow. I do not know how to do this inside of Netbeans, Codeblocks, etc, but
from the terminal you should just type: gcc Main.c Maze.c Maze.h MazeSolver.h

    To run the program, you will need to supply a single argument, which is the
    name of the maze file. I don't know how to do this from any IDE but I
    assure you it is possible. To run it from the terminal, just type ./a.out
    (a.exe) <file_name>

Explanation of maze files:

    0 - Wall
    1 - Empty Space
    2 - Player
    3 - Key
    4 - Exit

    The maximum size for a maze is 500x500.

Explanation of Maze.h:

  Here are the methods from Maze.h that you might want to use:

    int getNumSteps() - returns the number of steps which you have made so far.
    Might be useful for debugging purposes.

    void getCurrentPosition(int * arr) - this puts the player's row into
    arr[0], and the player's column into arr[1].

    void readMaze() - using this is considered cheating.

    void printMaze() - prints the current maze. Might be useful for debugging.

    int isComplete() - returns 1 if the maze is complete, and 0 otherwise.
    Useful for loop conditions

    int move(int newRow, int newCol) - if possible, it moves the player to the
    position specified. Returns 1 if move succeeds, 0 otherwise. If not
    possible, it prints a handy error message explaining what happened. You are
    definitely allowed to comment out the print statements if you get tired of
    seeing the error messages.

    int look(int lookRow, int lookCol) - if possible, it tells you what's in
    the square you are looking at. It also returns the integer value of what is
    in the square (see maze file explanation), or -1 on a failed look. If not
    possible, it prints a handy error message explaining what happened. You are
    definitely allowed to comment out the print statements if you get tired of
    seeing the error messages.

General rules:
    1. Leave the file names the same.
    2. Don't cheat the system! You know the intent of the project. Don't try to
       skirt the rules.
    3. If people find bugs in the files I've provided, I will post an
       announcement and update them on this site, AND YOU WILL BE GRADED ON THE
       UPDATED VERSIONS! Make sure your projects work with the final versions
       before you submit them. I guarentee that I won't make any changes to the
       files after 12/4. So 12/5 and 12/6 are the "safe" days to submit the
       project.

Grading Schema: 15 points for each of the 10 test cases.  -50 points for
cheating the system. You will know if this applies to you, don't worry.

Extra Credit:
    0. All of these points allow your grade to exceed the normal maximum of 110
       points.

    1. If your group is fastest, or tied for the fastest in any of the test
       cases, you will receive 3 points. This bonus may be applied multiple
       times.

        You are automaticlaly disqualified from the bonus if: a. You already
        have an A as your final grade in the course.  b. Applying the bonus
        would not affect your final letter grade.

        If you are disqualified, the bonus will go to second place, and so on
        until someone receives the bonus.

    2. If you find a bug and alert me to it, I will grant you 10 points. I
       consider a bug to be "A problem with my code that allows people to solve
       the project in unintended ways." If you think you've found a bug, let me
       know.

    3. If you submit a maze to me via email by 11/12 at 11pm, you will receive
       10 points. This maze must be at least 30 x 30, not 'trivial' (like an
       empty room maze or a straight line), and also contain at least two keys.
```

