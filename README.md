# Mastermind_C
A simple CLI based Mastermind game in C.

## Usage
If you choose Classic mode you can place 6 digits (1-6 representing the 6 available colors of the original game) in 4 spots, and you have 12 guesses to find the correct sequence. In "Super" mode you have 8 digits (1-8), 5 spots and still 12 guesses.

You can also chose to create a game with custom options, chosing between 2 to 10 digits (1-9 and 0 in the latter case), 2 to 10 spots and an arbitrary number of guesses (0 for unlimited).

## Rules
You probably already know the rules of the game, but I'll recap them quickly:

The game will generate a random digit sequence based on the chosen game mode, without showing it to you. 

E.g. in Classic mode it could be 3461, 3335, 1662 etc.
In "Super" mode it could be 83652, 11122, 54251 etc.
In Custom mode with 3 digits and 10 spots it could be 1221331232, 1112322231, 3322111232 etc.

Your task is to find this sequence using as few guesses as possible, at least within the limit given by the game mode or your choice in custom mode.

When you entered a guess, the game will evaluate the input and output a hint as a sequence of black and/or white dot (🌕 and/or 🌑), giving you information on how good your guess was. A black dot means that you have a correct digit at the correct position. A white dot means that you have a digit in your guess which is also present in the sequence, but it is not at the correct spot (not counting digits which were already marked with a black dot). The position of the dots is independent of the position of the digits in your guess. Black dots will always appear first, followed by white dots.

E.g. if the hidden sequence is 1234:

`guess | hint     | comment (not shown in the game)
------+----------+-------------------------------------------------------------
1561  | 🌕       | a black dot for the first 1
4321  | 🌑🌑🌑🌑 | all digits are there, but none in the correct spot
2154  | 🌕🌑🌑   | 1 and 2 are correct but in the wrong spot, a black dot for 4`

## To do's
- Let the user chose a time limit instead (or in addition to) a number of guesses
- Show the number of guesses and/or time needed to get the solution
- Allow the user to edit a guess before submission
- General cleanup
