#include "../minishell.h"

static char *join_path_cmd(const char *path, const char *cmd)
{
    char    *full_path;
    size_t  len_path;
    size_t  len_cmd;

    len_path = ft_strlen(path);
    len_cmd = ft_strlen(cmd);
    full_path = malloc(len_path + 1 + len_cmd + 1);
    if (!full_path)
        return (NULL);
    ft_memcpy(full_path, path, len_path);
    full_path[len_path] = '/';
    ft_memcpy(full_path + len_path + 1, cmd, len_cmd);
    full_path[len_path + 1 + len_cmd] = '\0';
    return (full_path);
}

static char *check_paths_for_cmd(char **paths, char *cmd_name)
{
    int     i;
    char    *full_path_attempt;

    i = 0;
    while (paths && paths[i])
    {
        full_path_attempt = join_path_cmd(paths[i], cmd_name);
        if (full_path_attempt)
        {
            if (access(full_path_attempt, X_OK) == 0)
                return (full_path_attempt);
            free(full_path_attempt);
        }
        i++;
    }
    return (NULL);
}

char *find_executable_path(char *cmd_name, t_env *env_list)
{
    char    *path_env_value;
    char    **paths_array;
    char    *temp_path_malloc;
    char    *final_path_gc;
    int     i;

    if (!cmd_name || cmd_name[0] == '\0')
        return (NULL);
    if (ft_strchr(cmd_name, '/'))
    {
        if (access(cmd_name, X_OK) == 0)
            return (ft_strdup(cmd_name));
        return (NULL);
    }
    path_env_value = get_env_value(env_list, "PATH");
    if (!path_env_value || path_env_value[0] == '\0')
    {
        free(path_env_value); 
        return (NULL);
    }
    paths_array = ft_split(path_env_value, ':');
    free(path_env_value);
    if (!paths_array)
        return (NULL);
    temp_path_malloc = check_paths_for_cmd(paths_array, cmd_name);
    final_path_gc = NULL;
    if (temp_path_malloc)
    {
        final_path_gc = ft_strdup(temp_path_malloc);
        free(temp_path_malloc);
    }
    i = 0;
    while (paths_array[i])
    {
        free(paths_array[i]);
        i++;
    }
    free(paths_array);
    return (final_path_gc);
}