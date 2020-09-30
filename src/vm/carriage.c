/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   carriage.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:57:29 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 09:22:09 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

t_carriage	*new_carriage(int id, t_carriage *next)
{
	t_carriage *carriage;

	if (!(carriage = (t_carriage*)ft_memalloc(sizeof(t_carriage))))
		handle_error("Malloc failed");
	if (REG_NUMBER)
		carriage->reg[0] = id * -1;
	carriage->id = id;
	carriage->alive = 1;
	carriage->next = next;
	return (carriage);
}

void		init_carriages(t_vm *vm)
{
	t_carriage	*head;
	t_player	*cur_player;

	head = NULL;
	cur_player = vm->players;
	while (cur_player)
	{
		head = new_carriage(cur_player->id, head);
		cur_player = cur_player->next;
	}
	vm->carriages = head;
}

int			count_carriages(t_vm *vm)
{
	t_carriage	*cur;
	int			j;

	j = 0;
	cur = vm->carriages;
	while (cur)
	{
		j++;
		cur = cur->next;
	}
	return (j);
}

void		set_carriage_positions(t_vm *vm)
{
	t_carriage	*cur_carriage;
	int			offset;
	int			i;

	i = vm->player_amount - 1;
	offset = MEM_SIZE / vm->player_amount;
	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		cur_carriage->position = i-- * offset;
		cur_carriage = cur_carriage->next;
	}
}
