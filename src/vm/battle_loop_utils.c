/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battle_loop_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:41:48 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:45:49 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	get_winner(t_vm *vm)
{
	t_player *cur_player;

	cur_player = vm->players;
	while (cur_player)
	{
		if (cur_player->id == vm->player_last_alive)
			break ;
		cur_player = cur_player->next;
	}
	ft_printf("Contestant %d, \"%s\", has won !\n",
		cur_player->id, cur_player->name);
}

int		check_carriages_alive(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->alive)
			return (1);
		cur_carriage = cur_carriage->next;
	}
	return (0);
}

void	dump_memory(t_vm *vm)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (!(i % 64))
			ft_printf("0x%04x : ", i);
		ft_printf("%02x ", (unsigned char)vm->arena[i++]);
		if (!(i % 64))
			ft_printf("\n");
	}
}

void	handle_dump(t_vm *vm)
{
	if (!check_carriages_alive(vm))
		get_winner(vm);
	else
		dump_memory(vm);
	exit(0);
}
