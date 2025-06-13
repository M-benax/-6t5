#include "../minishell.h"

static size_t	count_words(const char *s, char c)
{
    size_t	count;
    int		in_word;

    count = 0;
    in_word = 0;
    while (*s)
    {
        if (*s != c && !in_word)
        {
            in_word = 1;
            count++;
        }
        else if (*s == c)
            in_word = 0;
        s++;
    }
    return (count);
}

static char	*get_next_word(const char **s_ptr, char c)
{
    const char	*word_start;
    size_t		len;

    while (**s_ptr && **s_ptr == c) // Skip leading delimiters
        (*s_ptr)++;
    word_start = *s_ptr;
    len = 0;
    while ((*s_ptr)[len] && (*s_ptr)[len] != c)
        len++;
    if (len == 0)
        return (NULL); // Should not happen if count_words is correct
    *s_ptr += len; // Move main pointer past the word
    return (ft_strndup(word_start, len)); // Uses gc_malloc via ft_strndup
}

char	**ft_split(char const *s, char c)
{
    char		**result_array;
    size_t		word_count;
    size_t		i;
    const char	*s_runner; // To avoid modifying original s if not needed

    if (!s)
        return (NULL);
    s_runner = s;
    word_count = count_words(s_runner, c);
    result_array = (char **)gc_malloc(sizeof(char *) * (word_count + 1));
    if (!result_array)
        return (NULL);
    i = 0;
    s_runner = s; // Reset runner
    while (i < word_count)
    {
        result_array[i] = get_next_word(&s_runner, c);
        if (!result_array[i])
        {
            // Malloc error during get_next_word, cleanup previously allocated
            gc_free_array(result_array); // Assumes gc_free_array frees individual strings too
            return (NULL);
        }
        i++;
    }
    result_array[i] = NULL;
    return (result_array);
}