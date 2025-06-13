#ifndef MINISHELL_H
#define MINISHELL_H

// Standard Libraries
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

// Readline Library
#include <readline/readline.h>
#include <readline/history.h>

// -------------------- ERROR CODES (Example) --------------------
#define EXIT_CMD_NOT_FOUND 127
#define EXIT_CMD_CANNOT_EXECUTE 126
#define EXIT_SYNTAX_ERROR 2
#define EXIT_GENERAL_ERROR 1

// -------------------- STRUCTURE DEFINITIONS --------------------
typedef struct s_env {
    char            *var;
    char            *val;
    struct s_env    *next;
} t_env;

typedef struct s_gc_node {
    void                *ptr;
    struct s_gc_node    *next;
} t_gc_node;

typedef enum e_token_type {
    T_WORD, T_PIPE,
    T_REDIR_IN, T_REDIR_OUT,
    T_APPEND, T_HEREDOC
} t_token_type;

typedef struct s_cmd {
    char            **args;
    int             in_type;
    char            *in_file;
    int             out_type;
    char            *out_file;
    struct s_cmd    *next;
} t_cmd;

typedef struct s_data {
    t_env   *tenv;
    t_cmd   *com;
    int     last_exit_status;
} t_data;

typedef struct s_comand {
    t_token_type        type;
    char                *word;
    struct s_comand     *next;
} t_comand;

typedef struct s_pid_list {
    pid_t               pid;
    struct s_pid_list   *next;
} t_pid_list;

// -------------------- LIBFT / UTILS PROTOTYPES --------------------
size_t  ft_strlen(const char *s);
char    *ft_strdup(const char *s1);
char    *ft_strndup(const char *s1, size_t size);
void    *ft_memcpy(void *dst, const void *src, size_t n);
int     ft_strcmp(const char *s1, const char *s2);
char    *ft_strchr(const char *s, int c);
char    **ft_split(char const *s, char c);
char    *ft_strjoin(char const *s1, char const *s2);
char    *ft_itoa(int n);
int     ft_atoi(const char *str);
int     ft_isalnum(int c);
int     ft_isalpha(int c);
char    *ft_strcpy(char *s1, char *s2);
char    *ft_strcat(char *dest, const char *src);

// -------------------- MEMORY MANAGEMENT PROTOTYPES --------------------
void    *gc_malloc(size_t size);
void    gc_free_ptr(void *ptr);
void    gc_free_all(void);
void    gc_free_array(char **array);
void    gc_free_pid_list(t_pid_list *list);
void    gc_free_token_list(t_comand *list);
void    gc_free_cmd_structure(t_cmd *cmd_list);

// -------------------- PARSING PROTOTYPES --------------------
t_comand    *tokens(char *input);
int         is_space(char c);
int         is_s_char(char c);
t_comand    *new_node(char *token); // Originally from your ft_malloc.c, better in parsing utils
void        put_type(t_comand *tokens);
int         check_syntax(t_comand *tokens);
t_cmd       *parse_command(t_comand *tokens);
int         open_file(char **filename_ptr); // From your validation.c
char        *handle_quotes(char *token);    // From your utils_2.c

// -------------------- ENVIRONMENT & EXPANSION PROTOTYPES --------------------
t_env   *init_env(char **envp);
char    *get_env_value(t_env *env, char *key); // From your utils_bi2.c
char    *get_val(const char *env_line);
char    *get_var(const char *env_line);
t_env   *new_env_node(char *var, char *val);
int     expand_all_commands(t_data *data);
int     count_nodes(t_env *env);

// -------------------- BUILTINS (Original from 'bilds' + new signatures) ----
void    print_env(t_env *env);
void    print_export(t_env *env);
int     handel_export(t_data *data); // Modified to take t_data, returns int
int     unset(char **args, t_env **env_list_ptr); // Returns int

// -------------------- SIGNALS PROTOTYPES --------------------------------
void    signal_init(void);

// -------------------- EXECUTION PROTOTYPES ----------------------------
void    start_execution(t_data *data);
void    execute_command_in_child(t_cmd *cmd, t_data *data);
void    execute_single_external_command(t_cmd *cmd, t_data *data);
void    execute_pipeline(t_data *data);
void    handle_parent_wait(pid_t child_pid, int is_pipeline_last_cmd, t_data *data);
char    *find_executable_path(char *cmd_name, t_env *env_list);
char    **convert_env_list_to_array(t_env *env_list);
void    display_error_message(char *command, char *arg, char *message);
int     handle_child_redirections(t_cmd *cmd);
void    apply_redirection(int old_fd, int new_fd);
int     is_builtin(char *cmd_name);
void    execute_builtin_parent(t_cmd *cmd, t_data *data);
int     execute_builtin_child(t_cmd *cmd, t_data *data);
int     builtin_echo(char **args);
int     builtin_cd(char **args, t_env **env_list);
int     builtin_pwd(void);
int     builtin_exit(char **args, t_data *data);
void    set_env_var(t_env **env_list, char *var_name, char *var_value);
int     is_str_numeric(const char *str);

#endif