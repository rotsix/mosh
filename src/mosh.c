#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "built-in.h"


/* Prototypes */
static char* readLine(void);
static char** splitLine(const char *);
static int launch(const char **);
static int execute(const char **);

/* Structures */
typedef struct {
	const char *name;
	const char *func[10]; // NOTE Is that enough for "normal" aliases ?
} Alias;
typedef struct { // Used for built-in functions (dictionnary).
	const char *name;
	int (*func)(const char **);
} Function;

/* Variables */
static const char TOKENS[] = " \t\r\n\a";
static const Function built_in_functions[] = {
	{ "cd" , 	cd },
};



#include "config.h"



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
splitLine(const char *line)
{
	size_t buf = 64;
	size_t bufsize = buf;
	unsigned int position = 0;
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
launch(const char **args)
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

	return EXIT_FAILURE;
}



int
execute(const char **args)
{
	int i;
	int nb_built_in_functions = sizeof(built_in_functions)/sizeof(built_in_functions[0]);
	int nb_aliases = sizeof(aliases)/sizeof(aliases[0]);

	if (args[0] == NULL) {
		// An empty command was entered.
		return EXIT_SUCCESS;
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
