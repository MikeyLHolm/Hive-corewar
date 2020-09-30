/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_print_arena.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:47:29 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:49:59 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_footer(t_vm *vm)
{
	move_if_valid(vm, 62, 64 * 3 + 25);
	print_if_valid(vm, ft_sprintf("Made by: "));
	move_if_valid(vm, 65, 64 * 3 + 15);
	attron(COLOR_PAIR(5));
	print_if_valid(vm, ft_sprintf("Elindber"));
	attroff((COLOR_PAIR(5)));
	print_if_valid(vm, ft_sprintf(", "));
	attron(COLOR_PAIR(6));
	print_if_valid(vm, ft_sprintf("Mlindhol "));
	attroff((COLOR_PAIR(6)));
	print_if_valid(vm, ft_sprintf("and "));
	attron(COLOR_PAIR(7));
	print_if_valid(vm, ft_sprintf("Sadawi"));
	attroff((COLOR_PAIR(7)));
	print_if_valid(vm, ft_sprintf("."));
}

void	print_border(t_vm *vm)
{
	int i;
	int size;

	i = 0;
	size = 64 * 3;
	print_if_valid(vm, ft_sprintf("\n"));
	while (i < size)
	{
		print_if_valid(vm, ft_sprintf("-"));
		i++;
	}
	print_if_valid(vm, ft_sprintf("\n"));
}

void	print_visualizer_info(t_vm *vm)
{
	move_if_valid(vm, 2, 64 * 3 + 20);
	print_if_valid(vm, ft_sprintf("BATTLE INFORMATION"));
	move_if_valid(vm, 6, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLE:"));
	move_if_valid(vm, 6, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->cycles));
	move_if_valid(vm, 8, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLES_TO_DIE:"));
	move_if_valid(vm, 8, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->cycles_to_die));
	move_if_valid(vm, 71, 4);
	move_if_valid(vm, 10, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CARRIAGES AMOUNT:"));
	move_if_valid(vm, 10, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", count_carriages(vm)));
	move_if_valid(vm, 18, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("AUTOPLAY:"));
	move_if_valid(vm, 18, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%s", vm->controls.autoplay ? "ON" : "OFF"));
	move_if_valid(vm, 20, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("STEP SIZE:"));
	move_if_valid(vm, 20, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->controls.step_size));
	print_player_last_alive(vm, vm->player_last_alive);
	print_player_lives(vm);
}

void	print_controls(t_vm *vm)
{
	move_if_valid(vm, 46, 64 * 3 + 24);
	print_if_valid(vm, ft_sprintf("CONTROLS:"));
	move_if_valid(vm, 50, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Space:"));
	move_if_valid(vm, 50, 64 * 3 + 35);
	print_if_valid(vm, ft_sprintf("Toggle autoplay"));
	move_if_valid(vm, 52, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Left/Right:"));
	move_if_valid(vm, 52, 64 * 3 + 27);
	print_if_valid(vm, ft_sprintf("Move backwards/forwards"));
	move_if_valid(vm, 54, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Up/Down:"));
	move_if_valid(vm, 54, 64 * 3 + 23);
	print_if_valid(vm, ft_sprintf("Increace/Decrease step size"));
	move_if_valid(vm, 56, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Q:"));
	move_if_valid(vm, 56, 64 * 3 + 46);
	print_if_valid(vm, ft_sprintf("Quit"));
}

void	print_visualizer_state_info(t_vm *vm, t_state *cur_state)
{
	move_if_valid(vm, 2, 64 * 3 + 20);
	print_if_valid(vm, ft_sprintf("BATTLE INFORMATION"));
	move_if_valid(vm, 6, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLE:"));
	move_if_valid(vm, 6, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->cycle));
	move_if_valid(vm, 8, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLES_TO_DIE:"));
	move_if_valid(vm, 8, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->cycles_to_die));
	move_if_valid(vm, 71, 4);
	move_if_valid(vm, 10, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CARRIAGES AMOUNT:"));
	move_if_valid(vm, 10, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->carriage_amount));
	move_if_valid(vm, 18, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("AUTOPLAY:"));
	move_if_valid(vm, 18, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%s", vm->controls.autoplay ? "ON" : "OFF"));
	move_if_valid(vm, 20, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("STEP SIZE:"));
	move_if_valid(vm, 20, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->controls.step_size));
	print_player_last_alive(vm, cur_state->last_live_player);
	print_state_player_lives(vm, cur_state);
}
