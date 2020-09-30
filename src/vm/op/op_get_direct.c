/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_get_direct.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:41:15 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:41:52 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		get_direct(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = 0;
	if (g_op_tab[cur->statement - 1].size_t_dir)
	{
		arg += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
		arg += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
		return ((short)arg);
	}
	else
	{
		arg += vm->arena[(cur->position + offset) % MEM_SIZE] * 256 * 256 * 256;
		arg += vm->arena[(cur->position + offset + 1) % MEM_SIZE] * 256 * 256;
		arg += vm->arena[(cur->position + offset + 2) % MEM_SIZE] * 256;
		arg += vm->arena[(cur->position + offset + 3) % MEM_SIZE];
	}
	return (arg);
}
