#include "pa1.h"

int main(void) {
    char line[MAX_LENGTH];
    
    while(fgets(line, MAX_LENGTH, stdin)) {
        int len = strlen(line) - 1;
        line[len] = '\0';

        if (is_singleton(line))
            printf("singleton\n");
        if (is_arithmetic(line))
            printf("arithmetic\n");
        if (is_reverse_arithmetic(line))
            printf("reverse arithmetic\n");
        if (is_balanced_tripartite(line))
            printf("balanced tripartite\n");
        if (is_balanced_bipartite(line))
            printf("balanced bipartite\n");
        if (is_palindrome(line))
            printf("palindrome\n");
        printf("\n");

    }
}