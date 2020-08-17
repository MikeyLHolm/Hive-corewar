/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_intlen_uimax.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/28 08:56:55 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/15 12:36:24 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"
#include <stdlib.h>

int		ft_intlen_uimax(uintmax_t n)
{
	int		i;

	if (n < 0)
		n *= -1;
	i = 0;
	while (n >= 10)
	{
		n /= 10;
		i++;
	}
	return (1 + i);
}
