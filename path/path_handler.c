#include "../minishell.h"

char *join_path_cmd(const char *path, const char *cmd)
{
    char *full_path;
    size_t len_path;
    size_t len_cmd;

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

char *check_paths(char **paths, char *cmd_name)
{
    int i;
    char *full_path;

    i = 0;
    printf("[DEBUG check_paths] Checking for cmd: %s\n", cmd_name);
    while (paths && paths[i])
    {
        printf("[DEBUG check_paths] Trying path: %s\n", paths[i]);
        full_path = join_path_cmd(paths[i], cmd_name);
        if (full_path)
        {
            printf("[DEBUG check_paths] Testing full_path: %s\n", full_path);
            if (access(full_path, X_OK) == 0)
            {
                printf("[DEBUG check_paths] Found and executable: %s\n", full_path);
                return (full_path);
            }
            free(full_path);
        }
        i++;
    }
    printf("[DEBUG check_paths] Command %s not found in paths.\n", cmd_name);
    return (NULL);
}

char *find_executable_path(char *cmd_name, t_env *env_list)
{
    char *path_env_val;
    char **paths_array;
    char *result_path_malloc; // المسار المرجع من check_paths (malloc)
    char *final_path_gc;    // المسار النهائي (gc_malloc)
    int i;

    printf("[DEBUG find_exe] Searching for: %s\n", cmd_name);
    if (!cmd_name || cmd_name[0] == '\0')
        return (NULL);

    if (ft_strchr(cmd_name, '/')) // مسار نسبي أو مطلق
    {
        printf("[DEBUG find_exe] Path contains '/', checking directly: %s\n", cmd_name);
        if (access(cmd_name, X_OK) == 0)
        {
            printf("[DEBUG find_exe] Direct path accessible: %s\n", cmd_name);
            return (ft_strdup(cmd_name)); // افترض أن ft_strdup يستخدم gc_malloc
        }
        printf("[DEBUG find_exe] Direct path NOT accessible: %s\n", cmd_name);
        return (NULL);
    }

    path_env_val = get_env_value(env_list, "PATH");
    if (!path_env_val || path_env_val[0] == '\0')
    {
        printf("[DEBUG find_exe] PATH variable not set or empty.\n");
        free(path_env_val); // إذا كان get_env_value يرجع سلسلة مخصصة بـ malloc
        return (NULL);
    }
    printf("[DEBUG find_exe] PATH value: %s\n", path_env_val);

    paths_array = ft_split(path_env_val, ':');
    free(path_env_val); // تحرير قيمة PATH الأصلية
    if (!paths_array)
    {
        printf("[DEBUG find_exe] ft_split on PATH failed.\n");
        return NULL;
    }

    result_path_malloc = check_paths(paths_array, cmd_name);
    final_path_gc = NULL;
    if (result_path_malloc)
    {
        final_path_gc = ft_strdup(result_path_malloc); // تحويل إلى gc_malloc
        free(result_path_malloc);                     // تحرير النسخة المؤقتة
        printf("[DEBUG find_exe] Final GC_PATH: %s\n", final_path_gc);
    }

    i = 0;
    while(paths_array[i])
    {
        free(paths_array[i]); // إذا كان ft_split يخصص كل سلسلة بـ malloc
        i++;
    }
    free(paths_array); // تحرير مصفوفة المؤشرات نفسها
    return (final_path_gc);
}