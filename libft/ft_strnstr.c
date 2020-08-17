/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/18 10:10:52 by mlindhol          #+#    #+#             */
/*   Updated: 2019/11/27 13:53:07 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char	*ft_strnstr(const char *hstck, const char *ndl, size_t len)
{
	size_t		i;
	size_t		j;

	if (!*ndl)
		return ((char*)hstck);
	i = 0;
	while (hstck[i] != '\0' && i < len)
	{
		if (hstck[i] == ndl[0])
		{
			j = 1;
			while (ndl[j] != '\0' && hstck[i + j] == ndl[j] &&
					i + j < len)
				j++;
			if (ndl[j] == '\0')
				return ((char*)&hstck[i]);
		}
		i++;
	}
	return (NULL);
}
