/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zjmp.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:14:14 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:14:15 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_zjmp(t_vm *vm, t_carriage *cur)
{
	int arg1;

	if (!cur->carry)
		return ;
	arg1 = get_direct(vm, cur, 1);
	cur->position = positive_modulo(cur->position + arg1 % IDX_MOD, MEM_SIZE);
}
