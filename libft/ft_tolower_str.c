/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tolower_str.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/22 12:58:57 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/22 13:03:49 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

char		*ft_tolower_str(char *str, int len)
{
	char	*ret;
	int		i;

	i = 0;
	if (!(ret = (char *)malloc(sizeof(char) * len + 1)))
		exit(1);
	while (i < len)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
			ret[i] = str[i] + 32;
		else
			ret[i] = str[i];
		i++;
	}
	ret[len] = '\0';
	return (ret);
}
