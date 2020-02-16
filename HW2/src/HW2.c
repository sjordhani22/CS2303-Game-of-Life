/*
 ============================================================================
 Name        : HW2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tests.h"
#include "production.h"

int main(int argc, char* argv[])
{
	bool ok=false;

	puts("!!!Hi all. We're doing HW2!!!");
	if(tests())
	{
		ok=production(argc, argv);
		if (ok)
		{
			puts("Game of life seems to have worked.");
		}
		else
		{
			puts("Game of life seems not to have worked.");
		}
	}
	else
	{
		puts("Something went wrong in the tests.");
	}
	return EXIT_SUCCESS;
}
