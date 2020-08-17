/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/03 11:23:12 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/28 17:01:29 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Parses thru length flags: h, hh, l , ll, L, j and z
*/

void		parse_length(t_printf *p)
{
	if (p->f[p->i] != '\0')
	{
		if (p->f[p->i] == 'h' && (p->f[p->i + 1] == 'h'))
		{
			p->specifier = HH;
			p->i++;
		}
		else if (p->f[p->i] == 'h')
			p->specifier = H;
		else if (p->f[p->i] == 'j')
			p->specifier = J;
		else if (p->f[p->i] == 'l' && (p->f[p->i + 1] == 'l'))
		{
			p->specifier = LL;
			p->i++;
		}
		else if (p->f[p->i] == 'l')
			p->specifier = L;
		else if (p->f[p->i] == 'L')
			p->specifier = LD;
		else if (p->f[p->i] == 'z')
			p->specifier = Z;
		if (p->specifier)
			p->i++;
	}
}

/*
**	Parsing .precision specifier.
*/

void		parse_precision(t_printf *p)
{
	char	*prec;
	int		i;
	int		w_len;

	if (p->f[p->i] == '.')
		handle_dot(p);
	i = p->i;
	w_len = 0;
	while (p->f[i] >= 48 && p->f[i] <= 57)
	{
		w_len++;
		i++;
	}
	if (!(prec = (char *)malloc(sizeof(char *) * w_len + 1)))
		exit(-1);
	i = 0;
	while (i < w_len)
		prec[i++] = p->f[p->i++];
	prec[i] = '\0';
	p->p_value = ft_atoi(prec);
	free(prec);
}

/*
**	Parsing min field width.
*/

void		parse_width(t_printf *p)
{
	char	*wdth;
	int		i;
	int		w_len;

	i = p->i;
	w_len = 0;
	if (p->f[p->i] == '*')
		i += magic_pill(p);
	while (p->f[i] >= 48 && p->f[i] <= 57)
	{
		w_len++;
		i++;
	}
	if (!(wdth = (char *)malloc(sizeof(char) * w_len + 1)))
		exit(-1);
	i = 0;
	while (i < w_len)
		wdth[i++] = p->f[p->i++];
	wdth[i] = '\0';
	p->w_value = ft_atoi(wdth);
	p->wdth = (p->w_value > 0) ? 1 : 0;
	free(wdth);
}

/*
**	Parsing flags #, -, +, whitespace and 0./
*/

void		parse_flags(t_printf *p)
{
	while (ft_strchr("#-+ 0", (int)p->f[p->i]))
	{
		if (p->f[p->i] == '#')
			p->hash = 1;
		else if (p->f[p->i] == '-')
			p->minus = 1;
		else if (p->f[p->i] == '+')
			p->plus = 1;
		else if (p->f[p->i] == ' ')
			p->space = 1;
		else if (p->f[p->i] == '0')
			p->zero = 1;
		p->i++;
	}
}

/*
**	After gathering specifier data dispatch to right display function is done.
*/

void		dispatcher(t_printf *p)
{
	if (p->f[p->i] && ft_strchr("bcCdDfiopsuxX%", (int)p->f[p->i]))
	{
		if (p->f[p->i] == 'c' || p->f[p->i] == 'C')
			display_c(p);
		else if (p->f[p->i] == 'd' || p->f[p->i] == 'i' || p->f[p->i] == 'D')
			display_d(p);
		else if (p->f[p->i] == 'f')
			display_f(p);
		else if (p->f[p->i] == 'o')
			display_o(p);
		else if (p->f[p->i] == 'p')
			display_p(p);
		else if (p->f[p->i] == 's')
			display_s(p);
		else if (p->f[p->i] == 'u')
			display_u(p);
		else if (p->f[p->i] == 'x')
			display_x(p, p->caps = 0);
		else if (p->f[p->i] == 'X')
			display_x(p, p->caps = 1);
		else if (p->f[p->i] == '%')
			display_pct(p);
		else if (p->f[p->i] == 'b')
			display_b(p);
	}
}
