/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:39:47 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:40:22 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	load_player_colors(t_vm *vm, int *cursor_mem)
{
	t_player	*cur_player;
	int			offset;
	int			i;

	i = 0;
	cur_player = vm->players;
	offset = MEM_SIZE / vm->player_amount;
	while (i < vm->player_amount)
	{
		ft_memset(&cursor_mem[i++ * offset], cur_player->id + 4,
			cur_player->size * 4);
		cur_player = cur_player->next;
	}
}

int		ft_abs(int n)
{
	return (n > 0 ? n : -n);
}

void	move_if_valid(t_vm *vm, int y, int x)
{
	int max_x;
	int max_y;

	getmaxyx(stdscr, max_y, max_x);
	move(y, x);
	vm->cursor_y = y;
	vm->cursor_x = x;
}

int		visualizer_handle_controls(t_vm *vm)
{
	int key;

	key = getch();
	if (key == ' ')
	{
		vm->controls.autoplay = !vm->controls.autoplay;
		timeout(vm->controls.autoplay ? 1 : -1);
		return (0);
	}
	if (key == 'q')
	{
		endwin();
		exit(0);
	}
	if (key == KEY_RIGHT)
		return (0);
	if (vm->controls.autoplay)
		return (0);
	return (1);
}

void	handle_step_size_controls(t_vm *vm, int key)
{
	if (key == KEY_UP)
		vm->controls.step_size++;
	if (key == KEY_DOWN)
		vm->controls.step_size > 1 ? vm->controls.step_size-- : (void)vm;
}
