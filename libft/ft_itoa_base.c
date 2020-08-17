/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 13:49:34 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/22 12:53:48 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"
#include <stdlib.h>

int		value_len(uintmax_t nb, int base)
{
	int		len;

	len = 0;
	while (nb != 0)
	{
		nb /= base;
		len++;
	}
	return (len);
}

char	*ft_itoa_base(intmax_t value, int base)
{
	int					len;
	uintmax_t			nb;
	char				*ret;
	char				*arr;

	nb = (value < 0 && base == 10) ? -value : (uintmax_t)value;
	len = (value == 0 || (value < 0 && base == 10)) ? 1 : 0;
	len += value_len(nb, base);
	if (!(ret = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	arr = ft_strdup("0123456789abcdef");
	if (value < 0 && base == 10)
		ret[0] = '-';
	if (value == 0)
		ret[0] = '0';
	ret[len] = '\0';
	nb = (value < 0 && base == 10) ? -value : (uintmax_t)value;
	while (nb != 0)
	{
		ret[--len] = arr[nb % base];
		nb /= base;
	}
	free(arr);
	return (ret);
}
