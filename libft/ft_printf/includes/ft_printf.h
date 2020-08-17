/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/15 14:31:02 by mlindhol          #+#    #+#             */
/*   Updated: 2020/01/30 08:57:58 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

/*
**	Define text colors.
*/

# define BLACK			"\033[0:30m"
# define BOLD_BLACK		"\033[1:30m"
# define RED 			"\033[0;31m"
# define BOLD_RED 		"\033[1;31m"
# define GREEN 			"\033[0;32m"
# define BOLD_GREEN 	"\033[1;32m"
# define YELLOW 		"\033[0;33m"
# define BOLD_YELLOW 	"\033[01;33m"
# define BLUE 			"\033[0;34m"
# define BOLD_BLUE 		"\033[1;34m"
# define MAGENTA 		"\033[0;35m"
# define BOLD_MAGENTA 	"\033[1;35m"
# define CYAN 			"\033[0;36m"
# define BOLD_CYAN 		"\033[1;36m"
# define WHITE			"\033[0:37m"
# define BOLD_WHITE		"\033[1:37m"
# define RESET 			"\033[0m"

/*
**	Define background colors.
*/

# define BG_BLACK			"\033[0:40m"
# define BG_BOLD_BLACK		"\033[1:40m"
# define BG_RED 			"\033[0;41m"
# define BG_BOLD_RED 		"\033[1;41m"
# define BG_GREEN 			"\033[0;42m"
# define BG_BOLD_GREEN 		"\033[1;42m"
# define BG_YELLOW 			"\033[0;43m"
# define BG_BOLD_YELLOW 	"\033[1;43m"
# define BG_BLUE 			"\033[0;44m"
# define BG_BOLD_BLUE 		"\033[1;44m"
# define BG_MAGENTA 		"\033[0;45m"
# define BG_BOLD_MAGENTA 	"\033[1;45m"
# define BG_CYAN 			"\033[0;46m"
# define BG_BOLD_CYAN 		"\033[1;46m"
# define BG_WHITE			"\033[0:47m"
# define BG_BOLD_WHITE		"\033[1:47m"

/*
**	Other "necessities".
*/

# define BOLD 				"\033[1m"
# define UL 				"\033[4m"
# define REV 				"\033[7m"

/*
**	Include various libraries.
*/

# include "../../includes/libft.h"
# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

/*
**	Lenght specifier enums for condition checks.
*/

typedef enum
{
	I,
	H,
	HH,
	J,
	L,
	LD,
	LL,
	Z
}	t_length;

/*
**	All the data gathered during parsin is stored into this struct along
**	the index and list of arguments.
*/

typedef struct		s_printf
{
	char		*f;
	int			caps;
	int			fd;
	int			hash;
	int			i;
	int			len;
	int			minus;
	int			neg;
	int			plus;
	int			prec;
	int			p_star;
	int			p_value;
	int			space;
	int			wc_p_neg;
	int			wdth;
	int			w_star;
	int			w_value;
	int			zero;
	int			nil;
	t_length	specifier;
	va_list		ap;
}					t_printf;

int					ft_printf(const char *format, ...);
int					ft_dprintf(int fd, const char *format, ...);
void				dispatcher(t_printf *p);
void				display_b(t_printf *p);
void				display_c(t_printf *p);
void				display_d(t_printf *p);
void				display_f(t_printf *p);
void				display_o(t_printf *p);
void				display_p(t_printf *p);
void				display_pct(t_printf *p);
void				display_s(t_printf *p);
void				display_u(t_printf *p);
void				display_x(t_printf *p, int caps);
char				*fill_zeros(char *str, int p_value, int len);
char				*ft_strmagic(char *dst, const char *src);
char				*ft_str_zero(char *str);
long double			get_conv_double(t_printf *p);
intmax_t			get_conv_signed(t_printf *p);
uintmax_t			get_conv_unsigned(t_printf *p);
void				handle_dot(t_printf *p);
void				handle_wildcard(t_printf *p);
void				hex_helper(t_printf *p);
int					magic_pill(t_printf *p);
void				parse_flags(t_printf *p);
void				parse_length(t_printf *p);
void				parse_precision(t_printf *p);
void				parse_width(t_printf *p);
int					parser(t_printf *p);
void				print_until_pct(t_printf *p);
void				reinitialize(t_printf *p);

#endif
