/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_arguments.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:47:50 by elindber          #+#    #+#             */
/*   Updated: 2020/09/24 13:56:49 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

/*
** Reads four or two bytes and gets the value of those. Then changes the value
** to ascii-characters and writes it to the output file.
*/

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

/*
** Reads two bytes if argument is indirect, one if its a registry. Then changes
** it to ascii-characters and writes it to the output file.
*/

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

/*
** The size of the direct argument differs depending on the statement it
** follows. This function determines the size of the argument.
*/

int		get_dir_argument_size(t_command *cmnd)
{
	if ((cmnd->statement >= ZJMP && cmnd->statement <= FORK)
	|| cmnd->statement == LLDI || cmnd->statement == LFORK)
		return (2);
	else
		return (4);
}

/*
** Checks the current values in arg_types array in struct and determines the
** amount of bytes that will be read. Then writes the sign of the argument type
** and then the value followed by ", " if needed.
*/

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
