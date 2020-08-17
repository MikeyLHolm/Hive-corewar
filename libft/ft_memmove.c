/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/23 13:31:23 by mlindhol          #+#    #+#             */
/*   Updated: 2019/11/27 13:51:14 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*dst2;
	char		*src2;
	size_t		i;

	if (src == dst)
		return (dst);
	dst2 = (char*)dst;
	src2 = (char*)src;
	if (src < dst)
	{
		i = len;
		while (i-- > 0)
			dst2[i] = src2[i];
	}
	else
	{
		i = 0;
		while (i < len)
		{
			dst2[i] = src2[i];
			i++;
		}
	}
	return (dst);
}
