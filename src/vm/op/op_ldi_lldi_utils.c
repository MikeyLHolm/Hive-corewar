/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_ldi_lldi_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:53:53 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:54:57 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** This way works with all possible byte amounts
*/

int		read_bytes(t_vm *vm, int pos, int amount)
{
	int			arg;

	arg = 0;
	if (amount == 4)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256 * 256 * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE] * 256 * 256;
		arg += vm->arena[(pos + 2) % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 3) % MEM_SIZE];
	}
	else if (amount == 2)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE];
	}
	else
		arg += vm->arena[pos % MEM_SIZE];
	return (arg);
}

int		get_ldi_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 7) & 0x01) && !((cur->act >> 6) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 2;
	}
	else if (((cur->act >> 7) & 0x01) && ((cur->act >> 6) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg1 = get_register_index(vm, cur, *offset);
		arg1 = cur->reg[arg1];
		*offset += 1;
	}
	return (arg1);
}
