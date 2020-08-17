/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_c.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 16:07:28 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/28 11:34:15 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Prints char if %c.
*/

static void		put_padd(t_printf *p)
{
	int		i;

	i = 0;
	while (i++ < p->w_value - 1)
		write(p->fd, " ", 1);
}

void			display_c(t_printf *p)
{
	int		c;

	handle_wildcard(p);
	c = va_arg(p->ap, int);
	if (p->minus == 0)
	{
		put_padd(p);
		write(p->fd, &c, 1);
	}
	else if (p->minus == 1)
	{
		write(p->fd, &c, 1);
		put_padd(p);
	}
	p->len += (1 < p->w_value) ? p->w_value : 1;
}
