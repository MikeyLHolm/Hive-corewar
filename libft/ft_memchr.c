/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 12:24:28 by mlindhol          #+#    #+#             */
/*   Updated: 2019/11/27 13:50:56 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*str;
	unsigned char	d;

	i = 0;
	d = (unsigned char)c;
	str = (unsigned char*)s;
	while (i < n)
	{
		if (str[i] == d)
			return (&str[i]);
		i++;
	}
	return (NULL);
}
