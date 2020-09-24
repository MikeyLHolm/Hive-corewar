/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_statements.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:37:49 by elindber          #+#    #+#             */
/*   Updated: 2020/09/24 13:45:24 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

/*
** By using bit operations this function reads the argument type code byte, that
** carries the information of the types of the (up to 3) arguments that follows.
** 01 is REG, 10 is DIR and 11 is IND. 0 is no argument. Argument types are
** saved to an int array in struct.
*/

void	get_arg_types(int input, t_command *cmnd)
{
	int				i;
	int				bit;
	unsigned char	byte;
	int				value;

	i = 0;
	bit = 7;
	read(input, &byte, 1);
	cmnd->bytes_read++;
	value = (int)byte;
	while (i < 3)
	{
		if (((value >> bit) & 0x01) && ((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = IND_CODE;
		else if (!((value >> bit) & 0x01) && ((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = REG_CODE;
		else if (((value >> bit) & 0x01) && !((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = DIR_CODE;
		else if (!((value >> bit) & 0x01) && !((value >> (bit - 1)) & 0x01))
			cmnd->arg_types[i] = 0;
		bit -= 2;
		i++;
	}
}

/*
** Gets the firs non-zero value from binary file. THen check if that matches any
** statement from header file and writes that statement followed by a space.
*/

void	write_statement(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				tab;

	tab = 0;
	while (tab == 0)
	{
		read(input, &byte, 1);
		tab = (int)byte;
		cmnd->bytes_read++;
	}
	cmnd->statement = tab;
	tab--;
	if (tab >= 0 && tab < OP_CODE_AMOUNT)
	{
		write(output, g_op_tab[tab].op_name, ft_strlen(g_op_tab[tab].op_name));
		write(output, " ", 1);
	}
}

/*
** Loops until all bytes from the champion's code are read. First gets the
** statement and writes it. Then gets the argument types if necessary and
** finally writes arguments followed by a newline accordingly. After everything
** is read closes input and output files.
*/

void	write_instructions(int input, int output, t_command *cmnd)
{
	cmnd->bytes_read = 0;
	while (cmnd->bytes_read < cmnd->champion_size)
	{
		write_statement(input, output, cmnd);
		if (cmnd->statement != LIVE && cmnd->statement != ZJMP
		&& cmnd->statement != FORK && cmnd->statement != LFORK)
			get_arg_types(input, cmnd);
		else
		{
			cmnd->arg_types[0] = 2;
			cmnd->arg_types[1] = 0;
			cmnd->arg_types[2] = 0;
		}
		write_arguments(input, output, cmnd);
		write(output, "\n", 1);
	}
	close(output);
	close(input);
}
