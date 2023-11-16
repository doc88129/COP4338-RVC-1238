#include <string.h>

int is_arithmetic(char str[]) {

    for (int i = 1; i < strlen(str) - 1; i++) {
        if (str[i + 1] <= str[i])
            return 0;
    }
    return 1;
}

int is_reverse_arithmetic(char str[]) {
    for (int i = 1; i < strlen(str) - 1; i++) {
        if (str[i + 1] >= str[i])
            return 0;
    }
}

int is_balanced_bipartite(char str[]) {
    if (strlen(str) % 2 != 0 || strlen(str) < 2)
        return 0;

    int len = strlen(str);

    for(int i = 0; i < len / 2; i++) {
        if (str[i] != str[i + len / 2])
            return 0;
    }
    return 1;
}

int is_balanced_tripartite(char str[]) {
    if (strlen(str) % 3 != 0 || strlen(str) < 3)
        return 0;

    int len = strlen(str);

    for(int i = 0; i < len / 3; i++) {
        if (str[i] != str[i + len / 3] || str[i] != str[i + 2 * (len / 3)])
            return 0;
    }
    return 1;
}

int is_palindrome(char str[]) {
    int len = strlen(str);

    for(int i = 0; i < len; i++) {
        if (str[i] != str[len - i - 1])
            return 0;
    }
    return 1;
}

int is_singleton(char str[]) {
    char c = str[0];
    for (int i = 1; str[i] != '\0'; i++) {
        if (str[i] != c)
            return 0;
    }
    return 1;
}
