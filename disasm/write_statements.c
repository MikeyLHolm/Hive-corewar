/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_statements.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:37:49 by elindber          #+#    #+#             */
/*   Updated: 2020/09/10 15:49:10 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

void	get_arg_types(int input, int output, t_command *cmnd)
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
	if (tab >= 0 && tab <= 15)
	{
		write(output, g_op_tab[tab].op_name, ft_strlen(g_op_tab[tab].op_name));
		write(output, " ", 1);
	}
}

void	write_instructions(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				i;
	int				len;
	int				*arg_types;

	i = 0;
	while (cmnd->bytes_read < cmnd->champion_size)
	{
		write_statement(input, output, cmnd);
		if (cmnd->statement != LIVE && cmnd->statement != ZJMP
		&& cmnd->statement != FORK && cmnd->statement != LFORK)
			get_arg_types(input, output, cmnd);
		else
		{
			cmnd->arg_types[0] = 2;
			cmnd->arg_types[1] = 0;
			cmnd->arg_types[2] = 0;
		}
		write_arguments(input, output, cmnd);
		write(output, "\n", 1);
		i++;
	}
}
