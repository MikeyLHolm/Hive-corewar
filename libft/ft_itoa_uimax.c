/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_uimax.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 08:23:52 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/17 16:29:53 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"
#include <stdlib.h>

char			*ft_itoa_uimax(uintmax_t nb)
{
	char			*ret;
	int				len;

	len = ft_intlen_uimax(nb);
	if (!(ret = (char *)malloc(sizeof(char) * len + 1)))
		return (NULL);
	ret[len] = '\0';
	while (len-- > 0)
	{
		ret[len] = nb % 10 + '0';
		nb /= 10;
	}
	return (ret);
}
