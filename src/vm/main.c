/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 12:59:40 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		init_controls(t_vm *vm)
{
	vm->controls.step_size = 1;
}

t_vm		*init_vm(void)
{
	t_vm	*vm;

	if (!(vm = (t_vm*)ft_memalloc(sizeof(t_vm))))
		handle_error("Malloc failed at VM init");
	vm->cycles_to_die = CYCLE_TO_DIE;
	if (!(vm->changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	vm->color_mem = NULL;
	vm->dump_cycle = -1;
	if (!(vm->updated_color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	if (!(vm->updated_changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	if (!(vm->cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	init_controls(vm);
	return (vm);
}

void		introduce_contestants(t_vm *vm)
{
	t_player	*cur_player;

	cur_player = vm->players;
	ft_printf("Introducing contestants...\n");
	while (cur_player)
	{
		ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
		cur_player->id, cur_player->size,
		cur_player->name, cur_player->comment);
		cur_player = cur_player->next;
	}
}

int			main(int argc, char **argv)
{
	t_vm		*vm;

	if (argc < 2)
		print_usage();
	vm = init_vm();
	(void)argv;
	parse_input(vm, argc, argv);
	init_arena(vm);
	introduce_contestants(vm);
	if (vm->flags & (ADV_VISUALIZER | VISUALIZER))
		init_visualizer(vm);
	battle_loop(vm);
	if (vm->flags & ADV_VISUALIZER)
		visualize_states(vm);
	endwin();
	if (vm->flags & LEAKS)
		system("leaks corewar");
	exit(0);
	return (0);
}
