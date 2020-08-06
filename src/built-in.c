#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define RETURN_SUCCESS 1
#define RETURN_FAILURE 0


int
cd(char **args)
{
	char cwd[1024];
	if(getcwd(cwd, sizeof(cwd)) == NULL){
		perror("getcwd()");
	}

	if(args[1] == NULL || strcmp(args[1], "~") == 0){
		// Way back $HOME
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
		char *path;
		const char *home = getenv("HOME");
		// Replace ~ with $HOME
		if(!strchr(args[1], '~')) {
			path = args[1];
		} else {
			// path/to/~/to/path
			int i;
			path = (char *)malloc(strlen(args[1] - 1 + strlen(home)));
			for(i = 0; args[1][i] != '~'; i++) {
				path[i] = args[1][i];
			}
			// path/to/
			strncpy(path + i, home, strlen(home));
			// path/to/$HOME
			for(; i < strlen(args[1]); i++) {
				path[i + strlen(home)] = args[1][i+1];
			}
			// path/to/$HOME/to/path
		}

		if(chdir(path) != 0){
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
