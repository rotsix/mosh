#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RETURN_SUCCESS 1
#define RETURN_FAILURE 0


int
cd(const char **args)
{
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) == NULL){
		fprintf(stderr, "getcwd() error");
	}

	if(args[1] == NULL || strcmp(args[1], "~") == 0){
		// We go to $HOME
		const char *home = getenv("HOME");
		setenv("OLDPWD", cwd, 1);
		if(chdir(home) != 0){
			fprintf(stderr, "See `man cd` for more informations.\n");
			fprintf(stderr, "HOME may is not set.\n");
		} else {
			setenv("PWD", home, 1);
		}
		return RETURN_SUCCESS;
	} else if(args[2] != NULL && strcmp(args[1],"-L") != 0 && strcmp(args[1],"-P") != 0){
		fprintf(stderr, "See `man cd` for more informations.\n");
		return RETURN_SUCCESS;
	} else if(strcmp(args[1], "-") == 0){
		// Swap to previous directory
		char *oldpwd = getenv("OLDPWD");
		if(chdir(oldpwd) != 0){
			fprintf(stderr, "See `man cd` for more informations.\n");
			fprintf(stderr, "OLDPWD may is not set.\n");
		} else {
			setenv("OLDPWD", cwd, 1);
			setenv("PWD", oldpwd, 1);
		}
		printf("%s\n", getenv("PWD"));
		return RETURN_SUCCESS;
	} else {
		if(chdir(args[1]) != 0){
			fprintf(stderr, "Path not found.\n");
		} else {
			setenv("OLDPWD", cwd, 1);

			if(getcwd(cwd, sizeof(cwd)) == NULL){
				fprintf(stderr, "getcwd() error");
			}
			setenv("PWD", cwd, 1);
		}
		return RETURN_SUCCESS;
	}
}
