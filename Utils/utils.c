#include "../minishell.h"

int is_s_char(char c) // Special char for shell syntax
{
    return (c == '|' || c == '<' || c == '>');
}

int is_space(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' || \
            c == '\v' || c == '\f' || c == '\r');
}

size_t	ft_strlen(const char *s)
{
    size_t i = 0;
    if (!s) return (0);
    while (s[i])
        i++;
    return (i);
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
    unsigned char   *d;
    const unsigned char *s;

    if (!dst && !src) return (NULL);
    d = (unsigned char *)dst;
    s = (const unsigned char *)src;
    while (n--)
        *d++ = *s++;
    return (dst);
}

// Ensure this matches the prototype in minishell.h (const char *)
int	ft_strcmp(const char *s1, const char *s2)
{
    size_t i = 0;
    while (s1[i] && s2[i] && s1[i] == s2[i])
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
    char    *new_str;
    size_t  len1;
    size_t  len2;

    if (!s1 && !s2) return (NULL);
    if (!s1) return (ft_strdup(s2)); // Assumes ft_strdup handles NULL s2 if needed
    if (!s2) return (ft_strdup(s1));

    len1 = ft_strlen(s1);
    len2 = ft_strlen(s2);
    new_str = gc_malloc(len1 + len2 + 1); // Use gc_malloc
    if (!new_str) return (NULL);
    ft_memcpy(new_str, s1, len1);
    ft_memcpy(new_str + len1, s2, len2);
    new_str[len1 + len2] = '\0';
    return (new_str);
}

static int ft_isspace_local(char c) // دالة مساعدة محلية
{
    return (c == ' ' || c == '\t' || c == '\n' || \
            c == '\v' || c == '\f' || c == '\r');
}

int ft_isalnum(int c)
{
    return (ft_isalpha(c) || (c >= '0' && c <= '9'));
}

int ft_isalpha(int c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}


int ft_atoi(const char *str)
{
    int i;
    int sign;
    long result; // استخدم long لتجنب الـ overflow قبل التحقق

    i = 0;
    sign = 1;
    result = 0;
    while (str[i] && ft_isspace_local(str[i]))
        i++;
    if (str[i] == '-' || str[i] == '+')
    {
        if (str[i] == '-')
            sign = -1;
        i++;
    }
    while (str[i] >= '0' && str[i] <= '9')
    {
        // تحقق من الـ overflow لـ int
        if (result > (2147483647 - (str[i] - '0')) / 10 && sign == 1)
            return (-1); // Overflow (مثل atoi القياسية)
        if (result > (2147483648 - (str[i] - '0')) / 10 && sign == -1)
            return (0);  // Overflow (مثل atoi القياسية)
        result = result * 10 + (str[i] - '0');
        i++;
    }
    return ((int)(result * sign));
}