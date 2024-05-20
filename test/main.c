#include <stdio.h>
#include <string.h>

int main(void)
{
    char input[] = "This si a test of the tokenizor seven";
    char *tmp = strtok(input, " ");
    while (tmp != NULL) {
        printf("%s\n", tmp);
        tmp = strtok(NULL, " ");
    }
    printf("%s\n", input);
}
