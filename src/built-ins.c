#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
cd(char ** args)
{
	if(args[1] == NULL){
		// We stay in the same directory.
		return 0;
	} else if(args[2] != NULL && !strcmp(args[1],"-L") && !strcmp(args[1],"-P")){
		fprintf(stderr, "See `man cd` for more informations.\n");
		return 1;
	} else {
		return chdir(args[1]);
	}
}
