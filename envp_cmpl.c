#include "minishell.h"

void init_env(t_env **env_list, char **envp)
{
    int i = 0;
    char *key;
    char *value;
    char *equal_sign;

    while (envp[i])
    {
        equal_sign = ft_strchr(envp[i], '=');
        if (equal_sign)
        {
            key = ft_substr(envp[i], 0, equal_sign - envp[i]);
            value = ft_strdup(equal_sign + 1);
            set_env(env_list, key, value);
            free(key);
            free(value);
        }
        i++;
    }
}


char **env_to_envp(t_env *env_list)
{
    int count = 0;
    t_env *temp = env_list;

    while (temp)
    {
        count++;
        temp = temp->next;
    }

    char **envp = malloc((count + 1) * sizeof(char *));
    if (!envp)
        return (NULL);

    int i = 0;
    while (env_list)
    {
        char *key_equal = ft_strjoin(env_list->key ? env_list->key : "", "=");
        if (!key_equal)
            return NULL;

        char *full = ft_strjoin(key_equal, env_list->value ? env_list->value : "");
        free(key_equal);

        if (!full)
            return NULL;

        envp[i] = full;

        free(key_equal);

        if (!envp[i])
            return NULL;

        env_list = env_list->next;
        i++;
    }
    envp[i] = NULL;
    return (envp);
}


