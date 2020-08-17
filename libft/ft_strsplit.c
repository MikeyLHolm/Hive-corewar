/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 08:44:35 by mlindhol          #+#    #+#             */
/*   Updated: 2019/12/17 16:29:31 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"
#include <stdlib.h>

static size_t		word_count(char const *s, char c)
{
	size_t	count;
	int		token;

	count = 0;
	token = 0;
	while (*s)
	{
		if (token == 1 && *s == c)
			token = 0;
		if (token == 0 && *s != c)
		{
			token = 1;
			count++;
		}
		s++;
	}
	return (count);
}

static size_t		word_len(char const *s, char c)
{
	size_t		len;

	len = 0;
	while (*s != c && *s != '\0')
	{
		len++;
		s++;
	}
	return (len);
}

char				**ft_strsplit(char const *s, char c)
{
	char		**ret;
	size_t		wrd_count;
	size_t		word;

	word = 0;
	if (s == NULL)
		return (NULL);
	wrd_count = word_count(s, c);
	ret = (char **)malloc(sizeof(char *) * wrd_count + 1);
	if (ret == NULL)
		return (NULL);
	while (wrd_count--)
	{
		while (*s == c && *s != '\0')
			s++;
		ret[word] = ft_strsub(s, 0, word_len(s, c));
		if (ret[word] == NULL)
			return (NULL);
		s = s + word_len(s, c);
		word++;
	}
	ret[word] = NULL;
	return (ret);
}
