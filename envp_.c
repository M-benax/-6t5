#include "minishell.h"

t_env *find_env(t_env *env_list, const char *key)
{
    while (env_list)
    {
        if (ft_strcmp(env_list->key, (char *)key) == 0)
            return env_list;
        env_list = env_list->next;
    }
    return NULL;
}

void set_env(t_env **env_list, const char *key, const char *value)
{
    t_env *new_node = malloc(sizeof(t_env));
    new_node->key = ft_strdup(key);
    new_node->value = ft_strdup(value);
    new_node->next = *env_list;
    *env_list = new_node;
}

void unset_env(t_env **env_list, const char *key)
{
    t_env *current = *env_list;
    t_env *previous = NULL;

    while (current)
    {
        if (ft_strcmp(current->key, (char *)key) == 0)
        {
            if (previous)
                previous->next = current->next;
            else
                *env_list = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}


void free_env(t_env *env_list)
{
    while (env_list)
    {
        t_env *temp = env_list;
        env_list = env_list->next;
        free(temp->key);
        free(temp->value);
        free(temp);
    }
}
