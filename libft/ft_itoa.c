/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/24 08:23:52 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/25 15:41:01 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static int		return_size(int n)
{
	int		size;

	size = 0;
	if (n < 0)
		n *= -1;
	while (n >= 10)
	{
		n /= 10;
		size++;
	}
	size++;
	return (size);
}

char			*ft_itoa(int n)
{
	char			*str;
	int				i;
	int				size;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	size = (n < 0) ? return_size(n) + 1 : return_size(n);
	if (!(str = (char *)ft_memalloc(sizeof(char) * size + 1)))
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		n *= -1;
	}
	i = size - 1;
	while (n >= 10)
	{
		str[i] = (char)(n % 10 + 48);
		n /= 10;
		i--;
	}
	str[i] = (char)(n % 10 + 48);
	str[size] = '\0';
	return (str);
}
