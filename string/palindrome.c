#include <stdio.h>
#include <string.h>

static int is_palindrome(const char *s) {
    size_t left = 0;
    size_t right = strlen(s);
    if (right == 0) {
        return 1; // empty string is a palindrome
    }
    right--; // last valid index
    while (left < right) {
        if (s[left] != s[right]) {
            return 0;
        }
        left++;
        right--;
    }
    return 1;
}

int main(void) {
    char input[1024];

    if (!fgets(input, sizeof(input), stdin)) {
        return 1; // failed to read
    }

    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
        input[len - 1] = '\0'; // strip trailing newline
    }

    if (is_palindrome(input)) {
        printf("Palindrome\n");
    } else {
        printf("Not a palindrome\n");
    }

    return 0;
}


