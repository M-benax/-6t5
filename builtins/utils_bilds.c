#include "../minishell.h"

// No static
void print_env(t_env *env)
{
    while (env)
    {
        if (env->val) // env only prints variables that have a value
        {
            if (env->var) write(STDOUT_FILENO, env->var, ft_strlen(env->var));
            write(STDOUT_FILENO, "=", 1);
            // env->val could be an empty string, which is valid to print
            write(STDOUT_FILENO, env->val, ft_strlen(env->val));
            write(STDOUT_FILENO, "\n", 1);
        }
        env = env->next;
    }
}

// No static
int count_nodes(t_env *env)
{
    int count = 0;
    while (env)
    {
        count++;
        env = env->next;
    }
    return count;
}

// ft_strcpy, ft_strcat, ft_strchr from your original file.
// Ensure their prototypes in minishell.h match these definitions.
char    *ft_strcpy(char *s1, char *s2)
{
    int i = 0;
    while (s2[i]) { s1[i] = s2[i]; i++; }
    s1[i] = s2[i];
    return (s1);
}

char *ft_strcat(char *dest, const char *src)
{
    unsigned int i = 0;
    unsigned int j = 0;
    while (dest[i] != '\0') i++;
    while (src[j] != '\0') { dest[i] = src[j]; i++; j++; }
    dest[i] = '\0';
    return (dest);
}

char *ft_strchr(const char *s, int c)
{
    while (*s) {
        if (*s == (char)c) return ((char *)s);
        ++s;
    }
    if (c == '\0') return ((char *)s);
    return (0);
}