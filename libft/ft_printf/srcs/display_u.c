/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_u.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 13:52:22 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 16:58:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Print padding.
*/

static void		put_padd(t_printf *p, char c, int len)
{
	int		i;
	int		z;

	i = 0;
	z = (p->p_value > len) ? p->p_value : len;
	if (p->w_value > len)
		while (i++ < (p->w_value - z))
			write(p->fd, &c, 1);
}

/*
**	Print function when minus = 0.
*/

static void		put_unsign(t_printf *p, char *str, int len)
{
	if (p->prec == 0)
		p->zero == 1 ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	else
		put_padd(p, ' ', len);
	while (*str)
		write(p->fd, str++, 1);
}

/*
**	Print function when minus = 1.
*/

static void		put_unsign_minus(t_printf *p, char *str, int len)
{
	while (*str)
		write(p->fd, str++, 1);
	if (p->prec == 0)
		p->zero == 1 ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	else
		put_padd(p, ' ', len);
}

/*
**	Main display function for %u
*/

void			display_u(t_printf *p)
{
	char		*usign;
	char		*usign_p;
	int			len;
	uintmax_t	u;

	u = get_conv_unsigned(p);
	if (p->prec && p->p_value == 0 && u == 0)
		usign = ft_strdup("");
	else
		usign = ft_itoa_uimax(u);
	len = ft_strlen_int(usign);
	if (p->prec && p->p_value > len)
	{
		usign_p = fill_zeros(usign, p->p_value, len);
		free(usign);
		usign = ft_strdup(usign_p);
		free(usign_p);
		len = ft_strlen_int(usign);
	}
	p->minus == 0 ? put_unsign(p, usign, len) : put_unsign_minus(p, usign, len);
	p->len += (len < p->w_value) ? p->w_value : len;
	free(usign);
}
