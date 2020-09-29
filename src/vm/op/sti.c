/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sti.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:14:05 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:14:06 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		get_sti_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 2;
	}
	else if (((cur->act >> 5) & 0x01) && ((cur->act >> 4) & 0x01))
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

void	op_sti(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	reg_num = get_register_index(vm, cur, 2);
	offset = 3;
	arg1 = get_sti_first_arg(vm, cur, &offset);
	if (!((cur->act >> 3) & 0x01))
	{
		arg2 = get_register_index(vm, cur, offset);
		arg2 = cur->reg[arg2];
	}
	else
		arg2 = get_direct(vm, cur, offset);
	write_bytes(vm, cur, positive_modulo(cur->position + (arg1 + arg2)
		% IDX_MOD, MEM_SIZE), cur->reg[reg_num]);
}
