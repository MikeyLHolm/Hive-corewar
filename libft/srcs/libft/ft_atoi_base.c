/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/27 15:13:17 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/27 15:25:55 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_is_base_num(char c, int base)
{
	if (base <= 10)
		return (c >= '0' && c <= '9');
	return ((c >= '0' && c <= '9') || (c >= 'A' && c <= ('A' + base - 10)) || \
	(c >= 'a' && c <= ('a' + base - 10)));
}

static int	get_base_num(int result, int base, char c)
{
	if (c >= 'A' && 'F' >= c)
		result = (result * base) + (c - 'A' + 10);
	else if (c >= 'a' && 'f' >= c)
		result = (result * base) + (c - 'a' + 10);
	else
		result = (result * base) + (c - '0');
	return (result);
}

int			ft_atoi_base(const char *str, int base)
{
	int result;
	int i;
	int j;
	int flag;

	i = 0;
	result = 0;
	flag = 1;
	while (str[i] == ' ' || (8 < str[i] && str[i] < 14))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			flag = -1;
		i++;
	}
	j = i;
	while (ft_is_base_num(str[i], base))
		i++;
	while (i > j && ft_is_base_num(str[j], base))
		result = get_base_num(result, base, str[j++]);
	return (result * flag);
}
