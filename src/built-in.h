int m_cd(char **);
int m_help(char **);
int m_exit(char **);

char **builtin_str;
int (**builtin_func) (char **);

void myman(char *);

int num_builtins();
