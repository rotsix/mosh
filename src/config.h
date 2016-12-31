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
	{ "ls",				{ "ls", "-Fh", "--color=auto", NULL, } },
	{ "la",				{ "ls", "-AFh", "--color=auto", NULL, } },
	{ "ll",				{ "ls", "-Fhl", "--color=auto", NULL, } },
	{ "lla",			{ "ls", "-AFhl", "--color=auto", NULL, } },
};
