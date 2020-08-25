/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 14:26:55 by sadawi            #+#    #+#             */
/*   Updated: 2020/04/12 20:50:42 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "libft.h"

/*
**	ft_fprintf prints to specified file descriptor.
*/

int		ft_fprintf(int fd, const char *format, ...)
{
	va_list	args;
	t_data	*data;
	int		amount;

	amount = 0;
	init_data(&data);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
			amount += handle_flags(&format, &args, data);
		else
			data->output = join_char_to_str(data->output, *format);
		format++;
		amount++;
	}
	write(fd, data->output, ft_strlen(data->output));
	free(data->output);
	free(data);
	return (amount);
}

/*
**	ft_sprintf returns the string instead of printing.
*/

char	*ft_sprintf(const char *format, ...)
{
	va_list	args;
	t_data	*data;
	char	*output;

	init_data(&data);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
			handle_flags(&format, &args, data);
		else
			data->output = join_char_to_str(data->output, *format);
		format++;
	}
	output = data->output;
	free(data);
	return (output);
}

int		ft_printf(const char *format, ...)
{
	va_list	args;
	t_data	*data;
	int		amount;

	amount = 0;
	init_data(&data);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
			amount += handle_flags(&format, &args, data);
		else
			data->output = join_char_to_str(data->output, *format);
		format++;
		amount++;
	}
	write(1, data->output, ft_strlen(data->output));
	free(data->output);
	free(data);
	return (amount);
}

char	*toaddress(unsigned long n)
{
	char *address;
	char *tmp;

	address = (char*)ft_memalloc(15);
	ft_strcpy(address, "0x");
	tmp = ft_itoa_base_ul_low(n, 16);
	ft_strcat(address, tmp);
	free(tmp);
	return (address);
}

int		handle_output(char **output, char *flag, t_data *data)
{
	int len;

	handle_flag(output, flag);
	if (ft_strchr(flag, 'c'))
	{
		len = handle_char_output(output, flag, data);
		free(*output);
	}
	else
	{
		len = ft_strlen(*output);
		data->output = ft_strjoinfree(data->output, *output);
	}
	return (len);
}
