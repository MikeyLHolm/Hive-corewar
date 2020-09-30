/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_get_indirect.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:56:41 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:56:42 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		get_indirect_value_trunc(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256 * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)] * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 2, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 3, MEM_SIZE)];
	return (arg);
}

int		get_indirect_value(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256 * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)] * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 2, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 3, MEM_SIZE)];
	return (arg);
}

int		get_indirect_value_2_bytes(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)];
	return (arg);
}

int		get_indirect_address_trunc(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	return (relative_address);
}

int		get_indirect_address(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	return (relative_address);
}
