/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_get.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:32:45 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:33:27 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		*get_color_mem_old(t_vm *vm, int *prev_mem)
{
	t_carriage	*cur;
	int			*color_mem;
	int			i;

	if (!(color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	if (!prev_mem)
		load_player_colors(vm, color_mem);
	else
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_color_mem[i])
				color_mem[i] = vm->updated_color_mem[i];
			else if (prev_mem[i] > vm->player_amount && prev_mem[i] != 9)
				color_mem[i] = prev_mem[i];
			i++;
		}
	}
	ft_bzero(vm->updated_color_mem, MEM_SIZE * sizeof(int));
	free(prev_mem);
	return (color_mem);
}

int		*get_cursor_mem_old(t_vm *vm, int *prev)
{
	t_carriage	*cur;
	int			*cursor_mem;

	if (!(cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	(void)prev;
	while (cur)
	{
		cursor_mem[cur->position] = 1;
		cur = cur->next;
	}
	free(prev);
	return (cursor_mem);
}

int		*get_color_mem(t_vm *vm, t_state *prev)
{
	t_carriage	*cur;
	int			*color_mem;
	int			i;

	if (!(color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	if (!prev)
		load_player_colors(vm, color_mem);
	else
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_color_mem[i])
				color_mem[i] = vm->updated_color_mem[i];
			else if (prev->color_mem[i] > vm->player_amount
				&& prev->color_mem[i] != 9)
				color_mem[i] = prev->color_mem[i];
			i++;
		}
	}
	ft_bzero(vm->updated_color_mem, MEM_SIZE * sizeof(int));
	return (color_mem);
}

int		*get_cursor_mem(t_vm *vm, t_state *prev)
{
	t_carriage	*cur;
	int			*cursor_mem;

	if (!(cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	(void)prev;
	while (cur)
	{
		cursor_mem[cur->position] = 1;
		cur = cur->next;
	}
	return (cursor_mem);
}

t_state	*get_start_state(t_vm *vm)
{
	t_state	*cur_state;
	int		i;

	cur_state = vm->arena_history_head;
	i = 0;
	while (i < vm->start)
	{
		if (!cur_state->next)
			break ;
		cur_state = cur_state->next;
	}
	return (cur_state);
}
