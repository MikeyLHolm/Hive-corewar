/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:12:19 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:12:20 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_add(t_vm *vm, t_carriage *cur)
{
	int	arg1;
	int	arg2;
	int reg_num;

	arg1 = get_register_index(vm, cur, 2);
	arg1 = cur->reg[arg1];
	arg2 = get_register_index(vm, cur, 3);
	arg2 = cur->reg[arg2];
	reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = arg1 + arg2;
	cur->carry = !(arg1 + arg2);
}
