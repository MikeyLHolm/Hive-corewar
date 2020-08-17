/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_x.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/15 13:52:22 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 16:58:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Hash flag helper to print 0x.
*/

static void		put_hash(t_printf *p)
{
	if (p->hash && p->nil == 0)
	{
		if (p->caps == 0)
			write(p->fd, "0x", 2);
		else if (p->caps == 1)
			write(p->fd, "0X", 2);
	}
}

/*
**	Print padding.
*/

static void		put_padd(t_printf *p, char c, int len)
{
	int		i;

	i = 0;
	if (p->w_value > len)
		while (i++ < (p->w_value - len))
			write(p->fd, &c, 1);
}

/*
**	Print function when minus is off.
*/

static void		put_hex(t_printf *p, char *str, int len)
{
	if (p->hash && p->prec == 0 && p->zero)
	{
		put_hash(p);
		p->wdth && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	}
	else if (p->hash)
	{
		p->prec == 0 && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
		put_hash(p);
	}
	else if (p->hash == 0)
	{
		put_hash(p);
		p->prec == 0 && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	}
	if (p->nil == 0 || (p->nil && p->p_value))
		while (*str)
			write(p->fd, str++, 1);
	else if (p->nil)
		hex_helper(p);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Print function when minus is on.
*/

static void		put_hex_minus(t_printf *p, char *str, int len)
{
	put_hash(p);
	if (p->nil == 0 || (p->nil && p->p_value))
		while (*str)
			write(p->fd, str++, 1);
	else if (p->nil)
		hex_helper(p);
	put_padd(p, ' ', len);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Display %x and %X main function.
*/

void			display_x(t_printf *p, int caps)
{
	char		*hex;
	char		*hex_p;
	int			len;
	uintmax_t	h;

	if ((h = get_conv_unsigned(p)) == 0)
		p->nil = 1;
	if (p->prec && p->p_value == 0 && p->wdth == 0 && p->nil)
		return ;
	hex = ft_itoa_base(h, 16);
	len = ft_strlen_int(hex);
	if (caps == 1)
		hex = ft_toupper_str(hex, len);
	if (p->prec && p->p_value > len)
	{
		hex_p = fill_zeros(hex, p->p_value, len);
		free(hex);
		hex = ft_strdup(hex_p);
		free(hex_p);
		len = ft_strlen_int(hex);
	}
	len += (p->hash && p->nil == 0) ? 2 : 0;
	p->minus == 0 ? put_hex(p, hex, len) : put_hex_minus(p, hex, len);
	free(hex);
}
