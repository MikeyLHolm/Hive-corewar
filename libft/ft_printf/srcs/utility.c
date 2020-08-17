/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utility.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/09 09:01:48 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 09:28:35 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Handles presicion values after .
*/

void		handle_dot(t_printf *p)
{
	p->prec = 1;
	p->i++;
	if (p->f[p->i] == '*')
	{
		p->p_star = 1;
		p->i++;
	}
	if ((p->f[p->i] < 48 || p->f[p->i] > 57)
		&& p->f[p->i - 1] != '*')
		p->prec = -1;
}

/*
**	Prints out chars until % is found or string ends.
*/

void		print_until_pct(t_printf *p)
{
	int		i;

	i = p->i;
	while (p->f[p->i] != '%' && p->f[p->i] != '\0')
		p->i++;
	write(p->fd, &p->f[i], (p->i - i));
	p->len += (p->i - i);
}

/*
**	Prepend zeros if required.
*/

char		*fill_zeros(char *str, int p_value, int len)
{
	char	*str2;
	int		i;

	i = -1;
	if (!(str2 = (char*)malloc(sizeof(char) * p_value + 1)))
		exit(-1);
	while (++i < p_value - len)
		str2[i] = '0';
	while (i < p_value && *str)
	{
		str2[i] = *str;
		str++;
		i++;
	}
	str2[i] = '\0';
	return (str2);
}

/*
**	Helper function for display_x.
*/

void		hex_helper(t_printf *p)
{
	if (p->wdth == 0)
		write(p->fd, "0", 1);
	else if (p->prec == 0)
		write(p->fd, "0", 1);
	else
		write(p->fd, " ", 1);
}

/*
**	Helper function for parsing width.
*/

int			magic_pill(t_printf *p)
{
	p->w_star = 1;
	p->i++;
	return (1);
}
