/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   xor.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 12:55:36 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 12:56:29 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_xor(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_and_first_arg(vm, cur, &offset);
	arg2 = get_and_second_arg(vm, cur, &offset);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = arg1 ^ arg2;
	cur->carry = !(arg1 ^ arg2);
}
