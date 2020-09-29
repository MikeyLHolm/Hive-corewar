/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   st.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:12:37 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:12:39 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_st(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				arg1;
	int				arg2;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	arg1 = get_register_index(vm, cur, 2);
	if (!((act >> 5) & 0x01))
	{
		arg2 = get_register_index(vm, cur, 3);
		cur->reg[arg2] = cur->reg[arg1];
	}
	else
	{
		arg2 = get_indirect_address_trunc(vm, cur, 3, 0);
		write_bytes(vm, cur, positive_modulo(cur->position + arg2, MEM_SIZE),
			cur->reg[arg1]);
	}
}
