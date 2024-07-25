#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>

#define VERSION "0.1"
#define MAXOPTIONS 4


struct termios saved_attributes;


// Reset terminal to the state it had before
void reset_input_mode(void) {
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}


// Set terminal to raw mode
void set_raw_mode(void) {
    struct termios tattr;
    char *name;

    if (!isatty (STDIN_FILENO)) {
        fprintf (stderr, "Not a terminal.\n");
        exit (EXIT_FAILURE);
    }

    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit (reset_input_mode);

    tcgetattr (STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON);
    tattr.c_cc[VMIN] = 1;
    tattr.c_cc[VTIME] = 0;
    tcsetattr (STDIN_FILENO, TCSAFLUSH, &tattr);
}


// Evaluate useres guess
void evaluate(char* set, int d, int s, int g) {
    char* guess = (char*)malloc(s * sizeof(char));
    char* found = (char*)malloc(s * sizeof(char));
    int correct = 0, semi;

    for (int i = 0; i < s; i++)
        printf("%d", set[i]);
    printf("\n\n");

    while (correct < s && g--) {
        correct = 0;
        semi = 0;
        memset(found, 0, sizeof(found));

        for (int i = 0; i < s; i++) {
            read(STDIN_FILENO, &guess[i], 1);
            guess[i] -= '0';
            if (guess[i] == set[i]) {
                correct++;
                found[i] = 1;
            }
        }

        for (int i = 0; i < s; i++)
            for (int j = 0; j < s; j++) 
                if (guess[i] == set[j] && !found[i])
                    semi++;

        printf(" | ");

        for (int i = 0; i < correct; i++)
            printf("○ ");
        for (int i = 0; i < semi; i++)
            printf("● ");

        printf("\n");
    }
    
    if (correct == s)
        printf("Congratulations!\n");
    else
        printf("Sorry\n");
}


// Start a new game
void new_game(int d, int s, int g) {
    char* set = (char*)malloc(s * sizeof(char));

    if (g == 0)
        printf("New game with %d digits, %d spots and unlimited guesses\n", d, s);
    else
        printf("New game with %d digits, %d spots and %d guesses\n", d, s, g);

    for (int i = 0; i < s; i++) {
        set[i] = (1 + rand() % d) % 10;
    }

    printf("Please enter your first guess:\n\n");
    set_raw_mode();
    evaluate(set, d, s, g);
}


// Get parameters for a new game with custom settings
void custom_game(void) {
    int d, s, g;

    printf("\nPlease enter the number of possible digits (2-10): ");
    scanf("%d", &d);
    while (d < 2 || d > 10) {
        printf("Invalid number of digits, please enter a number from 2 to 10: ");
        scanf("%d", &d);
    }

    printf("\nPlease enter the number of spots (2-10): ");
    scanf("%d", &s);
    while (s < 2 || s > 10) {
        printf("Invalid number of spots, please enter a number from 2 to 10: ");
        scanf("%d", &s);
    }

    printf("\nPlease enter the number of guesses (0 for unlimited): ");
    scanf("%d", &g);
    while (g < 0) {
        printf("Invalid number of guesses, please enter a positive number (or 0 for unlimited): ");
        scanf("%d", &g);
    }

    new_game(d, s, g);
}


// Display the menu and get user's choice
void menu(void) {
    printf("+------------------------------------------------------------------------+\n");
    printf("|         Main menu - Enter your choice as a number from 1 to %d          |\n", MAXOPTIONS);
    printf("+------------------------------------------------------------------------+\n");
    printf("| 1 - Start a new game in Classic mode (6 digits, 4 spots, 12 guesses)   |\n");
    printf("| 2 - Start a new game in \"Super\" mode (8 digits, 5 spots, 12 guesses)   |\n");
    printf("| 3 - Start a new game with custom settings                              |\n");
    printf("| 4 - Quit (You can exit anytime by pressing Ctrl+C)                     |\n");
    printf("+------------------------------------------------------------------------+\n");
    
    switch (getchar()) {
        case '1': new_game(6, 4, 12); break;
        case '2': new_game(8, 5, 12); break;
        case '3': custom_game(); break;
        case '4': exit(0); break;
        default: printf("Invalid option, please enter a number from 1 to %d\n", MAXOPTIONS);
    }
}


int main(int argc, char **argv) {
    srand(time(NULL));
    printf("\nWelcome to Mastermind_C version %s\n\n", VERSION);
    menu();

    return EXIT_SUCCESS;
}
