#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define WORD_LEN 5
#define MAX_ATTEMPTS 6

// ANSI color codes
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define GREY "\033[1;90m"
#define RESET "\033[0m"

// Word list (expand as needed)
const char *word_list[] = {
    "apple", "river", "storm", "plane", "bring",
    "table", "crane", "peace", "light", "sound",
    "brave", "flame", "sword", "track", "sharp"
};
const int WORD_COUNT = sizeof(word_list) / sizeof(word_list[0]);

// Function declarations
int is_valid_word(const char *word);
void print_feedback(const char *guess, const char *target);
void to_lowercase(char *str);
void clear_input_buffer(void);
int play_wordle_round(void);

int main(void) {
    srand((unsigned)time(NULL));

    printf("\n===== WORDLE IN C =====\n");
    printf("Guess the %d-letter word in %d attempts!\n", WORD_LEN, MAX_ATTEMPTS);

    char choice;
    do {
        int result = play_wordle_round();
        if (result)
            printf(GREEN "\n🎉 You won!\n" RESET);
        else
            printf(GREY "\n❌ Better luck next time.\n" RESET);

        printf("\nPlay again? (Y/N): ");
        scanf(" %c", &choice);
        clear_input_buffer();
        choice = tolower(choice);
    } while (choice == 'y');

    printf("\nThanks for playing Wordle in C!\n");
    return 0;
}

/**
 * Play one full round of Wordle (returns 1 if player wins, 0 otherwise)
 */
int play_wordle_round(void) {
    const char *target = word_list[rand() % WORD_COUNT];
    char guess[WORD_LEN + 10]; // extra space for safety
    int attempts = 0;

    while (attempts < MAX_ATTEMPTS) {
        printf("\nAttempt %d/%d: ", attempts + 1, MAX_ATTEMPTS);
        if (!fgets(guess, sizeof(guess), stdin))
            return 0;

        guess[strcspn(guess, "\n")] = '\0'; // remove newline
        to_lowercase(guess);

        if (strlen(guess) != WORD_LEN) {
            printf(GREY "Word must be %d letters long.\n" RESET, WORD_LEN);
            continue;
        }

        if (!is_valid_word(guess)) {
            printf(GREY "Invalid word. Try again.\n" RESET);
            continue;
        }

        print_feedback(guess, target);
        attempts++;

        if (strcmp(guess, target) == 0) {
            printf(GREEN "\nYou guessed it in %d attempts!\n" RESET, attempts);
            return 1;
        }
    }

    printf("\nThe correct word was: " YELLOW "%s\n" RESET, target);
    return 0;
}

/**
 * Check if word is in the word list
 */
int is_valid_word(const char *word) {
    for (int i = 0; i < WORD_COUNT; i++) {
        if (strcmp(word, word_list[i]) == 0)
            return 1;
    }
    return 0;
}

/**
 * Convert string to lowercase
 */
void to_lowercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower((unsigned char)str[i]);
}

/**
 * Flush remaining stdin characters (after scanf)
 */
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * Print feedback with colors:
 *  - Green: correct letter & position
 *  - Yellow: correct letter wrong position
 *  - Grey: not in word
 */
void print_feedback(const char *guess, const char *target) {
    int target_used[WORD_LEN] = {0};
    char feedback[WORD_LEN];

    // Step 1: mark greens first
    for (int i = 0; i < WORD_LEN; i++) {
        if (guess[i] == target[i]) {
            feedback[i] = 'G'; // green
            target_used[i] = 1;
        } else {
            feedback[i] = ' ';
        }
    }

    // Step 2: mark yellows
    for (int i = 0; i < WORD_LEN; i++) {
        if (feedback[i] == 'G')
            continue;
        for (int j = 0; j < WORD_LEN; j++) {
            if (!target_used[j] && guess[i] == target[j]) {
                feedback[i] = 'Y'; // yellow
                target_used[j] = 1;
                break;
            }
        }
    }

    // Step 3: print colored feedback
    printf("Result: ");
    for (int i = 0; i < WORD_LEN; i++) {
        if (feedback[i] == 'G')
            printf(GREEN "%c " RESET, toupper(guess[i]));
        else if (feedback[i] == 'Y')
            printf(YELLOW "%c " RESET, toupper(guess[i]));
        else
            printf(GREY "%c " RESET, toupper(guess[i]));
    }
    printf("\n");
}
