#include <stdlib.h>
#include <stdio.h>

/*
 *   Function Declarations for builtin shell commands:
 *    */
int m_cd(char **args);
int m_help(char **args);
int m_exit(char **args);

/*
 *   List of builtin commands, followed by their corresponding functions.
 *    */
char *builtin_str[] = {
  "cd",
  "help",
  "exit"
};


int (*builtin_func[]) (char **) = {
  &m_cd,
  &m_help,
  &m_exit
};



int
num_builtins()
{
  return sizeof(builtin_str) / sizeof(char *);
}



/*
 *   Builtin function implementations.
 *   */
int
m_cd(char **args)
{
  if(args[1] == NULL){
		fprintf(stderr, "mosh: expected argument to \"cd\"\n");
	} else {
			if (chdir(args[1]) != 0) {
					perror("mosh");
				}
		}
  return 1;
}



int
m_help(char **args)
{
  int i;
  printf("My Own SHell\n");
  printf("Type program names and arguments, and hit enter.\n");
  printf("The following are built in:\n");

  for(i = 0; i < num_builtins(); i++){
		printf("  %s\n", builtin_str[i]);
	}

  printf("Use the man command for information on other programs.\n");
  return 1;
}



int
m_exit(char **args)
{
  return 0;
}

