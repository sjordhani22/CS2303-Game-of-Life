/*
 * production.h
 *
 *  Created on: January 22, 2018
 *      Author: Stefano Jordhani
 */

#ifndef PRODUCTION_H_
#define PRODUCTION_H_
#include <stdio.h>
bool production(int argc, char* argv[]);
void PlayOne (unsigned int nr, unsigned int nc, char* Old, char* New);
void usage(void);
char getLetter(int row, int col, int nCols, char* Old);
int howManyNeighbors(int row, int col, int nRows, int nCols, char* Old);
void fileIntoArray(int nRows, int nCols, int howManyLinesInFile, int maximumWidth, char* ar_p, FILE* fp);
int generate(int gens,  int nRows,  int nCols,  char* old_p, char* new_p, char* other_p, char print, char pause);
bool anyX(char* arr, int nRows, int nCols);
bool sameContent(char* one_p, char* another_p, int nRows, int nCols);
void printThis(int nRows, int nCols, char* old_p);
#endif /* PRODUCTION_H_ */
