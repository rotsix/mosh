char **builtin_str;
int (**builtin_func) (char **);

int num_builtins();
int m_cd(char **);
int m_help(char **);
int m_exit(char **);
