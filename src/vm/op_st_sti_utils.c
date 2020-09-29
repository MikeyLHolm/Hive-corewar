/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op_st_sti_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:49:57 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 15:50:16 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** This way works with all possible byte amounts
*/

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
