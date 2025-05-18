// minishell.h
#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/wait.h>

// Enum for redirection types
typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC
} t_token_type;

// Struct for environment variable
typedef struct s_env {
    char *key;
    char *value;
    struct s_env *next;
} t_env;

// Struct for command
typedef struct s_cmd {
    char **args;
    int    in_type;
    char  *in_file;
    int    out_type;
    char  *out_file;
    struct s_cmd *next;
} t_cmd;

// Declare your functions
char    *ft_strdup(const char *src);
void    execute_cmd(t_cmd *cmd, t_env *env);
void    apply_redirections(t_cmd *cmd);
void    init_env(t_env **env_list, char **envp);
char    **env_to_envp(t_env *env_list);
void    free_env(t_env *env_list);
t_env   *find_env(t_env *env_list, const char *key);
void    set_env(t_env **env_list, const char *key, const char *value);
void    unset_env(t_env **env_list, const char *key);
int     ft_strcmp(char *s1, char *s2);
char    *ft_strcpy(char *s1, char *s2);
size_t  ft_strlen(const char *s);
int     run_builtin(t_cmd *cmd, t_env **env);
int     ft_echo(t_cmd *cmd);
int     ft_cd(t_cmd *cmd, t_env *env);
int     ft_pwd(void);
int     ft_exit(t_cmd *cmd);
int     ft_export(t_cmd *cmd, t_env **env);
int     ft_unset(t_cmd *cmd, t_env **env);
int     ft_env(t_env *env);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strchr(const char *str, int c);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
char	*ft_strjoin(char const *s1, char const *s2);
int	ft_atoi(char *str);

#endif
