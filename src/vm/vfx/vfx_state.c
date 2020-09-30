/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_state.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:28:56 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:29:46 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	visualizer_state_turn_color_on(t_state *cur_state, int i)
{
	if (cur_state->cursor_mem[i])
		attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i]
			? cur_state->color_mem[i] - 4 : 9)));
	else if (cur_state->color_mem[i])
		attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i]
			+ (cur_state->changed_mem[i] ? 5 : 0))));
}

void	visualizer_state_draw_map(t_vm *vm, t_state *cur_state)
{
	int		i;
	int		row;

	draw_borders(vm);
	row = 2;
	move_if_valid(vm, row++, 3);
	i = 0;
	while (i < MEM_SIZE)
	{
		visualizer_state_turn_color_on(cur_state, i);
		print_if_valid(vm, ft_sprintf("%02x", cur_state->arena[i]));
		if (cur_state->cursor_mem[i])
			attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i]
				- 4)));
		else
			attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i]
				+ (cur_state->changed_mem[i] ? 5 : 0))));
		print_if_valid(vm, ft_sprintf(" "));
		if (!(++i % 64))
			move_if_valid(vm, row++, 3);
	}
}

int		visualizer_state_handle_controls(t_vm *vm, t_state **cur_state)
{
	int key;
	int	i;

	key = getch();
	if (vm->controls.autoplay && key == ERR)
		key = KEY_RIGHT;
	i = 0;
	if (key == KEY_LEFT)
		while (i++ < vm->controls.step_size)
			*cur_state = (*cur_state)->prev ? (*cur_state)->prev : *cur_state;
	if (key == KEY_RIGHT)
		while (i++ < vm->controls.step_size)
			*cur_state = (*cur_state)->next ? (*cur_state)->next : *cur_state;
	handle_step_size_controls(vm, key);
	if (key == 'q')
		return (0);
	if (key == ' ')
	{
		vm->controls.autoplay = !vm->controls.autoplay;
		timeout(vm->controls.autoplay ? 1 : -1);
	}
	if (!(*cur_state)->next)
		vm->controls.autoplay = 0;
	return (1);
}

void	visualize_states(t_vm *vm)
{
	t_state	*cur_state;

	cur_state = get_start_state(vm);
	while (1)
	{
		erase();
		visualizer_state_draw_map(vm, cur_state);
		print_visualizer_state_info(vm, cur_state);
		print_controls(vm);
		print_footer(vm);
		if (!(visualizer_state_handle_controls(vm, &cur_state)))
			break ;
		refresh();
	}
}
