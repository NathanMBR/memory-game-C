#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ASCII 65
#define SCORE_WIN 100
#define SCORE_LOSE 25

// Gets a valid position for the fillBoard function
void getValidPosition(char **memBoard, int *x, int *y, int *x1, int *y1, int *x2, int *y2) {
	*x1 = rand() % *x;
	*y1 = rand() % *y;
	*x2 = rand() % *x;
	*y2 = rand() % *y;
	
	if (memBoard[*x1][*y1] != 'X' || memBoard[*x2][*y2] != 'X' || *x1 == *x2 && *y1 == *y2)
		getValidPosition(memBoard, x, y, x1, y1, x2, y2);
}

// Fills the board randomly with pairs of characters
void fillBoard(char **memBoard, int *x, int *y) {
	int lettersQuantity = *x * *y / 2;
	char *letters = malloc(sizeof(char) * lettersQuantity);
	int i, x1, y1, x2, y2;
	
	for (i = 0; i < lettersQuantity; i++)
		letters[i] = (ASCII + i);
	
	
	for (i = 0; i < lettersQuantity; i++) {
		getValidPosition(memBoard, x, y, &x1, &y1, &x2, &y2);
		memBoard[x1][y1] = letters[i];
		memBoard[x2][y2] = letters[i];
	}
}

// Shows the score in the screen
void showScore(int *score) {
	printf("Score: %i\n\n", *score);
}

// Shows temporarily the board in the screen. Time is based in difficulty
void showBoardTemp(char **memBoard, int *x, int *y, int *difficulty) {
	int sleepTime = 8 - 2 * *difficulty, i, j;
	char command[13] = "timeout /T ", sleepTimeStr[2];
	
	snprintf(sleepTimeStr, 13, "%i", sleepTime);
	strcat(command, sleepTimeStr);
	
	printf("You have %i seconds to memorize:\n\n", sleepTime);
	
	for (i = 0; i < *x; i++) {
		for (j = 0; j < *y; j++)
			printf("%c ", memBoard[i][j]);
		printf("\n");
	}
	
	system(command);
	system("cls");
}

// Shows the board in the screen
void showBoard(char **board, int *x, int *y) {
	int i, j;
	
	for (i = 0; i < *x; i++) {
		for (j = 0; j < *y; j++)
			printf("%c ", board[i][j]);
		printf("\n");
	}
}

// Fills the board with 'X' characters
void clearBoard(char **board, int *x, int *y) {
	int i, j;
	
	for (i = 0; i < *x; i++)
		for (j = 0; j < *y; j++)
			board[i][j] = 'X';
}

// Allocates memory to a board and returns it
char **createBoard(int *x, int *y) {
	int i;
	
	char **board = malloc(sizeof(char *) * *x);
	for (i = 0; i < *x; i++)
		board[i] = malloc(sizeof(char) * *y);
	
	return board;
}

// Defines the width and height of the board
void defineBoardSize(int *difficulty, int *x, int *y) {
	switch(*difficulty) {
		case 1:
			// 12 cards (6 letters)
			*x = 3;
			*y = 4;
			break;
		case 2:
			// 20 cards (10 letters)
			*x = 4;
			*y = 5;
			break;
		default:
			// 40 cards (20 letters)
			*x = 5;
			*y = 8;
	}
}

// Gets the coordinates chosen by the player
void getPlayerAction(char **board, int *x0, int *y0, int *x, int *y) {
	scanf("%i %i", x, y);
	*x = *x - 1;
	*y = *y - 1;
	
	if (*x < 0 || *x > *x0 - 1 || *y < 0 || *y > *y0 - 1) {
		printf("Invalid coordinate. Please, try again: ");
		getPlayerAction(board, x0, y0, x, y);
	} else {
		if (board[*x][*y] != 'X') {
			printf("You can't choose a coordinate that you've already guessed. Please, choose again: ");
		}
	}
}

// Does the player action
void playerAction(char **board, char **memBoard, int *x, int *y, int *score) {
	int x1, y1, x2, y2, i, j;
	
	do {
		printf("\nType the first coordinate (Y, X): ");
		getPlayerAction(board, x, y, &x1, &y1);
		
		printf("Type the second coordinate (Y, X): ");
		getPlayerAction(board, x, y, &x2, &y2);
		
		if (x1 == x2 && y1 == y2) {
			system("cls");
			showScore(score);
			showBoard(board, x, y);
			printf("\nThe coordinates can't be the same!");
		}
	} while (x1 == x2 && y1 == y2);
	
	system("cls");
	
	for (i = 0; i < *x; i++) {
		for (j = 0; j < *y; j++) {
			if (i == x1 && j == y1 || i == x2 && j == y2)
				printf("%c ", memBoard[i][j]);
			else
				printf("%c ", board[i][j]);
		}
		printf("\n");
	}
	
	printf("\n");
	
	if (memBoard[x1][y1] == memBoard[x2][y2]) {
		*score = *score + SCORE_WIN;
		printf("You scored!");
		board[x1][y1] = memBoard[x1][y1];
		board[x2][y2] = memBoard[x2][y2];
	} else {
		*score = *score - SCORE_LOSE;
		printf("You didn't scored...");
	}
	
	printf(" Score: %i\n", *score);
	system("pause");
}

// Ends the game
void endGame(int *score, int *x, int *y) {
	int lettersQuantity = *x * *y / 2, choose;
	
	void startGame(int difficulty);
	
	system("cls");
	printf("You won! Score: %i\n", *score);
	if (*score == lettersQuantity * SCORE_WIN)
		printf("Congratulations, you got the maximum score for this difficulty!\n");
	printf("\nWould you like to play again?");
	printf("\n1 - Yes");
	printf("\n2 - No\n\n");
	
	do {
		scanf("%i", &choose);
		if (choose < 0 || choose > 2)
			printf("\nInvalid option. Please, try again: ");
	} while (choose < 0 || choose > 2);
	
	system("cls");
	
	if (choose == 1)
		startGame(chooseDifficulty());
	else
		menu();
}

// Verifies if the player has won
int verifyWin (char **board, int *x, int *y) {
	int win = 1, i, j;
	
	for (i = 0; i < *x; i++)
		for (j = 0; j < *y; j++)
			if (board[i][j] == 'X')
				win = 0;
	
	return win;
}

// Does the cicle of the gameplay
void loopGame(char **board, char **memBoard, int *x, int *y, int *score, int *isGameRunning) {
	if (*isGameRunning == 1) {
		system("cls");
		showScore(score);
		showBoard(board, x, y);
		
		playerAction(board, memBoard, x, y, score);
		if (verifyWin(board, x, y) == 1)
			*isGameRunning = 0;
		
		loopGame(board, memBoard, x, y, score, isGameRunning);
	} else {
		endGame(score, x, y);
	}
}

// Starts the game
void startGame(int difficulty) {
	int x, y, score = 0, isGameRunning = 1;
	
	defineBoardSize(&difficulty, &x, &y);
	char **board = createBoard(&x, &y);
	char **memBoard = createBoard(&x, &y);
	
	clearBoard(board, &x, &y);
	clearBoard(memBoard, &x, &y);
	fillBoard(memBoard, &x, &y);
	
	system("cls");
	showBoardTemp(memBoard, &x, &y, &difficulty);
	
	loopGame(board, memBoard, &x, &y, &score, &isGameRunning);
}

// Chooses the difficulty
int chooseDifficulty(void) {
	int difficulty;
	
	printf("Choose a difficulty:\n");
	printf("1 - Easy\n");
	printf("2 - Medium\n");
	printf("3 - Hard\n\n");
	scanf("%i", &difficulty);
	
	if (difficulty >= 1 && difficulty <= 3) {
		return difficulty;	
	} else {
		system("cls");
		printf("Invalid option. Please, try again.\n\n");
		return chooseDifficulty();
	}
}

// Exhibits the tutorial
void showTutorial(void) {
	system("cls");
	printf("A square board filled with X letters is displayed on the screen; the player must choose two positions to reveal its true letters.\n");
	printf("If they're the same, the player gains %i points; otherwise, he loses %i points.\n", SCORE_WIN, SCORE_LOSE);
	printf("The game is over when all the pairs of letters are found.\n");
	printf("The goal of the game is to obtain the most number of points possible.\n\n");
}

// Exhibits the menu
int menu(void) {
	int option, i;
	
	printf("Choose an option:\n");
	printf("1 - Play\n");
	printf("2 - Tutorial\n");
	printf("3 - Exit\n\n");
	scanf("%i", &option);
	printf("\n");
	
	if (option == 1 || option == 3) {
		return option;
	} else if (option == 2) {
		showTutorial();
		return menu();
	} else {
		system("cls");
		printf("Invalid option. Please, try again.\n\n");
		return menu();
	}
}

// Start of the program
int main(void) {
	srand(time(NULL));
	
	printf("-----| MEMORY GAME |-----\n\n");
	
	int menuOption = menu();
	system("cls");
	if (menuOption == 1) {
		startGame(chooseDifficulty());
	}
	
	return 0;
}