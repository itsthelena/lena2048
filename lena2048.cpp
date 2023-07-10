#include <algorithm>
#include <curses.h>
#include <ncurses.h>
using namespace std;

// This is a 2048 game

void createGrid(WINDOW *grid[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      grid[i][j] = newwin(5, 8, 20 + i * 5, 20 + j * 8);
      box(grid[i][j], 0, 0);
      wrefresh(grid[i][j]);
    }
  }
}

void renderGrid(WINDOW *grid[4][4], int board[4][4]) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      wclear(grid[i][j]);
      box(grid[i][j], 0, 0);
      if (board[i][j] != 0) {
        mvwprintw(grid[i][j], 2, 2, "%d", board[i][j]);
      }
      wrefresh(grid[i][j]);
    }
  }
}

int main() {

  initscr();

  WINDOW *grid[4][4];
  createGrid(grid);
  refresh();

  WINDOW *win = newwin(5, 8, 20, 20);
  box(win, 0, 0);
  mvwprintw(win, 1, 1, "Hello there");
  wrefresh(win);
  curs_set(0);

  while (true) {
    int ch = wgetch(win);
    wclear(win);

    box(win, 0, 0);
    if (ch == 'q') {
      break;
    }

    switch(ch) {
      case 'w':
        mvwprintw(win, 2, 2, "2048");
        break;
      case 's':
        mvwprintw(win, 2, 2, "1024");
        break;
      case 'd':
        mvwprintw(win, 2, 2, "6");
        break;
      case 'a':
        mvwprintw(win, 2, 2, "128");
        break;
    }
    wrefresh(win);
  }

  endwin();
  return 0;
}
