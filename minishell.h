#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>   // <<<<<<< تمت إضافته للتنفيذ
#include <string.h>     // <<<<<<< تمت إضافته (لـ strerror، أو استخدم بدائلك)
#include <limits.h>     // <<<<<<< تمت إضافته (لـ PATH_MAX)
#include <errno.h>      // <<<<<<< تمت إضافته (لـ errno)

#include <readline/readline.h>
#include <readline/history.h>


// ----- تراكيب البيانات الموجودة لديك -----
typedef struct s_env {
    char *var;
    char *val;
    struct s_env *next;
} t_env;

typedef struct s_gc_node {
    void *ptr;
    struct s_gc_node *next;
} t_gc_node;

typedef enum e_token_type {
    T_WORD,
    T_PIPE,
    T_REDIR_IN,
    T_REDIR_OUT,
    T_APPEND,
    T_HEREDOC
} t_token_type;

typedef struct s_cmd
{
    char **args;
    int    in_type;
    char  *in_file;
    int    out_type;
    char  *out_file;
    struct s_cmd *next;
} t_cmd;

typedef  struct s_data{
    t_env *tenv;
    t_cmd *com;
    // int last_exit_status; // يمكنك إضافته هنا إذا أردت
} t_data;

typedef struct s_comand
{
	t_token_type type;
	char *word;
	struct s_comand *next;
} t_comand;

// ----- تراكيب بيانات جديدة للتنفيذ -----
typedef struct s_pid_list {
    pid_t pid;
    struct s_pid_list *next;
} t_pid_list;


// ----- دوال الـ Parsing والـ Utils الموجودة -----
t_comand *tokens(char *input);
char    *tokens_1(char *input, int *i);
int     is_space(char c);
int     is_s_char(char c);
size_t	ft_strlen(const char *s);
char	*ft_strndup(const char *s1, size_t size);
void    *ft_memcpy(void *dst, const void *src, size_t n);
t_comand *new_node(char *token);
char	*ft_strdup(const char *s1);
void    put_type(t_comand *tokens);
int	    ft_strcmp(char *s1, char *s2);
int     check_syntax(t_comand *tokens); // <<<<<<< تم اختيار التهجئة الصحيحة
int ft_atoi(const char *str);
t_cmd	*parse_command(t_comand *tokens);
void    handel_heredoc(char *limiter, t_cmd *cmd);
int     open_file(char **filename);
char    *generate_tmp_filename(void);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_itoa(int n);
char    *handle_quotes(char *token);
char	**ft_split(char const *s, char c);
char    *ft_strcpy(char *s1, char *s2);
char    *ft_strcat(char *dest, const char *src);
char    *ft_strchr(const char *s, int c);
int	    ft_isalnum(int c);
int	    ft_isalpha(int c);
// ----- دوال إدارة الذاكرة -----
void    *gc_malloc(size_t size);
void    gc_free_all(void);
void    gc_free_ptr(void *ptr);
void    gc_free_array(char **array);       // <<<<<<< تمت إضافته
void    gc_free_pid_list(t_pid_list *list); // <<<<<<< تمت إضافته

// ----- دوال البيئة والـ Builtins الموجودة -----
t_env   *init_env(char **envp);
void    print_env(t_env *env);
int     count_nodes(t_env *env); // موجودة لديك
void    handel_export(t_data *command);
char    *get_val(const char *env_line);
char    *get_var(const char *env_line);
t_env   *new_env_node(char *var, char *val);
int     unset(char **args, t_env **env);
char    *get_env_value(t_env *env, char *key);

// ----- دوال الـ Expansion والإشارات الموجودة -----
int     expand_all_commands(t_data *data);
void    _sigint_handler(int sig);
void    signal_init(void);
void    biltings(t_data *comand); // توقيع دالة biltings موجود

// ----- دوال التنفيذ الجديدة -----
void    start_execution(t_data *data);
void    execute_command_in_child(t_cmd *cmd, t_data *data);
void    execute_single_external_command(t_cmd *cmd, t_data *data);
void    execute_pipeline(t_data *data);

// utils للتنفيذ
void    handle_parent_wait(pid_t child_pid, int is_pipeline_last_cmd);
char    *find_executable_path(char *cmd_name, t_env *env_list);
char    **convert_env_list_to_array(t_env *env_list);
void    set_global_exit_status(int status);
int     get_global_exit_status(void);

// redirections
void    handle_child_redirections(t_cmd *cmd);
void    apply_redirection(int old_fd, int new_fd);
int     open_input_file(char *filename);
int     open_output_file(char *filename, int type);

// builtins جديدة أو موسعة
int     is_builtin(char *cmd_name);
void    execute_builtin_parent(t_cmd *cmd, t_data *data);
int     execute_builtin_child(t_cmd *cmd, t_data *data);

int     builtin_echo(char **args);
int     builtin_cd(char **args, t_env **env_list);
int     builtin_pwd(void);
int     builtin_exit(char **args, t_data *data);
void    set_env_var(t_env **env_list, char *var_name, char *var_value);
int     is_str_numeric(const char *str);

// ----- دوال البيئة والـ Builtins الموجودة -----
t_env   *init_env(char **envp);
void    print_env(t_env *env);
int     count_nodes(t_env *env);
void    handel_export(t_data *command);
char    *get_val(const char *env_line);
char    *get_var(const char *env_line);
t_env   *new_env_node(char *var, char *val);
int     unset(char **args, t_env **env);
char    *get_env_value(t_env *env, char *key);
void    print_export(t_env *env); // <<<<<<< أضف هذا السطر إذا لم يكن موجودًا

// ... (باقي التوقيعات) ...

#endif