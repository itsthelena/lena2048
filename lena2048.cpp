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
      board[i][j] = newwin(5, 8, 20 + i * 5, 20 + j * 8);
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
  int stack[4];
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

void spawnNewNumber(int grid[4][4]) {
  int x, y;
  do {
    x = rand() % 4;
    y = rand() % 4;
  } while (grid[x][y] != 0);
  grid[x][y] = 2;
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

int main() {

  initscr();

  WINDOW *board[4][4];
  createBoard(board);

  refresh();

  while (true) {
    int grid[4][4] = {{0}};
    spawnNewNumber(grid);
    spawnNewNumber(grid);
    renderGrid(board, grid);
    bool quit = false;
    while (!hasLost(grid) || !quit) {
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
      default:
        continue;
      }
      spawnNewNumber(grid);
      renderGrid(board, grid);
    }
    mvprintw(10, 10, "You lost!");
    refresh();
    getch();
  }

  endwin();
  return 0;
}

