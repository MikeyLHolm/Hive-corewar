/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/17 16:27:35 by sadawi            #+#    #+#             */
/*   Updated: 2019/12/18 14:18:31 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H
# include <unistd.h>
# include <stdarg.h>
# include <stdlib.h>

typedef struct	s_data
{
	int			amount;
	char		*output;

}				t_data;

int				ft_printf(const char *format, ...);

int				ft_fprintf(int fd, const char *format, ...);

char			*ft_sprintf(const char *format, ...);

int				handle_flags(const char **format, va_list *args, t_data *data);

int				check_flag(char *flag);

int				print_flag(char *flag, va_list *args, t_data *data);

int				handle_signed(char *flag, va_list *args, t_data *data);

int				handle_unsigned(char *flag, va_list *args, t_data *data);

int				handle_pointer(char *flag, va_list *args, t_data *data);

int				handle_base(char *flag, va_list *args, t_data *data);

int				handle_float(char *flag, va_list *args, t_data *data);

int				handle_percent(char *flag, t_data *data);

char			*toaddress(unsigned long n);

int				handle_output(char **output, char *flag, t_data *data);

int				handle_char_output(char **output, char *flag, t_data *data);

void			handle_precision(char **output, char *flag);

void			handle_hashtag(char **output, char *flag);

void			handle_plus(char **output, char *flag);

void			handle_space(char **output, char *flag);

void			handle_flag(char **output, char *flag);

void			handle_width(char **output, char *flag);

int				handle_char_width(char **output, char *flag, int width);

void			handle_zero(char **output, char *flag);

void			add_width(char **output, char *flag, char *padding);

int				flag_integer(char *flag);

int				check_flag_thrice(char *flag, char *s1, char *s2, char *s3);

void			handle_char_minus(char **output, char *padding);

void			free_two(char **s1, char **s2);

void			handle_hex_precision(char **output, char *flag);

t_data			*join_data(t_data *data1, t_data *data2);

void			init_data(t_data **data);

char			*join_char_to_str(char *str, char c);

#endif
