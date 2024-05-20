// Author: 陳羿閔 B081020008
// Date: 2023-09-12
// Purpose: To print the magic matrices based on the given sizes

#include <stdio.h>
#include <stdlib.h>

// define a macro to iterate through the matrix sizes
#define for_each(sizes, len)                                                   \
    for (int i = 0, size = sizes[0]; i < len; size = sizes[++i])

void print_matrix(const int size, const int matrix[size][size]);
int prev(int cur_idx, int len);
int next(int cur_idx, int len);
void create_matrix(const int size, int matrix[size][size]);
void init_matrix(const int size, int matrix[size][size]);
void print_matrices(const int sizes[], const int total);

int main(void)
{
    // Initialize the matrix sizes
    const int MATRIX_SIZES[] = {1, 3, 5, 7, 9};
    const int TOTAL_MATRIX = sizeof(MATRIX_SIZES) / sizeof(MATRIX_SIZES[0]);

    // Start to print magic matrices based on the sizes
    print_matrices(MATRIX_SIZES, TOTAL_MATRIX);
}

void print_matrices(const int sizes[], const int total)
/* print the magic matrices based on the given sizes
 * sizes: the sizes of the matrices
 * total: the total number of the matrices
 */
{
    for_each(sizes, total)
    {
        printf("Matrix %d:\n", size);
        int matrix[size][size];
        init_matrix(size, matrix);
        create_matrix(size, matrix);
        print_matrix(size, matrix);
        putchar('\n');
    }
}

void print_matrix(const int size, const int matrix[size][size])
/* print the matrix
 * size: the size of the matrix
 * matrix: the matrix to be printed
 */
{
    for (int i = 0; i < size; ++i) {
        const int *row = matrix[i];
        for (int j = 0; j < size; ++j) {
            printf("%d\t", row[j]);
        }
        putchar('\n');
    }
}

int prev(int cur_idx, int len)
/* return the previous index of the matrix
 * cur_idx: current index position
 * len: the length of either row or column of the matrix
 */
{
    int decremented_idx = cur_idx - 1;
    return (decremented_idx) < 0 ? len - 1 : decremented_idx;
}

int next(int cur_idx, int len)
/* return the next index of the matrix
 * cur_idx: current index position
 * len: the length of either row or column of the matrix
 */
{
    int incremented_idx = cur_idx + 1;
    return (incremented_idx > len - 1) ? 0 : incremented_idx;
}

void create_matrix(const int size, int matrix[size][size])
/* create the magic matrix based on the given instructions
 * size: the size of the matrix
 * matrix: where the magic matrix will be stored
 */
{
    int k = 2;
    int mid = size / 2;
    matrix[0][mid] = 1;

    int i = 0, j = mid;
    while (k <= size * size) {
        int tmp_i = prev(i, size);
        int tmp_j = prev(j, size);
        if (matrix[tmp_i][tmp_j] == 0)
            i = tmp_i, j = tmp_j;
        else
            i = next(i, size);
        matrix[i][j] = k;
        ++k;
    }
}

void init_matrix(const int size, int matrix[size][size])
/* initialize the matrix with 0
 * size: the size of the matrix
 * matrix: the matrix to be initialized
 */
{
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = 0;
        }
    }
}
