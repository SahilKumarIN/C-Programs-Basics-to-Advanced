#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

// Function to convert string to lowercase and remove spaces
void preprocessString(char *str, char *result) {
    int j = 0;
    for (int i = 0; str[i]; i++) {
        if (str[i] != ' ') {
            result[j++] = tolower(str[i]);
        }
    }
    result[j] = '\0';
}

// Method 1: Using character frequency count
int isAnagramMethod1(char *str1, char *str2) {
    char s1[1000], s2[1000];
    preprocessString(str1, s1);
    preprocessString(str2, s2);
    
    // If lengths are different, they can't be anagrams
    if (strlen(s1) != strlen(s2)) {
        return 0;
    }
    
    // Count frequency of each character (assuming ASCII)
    int count[256] = {0};
    
    // Count characters in first string
    for (int i = 0; s1[i]; i++) {
        count[(unsigned char)s1[i]]++;
    }
    
    // Subtract count for characters in second string
    for (int i = 0; s2[i]; i++) {
        count[(unsigned char)s2[i]]--;
    }
    
    // Check if all counts are zero
    for (int i = 0; i < 256; i++) {
        if (count[i] != 0) {
            return 0;
        }
    }
    
    return 1;
}

// Method 2: Using sorting
int compareChars(const void *a, const void *b) {
    return (*(char*)a - *(char*)b);
}

int isAnagramMethod2(char *str1, char *str2) {
    char s1[1000], s2[1000];
    preprocessString(str1, s1);
    preprocessString(str2, s2);
    
    // If lengths are different, they can't be anagrams
    if (strlen(s1) != strlen(s2)) {
        return 0;
    }
    
    // Sort both strings
    qsort(s1, strlen(s1), sizeof(char), compareChars);
    qsort(s2, strlen(s2), sizeof(char), compareChars);
    
    // Compare sorted strings
    return strcmp(s1, s2) == 0;
}

// Function to display menu
void displayMenu() {
    printf("\n=== ANAGRAM CHECKER ===\n");
    printf("1. Check if two strings are anagrams (Method 1 - Frequency Count)\n");
    printf("2. Check if two strings are anagrams (Method 2 - Sorting)\n");
    printf("3. Find all anagrams from a list of words\n");
    printf("4. Exit\n");
    printf("Enter your choice: ");
}

// Function to find anagrams in a list of words
void findAnagramsInList() {
    int n;
    printf("Enter number of words: ");
    scanf("%d", &n);
    getchar(); // consume newline
    
    char words[n][100];
    printf("Enter %d words:\n", n);
    for (int i = 0; i < n; i++) {
        printf("Word %d: ", i + 1);
        fgets(words[i], sizeof(words[i]), stdin);
        words[i][strcspn(words[i], "\n")] = 0; // remove newline
    }
    
    printf("\nAnagram groups:\n");
    int used[n];
    for (int i = 0; i < n; i++) {
        used[i] = 0;
    }
    
    for (int i = 0; i < n; i++) {
        if (used[i]) continue;
        
        int found = 0;
        printf("Group: %s", words[i]);
        used[i] = 1;
        found = 1;
        
        for (int j = i + 1; j < n; j++) {
            if (!used[j] && isAnagramMethod1(words[i], words[j])) {
                printf(", %s", words[j]);
                used[j] = 1;
                found = 1;
            }
        }
        
        if (found) {
            printf("\n");
        }
    }
}

int main() {
    int choice;
    char str1[1000], str2[1000];
    
    while (1) {
        displayMenu();
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter first string: ");
                fgets(str1, sizeof(str1), stdin);
                str1[strcspn(str1, "\n")] = 0; // remove newline
                
                printf("Enter second string: ");
                fgets(str2, sizeof(str2), stdin);
                str2[strcspn(str2, "\n")] = 0; // remove newline
                
                if (isAnagramMethod1(str1, str2)) {
                    printf("✓ '%s' and '%s' are anagrams! (Method: Frequency Count)\n", str1, str2);
                } else {
                    printf("✗ '%s' and '%s' are NOT anagrams.\n", str1, str2);
                }
                break;
                
            case 2:
                printf("Enter first string: ");
                fgets(str1, sizeof(str1), stdin);
                str1[strcspn(str1, "\n")] = 0; // remove newline
                
                printf("Enter second string: ");
                fgets(str2, sizeof(str2), stdin);
                str2[strcspn(str2, "\n")] = 0; // remove newline
                
                if (isAnagramMethod2(str1, str2)) {
                    printf("✓ '%s' and '%s' are anagrams! (Method: Sorting)\n", str1, str2);
                } else {
                    printf("✗ '%s' and '%s' are NOT anagrams.\n", str1, str2);
                }
                break;
                
            case 3:
                findAnagramsInList();
                break;
                
            case 4:
                printf("Thank you for using the Anagram Checker!\n");
                exit(0);
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }
    
    return 0;
}
