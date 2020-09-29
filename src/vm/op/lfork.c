/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lfork.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 12:57:17 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:45:55 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

static void		copy_carriage(t_vm *vm, t_carriage *cur, int pos)
{
	int			reg_num;

	vm->carriages = new_carriage(cur->id, vm->carriages);
	vm->carriages->carry = cur->carry;
	vm->carriages->last_live_cycle = cur->last_live_cycle;
	reg_num = 0;
	while (reg_num < REG_NUMBER)
	{
		vm->carriages->reg[reg_num] = cur->reg[reg_num];
		reg_num++;
	}
	vm->carriages->position = pos % MEM_SIZE;
}

void			op_lfork(t_vm *vm, t_carriage *cur)
{
	int			arg;

	arg = get_direct(vm, cur, 1);
	copy_carriage(vm, cur, positive_modulo(cur->position + arg, MEM_SIZE));
}
