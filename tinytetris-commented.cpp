#include <ctime>
#include <curses.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "src/Piece.h"
#include "src/ZPiece.h"
#include "src/JPiece.h"
#include "src/OPiece.h"

// block layout is: {w-1,h-1}{x0,y0}{x1,y1}{x2,y2}{x3,y3} (two bits each)
int x = 431424, y = 598356, r = 427089, px = 247872, py = 799248, pr,
    c = 348480, p = 615696, tick, board[20][10],
    score = 0;

// int block[7][4] = {
//     {x, y, x, y},                     // Z (red)
//     {r, p, r, p},                     // S (green)
//     {c, c, c, c},                     // O (yellow) (square)
//     {599636, 431376, 598336, 432192}, // J (blue)
//     {411985, 610832, 415808, 595540}, // T (purple)
//     {px, py, px, py},                 // I (cyan)
//     {614928, 399424, 615744, 428369}  // L (white)
// };

// extract a 2-bit number from a block entry
// int NUM(int x, int y) { return 3 & block[p][x] >> y; }

int NUM2(int x, int y, Piece &piece)
{
  return 3 & piece.getCurrentState() >> y;
}

// create a new piece, don't remove old one (it has landed and should stick)
Piece new_piece()
{
  Piece piece = OPiece();
  y = py = 0;
  p = 0;
  r = pr = piece.getCurrentStateId();
  x = px = 4;
  return piece;
}

// draw the board and score
void frame()
{
  for (int i = 0; i < 20; i++)
  {
    move(1 + i, 1); // otherwise the box won't draw
    for (int j = 0; j < 10; j++)
    {
      board[i][j] && attron(262176 | board[i][j] << 8);
      printw("  ");
      attroff(262176 | board[i][j] << 8);
    }
  }
  move(21, 1);
  printw("Score: %d", score);
  refresh();
}

// set the value fo the board for a particular (x,y,r) piece
void set_piece(int x, int y, int r, int v, Piece &piece)
{
  for (int i = 0; i < 8; i += 2)
  {
    board[NUM2(r, i * 2, piece) + y][NUM2(r, (i * 2) + 2, piece) + x] = v;
  }
}

// move a piece from old (p*) coords to new
void update_piece(Piece &piece)
{
  set_piece(px, py, pr, 0, piece);
  set_piece(px = x, py = y, pr = r, p + 1, piece);
}

// remove line(s) from the board if they're full
void remove_line(Piece &piece)
{
  for (int row = y; row <= y + NUM2(r, 18, piece); row++)
  {
    c = 1;
    for (int i = 0; i < 10; i++)
    {
      c *= board[row][i];
    }
    if (!c)
    {
      continue;
    }
    for (int i = row - 1; i > 0; i--)
    {
      memcpy(&board[i + 1][0], &board[i][0], 40);
    }
    memset(&board[0][0], 0, 10);
    score++;
  }
}

// check if placing p at (x,y,r) will be a collision
int check_hit(int x, int y, int r, Piece &piece)
{
  if (y + NUM2(r, 18, piece) > 19)
  {
    return 1;
  }
  set_piece(px, py, pr, 0, piece);
  c = 0;
  for (int i = 0; i < 8; i += 2)
  {
    board[y + NUM2(r, i * 2, piece)][x + NUM2(r, (i * 2) + 2, piece)] && c++;
  }
  set_piece(px, py, pr, p + 1, piece);
  return c;
}

// slowly tick the piece y position down so the piece falls
bool do_tick(Piece &piece)
{
  if (++tick > 30)
  {
    tick = 0;
    if (check_hit(x, y + 1, r, piece))
    {
      if (!y)
      {
        return false;
      }
      remove_line(piece);
      piece = new_piece();
    }
    else
    {
      y++;
      update_piece(piece);
    }
  }
  return true;
}

// main game loop with wasd input checking
void runloop(Piece &piece)
{
  char left = 'q';
  char right = 'd';
  char down = 's';
  char rotate = 'z';
  char exit = 'e';
  while (do_tick(piece))
  {
    usleep(10000);
    if ((c = getch()) == left && x > 0 && !check_hit(x - 1, y, r, piece))
    {
      x--;
    }
    if (c == right && x + NUM2(r, 16, piece) < 9 && !check_hit(x + 1, y, r, piece))
    {
      x++;
    }
    if (c == down)
    {
      while (!check_hit(x, y + 1, r, piece))
      {
        y++;
        update_piece(piece);
      }
      remove_line(piece);
      piece = new_piece();
    }
    if (c == rotate)
    {
      piece.rotate();
      ++r %= 4;
      while (x + NUM2(r, 16, piece) > 9)
      {
        x--;
      }
      if (check_hit(x, y, r, piece))
      {
        x = px;
        r = pr;
      }
    }
    if (c == exit)
    {
      return;
    }
    update_piece(piece);
    frame();
  }
}

// init curses and start runloop
int main()
{
  srand(time(0));
  initscr();
  start_color();
  // colours indexed by their position in the block
  for (int i = 1; i < 8; i++)
  {
    init_pair(i, i, 0);
  }
  Piece p = new_piece();
  resizeterm(22, 22);
  noecho();
  timeout(0);
  curs_set(0);
  box(stdscr, 0, 0);
  runloop(p);
  endwin();
  std::cout << "Your score is: " << score << std::endl;
  return 0;
}
