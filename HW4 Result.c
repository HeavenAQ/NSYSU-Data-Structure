/* Author: Yi-Min Chen, b081020008
 * Date: 2023-10-14
 * Description: This program is to solve the knight's tour problem using
 * recursion.
 * */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Data structures
// ===============
// Return values for move_knight
enum MoveStatus { SUCCEEDED, FAILED, BACKTRACK };

/* Function: print_board
 * ----------------------
 *  Print the chessboard
 *
 *  n: the size of the chessboard
 *  board: the chessboard
 *
 *  return: the number of characters printed
 */
static int print_board(const int n, int board[n][n]);

/* Function: move_knight
 * ---------------------
 *  Move the knight to the next position
 *
 * n: the size of the chessboard
 * board: the chessboard
 * row: the row of the current position of the knight
 * col: the column of the current position of the knight
 * cur_step: the current step of the knight
 * from_old_pos: whether the knight is moving from a visited position, if so,
 *              try to start from the directions other than the old one.
 *
 * return: the status of the move
 */
static int move_knight(const int n, int board[][n], int row, int col,
                       int cur_step);
/* Function: init_board
 * --------------------
 *  Initialize the chessboard
 *
 *  n: the size of the chessboard
 *  board: the chessboard
 */

static inline void init_board(const int n, int board[n][n]);

/* Function: generate_board
 * ------------------------
 *  Generate the chessboard with the knight's travel path
 *
 *  n: the size of the chessboard
 *  board: the chessboard
 *
 *  return: 0 if the chessboard is generated successfully, -1 otherwise
 */

static int generate_board(int n, int board[][n]);

int main(void) {
  for (int n = 1; n <= 6; ++n) {
    printf("============ Chessboard size: %d x %d =============\n", n, n);
    int board[n][n];
    init_board(n, board);
    int ret = generate_board(n, board);
    (ret == FAILED) ? printf("No solution\n") : print_board(n, board);
    puts("");
  }
}

static inline void init_board(const int n, int board[n][n]) {
  memset(board, 0, n * n * 4);
  if (n == 1)
    board[0][0] = 1;
}

static int print_board(const int n, int board[][n]) {
  int ret = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ret += printf("%d%c", board[i][j], "\t\n"[j == n - 1]);
  return ret;
}

static int move_knight(const int n, int board[][n], int row, int col,
                       int cur_step) {
  // finish this recursive function
  if (n == 1)
    return SUCCEEDED;
  if (cur_step == n * n + 1)
    return SUCCEEDED;
  else if (row < 0 || row >= n || col < 0 || col >= n)
    return FAILED;
  else if (board[row][col] != 0)
    return FAILED;
  else
    board[row][col] = cur_step;

  // try all possible directions
  int arr[][2] = {{row - 2, col + 1}, {row - 1, col + 2}, {row + 1, col + 2},
                  {row + 2, col + 1}, {row + 2, col - 1}, {row + 1, col - 2},
                  {row - 1, col - 2}, {row - 2, col - 1}};
  for (int i = 0; i < 8; ++i) {
    int next_row = arr[i][0];
    int next_col = arr[i][1];
    int ret = move_knight(n, board, next_row, next_col, cur_step + 1);
    if (ret == SUCCEEDED) {
      return SUCCEEDED;
    } else if (ret == BACKTRACK) {
      board[next_row][next_col] = 0;
      continue;
    }
  }
  return (cur_step == 1) ? FAILED : BACKTRACK;
}

static int generate_board(const int n, int board[][n]) {
  int starting_row = 0;
  int starting_col = 0;
  int starting_step = 1;
  return move_knight(n, board, starting_row, starting_col, starting_step);
}
