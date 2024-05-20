// Author: 陳羿閔
// Date: 2023-11-26
// Description:
// This program reads a file named input.txt and sorts the numbers in the file
// with different sorting algorithms. The sorted numbers are then written to
// output.txt.

#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <stdio.h>
#include <time.h>

/* pointer to function type defnitions */
// compare function
typedef int (*_Nonnull CompFunc)(const void *a, const void *b);
// sorting function
typedef void (*SortFunc)(void *base, size_t len, size_t width, CompFunc compar);
// function and name (for printing)
typedef struct FuncWithName {
    const char *name;
    SortFunc func;
} FuncWithName;

/**
 * cmp_func - compare function for sorting algorithms (ascending order)
 * ---------------------------------------------------------------
 *  @a: pointer to the first element
 *  @b: pointer to the second element
 *
 *  Return: negative if a < b, 0 if a == b, positive if a > b
 */
inline static int cmp_func(const void *a, const void *b)
{
    return (*(int *)a - *(int *)b);
}

/**
 * get_input - read data from file and store in array
 * ---------------------------------------------------------------
 *  @arr: pointer to the array
 *  @len: length of the array
 *  @fin: input file
 */
inline static void get_input(int *arr, const int len, FILE *fin)
{
    for (int i = 0; i < len; i++)
        fscanf(fin, "%d", &arr[i]);
    fclose(fin);
}

/**
 * write_output - write sorted data to file
 * ---------------------------------------------------------------
 *  @arr: pointer to the array
 *  @len: length of the array
 *  @fout: output file
 *  @algo: name of the sorting algorithm
 */

inline static void write_output(const int *arr, const int len, FILE *fout,
                                const char *algo)
{
    fprintf(fout, "%s\n", algo);
    for (int i = 0; i < len; i++)
        fprintf(fout, "%d\n", arr[i]);
    fclose(fout);
}

/**
 * swap - swap two elements
 * ---------------------------------------------------------------
 *  @a: pointer to the first element
 *  @b: pointer to the second element
 *  @width: size of the 2 elements
 */
inline static void swap(void *a, void *b, size_t width)
{
    char temp[width];
    memcpy(temp, a, width);
    memcpy(a, b, width);
    memcpy(b, temp, width);
}

/**
 * sorting algorithms
 * ---------------------------------------------------------------
 *  @base: pointer to the array
 *  @len: length of the array
 *  @width: size of each element
 *  @compar: compare function
 *
 *  Note: only annotated once since every sorting algorithm has the same
 *      parameters
 */
inline static void selection_sort(void *base, size_t len, size_t width,
                                  CompFunc compar)
{
    for (size_t i = 0; i < len; i++) {
        size_t min = i;
        for (size_t j = i + 1; j < len; j++) {
            char *cur_ele = (char *)base + j * width;
            char *min_ele = (char *)base + min * width;
            if (compar(cur_ele, min_ele) < 0) {
                min = j;
            }
        }
        if (i != min) {
            char *cur_ele = (char *)base + i * width;
            char *min_ele = (char *)base + min * width;
            swap(cur_ele, min_ele, width);
        }
    }
}

/** heapify - create a min heap
 * ---------------------------------------------------------------
 * @base: pointer to the array
 * @len: length of the array
 * @width: size of each element
 * @compar: compare function
 * @i: index of the root node
 */
inline static void heapify(void *base, size_t len, size_t width,
                           CompFunc compar, size_t i)
{
    size_t largest = i;
    size_t left = 2 * i + 1;
    size_t right = 2 * i + 2;

    // the left node is smaller
    if (left < len) {
        char *left_ele = (char *)base + left * width;
        char *largest_ele = (char *)base + largest * width;
        if (compar(left_ele, largest_ele) > 0) {
            largest = left;
        }
    }

    // if the right node is smaller
    if (right < len) {
        char *right_ele = (char *)base + right * width;
        char *largest_ele = (char *)base + largest * width;
        if (compar(right_ele, largest_ele) > 0) {
            largest = right;
        }
    }

    // swap if any children is smaller than the original one
    if (largest != i) {
        char *largest_ele = (char *)base + largest * width;
        char *i_ele = (char *)base + i * width;
        swap(i_ele, largest_ele, width);
        heapify(base, len, width, compar, largest);
    }
}

inline static void heap_sort(void *base, size_t len, size_t width,
                             CompFunc compar)
{
    if (len < 2)
        return;

    for (int i = (int)(len / 2 - 1); i >= 0; i--)
        heapify(base, len, width, compar, i);

    for (size_t i = len - 1; i != 0; i--) {
        swap(base, (char *)base + i * width, width);
        heapify(base, i, width, compar, 0);
    }
}

/**
 * partition - partition the array
 * ---------------------------------------------------------------
 *  @base: pointer to the array
 *  @len: length of the array
 *  @width: size of each element
 *  @compar: compare function
 *  @i: index of the root node
 *
 *  Return: the index of the pivot
 */
inline static int partition(void *base, size_t len, size_t width,
                            CompFunc compar)
{
    size_t end = len - 1;
    char *pivot = (char *)base + end * width;
    size_t partition_idx = 0;

    for (size_t i = 0; i < end; i++) {
        char *cur = (char *)base + i * width;

        // if current element is smaller than pivot
        // swap it with the element at the partition index (since the element at
        // the partition index is larger than the pivot)
        if (compar(cur, pivot) <= 0) {
            char *swp_chk_ele = (char *)base + partition_idx * width;
            swap(cur, swp_chk_ele, width);
            partition_idx++;
        }
    }

    char *swp_chk_ele = (char *)base + partition_idx * width;
    swap(pivot, swp_chk_ele, width);
    return partition_idx;
}

inline static void quick_sort(void *base, size_t len, size_t width,
                              CompFunc compar)
{
    if (len <= 1)
        return;

    int partition_idx = partition(base, len, width, compar);
    quick_sort(base, partition_idx, width, compar);
    quick_sort((char *)base + partition_idx * width, len - partition_idx, width,
               compar);
}

/**
 * time_sorting - time the execution of sorting algorithms (cpp sort excluded)
 * ---------------------------------------------------------------
 *  @func: sorting function
 *  @arr: pointer to the array
 *  @len: length of the array
 */
inline static void time_sorting(FuncWithName func, int *arr, int len)
{
    // time and execute function
    clock_t start, end;
    start = clock();
    func.func(arr, len, sizeof(int), cmp_func);
    end = clock();

    // print execution time
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %f sec\n", func.name, cpu_time);
}

/**
 * perform_csorting - perform c sorting algorithms
 * ---------------------------------------------------------------
 *  @input_file: input file
 *  @i: index of the sorting algorithm
 *  @func: sorting function
 */
inline static void perform_csorting(const char *input_file, int i,
                                    const FuncWithName func)
{
    // set up output and input file
    char out_filename[20];
    snprintf(out_filename, 20, "output%c.txt", i + 65);
    FILE *fin = fopen(input_file, "r");
    FILE *fout = fopen(out_filename, "w");

    // read input
    int len;
    fscanf(fin, "%d", &len);
    int arr[len];
    get_input(arr, len, fin);

    // execute and output
    time_sorting(func, arr, len);
    write_output(arr, len, fout, func.name);
}

/**
 * perform_cppsorting - perform cpp sorting algorithm
 * ---------------------------------------------------------------
 *  @input_file: input file
 */
inline static void perform_cppsorting(const char *input_file)
{
    const char *func_name = "sort (cpp algorithm lib)";
    FILE *fin = fopen(input_file, "r");
    FILE *fout = fopen("outputE.txt", "w");

    // read input
    int len;
    fscanf(fin, "%d", &len);
    int arr[len];
    get_input(arr, len, fin);

    // time execution
    clock_t start, end;

    start = clock();
    std::sort(arr, arr + len);
    end = clock();

    // print execution time
    double cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("%s: %f sec\n", func_name, cpu_time);

    write_output(arr, len, fout, func_name);
}

int main(int argc, char **argv)
{
    // Please provide the input file as an argument
    if (argc != 2) {
        printf("Please provide the input file as an argument\n");
        printf("Current arguments: %d\n", argc - 1);
        printf("Usage: %s <input_file>\n", argv[0]);
        return 0;
    }

    // Different sorting algorithms
    FuncWithName sort_funcs[] = {
        FuncWithName{
            .name = "Selection Sort",
            .func = &selection_sort,
        },
        FuncWithName{
            .name = "Heap Sort",
            .func = &heap_sort,
        },
        FuncWithName{
            .name = "Quick Sort",
            .func = &quick_sort,
        },
        FuncWithName{
            .name = "qsort (c library)",
            .func = &heap_sort,
        },
    };

    // c sortings
    for (int i = 0; FuncWithName func : sort_funcs) {
        perform_csorting(argv[1], i, func);
        ++i;
    }

    // c++ sort
    perform_cppsorting(argv[1]);
}
