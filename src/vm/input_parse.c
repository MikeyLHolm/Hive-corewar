/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:13:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/15 09:48:46 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** Save player to linked list and parse player data.
*/

void		parse_player(t_vm *vm, char *filename, char *n)
{
	if (!vm->players)
	{
		vm->tail = save_player(vm, filename, n);
		vm->players = vm->tail;
	}
	else
	{
		vm->tail->next = save_player(vm, filename, n);
		vm->tail = vm->tail->next;
	}
}

/*
**	Parses arguments and saves flags to a int.
**	Dispatches to player parsing.
*/

void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
		{
			vm->flags = vm->flags | DUMP;
			vm->dump_cycle = ft_atoi(argv[i + 1]); //add error handling
			i++;
		}
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else if (ft_strequ(argv[i], "-a"))
			vm->flags = vm->flags | ADV_VISUALIZER;
		else if (ft_strequ(argv[i], "-s"))
		{
			vm->flags = vm->flags | START;
			vm->start = ft_atoi(argv[i + 1]); //add error handling
			i++;
		}
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;
			validate_n_flag(argv, i);
			parse_player(vm, argv[i + 2], argv[i + 1]);
			i += 2;
		}
		else
		{
			parse_player(vm, argv[i], 0);
		}
	}
	sort_players(vm);
	vm->player_last_alive = vm->player_amount;
}
