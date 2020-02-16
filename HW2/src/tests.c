/*
 * tests.c
 *
 *  Created on: January 22, 2018
 *      Author: Stefano Jordhani
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

bool tests(void)
{
	bool results=false;
	puts("During tests");
	bool ok1 = testReadInput();
	if(ok1)puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOne();
	if(ok3)puts("playOne is ok.");
	bool ok4 = testPlayOneBlinker();
	if(ok4)puts("Blinker works");
	bool ok5 = testPlayBlockOne();
	if(ok5)puts("Block works");
	bool ok7 = testPlayOneAllDie();
	if(ok7)puts("They all die");
	bool ok8 = testhowManyNeighbors();
	if(ok8)puts("howManyNeighbors works with no neighbors");
	bool ok9 = testhowManyNeighbors2();
	if(ok9)puts("howManyNeighbors works with neighbors");
	bool ok10 = testAnyX();
	if(ok10)puts("anyX works with atleast one living cell");
	bool ok11 = testAnyX2();
	if(!ok11)puts("anyX works with no living cells");
	bool ok12 = testsameContent();
	if(ok12)puts("sameContent works when the boards are the same");
	bool ok13 = testsameContent2();
	if(!ok13)puts("sameContent works correctly when the boards are different");
	puts("end of tests");
	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok7;
	printf("tests returning %d.\n",results);
	return results;
}
bool testReadInput(void)
{
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;
	bool ok1 = false;
	bool ok2 = false;
	bool ok3 = false;
	//
	//Here goes: create a file
	FILE* fp;
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false)
	{//it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if ((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4))
		{
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false)
	{//it opened, yay!
		ok2 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		int count =  fscanf(fp, "%s", oRow);
		//printf("First row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Second row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Third row is %s\n", oRow);
		if(0==strncmp(oRow, "xox", 4))
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);
		//printf("Fourth row is %s\n", oRow);
		if(0==strncmp(oRow, "oxo", 4))
		{//got the right data
			ok3 = true;
		}
	}
	results = ok1 && ok2 && ok3;
	return results;
}

bool testMakeArrays(void)
{
	bool results = false;
	bool ok1 = false;
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 4;
	char boardBefore[4][4]={
			{'o','x','o','\0'},
			{'x','o','x','\0'},
			{'x','o','x','\0'},
			{'o','x','o','\0'}
	};

	char boardAfter[nRows][nCols];
	//here's the test
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			boardAfter[row][col]=boardBefore[row][col];
		}
	}
	ok1 = true;
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if (boardAfter[row][col]!=boardBefore[row][col])
			{
				ok1=false;
			}
		}
	}

	results = ok1;
	return results;
}
// Tests to see if playOne works with a pattern that should repeat to the next generation
bool testPlayOne(void)
{
	bool results = false;
	bool ok1 = false;


	int nRows = 4;
	int nCols = 3;
	char boardBefore[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};
	char correctBoardAfter[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};

	char boardAfter[nRows][nCols];
	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!= correctBoardAfter[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results;

}

// Tests to see if playOne works with the Blinker pattern
bool testPlayOneBlinker(void)
{
	bool results = false;
	bool ok1 = false;


	int nRows2 = 4;
	int nCols2 = 3;
	char boardBefore[4][3]={
			{'o','o','o'},
			{'x','x','x'},
			{'o','o','o'},
			{'o','o','o'}
	};
	char correctBoardAfter[4][3]={
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','o','o'}
	};

	char boardAfter[nRows2][nCols2];
	//here's the invocation
	PlayOne(nRows2, nCols2, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row<nRows2;row++)
	{
		for(int col=0; col< nCols2; col++)
		{
			if(boardAfter[row][col]!= correctBoardAfter[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results ;

}

// Tests to see if playOne works with the Block pattern
bool testPlayBlockOne(void)
{
	bool results = false;
	bool ok1 = false;



	int nRows3 = 4;
	int nCols3 = 4;
	char boardBefore[4][4]={
			{'o','o','o','o'},
			{'o','x','x','o'},
			{'o','x','x','o'},
			{'o','o','o','o'}
	};
	char correctBoardAfter[4][4]={
			{'o','o','o','o'},
			{'o','x','x','o'},
			{'o','x','x','o'},
			{'o','o','o','o'}
	};

	char boardAfter[nRows3][nCols3];
	//here's the invocation
	PlayOne(nRows3, nCols3, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row< nRows3;row++)
	{
		for(int col=0; col< nCols3; col++)
		{
			if(boardAfter[row][col]!= correctBoardAfter[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results ;

}

// Tests to see if playOne works correctly with a pattern that causes all the cells to die
bool testPlayOneAllDie(void)
{
	bool results = false;
	bool ok1 = false;



	int nRows4 = 4;
	int nCols4 = 4;
	char boardBefore[4][4]={
			{'x','o','o','x'},
			{'o','o','o','o'},
			{'o','o','x','o'},
			{'x','o','o','o'}
	};
	char correctBoardAfter[4][4]={
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'}
	};

	char boardAfter[nRows4][nCols4];
	//here's the invocation
	PlayOne(nRows4, nCols4, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	for(int row=0;row< nRows4;row++)
	{
		for(int col=0; col< nCols4; col++)
		{
			if(boardAfter[row][col]!= correctBoardAfter[row][col])
			{//error found
				ok1 = false;
			}
		}
	}
	results = ok1;
	return results ;

}

// Tests to see if howManyNeighbors works when the desired cell has no neighbors
bool testhowManyNeighbors(void)
{
	bool results = false;
	int row = 0;
	int col = 0;
	int nRows = 4;
	int nCols = 4;

		char Old[4][4]={
			{'x','o','o','x'},
			{'o','o','o','o'},
			{'o','o','x','o'},
			{'x','o','o','o'}
	};
		int numNeighbors = howManyNeighbors(row,col, nRows, nCols, (char*)Old);
		if(numNeighbors == 0){
			results = true;
		}

		return results;
 }

// Tests to see if howManyNeighbors works when a desired cell has neighbors
// and results in the correct amount of neighbors
bool testhowManyNeighbors2(void)
{
	bool results = false;
	int row = 0;
	int col = 0;
	int nRows5 = 4;
	int nCols5 = 4;

		char Old[4][4]={
			{'x','o','o','x'},
			{'x','x','o','o'},
			{'o','o','x','o'},
			{'x','o','o','o'}
	};
		int numNeighbors = howManyNeighbors(row,col, nRows5, nCols5, (char*)Old);
		if(numNeighbors == 2){
			results = true;
		}

		return results;
 }
// Tests anyX to see if it returns true when there is a living cell ("x") present in the gameboard
bool testAnyX(void)
{
	bool results = false;
	bool ok1 = false;

	int nRows = 4;
	int nCols = 3;

	char board[4][3] ={
			{'o','o','x'},
			{'o','o','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	ok1 = anyX((char*)board, nRows, nCols);

	results = ok1;
	return results;
}
// Tests anyX to see if it returns false when there is no living cells ("x") present in the gameboard
bool testAnyX2(void)
{
	bool results = false;
	bool ok1 = false;

	int nRows = 4;
	int nCols = 3;

	char board[4][3] ={
			{'o','o','o'},
			{'o','o','o'},
			{'o','o','o'},
			{'o','o','o'}
	};
	ok1 = anyX((char*)board, nRows, nCols);

	results = ok1;
	return results;
}

// Tests to see if sameContent produces true when the two gameboards have the same pattern
bool testsameContent(void)
{
	bool ok1 = false;
	bool results = false;

	char board1[4][3] = {
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'}
	};

	char board2[4][3] = {
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'}
		};
	int nRows = 4;
	int nCols = 3;

	ok1 = sameContent((char*)board1, (char*)board2, nRows, nCols);
	results = ok1;
	return results;

}
// Tests to see if sameContent produces false when the two gameboards do not have the same pattern

bool testsameContent2(void)
{
	bool ok1 = false;
	bool results = false;

	char board1[4][3] = {
			{'o','x','o'},
			{'o','x','x'},
			{'o','x','x'},
			{'o','x','o'}
	};

	char board2[4][3] = {
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'}
		};
	int nRows = 4;
	int nCols = 3;

	ok1 = sameContent((char*)board1, (char*)board2, nRows, nCols);
	results = ok1;
	return results;

}






