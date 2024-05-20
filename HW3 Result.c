#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define auto_stack __attribute__((cleanup(destruct_stack)))

// Data structures
// ===============
/* Enum for all possible directions the knight moves */
enum Dir { TR = 0, RT, RB, BR, BL, LB, LT, TL };
/* Struct for the position the knight has visited */
typedef struct VisitedPos VisitedPos;
/* Struct for the node of stack */
typedef struct Node Node;
/* Struct for the stack */
typedef struct Stack Stack;

struct VisitedPos {
  int x;
  int y;
  enum Dir dir;
};

struct Node {
  VisitedPos *data;
  Node *next;
};

struct Stack {
  Node *top;
  void (*push)(Stack *self, VisitedPos *data);
  VisitedPos *(*pop)(Stack *self);
  int (*is_empty)(Stack *self);
};

// Function prototypes for stack
// =============================
/* Function: init_stack
 * --------------------
 * Initialize a stack
 *
 * return: the initialized stack
 */
Stack *init_stack(void);

/* Function: destruct_stack
 * ------------------------
 * Destruct a stack
 *
 * self: the stack to be destructed
 */

/* Function: push
 * ---------------
 * Push a VisitedPos to the stack
 *
 * self: the stack to be pushed
 * data: the VisitedPos data to be pushed
 */
static void push(Stack *self, VisitedPos *data);

/* Function: pop
 * --------------
 * Pop a VisitedPos from the stack
 *
 * self: the stack to be popped
 *
 * return: the popped VisitedPos
 */
static VisitedPos *pop(Stack *self);

/* Function: is_empty
 * -------------------
 * Check if the stack is empty
 *
 * self: the stack to be checked
 *
 * return: 1 if the stack is empty, 0 otherwise
 */
static int is_empty(Stack *self);

// Function prototypes
// ==================
/* Function: create_node
 * ---------------------
 * Creates and returns a node of for stack
 *
 * data: the VisitedPos data to be stored in the Node
 *
 * return: the created Node
 */
Node *create_node(VisitedPos *data);

/* Function: create_visited_pos
 * ----------------------------
 * Create a node of visited position for stack
 *
 * x: the x coordinate of the position from where the knight moved
 * y: the y coordinate of the position from where the knight moved
 * dir: the direction to where the knight moved
 *
 * return: the created VisitedPos
 */
VisitedPos *create_visited_pos(int x, int y, enum Dir dir);

/* Function: free_node
 * -------------------
 * Free the memory of a node
 *
 * node: the node to be freed
 */
void free_node(Node *node);

// Function prototypes
// ==================
/* Function: print_board
 * ---------------------
 * Print the chessboard
 *
 * n: the size of the chessboard
 * board: the chessboard to be printed
 *
 * return: the number of characters printed
 */
static int print_board(const int n, int board[n][n]);

/* Function: get_moveable_range
 * ----------------------------
 * Get the moveable range of the knight
 *
 * row: the row of the current position of the knight
 * col: the column of the current position of the knight
 *
 * return: the moveable range of the knight
 */
static Stack *get_moveable_range(const int row, const int col);

/* Function: try_start_from_old_pos
 * --------------------------------
 * Try to start from the old position if this position has been visited before:
 *    1. If the stack of previous positions is empty, return
 *    2. If the current position is not the same as the previous position,
 * return
 *    3. Pop the moveable_range stack until the direction of the
 *    moveable range is greater than the direction of the previous position
 *
 * moveable_range: the moveable range of the knight
 * prev_poses: the stack of the previous positions
 * cur_row: the row of the current position of the knight
 * cur_col: the column of the current position of the knight
 */
static void try_start_from_old_pos(Stack *moveable_range, Stack *prev_poses,
                                   const int cur_row, const int cur_col);

/* Function: move_knight
 * ---------------------
 *  Move the knight to the next position
 *
 * n: the size of the chessboard
 * board: the chessboard
 * moveable_range: the moveable range of the knight
 * prev_poses: the stack of the previous positions
 * cur_row: the row of the current position of the knight
 * cur_col: the column of the current position of the knight
 * cur_step: the current step of the knight
 * empty_space: the number of empty spaces on the chessboard
 *
 * return:
 *  1. the next position of the knight
 *  2. NULL if the knight cannot move to any position
 */
static VisitedPos *move_knight(const int n, int board[][n],
                               Stack *moveable_range);

/* Function: move_back
 * -------------------
 *  Move the knight back to the previous position
 *
 *  n: the size of the chessboard
 *  board: the chessboard
 *  row: the row of the current position of the knight
 *  col: the column of the current position of the knight
 *  prev_poses: the stack of the previous positions
 */
static void move_back(const int n, int board[][n], int *row, int *col,
                      Stack *prev_poses);

/* Function: init_board
 * --------------------
 *  Initialize the chessboard to 0
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

// Main function
int main(void) {
  for (int n = 1; n <= 6; ++n) {
    printf("============ Chessboard size: %d x %d =============\n", n, n);
    int board[n][n];
    init_board(n, board);
    int ret = generate_board(n, board);
    (ret == -1) ? printf("No solution\n") : print_board(n, board);
    puts("");
  }
}

// Function definitions for stack
// ==============================
Stack *init_stack(void) {
  Stack *stack = (Stack *)malloc(sizeof(Stack));
  stack->top = NULL;
  stack->push = &push;
  stack->pop = &pop;
  stack->is_empty = &is_empty;
  return stack;
}

__attribute__((always_inline)) inline void destruct_stack(Stack **self) {
  if (!*self) {
    return;
  }

  Node *tmp = (*self)->top;
  while (tmp) {
    Node *next = tmp->next;
    free_node(tmp);
    tmp = next;
  }
}

static void push(Stack *self, VisitedPos *data) {
  Node *new_node = create_node(data);
  new_node->next = self->top;
  self->top = new_node;
}

static VisitedPos *pop(Stack *self) {
  Node *tmp = self->top;
  self->top = self->top->next;
  VisitedPos *val = tmp->data;
  free(tmp);
  return val;
}

static int is_empty(Stack *self) { return self->top == NULL; }

// Function definitions for node
// =============================
Node *create_node(VisitedPos *data) {
  Node *new_node = (Node *)malloc(sizeof(Node));
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

// Function definitions
// ====================
VisitedPos *create_visited_pos(int x, int y, enum Dir dir) {
  VisitedPos *new_pos = (VisitedPos *)malloc(sizeof(VisitedPos));
  new_pos->x = x;
  new_pos->y = y;
  new_pos->dir = dir;
  return new_pos;
}

void free_node(Node *node) {
  free(node->data);
  free(node);
}

static int print_board(const int n, int board[n][n]) {
  int ret = 0;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++)
      ret += printf("%d%c", board[i][j], "\t\n"[j == n - 1]);
  return ret;
}

static Stack *get_moveable_range(const int row, const int col) {
  Stack *stack = init_stack();
  stack->push(stack, create_visited_pos(row - 2, col - 1, TL));
  stack->push(stack, create_visited_pos(row - 1, col - 2, LT));
  stack->push(stack, create_visited_pos(row + 1, col - 2, LB));
  stack->push(stack, create_visited_pos(row + 2, col - 1, BL));
  stack->push(stack, create_visited_pos(row + 2, col + 1, BR));
  stack->push(stack, create_visited_pos(row + 1, col + 2, RB));
  stack->push(stack, create_visited_pos(row - 1, col + 2, RT));
  stack->push(stack, create_visited_pos(row - 2, col + 1, TR));
  return stack;
}

static void try_start_from_old_pos(Stack *moveable_range, Stack *prev_poses,
                                   const int cur_row, const int cur_col) {
  if (prev_poses->is_empty(prev_poses))
    return;
  else if (prev_poses->top->data->x != cur_row &&
           prev_poses->top->data->y != cur_col)
    return;

  Node *most_recent_pos = prev_poses->top;
  while (!moveable_range->is_empty(moveable_range) &&
         moveable_range->top->data->dir <= most_recent_pos->data->dir) {
    moveable_range->pop(moveable_range);
  }
  prev_poses->pop(prev_poses);
}

static VisitedPos *move_knight(const int n, int board[][n],
                               Stack *moveable_range) {
  for (Node *tmp = moveable_range->top; tmp; tmp = tmp->next) {
    VisitedPos *pos = tmp->data;
    int visited = board[pos->x][pos->y];
    int is_neg_pos = pos->x < 0 || pos->y < 0;
    int is_out_of_bound_pos = pos->x >= n || pos->y >= n;
    int is_invalid_pos = visited || is_neg_pos || is_out_of_bound_pos;

    if (is_invalid_pos) {
      continue;
    }
    return pos;
  }
  return NULL;
}

static void move_back(const int n, int board[][n], int *row, int *col,
                      Stack *prev_poses) {
  board[*row][*col] = 0;
  *row = prev_poses->top->data->x;
  *col = prev_poses->top->data->y;
}

static inline void init_board(const int n, int board[n][n]) {
  memset(board, 0, (unsigned long)(n * n * 4));
  board[0][0] = 1;
}

static int generate_board(int n, int board[][n]) {
  int row = 0, col = 0;
  int cur_step = 1;
  int empty_space = n * n - 1;
  auto_stack Stack *prev_poses = init_stack();

  while (empty_space) {
    Stack *moveable_range = get_moveable_range(row, col);
    try_start_from_old_pos(moveable_range, prev_poses, row, col);
    VisitedPos *next_move = move_knight(n, board, moveable_range);

    if (!next_move) {
      int no_prev_pos =
          prev_poses->is_empty(prev_poses) || empty_space == n * n;
      if (no_prev_pos) {
        return -1;
      }
      move_back(n, board, &row, &col, prev_poses);
      empty_space++;
      cur_step--;
      continue;
    }

    prev_poses->push(prev_poses, create_visited_pos(row, col, next_move->dir));
    row = next_move->x;
    col = next_move->y;
    board[row][col] = ++cur_step;
    empty_space--;
  }
  return 0;
}
