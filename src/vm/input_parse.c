/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:13:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 16:04:34 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
** Save player to linked list and parse player data.
*/

void	parse_player(t_vm *vm, char *filename, char *n)
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
	if (!i)
		handle_error("No valid players specified");
	return (i);
}

/*
**	Parses arguments and saves flags to a int.
**	Dispatches to player parsing.
*/

int		parse_simple_flags(t_vm *vm, char **argv, int i)
{
	if (ft_strequ(argv[i], "-e"))
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
	else
		return (0);
	return (1);
}

int		parse_digit_flags(t_vm *vm, int argc, char **argv, int *i)
{
	if (ft_strequ(argv[*i], "-dump"))
	{
		vm->flags = vm->flags | DUMP;
		*i += 1;
		if (*i < argc)
			vm->dump_cycle = ft_atoi(argv[*i]);
		else
			handle_error("Dump flag not followed with digit");
	}
	else if (ft_strequ(argv[*i], "-s"))
	{
		vm->flags = vm->flags | START;
		*i += 1;
		if (*i < argc)
			vm->start = ft_atoi(argv[*i]);
		else
			handle_error("Start flag not followed with digit");
	}
	else
		return (0);
	return (1);
}

void	parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;

	i = 0;
	while (++i < argc)
	{
		if (parse_simple_flags(vm, argv, i))
			;
		else if (parse_digit_flags(vm, argc, argv, &i))
			;
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;
			validate_n_flag(argv, i);
			parse_player(vm, argv[i + 2], argv[i + 1]);
			i += 2;
		}
		else
			parse_player(vm, argv[i], 0);
	}
	vm->player_amount = count_players(vm);
	if (vm->player_amount > MAX_PLAYERS)
		handle_error(ft_sprintf(
			"Too many players, maximum of %d players allowed", MAX_PLAYERS));
	sort_players(vm);
	check_flags_valid(vm);
}
