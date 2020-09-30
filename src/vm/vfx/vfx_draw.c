/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_draw.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:31:56 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:36:43 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
