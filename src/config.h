/*
*	Here is the configuration file.
*/

/* Prompt */
char *
prompt(void)
{
	char *prompt = "$ ";
	return prompt;
}

/* Aliases */
static Alias aliases[] = {
	/* name				command */
	{ "syu",			{ "sudo", "pacman", "-Syu", NULL, } },
	{ "ls",				{ "ls", "-Fh", "--color=auto" } },
	{ "la",				{ "ls", "-AFh", "--color=auto" } },
	{ "ll",				{ "ls", "-Fhl", "--color=auto" } },
	{ "lla",			{ "ls", "-AFhl", "--color=auto" } },
};
