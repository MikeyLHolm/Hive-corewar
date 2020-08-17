/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display_s.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/02 16:14:02 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/29 16:58:30 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_printf.h"

/*
**	Actual string printer when minus switch is off.
*/

static void		put_str(t_printf *p, char *str, int len)
{
	int		i;
	int		limit;

	i = 0;
	limit = (p->prec && len > 0 && p->p_value < len) ? p->p_value : len;
	if (p->w_value > limit)
		while (i++ < (p->w_value - limit))
			write(p->fd, " ", 1);
	i = 0;
	if (p->prec)
		while (str[i] && i < p->p_value)
			write(p->fd, &str[i++], 1);
	else
		while (str[i])
			write(p->fd, &str[i++], 1);
}

/*
**	Actual string printer when minus switch is on.
*/

static void		put_str_minus(t_printf *p, char *str, int len)
{
	int		i;
	int		limit;

	i = 0;
	if (p->prec)
		while (str[i] && i < p->p_value)
			write(p->fd, &str[i++], 1);
	else
		while (str[i])
			write(p->fd, &str[i++], 1);
	i = 0;
	limit = (p->prec && len > 0 && p->p_value < len) ? p->p_value : len;
	if (p->w_value > limit)
		while (i++ < (p->w_value - limit))
			write(p->fd, " ", 1);
}

/*
**	Prints string if %s.
*/

void			display_s(t_printf *p)
{
	char		*s;
	int			len;

	len = 0;
	handle_wildcard(p);
	s = va_arg(p->ap, char*);
	if (s && (p->prec == -1 || (p->prec && p->p_value == 0)))
		s = ft_strndup(s, p->p_value);
	else if (s)
		s = ft_strdup(s);
	else if (!s && p->prec == -1)
		s = ft_strndup("(null)", p->p_value);
	else if (!s)
		s = ft_strdup("(null)");
	len = ft_strlen_int(s);
	if (p->p_star && p->wc_p_neg)
		p->p_value = len;
	len = (p->p_value && len && p->p_value < len) ? p->p_value : len;
	p->minus == 0 ? put_str(p, s, len) : put_str_minus(p, s, len);
	p->len += (p->w_value > len) ? p->w_value : len;
	free(s);
}
