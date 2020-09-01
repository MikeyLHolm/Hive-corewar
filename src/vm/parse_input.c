/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/01 12:13:13 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/01 13:30:17 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		validate_n_flag(char **argv, int i)
{
	if (!(argv[i + 1] && argv[i + 2]))
		handle_error("EOS, check flag syntax!");
	if (ft_atoi(argv[i + 1]) < 1)
		handle_error("-n value < 1. Insert 1 <= x <= players_amount");
}

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

void		display_list(t_player *head)
{
	if (head != NULL)
	{
		ft_printf("\nSTATS START:: id = %d || n = %d\n", head->id, head->n);
		display_list(head->next);
	}
}

t_player	*swap_nodes(t_player *cur, t_player *nxt)
{
	t_player	*tmp;

	tmp = nxt->next;
	nxt->next = cur;
	cur->next = tmp;
	return (nxt);
}

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

void		update_ids(t_vm *vm)
{
	int			cur_n;
	int			cur_id;
	int			i;
	t_player	*head;

	cur_id = 0;
	cur_n = 0;
	i = -1;
	while (++i < vm->player_amount)
	{
		head = vm->players;
		while (head)
		{
			if (head->n > 0)
			{
				cur_id = head->id;
				cur_n = head->n;
				head->id = cur_n;
				// recursion??
				// update_ids(vm);
				i = -1;
				break
				// update(vm, cur_id, cur_n);
			}
			else
			{
				if (head->id == cur_n)
					while(head->id < cur_id)
						head->id++;
			}
			head = head->next;
		}


	}
}

void		sort_players(t_vm *vm)
{
	int				is_n;

	t_player		*cur;

	cur = vm->players;
	is_n = 0;
	while (cur)
	{
		ft_printf("\nSTATS START:: id = %d || n = %d\n", cur->id, cur->n);
		// if (cur->n != 0)
		// {
		// 	cur->id = cur->n;
		// 	is_n = 1;
		// }
		// ft_printf("STATS END:: id = %d || n = %d\n", cur->id, cur->n);
		cur = cur->next;
		vm->player_amount++;
	}
	ft_printf("\nNR of players: %d\n", vm->player_amount);
	cur = vm->players;
	check_duplicate_n(cur, vm);
	if ((vm->players && vm->players->next)/*  && is_n != 0 */)
	{
		// update id values. ++id if 2 same and n = different
		update_ids(vm);
		bubble_sort(&vm->players, vm->player_amount, 0, -1);
	}
	display_list(vm->players);
}

void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
			vm->flags = vm->flags | DUMP;
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
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
}
