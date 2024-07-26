#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>
#include <time.h>
#include <string.h>

#define VERSION "0.5"
#define MAXOPTIONS 4


struct termios saved_attributes;

// Reset terminal to the state it had before
void reset_input_mode(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &saved_attributes);
}


// Set terminal to raw mode
void set_raw_mode(void) {
    struct termios tattr;

    if (!isatty (STDIN_FILENO)) {
        fprintf(stderr, "Not a terminal.\n");
        exit(EXIT_FAILURE);
    }

    tcgetattr(STDIN_FILENO, &saved_attributes);
    atexit(reset_input_mode);

    tcgetattr(STDIN_FILENO, &tattr);
    tattr.c_lflag &= ~(ICANON|ECHO);
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &tattr);
    setvbuf(stdout, NULL, _IONBF, 0);
}


// Evaluate useres guess
void evaluate(char *set, int digits, int spots, int guesses, int timelimit) {
    char guess[10] = {0};
    char found[10] = {0};
    char c;
    int correct = 0, semi;

    int guesscount = guesses == 0 ? 1 : 0;
    time_t start = time(NULL);

    while (correct < spots && guesses != guesscount) {
        if (timelimit > 0 && timelimit < (int)(time(NULL) - start)) {
            timelimit = 1;
        break;
                                                    }
        correct = 0;
        semi = 0;
        guesscount++;

        memcpy(found, set, spots * sizeof(*set));

        for (int i = 0; i < spots;) {
            read(STDIN_FILENO, &c, 1);
            
            if (c < '0' || c > digits + '0')  
                continue;
                
            guess[i] = c - '0';
            printf("%c ", c);
             
            if (guess[i] == set[i]) {
                correct++;
                found[i] = 99;
            }

            i++;
        }

        for (int i = 0; i < spots; i++)
            for (int j = 0; j < spots; j++)
                if (guess[i] == found[j]) {
                    found[j] = 99;
                    semi++;
                    break;
                }

        printf("| ");

        for (int i = 0; i < correct; i++)
            printf("○ ");
        for (int i = 0; i < semi; i++)
            printf("● ");

        printf("\n");
    }
    
    if (correct == spots) {
        printf("Congratulations! You solved the puzzle in %d seconds, using %d guesses.\n\n",
               (int)(time(NULL) - start), guesses == 0 ? --guesscount : guesscount);
    }
    else if (timelimit == 1)
        printf("Sorry, time's up!\n\n");
    else
        printf("Sorry, you didn't solve the puzzle within %d guesses.\n\n", guesscount);
}


// Start a new game
void new_game(int digits, int spots, int guesses, int timelimit) {
    char set[10] = {0};

    for (int i = 0; i < spots; i++) {
        set[i] = (1 + rand() % digits) % 10;
    }

    printf("Please enter your first guess:\n\n");
    set_raw_mode();
    evaluate(set, digits, spots, guesses, timelimit);
}


// Get parameters for a new game with custom settings
void custom_game(void) {
    char buf[10];
    char *endptr;
    int digits = 0, spots = 0, guesses = -1, timelimit = -1;

    while (digits < 2 || digits > 10) {
        printf("Please enter the number of possible digits (2-10): ");
        fgets(buf, 10, stdin);
        digits = strtol(buf, &endptr, 10);
    }

    printf("\n");

    while (spots < 2 || spots > 10) {
        printf("Please enter the number of spots (2-10): ");
        fgets(buf, 10, stdin);
        spots = strtol(buf, &endptr, 10);
    }

    printf("\n");

    while (guesses < 0) {
        printf("Please enter the number of guesses (0 for unlimited): ");
        fgets(buf, 10, stdin);
        guesses = strtol(buf, &endptr, 10); 
    }

    printf("\n");

    while (timelimit < 0) {
        printf("Please enter the time limit in seconds (0 for unlimited): ");
        fgets(buf, 10, stdin);
        timelimit = strtol(buf, &endptr, 10);
    }

    new_game(digits, spots, guesses, timelimit);
}


// Display the menu and get choice
void menu(void) {
    char buf[10] = {0};
    char *endptr;
    int choice = 0;

    printf("+-----------------------------------------------------------------------------+\n"
           "|            Main menu - Enter your choice as a number from 1 to %d            |\n"
           "+-----------------------------------------------------------------------------+\n"
           "| 1 - New game in Classic mode (6 digits, 4 spots, 12 guesses, no time limit) |\n"
           "| 2 - New game in \"Super\" mode (8 digits, 5 spots, 12 guesses, no time limit) |\n"
           "| 3 - New game with custom settings                                           |\n"
           "| 4 - Quit (You can exit anytime by pressing Ctrl+C)                          |\n"
           "+-----------------------------------------------------------------------------+\n\n"
           "-> ", MAXOPTIONS);

    while(!choice) {
        fgets(buf, 10, stdin);
        choice = strtol(buf, &endptr, 10);

        switch (choice) {
            case 1: new_game(6, 4, 12, 0); break;
            case 2: new_game(8, 5, 12, 0); break;
            case 3: custom_game(); break;
            case 4: exit(0); break;
            default: {
                printf("Invalid option, please enter a number from 1 to %d\n-> ", MAXOPTIONS);
                choice = 0;
            }
            break;
        }
    }
}


int main(int argc, char **argv) {
    srand(time(NULL));
    printf("\nWelcome to Mastermind_C version %s\n\n", VERSION);
    char c = 0;

    if (argc > 1) {
        if (!strcmp(argv[1], "classic"))
            new_game(6, 4, 12, 0);
        else if (!strcmp(argv[1], "super"))
            new_game(8, 5, 12, 0);
        else if (!strcmp(argv[1], "custom"))
            custom_game();
        else
            menu();
    }
    else
        menu();

    return EXIT_SUCCESS;
}
