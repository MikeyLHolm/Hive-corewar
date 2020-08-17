/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ftoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 14:40:48 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/25 15:39:01 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/libft.h"

static long double	rounding(int precision, long double f)
{
	long double	rounding;
	int			d;

	rounding = 0.5;
	if (f < 0)
		rounding *= -1;
	d = 0;
	while (d++ < precision)
		rounding /= 10.0;
	return (rounding);
}

char				*ft_ftoa(long double f, int precision, int dot)
{
	unsigned long long	dec;
	char				*ipart;
	char				*fpart;
	char				*joint;
	int					i;

	f = f + rounding(precision, f);
	f *= (f < 0) ? -1 : 1;
	dec = f;
	ipart = ft_itoa_uimax(dec);
	f = precision ? (f - dec) : 0;
	fpart = ft_strnew(precision + 2);
	fpart[0] = (dot == 1 && precision > 0) ? '.' : '\0';
	i = 1;
	while (precision-- > 0)
	{
		f *= 10;
		dec = f;
		f -= dec;
		fpart[i++] = dec + '0';
	}
	joint = ft_strjoin(ipart, fpart);
	free(fpart);
	free(ipart);
	return (joint);
}
