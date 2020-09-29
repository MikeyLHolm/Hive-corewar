/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lldi.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:13:01 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:56:53 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
