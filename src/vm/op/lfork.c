/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lfork.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 12:57:17 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 12:57:45 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_lfork(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_direct(vm, cur, 1);
	copy_carriage(vm, cur, positive_modulo(cur->position + arg, MEM_SIZE));
}
