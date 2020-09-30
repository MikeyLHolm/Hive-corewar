/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_print_player.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:49:25 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:24:08 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_player_last_alive(t_vm *vm, int player_num)
{
	t_player *cur;

	move_if_valid(vm, 25, 64 * 3 + 18);
	print_if_valid(vm, ft_sprintf("PLAYER WITH LAST LIVE"));
	cur = vm->players;
	while (cur)
	{
		if (cur->id == player_num)
			break ;
		cur = cur->next;
	}
	move_if_valid(vm, 27, 64 * 3 + 7);
	attron(COLOR_PAIR((unsigned char)cur->id + 4));
	print_if_valid(vm, ft_sprintf("PLAYER %d %34s", player_num, cur->name));
	attroff(COLOR_PAIR((unsigned char)cur->id + 4));
}

void	print_player_lives(t_vm *vm)
{
	t_player	*cur;
	int			offset;

	cur = vm->players;
	move_if_valid(vm, 31, 64 * 3 + 21);
	print_if_valid(vm, ft_sprintf("LAST LIVE CYCLES"));
	offset = 34;
	while (cur)
	{
		move_if_valid(vm, offset, 64 * 3 + 7);
		attron(COLOR_PAIR((unsigned char)cur->id + 4));
		print_if_valid(vm, ft_sprintf("PLAYER %d (%s):", cur->id, cur->name));
		attroff(COLOR_PAIR((unsigned char)cur->id + 4));
		move_if_valid(vm, offset, 64 * 3 + 47);
		print_if_valid(vm, ft_sprintf("%d", cur->last_live_cycle));
		cur = cur->next;
		offset += 2;
	}
}

void	print_state_player_lives(t_vm *vm, t_state *state)
{
	t_player	*cur;
	int			offset;
	int			pointer_offset;

	cur = vm->players;
	move_if_valid(vm, 31, 64 * 3 + 21);
	print_if_valid(vm, ft_sprintf("LAST LIVE CYCLES"));
	offset = 34;
	pointer_offset = 0;
	while (cur)
	{
		move_if_valid(vm, offset, 64 * 3 + 7);
		attron(COLOR_PAIR((unsigned char)cur->id + 4));
		print_if_valid(vm, ft_sprintf("PLAYER %d (%s):", cur->id, cur->name));
		attroff(COLOR_PAIR((unsigned char)cur->id + 4));
		move_if_valid(vm, offset, 64 * 3 + 47);
		print_if_valid(vm, ft_sprintf("%d",
			*(&state->player1_last_live + pointer_offset)));
		cur = cur->next;
		offset += 2;
		pointer_offset += 1;
	}
}
