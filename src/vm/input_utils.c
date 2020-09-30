/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 09:48:01 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:52:29 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

/*
**	Check if -n is at least 3 args away from EOS. Also removes < 1 -n values.
*/

void		validate_n_flag(char **argv, int i)
{
	if (!(argv[i + 1] && argv[i + 2]))
		handle_error("EOS, check flag syntax!");
	if (ft_atoi(argv[i + 1]) < 1)
		handle_error("-n value < 1. Insert 1 <= x <= players_amount");
}

/*
**	Validates -n flag values.
**	Duplicate and > amount of players amounts not supported.
*/

void		check_duplicate_n(t_player *head, t_vm *vm)
{
	t_player	*tmp;

	while (head)
	{
		if (head->player_nbr > vm->player_amount)
			handle_error("-n value > players_amount");
		tmp = head->next;
		while (tmp)
		{
			if (head->player_nbr == tmp->player_nbr && head->player_nbr != 0)
				handle_error("duplicate -n value");
			tmp = tmp->next;
		}
		head = head->next;
	}
}

void		check_flags_valid(t_vm *vm)
{
	if (vm->flags & VISUALIZER && vm->flags & ADV_VISUALIZER)
		handle_error("Only one visualizer allowed at once");
	if (vm->flags & DUMP && vm->flags & (VISUALIZER | ADV_VISUALIZER))
		handle_error("Memory dumping not possible with visualizer");
	if (vm->flags & START && (!(vm->flags & ADV_VISUALIZER)))
		handle_error("Start flag only valid with advanced visualizer");
}

t_player	*save_player(t_vm *vm, char *filename, char *n)
{
	t_player	*player;
	static int	id;

	(void)vm;
	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
	player->id = ++id;
	validate_filename(filename, ".cor");
	player->filename = filename;
	player->player_nbr = n ? ft_atoi(n) : 0;
	get_player_info(player);
	return (player);
}

/*
**	Auxilliary function to display LL recursively.
*/

void		display_list(t_player *head)
{
	if (head != NULL)
		display_list(head->next);
}
