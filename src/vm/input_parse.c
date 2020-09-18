/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:13:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/18 14:16:13 by sadawi           ###   ########.fr       */
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

int		count_players(t_vm *vm)
{
	t_player	*cur;
	int			i;

	cur = vm->players;
	i = 0;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	return (i);
}

void	check_flags_valid(t_vm *vm)
{
	if (vm->flags & VISUALIZER && vm->flags & ADV_VISUALIZER)
		handle_error("Only one visualizer allowed at once");
	if (vm->flags & DUMP && vm->flags & (VISUALIZER | ADV_VISUALIZER))
		handle_error("Memory dumping not possible with visualizer");
	if (vm->flags & START && (!(vm->flags & ADV_VISUALIZER)))
		handle_error("Start flag only valid with advanced visualizer");
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
			i++;
			if (i < argc)
				vm->dump_cycle = ft_atoi(argv[i]);
			else
				handle_error("Dump flag not followed with digit");
		}
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else if (ft_strequ(argv[i], "-a"))
			vm->flags = vm->flags | ADV_VISUALIZER;
		else if (ft_strequ(argv[i], "-f"))
			vm->flags = vm->flags | LLD_FIX;
		else if (ft_strequ(argv[i], "-z"))
			vm->flags = vm->flags | AFF_PRINT;
		else if (ft_strequ(argv[i], "-l"))
			vm->flags = vm->flags | LIVE_PRINT;
		else if (ft_strequ(argv[i], "-s"))
		{
			vm->flags = vm->flags | START;
			i++;
			if (i < argc)
				vm->start = ft_atoi(argv[i]);
			else
				handle_error("Start flag not followed with digit");
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
	vm->player_amount = count_players(vm);
	if (!vm->players)
		handle_error("No valid players specified");
	if (vm->player_amount > MAX_PLAYERS)
		handle_error(ft_sprintf("Too many players, maximum of %d players allowed", MAX_PLAYERS));
	sort_players(vm);
	vm->player_last_alive = vm->player_amount;
	check_flags_valid(vm);
}
