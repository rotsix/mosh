#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define RETURN_SUCCESS 1
#define RETURN_FAILURE 0


#include "built-in.h"


/* Prototypes */
static char* readLine(void);
static char** splitLine(char *);
static int launch(char **);
static int execute(char **);

/* Structures */
typedef struct {
	char *name;
	char *func[10]; // NOTE Is that enough for "normal" aliases ?
} Alias;
typedef struct { // Used for built-in functions (dictionnary).
	char *name;
	int (*func)(char **);
} Function;

/* Variables */
static char TOKENS[] = " \t\r\n\a";
static Function built_in_functions[] = {
	{ "cd" , 	cd },
};



#include "config.h"



char *
readLine(void)
{
	char *line = NULL;
	size_t bufsize = 0;

	// Here we should build the auto-completion using this :
	// size_t strlen(char *s);
	// int strncmp(char *s1, char *s2, size_t n);
	// where s1 is the initial string and s2 the stdin

	// strncmp(s1, s2, strlen(s2)) == 0 or something like this

	if(getline(&line,&bufsize,stdin) == -1){
		perror("mosh: reading line error\n");
		exit(EXIT_FAILURE);
	}
	return line;
}



char **
splitLine(char *line)
{
	size_t buf = 64;
	size_t bufsize = buf;
	unsigned int position = 0;
	char **tokens = malloc(sizeof(char*) * buf);
	char *token;

	if(!tokens){
		perror("mosh: allocation error\n");
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
				perror("mosh: allocation error\n");
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

	return RETURN_SUCCESS;
}



int
execute(char **args)
{
	int i;
	int nb_built_in_functions = sizeof(built_in_functions)/sizeof(built_in_functions[0]);
	int nb_aliases = sizeof(aliases)/sizeof(aliases[0]);

	if (args[0] == NULL) {
		// An empty command was entered.
		return RETURN_SUCCESS;
	}

	// We check into our aliases
	for(i = 0; i < nb_aliases; i++){
		if(strcmp(args[0], aliases[i].name) == 0){
			return launch(aliases[i].func);
		}
	}

	// We check into our built-in functions.
	for(i = 0; i < nb_built_in_functions; i++){
		if(strcmp(args[0], built_in_functions[i].name) == 0){
			return (*(built_in_functions[i].func))(args);
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
