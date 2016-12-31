#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int
cd(const char ** args)
{
	if(args[1] == NULL){
		// We stay in the same directory.
		return EXIT_SUCCESS;
	} else if(args[2] != NULL && strcmp(args[1],"-L") != 0 && strcmp(args[1],"-P") != 0){
		fprintf(stderr, "See `man cd` for more informations.\n");
		return EXIT_FAILURE;
	} else if(strcmp(args[1], "-") == 0){
		char * oldpwd = getenv("OLDPWD");
		return chdir(oldpwd);
	} else {
		return chdir(args[1]);
	}
}
