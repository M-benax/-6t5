#include "../minishell.h"

// These ft_is* functions are usually part of libft
// If so, remove them from here and use the libft versions.
// Ensure prototypes are in minishell.h if they are defined here and non-static.

// Made static as they are simple helpers and ft_isalpha/alnum from libft should be used.
// If you must keep them, and they are only used in this file OR by static functions
// in this file, then static is fine. Otherwise, they need prototypes in .h.
// For now, assuming you have ft_isalpha/alnum from your libft via minishell.h.

// static int local_ft_isalpha(int c)
// {
//     return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
// }

// static int local_ft_isalnum(int c)
// {
//     return ((c >= '0' && c <= '9') || local_ft_isalpha(c));
// }

// get_env_value: retrieves value of an environment variable
// Returns a gc_malloc'd copy of the value, or a gc_malloc'd empty string if not found or no value.
char *get_env_value(t_env *env_list, char *key)
{
    if (!key) return (ft_strdup("")); // Handle NULL key

    while (env_list)
    {
        if (env_list->var && ft_strcmp(env_list->var, key) == 0)
        {
            if (env_list->val)
                return (ft_strdup(env_list->val));
            else
                return (ft_strdup("")); // Variable exists but has no value (e.g. export VAR)
        }
        env_list = env_list->next;
    }
    return (ft_strdup("")); // Variable not found
}