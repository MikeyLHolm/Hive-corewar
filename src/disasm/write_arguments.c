/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:47:50 by elindber          #+#    #+#             */
/*   Updated: 2020/09/16 12:47:23 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

void	write_dir_value(int input, int output, int size)
{
	unsigned char	byte;
	char			*str;
	int				result;

	result = 0;
	if (size == 2)
	{
		read(input, &byte, 1);
		result += byte * 256;
	}
	else
	{
		read(input, &byte, 1);
		result += byte * 256 * 256 * 256;
		read(input, &byte, 1);
		result += byte * 256 * 256;
		read(input, &byte, 1);
		result += byte * 256;
	}
	read(input, &byte, 1);
	result += byte;
	result = size == 2 ? (short)result : result;
	str = ft_itoa(result);
	write(output, str, ft_strlen(str));
	free(str);
}

void	write_ind_reg_value(int input, int output, int size)
{
	unsigned char		byte;
	int					result;
	char				*str;

	result = 0;
	if (size == 2)
	{
		read(input, &byte, 1);
		result += byte * 256;
	}
	read(input, &byte, 1);
	result += byte;
	result = size == 2 ? (short)result : result;
	str = ft_itoa(result);
	write(output, str, ft_strlen(str));
	free(str);
}

void	write_arg_sign(int output, int i, t_command *cmnd)
{
	if (cmnd->arg_types[i] == REG_CODE)
		write(output, "r", 1);
	else if (cmnd->arg_types[i] == DIR_CODE)
		write(output, "%", 1);
}

int		get_dir_argument_size(t_command *cmnd)
{
	if ((cmnd->statement >= 9 && cmnd->statement <= 12) || cmnd->statement == 14
	|| cmnd->statement == 15)
		return (2);
	else
		return (4);
}

void	write_arguments(int input, int output, t_command *cmnd)
{
	int				i;
	int				size;

	i = 0;
	while (i < 3)
	{
		if (cmnd->arg_types[i] == 0)
			break ;
		else if (cmnd->arg_types[i] == REG_CODE)
			size = 1;
		else if (cmnd->arg_types[i] == IND_CODE)
			size = 2;
		else if (cmnd->arg_types[i] == DIR_CODE)
			size = get_dir_argument_size(cmnd);
		cmnd->bytes_read += size;
		write_arg_sign(output, i, cmnd);
		if (cmnd->arg_types[i] == DIR_CODE)
			write_dir_value(input, output, size);
		else
			write_ind_reg_value(input, output, size);
		if (i != 2 && cmnd->arg_types[i + 1] != 0)
			write(output, ", ", 2);
		i++;
	}
}
