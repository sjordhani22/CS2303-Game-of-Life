/*
 * production.c
 *
 *  Created on: January 22, 2018
 *      Author: Stefano Jordhani
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>

/**
 * production Takes in the user input
 * @param argc Number of words on the command line
 * @param argv Array of pointers to character strings representing the words on the command line
 * @return true if program was able to play the Game Of Life
 * 			false if user entered bad input or something went wrong
 */
bool production(int argc, char* argv[]) {
	bool results = false;
	bool ok2; //for opening file
	bool done = false;
	int nRows = -1;
	int nCols = -1;
	int gens = 0; //number of generations to play
	int howManyLinesInFile = 0;
	int maximumWidth = 0;
	char filename[100];
	for (int i = 0; i < 100; i++) {
		filename[i] = '\0';
	}
	char print = 'n';
	char pause = 'n';
	//etc.
	//get the NR NC gens input [print] [pause], Usage as needed.
	if (argc < 5)	//not all mandatory args provided
			{
		usage();
		done = true;
	}
	if (!done && (argc >= 6)) {
		print = argv[5][0]; // This would look at index 5 to grab the character print is looking for
	} else if (argc >= 7) {
		pause = argv[6][0]; // This would look at index 5 to grab the character pause is looking for
	}

	if (!done)	//must be greater than or equal to 5, so get the mandatory vals
	{

		char* ptr = 0;
		long nr_l = strtol(argv[1], &ptr, 10); // These couple of lines are taking information from the command line
		long nGens_l = strtol(argv[3], &ptr, 10); // and setting them to variables that we will use in other functions
		nRows = (int) nr_l;
		gens = (int) nGens_l;

		if (nRows < 1) {   // When there aren't any rows recognized
			printf("Usage: Rows should be greater than 0, received %d.\n",
					nRows);
			done = true;
		}

		long nc_l = strtol(argv[2], &ptr, 10); // get the number of columns
		nCols = (int) nc_l;
		if (nCols < 1) {
			printf("Usage: Columns should be greater than 0, recieved %d.\n",
					nCols);
			done = true;
		}

		if (nGens_l < 1) {
			printf(
					"Usage: generations should be greater than 0, recieved %d.\n",
					gens);
			done = true;
		}

		strcpy(filename, argv[4]); //strcpy copies a characters in a string
		//now we have the command line
		//Let's read the input file
		FILE* fp = fopen(filename, "r"); //we try to read it
		if (fp != false) { //it opened
			printf("Opened %s.\n", filename);
			ok2 = true; // this is where ok2 is used but if the file doesnt open then the warning "ok2 unused" will appear
			char oRow[100];


			bool doneReadingFile = false;
			while (!doneReadingFile) {
				oRow[0] = '\0';

				fscanf(fp, "%s", oRow);

				if (strlen(oRow) != 0)	//there was something there
						{
					howManyLinesInFile++; // updates rows
					if (maximumWidth < strlen(oRow)) { //Updates the maximumWidth based on what the file says
						maximumWidth = strlen(oRow);
					}
				} else {
					doneReadingFile = true;
					fclose(fp);
				}
			}

		} else {
			puts("Cannot find that file,"); //If file not found
			done = true; //Says we are done with the program
		}
		if (!done) {
			char A[nRows][nCols]; //Creating the arrays
			char B[nRows][nCols];
			char C[nRows][nCols];
			char* nextGen_p = &A[0][0]; // Creating the pointers to the arrays
			char* curr_p = &B[0][0];
			char* beforeCurr_p = &C[0][0];

			for (int row = 0; row < nRows; row++) {
				for (int col = 0; col < nCols; col++) {
					A[row][col] = 'o';   // We are initializing the arrays to be empty to start
					B[row][col] = 'o';
					C[row][col] = 'o';

				}
			}
			FILE* fp = fopen(filename, "r"); //we read it before, we expect we can read it again
			fileIntoArray(nRows, nCols, howManyLinesInFile, maximumWidth, // This is where we convert the file into an array
					nextGen_p, fp);

			int howManyGens = generate(gens, nRows, nCols, nextGen_p, curr_p, // Determines how many gens ran and sets
					beforeCurr_p, print, pause);							// it took the varaible "howManyGens"
			printf("Ran %d generations\n", howManyGens); //This is what prints in the console
		}
		fclose(fp); // We need to make sure to close the file
	}
	results = !done;
	return results;

}
/**
 * PlayOne carries out one generation
 * @param unsigned int nr, the number of rows in the array
 * @param unsigned int nc, the number of columns in the array
 * @param char* Old The pointer to the array of the original starting array
 * @param char* New The pointer to the array of the next generation array
 * @return There is none, because results are in New array
 */
void PlayOne(unsigned int nr, unsigned int nc, char* Old, char* New) {
	int nRows = nr;
	int nCols = nc;
	//loop over all locations in the new dish
	//figure out whether occupied or not
	for (int row = 0; row < nRows; row++) {
		for (int col = 0; col < nCols; col++) { //This for loop will be used to check each postition in the board
			//There are three rules,
			// use occupied in old
			//use neighbors in old
			bool occupied = true;
			occupied = (getLetter(row, col, nCols, Old) == 'x'); // occupied if there is a living cell

			if (occupied
					&& ((howManyNeighbors(row, col, nRows, nCols, Old) < 2)
							|| (howManyNeighbors(row, col, nRows, nCols, Old)
									> 3))) { //Death rule
				*(New + (row * nCols) + col) = 'o';
			} else if (occupied) {			        //Survival rule
				*(New + (row * nCols) + col) = 'x';
			} else if (howManyNeighbors(row, col, nRows, nCols, Old) == 3) {//Birth rule
				*(New + (row * nCols) + col) = 'x'; // Checks to see if there is three neighbors around the position
													// If there is a living cell will appear in the New array
			} else {
				*(New + (row * nCols) + col) = 'o'; // otherwise if the array doesnt meet any terminating or survival,
			}										// death, or birth rules then it will just remain an unoccupied space
		}
	}
}

/**
 * usage This function outputs an error message when the user enters a bad input
 * @return void There is no return becuase it prints out an error message
 */
void usage(void) {
	puts("Usage: HW2 NR NC gens input [print] [pause]");
}
/**
 * getLetter Finds whether there is a living ("X") OR dead ("o") cell in a certain position
 * @param int row This is the row of the desired position
 * @param int col This is the column of the desired position
 * @param int nCols This is the number of columns in the multi-dimensional array
 * @param char* Pointer to the array that contains the board
 * @return char This function returns the character that is locacted in the desired location
 */
char getLetter(int row, int col, int nCols, char* Old) {
	return *(Old + (row * nCols) + col);
}
/**
 * howManyNeighbors Checks to see how many living cells are neighboring a desired cell
 * @param int row The row of the cell we are looking for
 * @param int col The column of the cell we are looking for
 * @param int nRows The number of rows in the array
 * @param int nCols The number of columns in the array
 * @param char* Pointer to the array that contains the board
 * @return int Returns the number of living neighbor cells that a cell has
 */
int howManyNeighbors(int row, int col, int nRows, int nCols, char* Old) {
	int numNeighbors = 0; // This will keep track of all the neighbors
	//there could be as many as 8 neighbors
	//cells on an edge or corner have fewer neighbors
	//we will search for neighbors clockwise from NorthWest
	if (row > 0) {
		if (col > 0) {
			if (getLetter(row - 1, col - 1, nCols, Old) == 'x')	// checks the NW position
					{
				numNeighbors++;
			}
		}
		if (getLetter(row - 1, col, nCols, Old) == 'x')	// checks the N position
				{
			numNeighbors++;
		}
		if (col < (nCols - 1)) {
			if (getLetter(row - 1, col + 1, nCols, Old) == 'x')	// checks the NE position
					{
				numNeighbors++;
			}
		}
	}	//can look north
	if (col > 0) {
		if (getLetter(row, col - 1, nCols, Old) == 'x')	// checks the W position
				{
			numNeighbors++;
		}
	}
	if (col < (nCols - 1)) {
		if (getLetter(row, col + 1, nCols, Old) == 'x')	// checks the E position
				{
			numNeighbors++;
		}
	}
	if (row < nRows - 1) {
		if (col > 0) {
			if (getLetter(row + 1, col - 1, nCols, Old) == 'x')	// checks the SW position
					{
				numNeighbors++;
			}
		}
		if (getLetter(row + 1, col, nCols, Old) == 'x')	//checks the S position
				{
			numNeighbors++;
		}
		if (col < (nCols - 1)) {
			if (getLetter(row + 1, col + 1, nCols, Old) == 'x')	// checks the SE postion
					{
				numNeighbors++;
			}
		}
	}	//can look south

	return numNeighbors;
}

/**
 * fileIntoArray takes the given pattern file and translates it into a multidimensional array
 * @param int nRows The number of rows in the array
 * @param int nCols The number of columns in the array
 * @param int howManyLinesInFile The number of lines that are in the input pattern file
 * @param int maximumWidth The number of columns in the file
 * @param char* ar_p The pointer to the array that will be created from the file
 * @param FILE* fp The pointer to the file
 * @return There is no return because the results are in ar_p
 */
void fileIntoArray(int nRows, int nCols, int howManyLinesInFile,
		int maximumWidth, char* ar_p, FILE* fp) {
	for (int row = 0; row < nRows; row++) {
		for (int col = 0; col < nCols; col++) {
			*(ar_p + (row * nCols) + col) = 'o';
		}
	}
	for (int frow = 0; frow < howManyLinesInFile; frow++) { //Keeps looping until we have reached the number of lines in the file
		char fromFile[maximumWidth];

		for (int i = 0; i < maximumWidth; i++) { // Keeps on looping until we have reached the max width
			fromFile[i] = 'o';
		}
		fscanf(fp, "%s", fromFile);
		for (int fcol = 0; fcol < maximumWidth; fcol++) {
			if (fromFile[fcol] == 'x') {
				int targetRow = frow + (nRows - howManyLinesInFile) / 2;
				int targetCol = fcol + (nCols - maximumWidth) / 2;
				*(ar_p + (targetRow * nCols) + targetCol) = 'x';

			}
		}
	}
}

/**
 * generate Plays through the number of generations requested
 * @param int gens The number of generations that the function should play
 * @param int nRows The number of rows in the array
 * @param int nCols The number of columns in the array
 * @param char* nextGen_p The pointer to the array for the next generation created
 * @param char* curr_p The pointer to the array of the current generation we are on
 * @param char* beforeCurr_p The pointer to the array of the previous generation
 * @param char print This is the character that is inputted by the user that decides if the program should print after every generation
 * @param char pause This is the character that decides if the program should pause after every generation
 * @return int The number of generations played
 */
int generate(int gens, int nRows, int nCols, char* nextGen_p, char* curr_p,
		char* beforeCurr_p, char print, char pause) {
	int numGen = 0;
	bool allOrganismsDead = false;
	bool patternRepeated = false;
	bool done = false;
	bool firstTime = true;
			// Will keep looping until the program is not done and as long as it has not reached the number of gens
	for (int gensDone = 0; !done && (gensDone < gens); gensDone++) {
		if (!anyX(nextGen_p, nRows, nCols)) { //all organisms are dead
			allOrganismsDead = true;
			done = true;
			puts("All organisms dead.");
			printThis(nRows, nCols, nextGen_p);
		}
		PlayOne(nRows, nCols, nextGen_p, curr_p); // We would just call playOne to generate our generations for us as
		// many times as we need
		numGen++;
		if (sameContent(nextGen_p, curr_p, nRows, nCols)) { // We will check to see if the pattern is the same
			patternRepeated = true;
			done = true;
			puts("Pattern repeated in one generation.");
			printThis(nRows, nCols, nextGen_p);
		}
		if (firstTime) { //firstTime is the variable used to determine if the pattern is seen for the first time or not
			firstTime = false;
		} else {
			if (sameContent(beforeCurr_p, curr_p, nRows, nCols)) {

				patternRepeated = true;
				puts("Pattern repeated after two generations.");
				printThis(nRows, nCols, beforeCurr_p);
				done = true;

			}
		}
		if (!done) {
			if (print == 'y') {   //Checks the [PRINT] arg in the command line
				puts("New generation");
				printThis(nRows, nCols, curr_p);
			}
			if (pause == 'y') { // Checks the [PAUSE] arg in the commmand line
				puts("Paused waiting for input.");
				getc(stdin); //just waits for user input
			}
			char* temp = beforeCurr_p; // This is what's known as the musical chairs that shifts the arrays around
			beforeCurr_p = nextGen_p;		// when we want to more than one generation
			nextGen_p = curr_p;
			curr_p = temp;
		}

	}

	return numGen;
}

/**
 * anyX Looks through a game board (multi-dimensional array) and determines if there is any living cells ("X")
 * @param char* arr The pointer to the array that is being searched
 * @param int nRows The number of rows in the array
 * @param int nCols The number of columns in the array
 * @return bool returns a boolean, true or false, whether there is a living cell in the array or not
 */
bool anyX(char* arr, int nRows, int nCols) {
	bool any = false;
	for (int row = 0; !any && (row < nRows); row++) { // These nested loops are like latitude and longitude (rows and
		for (int col = 0; !any && (col < nCols); col++) { // columns) of the board which allow us to look at every position
			if (getLetter(row, col, nCols, arr) == 'x') {
				any = true;
			}
		}
	}
	return any;
}
/**
 * sameContent Compares two gameboards and determines if they have the same patttern or not
 * @param char* one_p This is the pointer to the array for a gameboard
 * @param char* another_p This is the pointer to the array for another gameboard being compared to the first one
 * @param int nRows The number of rows in the array
 * @param int nCols The number of columns in the array
 * @return bool returns true if the two gameboards have the same pattern and false if they're different
 */
bool sameContent(char* one_p, char* another_p, int nRows, int nCols) {
	bool same = true; //for all the comparisons that have been made so far

	for (int row = 0; same && (row < nRows); row++) {
		for (int col = 0; same && (col < nCols); col++) {
			if (getLetter(row, col, nCols, one_p)      // This compares the character present in the same two positions
					!= getLetter(row, col, nCols, another_p)) {// of different boards
				same = false;
			}
		}
	}
	return same;
}

/**
 * printThis Prints out the gameboard with the appropriate living and dead cells in their correct position
 * @param int nRows The number of rows that will be in the printed array
 * @param int nCols The number of columns that will be in the printer array
 * @param char* nextGen_p The pointer to the array of the next generation to be printed
 * @return void Returns nothing because the gameboard will be printed in nextGen_p
 */
void printThis(int nRows, int nCols, char* nextGen_p) {
	for (int row = 0; row < nRows; row++) {
		for (int col = 0; col < nCols; col++) {
			printf("%c", getLetter(row, col, nCols, nextGen_p)); //%c is used for characters
		}
		printf("\n"); // Begins a new line when we have gotten all character in the row
	}
}
