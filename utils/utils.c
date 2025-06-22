/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaboudra <aaboudra@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/03 00:37:17 by aaboudra          #+#    #+#             */
/*   Updated: 2025/06/17 17:59:04 by aaboudra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../minishell.h"

static int ft_isspace_local(char c) // دالة مساعدة محلية
{
    return (c == ' ' || c == '\t' || c == '\n' || \
            c == '\v' || c == '\f' || c == '\r');
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

int is_s_char(char c)
{
    return (c == '|' || c == '<' || c == '>');
}

int is_space(char s)
{
    return (s == ' ' || s == '\t' || s == '\n');
}

size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

void *ft_memcpy(void *dst, const void *src, size_t n)
{
	const unsigned char	*p_sr;
	unsigned char		*p_dst;

	if (dst == NULL && src == NULL)
		return (NULL);
	if (dst == src)
		return (dst);
	p_sr = (const unsigned char *)src;
	p_dst = (unsigned char *) dst;
	while (n--)
	{
		*p_dst++ = *p_sr++;
	}
	return (dst);
}
int	ft_strcmp(char *s1, char *s2)
{
	int	c;

	c = 0;
	while (s1[c] == s2[c] && s1[c] != '\0' && s2[c] != '\0')
		c++;
	return (s1[c] - s2[c]);
}
char	*ft_strjoin(char const *s1, char const *s2, t_data *data)
{
	char	*p;
	size_t	i1;
	size_t	i2;

	if (!s1 && !s2)
		return (NULL);
	if (!s1 && s2)
		return ((char *)s2);
	if (s1 && !s2)
		return ((char *)s1);
	i1 = ft_strlen(s1);
	i2 = ft_strlen(s2);
	p = (char *)gc_malloc((i1 + i2 + 1) * sizeof (char), data);
	ft_memcpy(p, s1, i1);
	ft_memcpy(p + i1, s2, i2);
	p[i1 + i2] = '\0';
	return (p);
}

