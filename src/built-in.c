#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef RETURN_SUCCESS
#define RETURN_SUCCESS 1

int
cd(const char **args)
{
	if(args[1] == NULL){
		// We go to $HOME
		const char *home = getenv("HOME");
		if(chdir(home) != 0){
			fprintf(stderr, "See `man cd` for more informations.\n");
		}
		return RETURN_SUCCESS;
	} else if(args[2] != NULL && strcmp(args[1],"-L") != 0 && strcmp(args[1],"-P") != 0){
		fprintf(stderr, "See `man cd` for more informations.\n");
		return RETURN_SUCCESS;
	} else if(strcmp(args[1], "-") == 0){
		char *oldpwd = getenv("OLDPWD");
		if(chdir(oldpwd) != 0){
			fprintf(stderr, "See `man cd` for more informations.\n");
			fprintf(stderr, "OLDPWD may is not set.\n");
		}
		return RETURN_SUCCESS;
	} else {
		if(chdir(args[1]) != 0){
			fprintf(stderr, "Path not found.\n");
		}
		return RETURN_SUCCESS;
	}
}


#endif
#undef RETURN_SUCCESS
