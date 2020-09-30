/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arena.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:34:56 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 16:39:47 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	load_players(t_vm *vm)
{
	t_player	*cur_player;
	int			offset;
	int			i;

	i = 0;
	cur_player = vm->players;
	offset = MEM_SIZE / vm->player_amount;
	while (i < vm->player_amount)
	{
		if (i * offset + cur_player->size >= MEM_SIZE)
			handle_error("Player does not fit into arena");
		ft_memcpy(&vm->arena[i++ * offset], cur_player->code, cur_player->size);
		cur_player = cur_player->next;
	}
}

void	init_arena(t_vm *vm)
{
	load_players(vm);
	init_carriages(vm);
	set_carriage_positions(vm);
}
