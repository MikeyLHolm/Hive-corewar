/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statements.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 16:04:19 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/29 15:46:04 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		positive_modulo(int n, int mod)
{
	if (n < 0)
	{
		n %= mod;
		n += mod;
	}
	else
		n %= mod;
	return (n);
}

/*
**	Used Nowhere??
*/

t_carriage	*get_carriage_by_id(t_vm *vm, int id)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->id == id)
			return (cur_carriage);
		cur_carriage = cur_carriage->next;
	}
	return (NULL);
}

/*
** This way works with all possible byte amounts
*/

int		read_bytes(t_vm *vm, int pos, int amount)
{
	int		arg;

	arg = 0;
	if (amount == 4)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256 * 256 * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE] * 256 * 256;
		arg += vm->arena[(pos + 2) % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 3) % MEM_SIZE];
	}
	else if (amount == 2)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE];
	}
	else
		arg += vm->arena[pos % MEM_SIZE];
	return (arg);
}

void	update_advanced_visualizer_memory(t_vm *vm, t_carriage *cur, int pos,
	int offset)
{
	if (offset == 0)
	{
		vm->updated_color_mem[pos % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[pos % MEM_SIZE] = 49;
	}
	else if (offset == 1)
	{
		vm->updated_color_mem[(pos + 1) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 1) % MEM_SIZE] = 49;
	}
	else if (offset == 2)
	{
		vm->updated_color_mem[(pos + 2) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 2) % MEM_SIZE] = 49;
	}
	else if (offset == 3)
	{
		vm->updated_color_mem[(pos + 3) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 3) % MEM_SIZE] = 49;
	}
}

void	update_visualizer_memory(t_vm *vm, t_carriage *cur, int pos,
	int offset)
{
	if (offset == 0)
	{
		vm->color_mem[pos % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[pos % MEM_SIZE] = 50;
	}
	else if (offset == 1)
	{
		vm->color_mem[(pos + 1) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 1) % MEM_SIZE] = 50;
	}
	else if (offset == 2)
	{
		vm->color_mem[(pos + 2) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 2) % MEM_SIZE] = 50;
	}
	else if (offset == 3)
	{
		vm->color_mem[(pos + 3) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 3) % MEM_SIZE] = 50;
	}
}

void	write_bytes(t_vm *vm, t_carriage *cur, int pos, unsigned int byte)
{
	vm->arena[pos % MEM_SIZE] = byte / 256 / 256 / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 0);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 0);
	vm->arena[(pos + 1) % MEM_SIZE] = byte / 256 / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 1);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 1);
	vm->arena[(pos + 2) % MEM_SIZE] = byte / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 2);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 2);
	vm->arena[(pos + 3) % MEM_SIZE] = byte % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 3);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 3);
}

int		get_and_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 7) & 0x01) && !((cur->act >> 6) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 4;
	}
	else if (((cur->act >> 7) & 0x01) && ((cur->act >> 6) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg1 = get_register_index(vm, cur, *offset);
		arg1 = cur->reg[arg1];
		*offset += 1;
	}
	return (arg1);
}

int		get_and_second_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg2;

	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, *offset);
		*offset += 4;
	}
	else if (((cur->act >> 5) & 0x01) && ((cur->act >> 4) & 0x01))
	{
		arg2 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, *offset);
		arg2 = cur->reg[arg2];
		*offset += 1;
	}
	return (arg2);
}
