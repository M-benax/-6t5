#include "../minishell.h" // Adjust path if necessary

// Global list for garbage collection
t_gc_node *g_gc_list = NULL;

void *gc_malloc(size_t size)
{
    void        *ptr;
    t_gc_node   *node;

    if (size == 0) return (NULL); // Or return a unique ptr like ((void*)1) if subject allows

    ptr = malloc(size);
    if (!ptr)
    {
        // Attempt to free some GC memory and retry? Or just fail.
        // For Minishell, usually fail hard if malloc fails.
        perror("minishell: malloc failed in gc_malloc");
        // gc_free_all(); // Freeing all here might be too drastic if it's not the final exit
        exit(EXIT_FAILURE); // Critical failure
    }
    node = malloc(sizeof(t_gc_node)); // This malloc is not part of GC itself
    if (!node)
    {
        free(ptr); // Free the ptr we just malloc'd
        perror("minishell: malloc failed for gc_node");
        // gc_free_all();
        exit(EXIT_FAILURE);
    }
    node->ptr = ptr;
    node->next = g_gc_list;
    g_gc_list = node;
    return (ptr);
}

void gc_free_ptr(void *ptr_to_free)
{
    t_gc_node   *current;
    t_gc_node   *prev;

    if (!ptr_to_free || !g_gc_list)
        return;
    current = g_gc_list;
    prev = NULL;
    while (current)
    {
        if (current->ptr == ptr_to_free)
        {
            if (prev)
                prev->next = current->next;
            else
                g_gc_list = current->next;
            free(current->ptr); // Free the actual memory
            free(current);      // Free the gc_node
            return;
        }
        prev = current;
        current = current->next;
    }
    // If ptr_to_free was not found, it might have been freed already or not gc_managed
}

void gc_free_all(void)
{
    t_gc_node *current;
    t_gc_node *next_node;

    current = g_gc_list;
    while (current)
    {
        next_node = current->next;
        if (current->ptr) // Should always be true if node exists
            free(current->ptr);
        free(current);
        current = next_node;
    }
    g_gc_list = NULL;
}

// ft_strdup using gc_malloc (from your original file)
char	*ft_strdup(const char *s1)
{
    char	*p;
    size_t	len;

    if (!s1) return (NULL); // Or strdup("") for ""? Standard strdup behavior for NULL is UB.
                            // Let's assume NULL returns NULL.
    len = ft_strlen(s1);
    p = (char *)gc_malloc(len + 1);
    if (!p) return (NULL);
    ft_memcpy(p, s1, len + 1);
    return (p);
}

// ft_strndup using gc_malloc (from your original file)
char	*ft_strndup(const char *s1, size_t n)
{
    char	*p;
    size_t	len;

    if (!s1) return (NULL);
    len = ft_strlen(s1);
    if (n < len)
        len = n;
    p = (char *)gc_malloc(len + 1);
    if (!p) return (NULL);
    ft_memcpy(p, s1, len);
    p[len] = '\0';
    return (p);
}

// new_node for t_comand (from your original file)
// This should probably be in parsing utils, but kept here as per your original structure
t_comand *new_node(char *token_word)
{
    t_comand *node;

    node = (t_comand *)gc_malloc(sizeof(t_comand));
    if (!node) return (NULL);
    node->word = ft_strdup(token_word); // Duplicates the word, uses gc_malloc
    if (!node->word && token_word) { // Check if ft_strdup failed
        gc_free_ptr(node);
        return NULL;
    }
    node->type = T_WORD; // Default type, put_type will set it correctly later
    node->next = NULL;
    return (node);
}

// gc_free_array, gc_free_pid_list, gc_free_token_list, gc_free_cmd_structure
// These should be defined here or in a separate gc_utils.c
void gc_free_array(char **array)
{
    int i;
    if (!array) return;
    i = 0;
    while (array[i])
    {
        gc_free_ptr(array[i]); // Assumes strings in array were gc_malloc'd
        i++;
    }
    gc_free_ptr(array); // Free the array of pointers itself
}

void gc_free_pid_list(t_pid_list *list)
{
    t_pid_list *tmp;
    while (list)
    {
        tmp = list;
        list = list->next;
        gc_free_ptr(tmp);
    }
}

void gc_free_token_list(t_comand *list)
{
    t_comand *tmp;
    while (list)
    {
        tmp = list;
        list = list->next;
        gc_free_ptr(tmp->word); // word was ft_strdup'd
        gc_free_ptr(tmp);
    }
}

void gc_free_cmd_structure(t_cmd *cmd_list)
{
    t_cmd   *current_cmd;
    t_cmd   *next_cmd;

    current_cmd = cmd_list;
    while (current_cmd)
    {
        next_cmd = current_cmd->next;
        gc_free_array(current_cmd->args); // Free args array and its strings
        gc_free_ptr(current_cmd->in_file);
        gc_free_ptr(current_cmd->out_file);
        gc_free_ptr(current_cmd);
        current_cmd = next_cmd;
    }
}