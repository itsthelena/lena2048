#include <algorithm>
#include <cstdlib>
#include <curses.h>
#include <ncurses.h>
using namespace std;

// This is a 2048 game

enum Direction { UP, DOWN, LEFT, RIGHT };

void createBoard(WINDOW *board[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      board[i][j] = newwin(5, 8, 10 + i * 5, 10 + j * 8);
      box(board[i][j], 0, 0);
      wrefresh(board[i][j]);
    }
  }
}

void renderGrid(WINDOW *board[4][4], int grid[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      wclear(board[i][j]);
      box(board[i][j], 0, 0);
      if (grid[i][j] != 0) {
        mvwprintw(board[i][j], 2, 2, "%d", grid[i][j]);
      }
      wrefresh(board[i][j]);
    }
  }
}

void stackAtTheEnd(int column[4]) {
  int stack[4] = {0, 0, 0, 0};
  int stackIndex = 0;
  for (int i = 0; i < 4; i++) {
    if (column[i] != 0) {
      stack[stackIndex] = column[i];
      stackIndex++;
    }
  }
  for (int i = 0; i < 4; i++) {
    column[i] = stack[i];
  }
}

void processMove(int column[4]) {
  stackAtTheEnd(column);
  for (int i = 0; i < 3; i++) {
    if (column[i] == column[i + 1]) {
      column[i] *= 2;
      column[i + 1] = 0;
    }
  }
  stackAtTheEnd(column);
}

void transpose(int grid[4][4]) {
  int temp;
  for (int i = 0; i < 4; i++) {
    for (int j = i; j < 4; j++) {
      temp = grid[i][j];
      grid[i][j] = grid[j][i];
      grid[j][i] = temp;
    }
  }
}

bool hasEmptySpace(int grid[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (!grid[i][j]) {
        return true;
      }
    }
  }
  return false;
}

void spawnNewNumber(int grid[4][4]) {
  if (!hasEmptySpace(grid)) {
    return;
  }
  int x, y;
  do {
    x = rand() % 4;
    y = rand() % 4;
  } while (grid[x][y] != 0);
  grid[x][y] = 2;
}

WINDOW *createScoreWindow() {
  WINDOW *scoreWindow = newwin(5, 8, 0, 0);
  box(scoreWindow, 0, 0);
  wrefresh(scoreWindow);
  return scoreWindow;
}

void renderScore(WINDOW *scoreWindow, int score) {
  wclear(scoreWindow);
  box(scoreWindow, 0, 0);
  mvwprintw(scoreWindow, 2, 2, "%d", score);
  wrefresh(scoreWindow);
}

// TODO: test this
bool hasLost(int grid[4][4]) {
  int temp[4][4];
  copy(&grid[0][0], &grid[0][0] + 16, &temp[0][0]);
  for (int i = 0; i < 4; i++) {
    processMove(temp[i]);
  }
  transpose(temp);
  for (int i = 0; i < 4; i++) {
    processMove(temp[i]);
  }
  transpose(temp);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 3; j++) {
      if (temp[i][j] == temp[i][j + 1]) {
        return false;
      }
    }
  }
  return true;
}

int getscore(int grid[4][4]) {
  int score = 0;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      score += grid[i][j];
    }
  }
  return score;
}

void processGridMove(int grid[4][4], Direction direction) {
  switch (direction) {
  case UP:
    transpose(grid);
    for (int i = 0; i < 4; i++) {
      processMove(grid[i]);
    }
    transpose(grid);
    break;
  case DOWN:
    transpose(grid);
    for (int i = 0; i < 4; i++) {
      reverse(grid[i], grid[i] + 4);
      processMove(grid[i]);
      reverse(grid[i], grid[i] + 4);
    }
    transpose(grid);
    break;
  case LEFT:
    for (int i = 0; i < 4; i++) {
      processMove(grid[i]);
    }
    break;
  case RIGHT:
    for (int i = 0; i < 4; i++) {
      reverse(grid[i], grid[i] + 4);
      processMove(grid[i]);
      reverse(grid[i], grid[i] + 4);
    }
    break;
  }
}

void makeGridLose(int grid[4][4]) {
  grid[0][0] = 2;
  grid[0][1] = 4;
  grid[0][2] = 8;
  grid[0][3] = 16;
  grid[1][0] = 32;
  grid[1][1] = 64;
  grid[1][2] = 128;
  grid[1][3] = 256;
  grid[2][0] = 512;
  grid[2][1] = 1024;
  grid[2][2] = 2048;
  grid[2][3] = 4096;
  grid[3][0] = 8192;
  grid[3][1] = 16384;
  grid[3][2] = 32768;
  grid[3][3] = 65536;
}

void clearBoard(WINDOW *board[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      wclear(board[i][j]);
      box(board[i][j], 0, 0);
      wrefresh(board[i][j]);
    }
  }
}

int main() {

  initscr();

  WINDOW *board[4][4];
  createBoard(board);

  WINDOW *scoreWindow = createScoreWindow();

  refresh();

  while (true) {
    int grid[4][4] = {{0}};
    spawnNewNumber(grid);
    spawnNewNumber(grid);
    renderGrid(board, grid);
    bool quit = false;
    while (!hasLost(grid) && !quit) {
      int ch = getch();
      switch (ch) {
      case 'w':
        processGridMove(grid, UP);
        break;
      case 's':
        processGridMove(grid, DOWN);
        break;
      case 'a':
        processGridMove(grid, LEFT);
        break;
      case 'd':
        processGridMove(grid, RIGHT);
        break;
      case 'q':
        quit = true;
        break;
      case 'l':
        makeGridLose(grid);
        break;
      default:
        continue;
      }
      spawnNewNumber(grid);
      renderGrid(board, grid);
      renderScore(scoreWindow, getscore(grid));
    }
    if (quit)
      break;
    clearBoard(board);
    mvprintw(10, 10, "You lost!");
    refresh();
    getch();
  }

  endwin();
  return 0;
}
