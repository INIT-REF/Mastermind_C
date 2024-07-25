#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>

#define VERSION "0.2"
#define MAXOPTIONS 4


struct termios saved_attributes;
void reset_input_mode(void);
void set_raw_mode(void);
void evaluate(char* set, int digits, int spots, int guesses, int timelimit);
void new_game(int digits, int spots, int guesses, int timelimit);
void custom_game(void);
void menu(void);


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
void evaluate(char* set, int digits, int spots, int guesses, int timelimit) {
    char* guess = (char*)malloc(spots * sizeof(char));
    char* found = (char*)malloc(spots * sizeof(char));
    int correct = 0, semi;

    int guesscount = guesses == 0 ? 1 : 0;
    time_t start = time(NULL);

    while (correct < spots && guesses != guesscount) {
        if (timelimit > 0 && timelimit < (int)(time(NULL) - start)) {
            timelimit = 0;
        break;
                                                    }
        correct = 0;
        semi = 0;
        guesscount++;

        memcpy(found, set, sizeof(set));

        for (int i = 0; i < spots; i++) {
            read(STDIN_FILENO, &guess[i], 1);
            guess[i] -= '0';
            if (guess[i] == set[i]) {
                correct++;
                found[i] = 99;
            }
        }

        for (int i = 0; i < spots; i++)
            for (int j = 0; j < spots; j++)
                if (guess[i] == found[j]) {
                    found[j] = 99;
                    semi++;
                }

        printf(" | ");

        for (int i = 0; i < correct; i++)
            printf("○ ");
        for (int i = 0; i < semi; i++)
            printf("● ");

        printf("\n");
    }

    free(found);
    free(guess);
    
    if (correct == spots) {
        printf("Congratulations! You solved the puzzle in ");
        printf("%d seconds, ", (int)(time(NULL) - start));
        printf("using %d guesses.\n\n", guesses == 0 ? --guesscount : guesscount);
    }
    else if (timelimit == 0)
        printf("Sorry, time's up!\n\n");
    else
        printf("Sorry, you didn't solve the puzzle within %d guesses.\n\n", guesscount);
}


// Start a new game
void new_game(int digits, int spots, int guesses, int timelimit) {
    char* set = (char*)malloc(spots * sizeof(char));

    for (int i = 0; i < spots; i++) {
        set[i] = (1 + rand() % digits) % 10;
    }

    printf("Please enter your first guess:\n\n");
    set_raw_mode();
    evaluate(set, digits, spots, guesses, timelimit);
    free(set);
}


// Get parameters for a new game with custom settings
void custom_game(void) {
    int digits, spots, guesses, timelimit;

    printf("\nPlease enter the number of possible digits (2-10): ");
    scanf("%d", &digits);
    while (digits < 2 || digits > 10) {
        printf("Invalid number of digits, please enter a number from 2 to 10: ");
        scanf("%d", &digits);
    }

    printf("\nPlease enter the number of spots (2-10): ");
    scanf("%d", &spots);
    while (spots < 2 || spots > 10) {
        printf("Invalid number of spots, please enter a number from 2 to 10: ");
        scanf("%d", &spots);
    }

    printf("\nPlease enter the number of guesses (0 for unlimited): ");
    scanf("%d", &guesses);
    while (guesses < 0) {
        printf("Invalid number of guesses, please enter a positive number (or 0 for unlimited): ");
        scanf("%d", &guesses);
    }

    printf("\nPlease enter the time limit in seconds (0 for unlimited): ");
    scanf("%d", &timelimit);
    while (timelimit < 0) {
        printf("Invalid time limit, please enter a positive number (or 0 for unlimited): ");
        scanf("%d", &timelimit);
    }

    new_game(digits, spots, guesses, timelimit);
}


// Display the menu and get user's choice
void menu(void) {
    char c = 0;

    printf("+-----------------------------------------------------------------------------+\n");
    printf("|            Main menu - Enter your choice as a number from 1 to %d            |\n", MAXOPTIONS);
    printf("+-----------------------------------------------------------------------------+\n");
    printf("| 1 - New game in Classic mode (6 digits, 4 spots, 12 guesses, no time limit) |\n");
    printf("| 2 - New game in \"Super\" mode (8 digits, 5 spots, 12 guesses, no time limit) |\n");
    printf("| 3 - New game with custom settings                                           |\n");
    printf("| 4 - Quit (You can exit anytime by pressing Ctrl+C)                          |\n");
    printf("+-----------------------------------------------------------------------------+\n\n");
    printf("-> ");

    while(!c) {
        c = getchar();
        switch (c) {
            case '1': new_game(6, 4, 12, 0); break;
            case '2': new_game(8, 5, 12, 0); break;
            case '3': custom_game(); break;
            case '4': exit(0); break;
            default: {
                printf("Invalid option, please enter a number from 1 to %d\n-> ", MAXOPTIONS);
                while((c = getchar()) != '\n' && c != EOF);
                c = 0;
            }
            break;
        }
    }
}


int main(int argc, char **argv) {
    srand(time(NULL));
    printf("\nWelcome to Mastermind_C version %s\n\n", VERSION);
    menu();

    return EXIT_SUCCESS;
}
