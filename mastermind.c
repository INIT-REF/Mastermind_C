#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <termios.h>

#define VERSION 0.1

struct termios saved_attributes;

// Reset terminal to the state it had before
void reset_input_mode (void) {
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes);
}

// Set terminal to raw mode
void set_raw_mode (void) {
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

void welcome(void) {
    printf("\nWelcome to Mastermind_C version VERSION\n");
}

void menu(void) {
}

// Evaluate useres guess
void evaluate(char* set, char* guess) {
    printf("🌑🌑🌑🌕🌕🌕\n");
}

int main(int argc, char **argv) {
    char c;

    set_raw_mode ();

    welcome();
    menu();

    while (1) {
        read (STDIN_FILENO, &c, 1);
        if (c == '\004')          /* C-d */
            break;
    }

  return EXIT_SUCCESS;
}
