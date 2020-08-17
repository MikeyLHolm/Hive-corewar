/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_pct.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 16:07:28 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/28 11:34:15 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Prints % if %%.
*/

void	display_pct(t_printf *p)
{
	int		i;

	i = 0;
	handle_wildcard(p);
	if (p->minus)
	{
		write(p->fd, "%", 1);
		while (i++ < p->w_value - 1)
			write(p->fd, " ", 1);
	}
	else if (p->minus == 0)
	{
		while (i++ < p->w_value - 1)
			p->zero ? write(p->fd, "0", 1) : write(p->fd, " ", 1);
		write(p->fd, "%", 1);
	}
	p->len += (p->w_value > 1) ? p->w_value : 1;
}
