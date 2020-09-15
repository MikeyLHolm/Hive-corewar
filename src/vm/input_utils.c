/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/15 09:48:01 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/15 09:48:30 by mlindhol         ###   ########.fr       */
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
		tmp = head->next;
		while (tmp)
		{
			if (head->n == tmp->n && head->n != 0)
				handle_error("duplicate -n value");
			if (head->n > vm->player_amount || tmp->n > vm->player_amount)
				handle_error("-n value > players_amount");
			tmp = tmp->next;
		}
		head = head->next;
	}
}

/*
**	Auxilliary function to display LL recursively.
*/

void		display_list(t_player *head)
{
	if (head != NULL)
	{
		//ft_printf("\nSTATS START:: id = %d || n = %d\n", head->id, head->n);
		display_list(head->next);
	}
}