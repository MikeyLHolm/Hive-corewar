/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statement_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:17:02 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:32:38 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	move_carriage_next_statement(t_carriage *cur)
{
	if (cur->statement != 9 || !cur->carry)
		cur->position = (cur->position + cur->bytes_to_jump) % MEM_SIZE;
	cur->bytes_to_jump = 0;
}

void	get_statement(t_vm *vm, t_carriage *cur)
{
	cur->statement = vm->arena[cur->position];
	if (cur->statement > 0 && cur->statement <= OP_CODE_AMOUNT)
		cur->cycles_left = g_op_tab[cur->statement - 1].cycles;
}

int		skip_direct_bytes(t_carriage *cur)
{
	if (g_op_tab[cur->statement - 1].size_t_dir)
		return (2);
	else
		return (4);
}

void	count_bytes_to_skip(t_carriage *cur)
{
	unsigned char	act;
	int				n;
	int				bit;

	act = cur->act;
	n = 0;
	bit = 7;
	if (g_op_tab[cur->statement - 1].args_type_code)
		cur->bytes_to_jump = 2;
	else
	{
		cur->bytes_to_jump = g_op_tab[cur->statement - 1].size_t_dir ? 3 : 5;
		return ;
	}
	while (n < g_op_tab[cur->statement - 1].args_n)
	{
		if (((act >> bit) & 0x01) && !((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += skip_direct_bytes(cur);
		else if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 1;
		else if (((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 2;
		bit -= 2;
		n++;
	}
}
