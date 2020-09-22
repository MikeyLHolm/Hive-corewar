/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   visualize.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/21 12:02:24 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/22 11:22:44 by sadawi           ###   ########.fr       */
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

void	print_if_valid(t_vm *vm, char *str)
{
	int max_x;
	int max_y;

	getmaxyx(stdscr, max_y, max_x);
	if (vm->cursor_y < max_y && vm->cursor_x < max_x)
	{
		if ((int)ft_strlen(str) > max_x - vm->cursor_x)
			str[max_x - vm->cursor_x] = '\0';
		printw(str);
		vm->cursor_x += ft_strlen(str);
		if (ft_strchr(str, '\n'))
			vm->cursor_x = 0;
	}
	free(str);
}

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

void	draw_vertical_borders(t_vm *vm)
{
	int i;
	int size;

	i = 0;
	size = 64 * 3 + 2 + 50;
	while (i < 68)
	{
		move_if_valid(vm, i++, 0);
		print_if_valid(vm, ft_sprintf(".."));
	}
	i = 0;
	while (i < 68)
	{
		move_if_valid(vm, i++, size + 1);
		print_if_valid(vm, ft_sprintf(".."));
	}
	i = 0;
	while (i < 68)
	{
		move_if_valid(vm, i++, size - 49);
		print_if_valid(vm, ft_sprintf(".."));
	}
}

void	draw_info_borders(t_vm *vm)
{
	int i;
	int size;

	i = 0;
	size = 64 * 3 + 2 + 50;
	while (i < 50)
		print_if_valid(vm, ft_sprintf(".", i++));
	move_if_valid(vm, 29, size - 49);
	i = 0;
	while (i < 50)
		print_if_valid(vm, ft_sprintf(".", i++));
	move_if_valid(vm, 43, size - 49);
	i = 0;
	while (i < 50)
		print_if_valid(vm, ft_sprintf(".", i++));
	move_if_valid(vm, 60, size - 49);
	i = 0;
	while (i < 50)
		print_if_valid(vm, ft_sprintf(".", i++));
}

void	draw_borders(t_vm *vm)
{
	int i;
	int size;

	attron(COLOR_PAIR(9));
	move_if_valid(vm, 0, 0);
	i = 0;
	size = 64 * 3 + 2 + 50;
	while (i < size + 1)
		print_if_valid(vm, ft_sprintf(".", i++));
	move_if_valid(vm, 64 + 3, 0);
	i = 0;
	while (i < size + 1)
		print_if_valid(vm, ft_sprintf(".", i++));
	move_if_valid(vm, 23, size - 49);
	draw_info_borders(vm);
	draw_vertical_borders(vm);
	attroff((COLOR_PAIR(9)));
}

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

void	visualizer_draw_map(t_vm *vm)
{
	int i;
	int	row;

	draw_borders(vm);
	row = 2;
	move_if_valid(vm, row++, 3);
	i = 0;
	while (i < MEM_SIZE)
	{
		if (vm->cursor_mem[i])
			attron(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i]
			? vm->color_mem[i] - 4 : 9)));
		else if (vm->color_mem[i])
			attron(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i]
			+ (vm->changed_mem[i] ? 5 : 0))));
		print_if_valid(vm, ft_sprintf("%02x", (unsigned char)vm->arena[i]));
		if (vm->cursor_mem[i])
			attroff(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i] - 4)));
		else
			attroff(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i]
			+ (vm->changed_mem[i] ? 5 : 0))));
		print_if_valid(vm, ft_sprintf(" "));
		if (!(++i % 64))
			move_if_valid(vm, row++, 3);
	}
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

t_state	*get_start_state(t_vm *vm)
{
	t_state	*cur_state;
	int		i;

	cur_state = vm->arena_history_head;
	i = 0;
	while (i < vm->start)
	{
		if (!cur_state)
			break ;
		cur_state = cur_state->next;
	}
	return (cur_state);
}

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

void	handle_step_size_controls(t_vm *vm, int key)
{
	if (key == KEY_UP)
		vm->controls.step_size++;
	if (key == KEY_DOWN)
		vm->controls.step_size > 1 ? vm->controls.step_size-- : (void)vm;
}

int		visualizer_state_handle_controls(t_vm *vm, t_state *cur_state)
{
	int key;
	int	i;

	key = getch();
	if (vm->controls.autoplay && key == ERR)
		key = KEY_RIGHT;
	i = 0;
	if (key == KEY_LEFT)
		while (i++ < vm->controls.step_size)
			cur_state = cur_state->prev ? cur_state->prev : cur_state;
	if (key == KEY_RIGHT)
		while (i++ < vm->controls.step_size)
			cur_state = cur_state->next ? cur_state->next : cur_state;
	handle_step_size_controls(vm, key);
	if (key == 'q')
		return (0);
	if (key == ' ')
	{
		vm->controls.autoplay = !vm->controls.autoplay;
		timeout(vm->controls.autoplay ? 1 : -1);
	}
	if (!cur_state->next)
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
		if (!(visualizer_state_handle_controls(vm, cur_state)))
			break ;
		refresh();
	}
}
