#include "../minishell.h"

// new_env_node should be here or in a shared env_utils.c
// It's also in minishell.h as a prototype
t_env *new_env_node(char *var_name, char *var_value)
{
    t_env *node;

    node = gc_malloc(sizeof(t_env));
    if (!node) return (NULL);
    node->var = ft_strdup(var_name); // Use ft_strdup which should use gc_malloc
    node->val = ft_strdup(var_value);
    if ((var_name && !node->var) || (var_value && !node->val))
    { // Malloc failed for var or val
        gc_free_ptr(node->var);
        gc_free_ptr(node->val);
        gc_free_ptr(node);
        return (NULL);
    }
    node->next = NULL;
    return (node);
}

// get_var and get_val are helpers for init_env
// They should be static if only used here.
// Prototypes are in minishell.h, so they should not be static.
char *get_var(const char *env_line)
{
    int i;
    i = 0;
    while (env_line[i] && env_line[i] != '=')
        i++;
    return (ft_strndup(env_line, i)); // ft_strndup should use gc_malloc
}

char *get_val(const char *env_line)
{
    int i;
    i = 0;
    while (env_line[i] && env_line[i] != '=')
        i++;
    if (env_line[i] == '=')
        return (ft_strdup(env_line + i + 1)); // ft_strdup should use gc_malloc
    return (ft_strdup("")); // Variable exists but has no value after '='
}


t_env *init_env(char **envp)
{
    t_env   *head_env_list;
    t_env   *current_node;
    t_env   *new_node_instance;
    char    *var_name;
    char    *var_value;
    int     i;

    head_env_list = NULL;
    current_node = NULL;
    i = 0;
    if (!envp) return (NULL); // No environment passed

    while (envp[i])
    {
        var_name = get_var(envp[i]);
        var_value = get_val(envp[i]);
        if (!var_name || !var_value) { /* Malloc error in get_var/get_val */ 
            gc_free_ptr(var_name); gc_free_ptr(var_value); i++; continue; 
        }

        new_node_instance = new_env_node(var_name, var_value);
        gc_free_ptr(var_name); // Free temp strings from get_var/get_val
        gc_free_ptr(var_value); // as new_env_node duplicates them

        if (!new_node_instance) { /* Malloc error in new_env_node, critical */
             // gc_free_env_list(head_env_list); // Need a function to free partial list
             return (NULL); // Or handle more gracefully
        }

        if (!head_env_list)
            head_env_list = new_node_instance;
        else
            current_node->next = new_node_instance;
        current_node = new_node_instance;
        i++;
    }
    return (head_env_list);
}