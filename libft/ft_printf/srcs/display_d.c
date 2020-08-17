/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_d.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 14:12:06 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 16:58:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Custom intmax itoa for ints.
*/

static char		*ft_itoa_imax(t_printf *p, intmax_t n)
{
	char			*ret;
	int				len;
	long long		nb;

	if (n == -9223372036854775807 - 1)
		return (ft_strdup("-9223372036854775808"));
	if (n < 0)
		p->neg = 1;
	if (n == 0)
		p->nil = 1;
	nb = ft_abs(n);
	len = ft_intlen_uimax(nb);
	if (!(ret = (char *)malloc(sizeof(char) * len + 1)))
		exit(-1);
	ret[len] = '\0';
	while (len-- > 0)
	{
		ret[len] = nb % 10 + '0';
		nb /= 10;
	}
	if (p->plus)
		p->space = 0;
	if (p->minus)
		p->zero = 0;
	return (ret);
}

/*
**	Prints padding: spaces or zeros.
*/

static void		put_padd(t_printf *p, char c, int len)
{
	int		i;

	i = 0;
	if (p->minus || p->prec)
		c = ' ';
	if (p->w_value > len)
		while (i++ < (p->w_value - len))
			write(p->fd, &c, 1);
}

/*
**	Print function when minus = 0
**	Flow: flags->padding->string.
*/

static void		put_nbr(t_printf *p, char *str, int len)
{
	len += (p->neg || (p->plus) || p->space) ? 1 : 0;
	if (p->nil && p->prec && p->p_value == 0)
		len--;
	if (p->neg && p->zero && ((p->w_value <= len) ||
							(p->w_value > len && !p->prec)))
		write(p->fd, "-", 1);
	else if (p->plus && !p->neg && p->zero && !p->prec)
		write(p->fd, "+", 1);
	else if (p->space && !p->neg)
		write(p->fd, " ", 1);
	p->zero == 1 ? put_padd(p, '0', len) : put_padd(p, ' ', len);
	if (p->neg && (!p->zero || (p->zero && p->w_value > len && p->prec)))
		write(p->fd, "-", 1);
	if (p->plus && !p->neg && (!p->zero || (p->zero && p->prec)))
		write(p->fd, "+", 1);
	if (!(p->nil && p->prec && p->p_value == 0))
		while (*str)
			write(p->fd, str++, 1);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Print function when minus = 1
**	Flow: flags->string->padding.
*/

static void		put_nbr_minus(t_printf *p, char *str, int len)
{
	len += (p->neg || p->plus || p->space) ? 1 : 0;
	if (p->neg)
		write(p->fd, "-", 1);
	if (!p->neg && (p->plus || p->space))
		p->plus ? write(p->fd, "+", 1) : write(p->fd, " ", 1);
	while (*str)
		write(p->fd, str++, 1);
	put_padd(p, ' ', len);
	p->len += (len < p->w_value) ? p->w_value : len;
}

/*
**	Prints int if %d, %i or %D.
*/

void			display_d(t_printf *p)
{
	char			*n;
	char			*n_p;
	int				len;
	intmax_t		d;

	d = get_conv_signed(p);
	if (p->prec && p->p_value == 0 && p->w_value == 0 && d == 0)
	{
		if (p->plus)
			write(p->fd, "+", 1);
		return ;
	}
	n = ft_itoa_imax(p, d);
	n = (d == 0 && p->prec && p->p_value == 0 && p->wdth) ? ft_strdup(" ") : n;
	len = ft_strlen_int(n);
	if (p->prec && p->p_value > len)
	{
		n_p = fill_zeros(n, p->p_value, len);
		free(n);
		n = ft_strdup(n_p);
		free(n_p);
		len = ft_strlen_int(n);
	}
	p->minus == 0 ? put_nbr(p, n, len) : put_nbr_minus(p, n, len);
	free(n);
}
