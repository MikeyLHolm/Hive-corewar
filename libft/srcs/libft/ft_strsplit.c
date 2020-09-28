/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/01 13:16:14 by elindber          #+#    #+#             */
/*   Updated: 2020/09/23 13:44:24 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(const char *str, int d)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str && str[i])
	{
		if ((str[i] != d && str[i + 1] == d) ||
				(str[i] != d && str[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

char		**ft_strsplit(char const *s, char c)
{
	unsigned int	i;
	size_t			start;
	char			**str;
	int				y;

	y = 0;
	i = 0;
	start = 0;
	if (!s)
		return (NULL);
	if (!(str = (char**)ft_memalloc(sizeof(char*) * (ft_count(s, (int)c) + 1))))
		return (NULL);
	while (y < ft_count(s, (int)c))
	{
		while (s[i] == (int)c)
			i++;
		start = i;
		while (s[i] && s[i] != (int)c)
			i++;
		if (!(str[y] = ft_strsub(s, start, (i - start))))
			return (NULL);
		i++;
		y++;
	}
	return (str);
}
