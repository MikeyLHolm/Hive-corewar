/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 14:04:54 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/09 14:05:15 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

char			*ft_strndup(char *src, int n)
{
	char	*s2;
	int		i;
	int		size;

	size = 0;
	while (src[size] && size < n)
		++size;
	if (!(s2 = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	i = 0;
	while (src[i] && i < n)
	{
		s2[i] = src[i];
		i++;
	}
	s2[i] = '\0';
	return (s2);
}
