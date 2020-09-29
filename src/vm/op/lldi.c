/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lldi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:13:01 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:13:03 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

void	op_lldi(t_vm *vm, t_carriage *cur)
{
	int				num;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_ldi_first_arg(vm, cur, &offset);
	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, offset);
		arg2 = cur->reg[arg2];
		offset += 1;
	}
	num = read_bytes(vm, positive_modulo(cur->position + (arg1 + arg2),
		MEM_SIZE), 4);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}
