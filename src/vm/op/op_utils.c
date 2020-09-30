/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:56:30 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:59:10 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int			positive_modulo(int n, int mod)
{
	if (n < 0)
	{
		n %= mod;
		n += mod;
	}
	else
		n %= mod;
	return (n);
}

/*
**	Used Nowhere??
*/

t_carriage	*get_carriage_by_id(t_vm *vm, int id)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->id == id)
			return (cur_carriage);
		cur_carriage = cur_carriage->next;
	}
	return (NULL);
}

int			get_and_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 7) & 0x01) && !((cur->act >> 6) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 4;
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

int			get_and_second_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg2;

	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, *offset);
		*offset += 4;
	}
	else if (((cur->act >> 5) & 0x01) && ((cur->act >> 4) & 0x01))
	{
		arg2 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, *offset);
		arg2 = cur->reg[arg2];
		*offset += 1;
	}
	return (arg2);
}
