#include "../minishell.h"

static int  count_digits(long n)
{
    int count;
    if (n == 0) return (1);
    count = 0;
    if (n < 0) {
        count++; // For sign
        n = -n;
    }
    while (n > 0) {
        n /= 10;
        count++;
    }
    return (count);
}

char	*ft_itoa(int n_int)
{
    char    *str;
    long    n_long; // Use long to handle INT_MIN
    int     len;
    int     is_negative;

    n_long = n_int;
    len = count_digits(n_long);
    str = gc_malloc(sizeof(char) * (len + 1)); // Use gc_malloc
    if (!str) return (NULL);
    str[len] = '\0';
    is_negative = 0;
    if (n_long == 0) str[0] = '0';
    if (n_long < 0) {
        is_negative = 1;
        n_long = -n_long;
        str[0] = '-';
    }
    while (n_long > 0) {
        str[--len] = (n_long % 10) + '0';
        n_long /= 10;
    }
    return (str);
}


// handle_quotes: Removes the outermost layer of quotes.
// This is a simplified version. Proper shell quote handling is complex.
char *handle_quotes(char *token_str_orig) // Takes original, returns new gc_malloc'd string
{
    int     i;
    int     j;
    char    *new_str;
    size_t  len;
    char    first_char;
    char    last_char;

    if (!token_str_orig) return (NULL);
    len = ft_strlen(token_str_orig);
    if (len < 2) return (ft_strdup(token_str_orig)); // Not quoted or too short

    first_char = token_str_orig[0];
    last_char = token_str_orig[len - 1];

    if ((first_char == '\'' && last_char == '\'') || \
        (first_char == '"' && last_char == '"'))
    {
        // Allocate for string without the two quotes
        new_str = gc_malloc(len - 2 + 1);
        if (!new_str) return (NULL); // Malloc error
        j = 0;
        i = 1; // Start after the first quote
        while (i < (int)len - 1) // Go until before the last quote
        {
            new_str[j++] = token_str_orig[i++];
        }
        new_str[j] = '\0';
        // gc_free_ptr(token_str_orig); // Caller should free original if it was also gc_malloc'd
        return (new_str);
    }
    return (ft_strdup(token_str_orig)); // No surrounding quotes of same type
}