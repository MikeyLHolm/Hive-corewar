/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_get_registry.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:41:18 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:41:42 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		get_register(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = vm->arena[(cur->position + offset) % MEM_SIZE];
	return (arg);
}

int		get_register_index(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = vm->arena[(cur->position + offset) % MEM_SIZE];
	return (arg - 1);
}
