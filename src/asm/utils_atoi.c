/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_atoi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 11:18:36 by elindber          #+#    #+#             */
/*   Updated: 2020/09/30 11:18:42 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int				ft_atoi_edgecase_negative(char *str)
{
	size_t	i;
	char	*long_min;

	long_min = ft_strdup("-9223372036854775808");
	if (ft_strlen(str) > ft_strlen(long_min))
	{
		free(long_min);
		return (0);
	}
	if (ft_strlen(str) == ft_strlen(long_min))
	{
		i = 0;
		while (i < ft_strlen(str))
		{
			if (str[i] > long_min[i])
			{
				free(long_min);
				return (0);
			}
			i++;
		}
	}
	free(long_min);
	return (ft_atoi(str));
}

int				ft_atoi_edgecase_positive(char *str)
{
	size_t	i;
	char	*long_max;

	long_max = ft_strdup("9223372036854775807");
	if (ft_strlen(str) > ft_strlen(long_max))
	{
		free(long_max);
		return (UINT_MAX);
	}
	if (ft_strlen(str) == ft_strlen(long_max))
	{
		i = 0;
		while (i < ft_strlen(str))
		{
			if (str[i] > long_max[i])
			{
				free(long_max);
				return (UINT_MAX);
			}
			i++;
		}
	}
	free(long_max);
	return (ft_atoi(str));
}

int				ft_atoi_edgecase(char *str)
{
	int		i;

	i = 0;
	if (str[0] == '-')
		return (ft_atoi_edgecase_negative(str));
	else
		return (ft_atoi_edgecase_positive(str));
}
