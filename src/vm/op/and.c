/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   and.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:13:35 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:13:37 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_and(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_and_first_arg(vm, cur, &offset);
	arg2 = get_and_second_arg(vm, cur, &offset);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = arg1 & arg2;
	cur->carry = !(arg1 & arg2);
}
