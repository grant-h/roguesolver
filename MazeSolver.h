/* Maze Solver by Grant Hernandez
 * Written: Feb  3 2017
 *
 * Uses a history stack for backtracking and pathfinding
 * for running back to the exit and to the last known
 * good tile.
 *
 * Some more optimizations would include using more knowledge of the map
 * in order to prioritize visiting areas that are closer and less explored.
 */
#include "Maze.h"

#include <stdlib.h>
#include <memory.h>
#include <string.h>

// Uncomment for a cool display
#define SHOWOFF
static int SPEED = 100000; // higher is slower (microseconds)

typedef struct _pos {
  int r,c;
} pos_t;

struct tile {
  int value;

  int visited;
};

typedef enum _movement {
  UP, RIGHT, DOWN, LEFT, NONE
} movement_t;
typedef enum _tiletype {
  WALL = 0, EMPTY, ME, KEY, EXIT
} tiletype_t;

// What we DO know about the maze from looking
struct tile ourMaze[500][500] = {0};
static int P[2] = {-1};
static pos_t exit_pos = {-1, -1};

#define ROW P[0]
#define COL P[1]
#define GCP getCurrentPosition(P)

movement_t flip(movement_t action) {
  switch(action)
  {
    case UP:
      return DOWN;
    case DOWN:
      return UP;
    case LEFT:
      return RIGHT;
    case RIGHT:
      return LEFT;
    default:
      return NONE;
  }
}

void printOurMaze(){
  // determine the bounding box of the maze in order to make printing easier
  // a[r][c]
  int r,c,i;
  int rmin = 500, rmax = 0, cmin = 500, cmax = 0;

  for(r=0; r<500; r++){
    for(c=0; c<500; c++){
      if(ourMaze[r][c].value != -1) {
        if(r < rmin)
          rmin = r;
        if(r > rmax)
          rmax = r;
        if(c < cmin)
          cmin = c;
        if(c > cmax)
          cmax = c;
      }
    }
  }

  //printf("Bounds <%d, %d> --> <%d, %d>\n", rmin, cmin, rmax, cmax);

  for(r=rmin; r<=rmax; r++){
    for(c=cmin; c<=cmax; c++){
      char outchar;
      int v = ourMaze[r][c].value;

      //printf("%d,%d[%d]", r,c,v);
      switch(v) {
        case -1:
          outchar = '?';
          break;
        case 0:
          outchar = '#';
          break;
        case 1:
          outchar = '.';
          break;
        case 2:
          outchar = '@';
          break;
        case 3:
          outchar = '$';
          break;
        case 4:
          outchar = '>';
          break;
        default:
          outchar = '~';
          break;
      }

      if(c == cmin)
      {
        if(v == 1)
          printf("?");
        else
          printf(" ");
      }

      printf("%c", outchar);

      if(c == cmax)
      {
        if(v == 1)
          printf("?");
        else
          printf(" ");
      }
    }
    printf("\n");
  }

  for(i=0;i<cmax-cmin+1;i++){
    printf("-");
  }

  printf("\n");
}


static movement_t * history = NULL;
static int historySize = 0;
static int historyPointer = -1;

void fatal_error(const char * error)
{
  printf("%s\n", error);
  exit(1);
}

void history_push(movement_t action)
{
  if((historyPointer+1) >= historySize) {
    historySize += 100;
    history = realloc(history, historySize*sizeof(movement_t));

    if(!history)
      fatal_error("Failed to expand history buffer");
  }


  historyPointer += 1;
  history[historyPointer] = action;
}

movement_t history_pop()
{
  if(historyPointer == -1) {
    fatal_error("Popped more history than we had");
  }

  movement_t m = history[historyPointer];
  historyPointer--;
  return m;
}

pos_t mov2pos_rc(movement_t action, int r, int c)
{
  pos_t p;

  switch(action)
  {
    case UP:
      p.r = r-1;
      p.c = c;
      break;
    case DOWN:
      p.r = r+1;
      p.c = c;
      break;
    case LEFT:
      p.r = r;
      p.c = c-1;
      break;
    case RIGHT:
      p.r = r;
      p.c = c+1;
      break;
    case NONE:
      p.r = r;
      p.c = c;
      break;
  }

  return p;
}

pos_t mov2pos(movement_t action)
{
  return mov2pos_rc(action, ROW, COL);
}

struct tile * mov2tile(movement_t action)
{
  pos_t p = mov2pos(action);

  return &ourMaze[p.r][p.c];
}

///////////////////////////////////////////////

int pathfind(pos_t from, pos_t to, movement_t ** out_path)
{
  int r,c,i;
  int rmin = 500, rmax = 0, cmin = 500, cmax = 0;

  int * cost = malloc(500*500*sizeof(int));
  int frontierSize = 100;
  int frontierIter = 0;
  struct node {
    pos_t p;
    int dist;
  };
  struct node * frontier = malloc(sizeof(struct node)*frontierSize);

#define CI(r,c) r*500+c

  for(r=0; r<500; r++){
    for(c=0; c<500; c++){
      cost[CI(r,c)] = -1;

      if(ourMaze[r][c].value != -1) {
        if(r < rmin)
          rmin = r;
        if(r > rmax)
          rmax = r;
        if(c < cmin)
          cmin = c;
        if(c > cmax)
          cmax = c;
      }
    }
  }

  // push the first node for searching
  frontier[frontierIter].p = to;
  frontier[frontierIter].dist = 0;
  frontierIter++;

  //printf("EQ<%d, %d>\n", to.r, to.c);

  while(frontierIter > 0) {
    // pop one node
    frontierIter--;
    struct node n = frontier[frontierIter];

    //printf("DQ<%d, %d>\n", n.p.r, n.p.c);

    cost[CI(n.p.r, n.p.c)] = n.dist;

    movement_t lookups[4] = { UP, DOWN, LEFT, RIGHT };

    for(i = 0; i < 4; i++) {
      pos_t nextpos = mov2pos_rc(lookups[i], n.p.r, n.p.c);

      if(nextpos.r < rmin || nextpos.r > rmax || nextpos.c < cmin || nextpos.c > cmax)
        continue;

      // ignore unexplored space and walls
      // dont revisit squares unless we have a better cost
      if(ourMaze[nextpos.r][nextpos.c].value == -1 || ourMaze[nextpos.r][nextpos.c].value == WALL || (cost[CI(nextpos.r, nextpos.c)] != -1 && cost[CI(nextpos.r, nextpos.c)] <= n.dist+1))
        continue;

      if(frontierIter+1 >= frontierSize) {
        frontierSize += 400;
        frontier = realloc(frontier, sizeof(struct node)*frontierSize);
      }

      //printf("EQ<%d, %d>=%d\n", nextpos.r, nextpos.c, n.dist+1);
      frontier[frontierIter].p = nextpos;
      frontier[frontierIter].dist = n.dist+1;
      frontierIter++;
    }
  }

  // build the path from the source to the sink
  movement_t * path = NULL;
  int pathSize = 0;
  int pathIter = 0;
  pos_t curpos = from, curposnext;

  while(curpos.r != to.r || curpos.c != to.c) {
    movement_t lookups[4] = { UP, DOWN, LEFT, RIGHT };
    int min = 200000000;
    movement_t dir = NONE;
    //printf("Cur <%d, %d>\n", curpos.r, curpos.c);

    for(i = 0; i < 4; i++) {
      pos_t nextpos = mov2pos_rc(lookups[i], curpos.r, curpos.c);
      //printf("<%d, %d> = %d\n", nextpos.r, nextpos.c, c);
      //printf("Next <%d, %d>\n", nextpos.r, nextpos.c);

      if(nextpos.r < rmin || nextpos.r > rmax || nextpos.c < cmin || nextpos.c > cmax)
        continue;

      int c = cost[CI(nextpos.r,nextpos.c)];

      if(c == -1)
        continue;

      if(c < min) {
        min = c;
        dir = lookups[i];
        curposnext = nextpos;
      }
    }

    if(dir == NONE) {
      fatal_error("Pathfinding reached a dead end");
    }

    if(pathIter+1 >= pathSize) {
      pathSize += 100;
      path = realloc(path, sizeof(movement_t)*pathSize);
    }

    curpos = curposnext;
    path[pathIter] = dir;
    pathIter++;
  }

  //printf("Found path from <%d, %d> to <%d, %d> of length %d\n",
  //    from.r, from.c, to.r, to.c, pathIter);

  free(frontier);
  free(cost);

  *out_path = path;
  return pathIter;
}


void lookAround() {
  int i;
  movement_t lookups[5] = { NONE, UP, DOWN, LEFT, RIGHT };

  for(i = 0; i < 5; i++) {
    pos_t p = mov2pos(lookups[i]);

    if(p.r < 0 || p.r >= 500 || p.c < 0 || p.c >= 500)
      continue;

    ourMaze[p.r][p.c].value = look(p.r, p.c);

    // note the exit position for later pathfinding
    if(ourMaze[p.r][p.c].value == EXIT) {
      exit_pos = p;
    }
  }
}


int do_move(movement_t action)
{
  int res = 0;

  pos_t pos = mov2pos(action);

  if(action != NONE)
    res = move(pos.r, pos.c);
  else
    res = 1;

  if(res) {
    ourMaze[pos.r][pos.c].visited++;
    //printf("Moved to <%d, %d>\n", pos.r, pos.c);
  }

  return res;
}

void init() {
  int r, c;
  for(r = 0; r < 500; r++) {
    for(c = 0; c < 500; c++) {
      ourMaze[r][c].value = -1;
      ourMaze[r][c].visited = 0;
    }
  }

  // initialize our current position
  GCP;
  do_move(NONE);
}

int canmovehere(pos_t pos)
{
  struct tile n = ourMaze[pos.r][pos.c];

  // dont revisit places on purpose
  if(n.visited != 0)
    return 0;

  // dont visit walls
  if(n.value == WALL)
    return 0;

  // dont exit with out all the keys
  if(n.value == EXIT && getTotalKeys() != getCurrentKeys())
    return 0;

  return 1;
}

void run_path(movement_t * path, int pathlen)
{
  int i;

  for(i = 0; i < pathlen; i++) {
    if(!do_move(path[i])) {
      fatal_error("Pathfinding gave bad path");
    }

    //history_push(path[i]);

    GCP;
    lookAround();

#ifdef SHOWOFF
    printOurMaze();
    usleep(SPEED);
#endif
  }
}

static int isInteresting(pos_t pos)
{
  int j;
  movement_t lookups[5] = { UP, DOWN, LEFT, RIGHT, NONE };

  // only decide to visit it if there is some benefit to us
  for(j = 0; j < 5; j++) {
    pos_t interesting = mov2pos_rc(lookups[j], pos.r, pos.c);
    struct tile * t = &ourMaze[interesting.r][interesting.c];

    // interesting is defined as unexplored or a KEY
    if(t->value == -1 || t->value == KEY)
      return 1;
  }

  // no reason to go there
  return 0;
}

static void solveMaze() {
  init();

  while(!isComplete()) {
#ifdef SHOWOFF
    int step = getNumSteps();
    printf(">>> Step %d\n", step);
#endif

    GCP;
    lookAround();

    if(getTotalKeys() == getCurrentKeys() && exit_pos.r != -1) {
      printf("Heading to the exit!\n");

      pos_t cur = {ROW, COL};
      movement_t * path = NULL;

      int i;
      int pathlen = pathfind(cur, exit_pos, &path);

      run_path(path, pathlen);

      return;
    }

    int i;
    movement_t dir = NONE;
    movement_t lookups[4] = { RIGHT, UP, DOWN, LEFT};

    for(i = 0; i < 4; i++) {
      pos_t pos = mov2pos(lookups[i]);
      int v = ourMaze[pos.r][pos.c].value;

      if(!canmovehere(pos))
        continue;

      if(!isInteresting(pos))
        continue;

      // we have a direction already, but prioritize keys
      if(dir != NONE && v != KEY)
        continue;

      dir = lookups[i];
    }

    if(dir != NONE) {
      // try and move, but if we fail, backtrack
      if(do_move(dir)) {
        history_push(dir);
      } else {
        dir = NONE;
      }
    }

    // no good moves, backtrack
    if(dir == NONE) {
      pos_t cur = {ROW, COL};
      pos_t target = cur;

      //printf("Unable to move from <%d, %d>\n", cur.r, cur.c);

      // pop our history stack until we reach a node that we can
      // move to while still being able to move from it
      // pathfind to it to avoid going in a long path back
      while(1) {
        movement_t dir = flip(history_pop());
        pos_t next = mov2pos_rc(dir, target.r, target.c);

        // check to see if this tile can be moved to
        for(i = 0; i < 4; i++) {
          pos_t neighbor = mov2pos_rc(lookups[i], next.r, next.c);

          if(canmovehere(neighbor) && isInteresting(neighbor))
          {
            //printf("Can move to <%d, %d>\n", neighbor.r, neighbor.c);
            break;
          }
        }

        // we can move here and have somewhere to go...
        if(i < 4) {
          movement_t * path = NULL;
          int pathlen = pathfind(cur, next, &path);
          // dont save any path history as we dont want to come back to cur
          run_path(path, pathlen);
          break;
        } else {
          target = next;
        }
      }
    }

#ifdef SHOWOFF
    GCP;
    lookAround();

    printf("Position <%d, %d>\n", ROW, COL);
    printOurMaze();

    usleep(SPEED);
#endif
  }
}
