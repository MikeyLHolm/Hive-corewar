/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 12:02:24 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/30 08:44:04 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	init_visualizer(t_vm *vm)
{
	(void)vm;
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	if (has_colors())
	{
		use_default_colors();
		start_color();
		init_pair(1, -1, COLOR_BLUE);
		init_pair(2, -1, COLOR_GREEN);
		init_pair(3, -1, COLOR_RED);
		init_pair(4, -1, COLOR_YELLOW);
		init_pair(5, COLOR_BLUE, -1);
		init_pair(6, COLOR_GREEN, -1);
		init_pair(7, COLOR_RED, -1);
		init_pair(8, COLOR_YELLOW, -1);
		init_pair(9, COLOR_WHITE, COLOR_WHITE);
		init_pair(10, COLOR_BLUE, COLOR_WHITE);
		init_pair(11, COLOR_GREEN, COLOR_WHITE);
		init_pair(12, COLOR_RED, COLOR_WHITE);
		init_pair(13, COLOR_YELLOW, COLOR_WHITE);
	}
}

void	visualize(t_vm *vm)
{
	while (1)
	{
		erase();
		visualizer_draw_map(vm);
		print_visualizer_info(vm);
		print_controls(vm);
		print_footer(vm);
		if (!(visualizer_handle_controls(vm)))
			break ;
		refresh();
	}
}

void	save_visualizer_memory(t_vm *vm)
{
	vm->color_mem = get_color_mem_old(vm, vm->color_mem);
	vm->cursor_mem = get_cursor_mem_old(vm, vm->cursor_mem);
	update_changed_memory(vm);
}

void	handle_visualization(t_vm *vm)
{
	if (vm->flags & VISUALIZER)
		visualize(vm);
	if (vm->flags & ADV_VISUALIZER)
		save_state(vm);
}
