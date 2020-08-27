/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/27 14:18:23 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}

t_vm		*init_vm(void)
{
	t_vm	*vm;

	if (!(vm = (t_vm*)ft_memalloc(sizeof(t_vm))))
		handle_error("Malloc failed at VM init");
	return (vm);
}

// void		save_flag(t_vm *vm, char flag)
// {
// 	if (flag == 'd')
// 		vm->flags = vm->flags | DUMP;
// 	else if (flag == 'e')
// 		vm->flags = vm->flags | ERROR;
// 	else if (flag == 'l')
// 		vm->flags = vm->flags | LEAKS;
// 	else if (flag == 'n')
// 		vm->flags = vm->flags | N;
// 	else if (flag == 'v')
// 		vm->flags = vm->flags | VISUALIZER;
// }
/*
void		sort_players(t_player *players)
{
	while (players)
	{
		ft_printf("id = %d || n = %d\n", players->id, players->n);
		players = players->next;
	}
}

t_player	*save_player(t_vm *vm, char **argv, char *n)
{
	t_player	*player;

	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
	player->id = 0;
	player->n = !n ? ft_atoi(n) : 0;
	//read_file(player);
	return (player);
}

void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;
	t_player	*players;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
			vm->flags = vm->flags | DUMP;
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;
			if (argv[i + 1] && argv[i + 2])
				players = save_player(vm, argv[i + 2], argv[i + 1]);
			else
				handle_error("-n flag in wrong place");
			i += 2;
		}
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else
			players = save_player(vm, argv[i], NULL);
		players = players->next;
	}
	players = NULL;
	sort_players(players);
}
*/
int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	(void)argv;
	// parse_input(vm, argc, argv);
	//read_input();
	//validate();
	//init_arena();
	//introduce_contestants();
	//fight();
	// if (vm->flags & LEAKS)
	// 	system("leaks corewar");
	return (0);
}
