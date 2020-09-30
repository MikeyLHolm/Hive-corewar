/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   save_state.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 09:11:28 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 09:14:01 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	state_get_lives(t_vm *vm, t_state *state)
{
	t_player *cur_player;

	cur_player = vm->players;
	state->player1_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player2_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player3_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player4_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->last_live_player = vm->player_last_alive;
}

t_state	*new_state(t_vm *vm, t_state *prev)
{
	t_state	*state;
	int		i;

	if (!(state = (t_state*)ft_memalloc(sizeof(t_state))))
		handle_error("Malloc failed");
	i = 0;
	while (i < MEM_SIZE)
	{
		state->arena[i] = vm->arena[i];
		i++;
	}
	state->cursor_mem = get_cursor_mem(vm, prev);
	state->color_mem = get_color_mem(vm, prev);
	state->changed_mem = get_changed_memory(vm, prev);
	state->carriage_amount = count_carriages(vm);
	state->cycle = vm->cycles;
	state->cycles_to_die = vm->cycles_to_die;
	state->prev = prev;
	state_get_lives(vm, state);
	return (state);
}

void	save_state(t_vm *vm)
{
	if (!vm->arena_history_head)
	{
		vm->arena_history_head = new_state(vm, NULL);
		vm->cur_state = vm->arena_history_head;
	}
	else
	{
		vm->cur_state->next = new_state(vm, vm->cur_state);
		vm->cur_state = vm->cur_state->next;
	}
}
