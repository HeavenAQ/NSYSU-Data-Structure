/* Author: 陳羿閔 B081020008
 * Date: 2023-09-12
 * Purpose: A program that can produce and print each factorial from 1 to n (n
 * is given by user) in the form of n! = xxxxxxxxxx. The maximum n is 50 and
 * user can input multiple numbers at once (separated by newline). Hit 0 and
 * enter to stop input.
 */

#include <stdio.h>
#define INPUT_LEN 100
void store_digits(int digits[]);
void multiply(int output[], const int n);
void produce_factorial(const int n, int output[]);
void print_factorial(int output[], int len, const int factorial_base);
void produce_n_print_each_factorial(const int n);

int main(void)
{
    // store input digits
    int digits[INPUT_LEN] = {0};
    puts("Enter numbers to be factorized: ");
    store_digits(digits);

    // produce and print each factorial
    for (int i = 0; i < INPUT_LEN; ++i) {
        if (digits[i] == 0)
            break;
        produce_n_print_each_factorial(digits[i]);
        putchar('\n');
    }
}

void store_digits(int digits[])
/* store digits in digits array
 * stop storing when 0 is encountered
 * digits: array to store digits
 */
{
    for (int i = 0; i < INPUT_LEN; ++i) {
        scanf("%d", &digits[i]);
        if (digits[i] == 0)
            break;
    }
}

void multiply(int output[], const int n)
/* multiply output array by n
 * output: array to undergo multiplication and store the result
 * n: number to multiply
 */
{
    int carry = 0;
    for (int i = INPUT_LEN - 1; i >= 0; --i) {
        int tmp = output[i] * n + carry;
        output[i] = tmp % 10;
        carry = tmp / 10;
    }
}

void produce_factorial(const int n, int output[])
/* produce factorial of n and store it in output array
 * output: array to store the result
 * n: number to produce factorial
 */
{
    output[INPUT_LEN - 1] = 1;
    for (int i = 1; i <= n; ++i) {
        multiply(output, i);
    }
}

void print_factorial(int output[], int len, const int factorial_base)
/* print factorial in the form of n! = xxxxxxxxxx
 * output: array to print
 * len: length of output array
 * factorial_base: number to produce factorial
 */
{
    int *tmp = output;
    while (*tmp == 0) {
        ++tmp;
        --len;
    }
    printf("%d! = ", factorial_base);
    for (int i = 0; i < len; ++i)
        printf("%d", tmp[i]);
    putchar('\n');
}

void produce_n_print_each_factorial(const int n)
/* produce and print each factorial from 1 to n
 * n: number to produce factorial
 */
{
    for (int i = 1; i <= n; ++i) {
        int output[INPUT_LEN] = {0};
        produce_factorial(i, output);
        print_factorial(output, INPUT_LEN, i);
    }
}
