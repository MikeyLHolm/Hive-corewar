/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vfx_print_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:23:55 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:24:19 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

void	print_cycle_loading(t_vm *vm)
{
	clear();
	print_if_valid(vm, ft_sprintf("Saving cycle: %d\n", vm->cycles));
	refresh();
	clear();
}
