#include "../minishell.h"

static char **extract_vars(t_env *env)
{
    int     i;
    char    **arr;
    int     size;

    if (!env) return (gc_malloc(sizeof(char *))); // Return empty array if env is NULL
    size = count_nodes(env);
    arr = gc_malloc(sizeof(char *) * (size + 1));
    if (!arr) return (NULL);
    i = 0;
    while (env)
    {
        arr[i++] = env->var; // Assumes var is a valid string, might need strdup if freeing env nodes separately
        env = env->next;
    }
    arr[i] = NULL;
    return (arr);
}

static void sort_vars(char **vars)
{
    int     i;
    int     j;
    char    *tmp;

    if (!vars) return;
    i = 0;
    while (vars[i])
    {
        j = i + 1;
        while (vars[j])
        {
            if (ft_strcmp(vars[i], vars[j]) > 0)
            {
                tmp = vars[i];
                vars[i] = vars[j];
                vars[j] = tmp;
            }
            j++;
        }
        i++;
    }
}

static t_env *find_var_in_env(t_env *env, char *var_name)
{
    if (!var_name) return NULL;
    while (env)
    {
        if (env->var && ft_strcmp(env->var, var_name) == 0)
            return env;
        env = env->next;
    }
    return NULL;
}

// No static here
void print_export(t_env *env)
{
    int     i;
    char    **vars;
    t_env   *node;

    vars = extract_vars(env);
    if (!vars) return;
    sort_vars(vars);
    i = 0;
    while (vars[i])
    {
        node = find_var_in_env(env, vars[i]);
        if (node)
        {
            if (node->val)
                printf("declare -x %s=\"%s\"\n", node->var, node->val);
            else
                printf("declare -x %s\n", node->var); // Variables without value
        }
        i++;
    }
    gc_free_ptr(vars); // Free the array of pointers, not the strings themselves if they are from env
}

// Consider removing or refactoring 'biltings' if 'start_execution' handles all
// void biltings(t_data *comand)
// {
//     // ... logic was moved to execute_builtin_parent
// }