/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lld.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:12:07 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:12:12 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_lld(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				reg_num;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	if (!((act >> 6) & 0x01))
		num = get_direct(vm, cur, 2);
	else
	{
		if (vm->flags & LLD_FIX)
			num = get_indirect_value(vm, cur, 2, 0);
		else
			num = get_indirect_value_2_bytes(vm, cur, 2, 0);
	}
	if (!((act >> 6) & 0x01))
		reg_num = get_register_index(vm, cur, 6);
	else
		reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}
