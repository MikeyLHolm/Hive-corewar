/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 12:56:51 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 12:57:47 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_fork(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_direct(vm, cur, 1);
	copy_carriage(vm, cur, positive_modulo(cur->position + arg % IDX_MOD,
		MEM_SIZE));
}
