/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:13:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/07 18:50:07 by sadawi           ###   ########.fr       */
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

/*
**	Swap two nodes with each other during bubble sort.
*/

t_player	*swap_nodes(t_player *cur, t_player *nxt)
{
	t_player	*tmp;

	tmp = nxt->next;
	nxt->next = cur;
	cur->next = tmp;
	return (nxt);
}

/*
**	Sort LL players based on new IDs using bubble sort.
*/

void		bubble_sort(t_player **head, int count, int swapped, int i)
{
	int			j;
	t_player	**h;
	t_player	*p1;
	t_player	*p2;

	while (++i < (count - 1))
	{
		h = head;
		swapped = 0;
		j = -1;
		while (++j < (count - i - 1))
		{
			p1 = *h;
			p2 = p1->next;
			if (p2 && p1->id > p2->id)
			{
				*h = swap_nodes(p1, p2);
				swapped = 1;
			}
			h = &(*h)->next;
		}
		if (swapped == 0)
			break ;
	}
}

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
**	Reorder ids based on int tab.
*/

void		reorder_ids(int *tab, t_vm *vm)
{
	int			i;
	int			count;
	t_player	*head;

	count = vm->player_amount;
	head = vm->players;
	while (head)
	{
		i = 0;
		while (tab[i] != head->id)
			i++;
		head->id = i + 1;
		head = head->next;
	}
}

/*
**	Add first players with n value to the indexes in tab-array.
**	Follow with players without n value and insert them to
**	empty array slots in id order.
*/

void		add_to_tab(t_vm *vm, int count)
{
	int			i;
	int			tab[count];
	t_player	*head;

	i = -1;
	while (++i < count)
		tab[i] = 0;
	head = vm->players;
	while (head)
	{
		if (head->n != 0)
		{
			//ft_printf("Head id [%d] || Head n-1 [%d]\n", head->id, (head->n - 1));
			tab[(head->n - 1)] = head->id;
		}
		head = head->next;
	}
	i = 0;
	head = vm->players;
	while (head)
	{
		while (tab[i] != 0)
			i++;
		if (head->n == 0)
			tab[i] = head->id;
		head = head->next;
	}
	// i = -1;
	// while (++i < count)
	// {
	// 	ft_printf("Le i [%d] = [%d]\n", i, tab[i]);
	// }
	reorder_ids(tab, vm);
}

/*
**	Main dispatcher to sort player LL based on IDs.
*/

void		sort_players(t_vm *vm)
{
	t_player		*cur;

	cur = vm->players;
	while (cur)
	{
		//ft_printf("\nSTATS START:: id = %d || n = %d\n", cur->id, cur->n);
		cur = cur->next;
		vm->player_amount++;
	}
	//ft_printf("\nNR of players: %d\n", vm->player_amount);
	cur = vm->players;
	check_duplicate_n(cur, vm);
	if (vm->players && vm->players->next && vm ->flags & N)
	{
		add_to_tab(vm, vm->player_amount);
		bubble_sort(&vm->players, vm->player_amount, 0, -1);
	}
	display_list(vm->players);
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
