/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 10:16:30 by mlindhol          #+#    #+#             */
/*   Updated: 2019/11/27 13:52:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dsize)
{
	size_t		len;
	size_t		slen;

	len = 0;
	slen = ft_strlen(src);
	while (*dst && dsize > 0)
	{
		dst++;
		len++;
		dsize--;
	}
	while (*src && dsize > 1)
	{
		*dst++ = *src++;
		dsize--;
	}
	if (dsize == 1 || *src == 0)
		*dst = '\0';
	return (len + slen);
}
