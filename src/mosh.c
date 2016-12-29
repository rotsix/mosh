#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "built-ins.h"
#include "config.h"

#define AUTHOR "rotsix"
#define VERSION "0.1.0"
#define LICENSE "LGPL"

#define TOKENS " \t\r\n\a"


/* prototypes */
char * readline(void);
char ** splitLine(char *);
int launch(char **);
int execute(char **);

/* variables */
static const int nb_built_in_functions = 1;
static const void * built_in_functions[] = {
	cd,
};



char *
readLine(void)
{
	char *line = NULL;
	size_t bufsize = 0;
	if(getline(&line,&bufsize,stdin) == -1){
		fprintf(stderr, "mosh: reading line error\n");
		exit(EXIT_FAILURE);
	}
	return line;
}



char **
splitLine(char *line)
{
	int buf = 64;
	int bufsize = buf;
	int position = 0;
	char **tokens = malloc(sizeof(char*) * buf);
	char *token;

	if(!tokens){
		fprintf(stderr, "mosh: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TOKENS);
	while(token != NULL){
		tokens[position] = token;
		position++;

		if(position >= bufsize){
			bufsize += buf;
			tokens = realloc(tokens, sizeof(char*) * bufsize);
			if(!tokens){
				fprintf(stderr, "mosh: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TOKENS);
	}
	tokens[position] = NULL;
	return tokens;
}



int
launch(char **args)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0) {
		// Child process
		if(execvp(args[0], args) == -1){
			perror("mosh");
		}
		exit(EXIT_FAILURE);
	} else if (pid < 0) {
		// Error forking
		perror("mosh");
	} else {
		// Parent process
		do {
			wpid = waitpid(pid, &status, WUNTRACED);
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}



int
execute(char **args)
{
	int i;

	if (args[0] == NULL) {
		// An empty command was entered.
		return 0;
	}

	// We check into our built-in functions.
	for(i = 0; i < nb_built_in_functions; i++){
		if(strcmp(args[0], built_in_functions[i])){
			return built_in_functions[i](args);
		}
	}

	return launch(args);
}



int
main(int argc, char *argv[])
{
	// NOTE getopt in unistd.h if needed

	char *line;
	char **args;
	int status;
	do {
		printf(prompt());
		line = readLine();
		args = splitLine(line);
		status = execute(args);

		free(line);
		free(args);
	} while(status);

	return EXIT_SUCCESS;
}
