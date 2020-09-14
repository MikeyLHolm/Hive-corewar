/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_players.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/03 09:02:18 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/14 13:37:50 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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
		if (head->player_nbr != 0)
		{
			//ft_printf("Head id [%d] || Head n-1 [%d]\n", head->id, (head->n - 1));
			tab[(head->player_nbr - 1)] = head->id;
		}
		head = head->next;
	}
	i = 0;
	head = vm->players;
	while (head)
	{
		while (tab[i] != 0)
			i++;
		if (head->player_nbr == 0)
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
	//display_list(vm->players);
}
