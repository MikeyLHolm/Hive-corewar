/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_o.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/16 14:21:49 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 16:58:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	# flag helper.
*/

static void		put_hash(t_printf *p, int len, char *str)
{
	if (p->hash && p->nil == 0 && p->p_value < len)
		write(p->fd, "0", 1);
	else if (p->p_value == len && p->nil == 0 && str[0] != '0' && p->hash)
	{
		write(p->fd, "0", 1);
		p->len++;
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
**	Main print function when minus = 0
*/

void			put_octal(t_printf *p, char *str, int len)
{
	if (p->hash && p->prec == 0 && p->zero)
	{
		put_hash(p, len, str);
		p->wdth && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	}
	else if (p->hash)
	{
		p->prec == 0 && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
		put_hash(p, len, str);
	}
	else if (p->hash == 0)
	{
		put_hash(p, len, str);
		p->prec == 0 && p->zero ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	}
	if (p->nil == 0 || (p->nil && p->p_value))
		while (*str)
			write(p->fd, str++, 1);
	else if (p->nil && p->p_value == 0 && p->prec && p->hash == 0)
		write(p->fd, " ", 1);
	else if (p->nil)
		write(p->fd, "0", 1);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Main print function when minus == 1.
*/

void			put_octal_minus(t_printf *p, char *str, int len)
{
	put_hash(p, len, str);
	if (p->nil == 0 || (p->nil && p->p_value))
		while (*str)
			write(p->fd, str++, 1);
	else if (p->nil && p->p_value == 0 && p->prec && p->hash == 0)
		write(p->fd, " ", 1);
	else if (p->nil)
		write(p->fd, "0", 1);
	put_padd(p, ' ', len);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Main display function for %o
*/

void			display_o(t_printf *p)
{
	char		*octal;
	char		*octal_p;
	int			len;
	uintmax_t	o;

	if ((o = get_conv_unsigned(p)) == 0)
		p->nil = 1;
	if (p->prec && p->p_value == 0 && p->wdth == 0 && p->nil && p->hash == 0)
		return ;
	octal = ft_itoa_base(o, 8);
	len = ft_strlen_int(octal);
	if (p->prec && p->p_value > len)
	{
		octal_p = fill_zeros(octal, p->p_value, len);
		free(octal);
		octal = ft_strdup(octal_p);
		free(octal_p);
		len = ft_strlen_int(octal);
	}
	len += (p->hash && p->nil == 0 && p->p_value < len) ? 1 : 0;
	p->minus == 0 ? put_octal(p, octal, len) : put_octal_minus(p, octal, len);
	free(octal);
}
