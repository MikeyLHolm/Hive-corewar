/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:14:23 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:14:24 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	print_player_alive(t_vm *vm, int player_num)
{
	t_player *cur;

	cur = vm->players;
	while (cur)
	{
		if (cur->id == player_num)
			break ;
		cur = cur->next;
	}
	cur->last_live_cycle = vm->cycles;
	if (vm->flags & LIVE_PRINT)
		ft_printf("A process shows that player %d (%s) is alive.\n",
			player_num, cur->name);
}

void	op_live(t_vm *vm, t_carriage *cur)
{
	int			arg;

	arg = get_direct(vm, cur, 1);
	arg *= -1;
	cur->last_live_cycle = vm->cycles;
	if (arg > 0 && arg <= vm->player_amount)
	{
		vm->player_last_alive = arg;
		print_player_alive(vm, arg);
	}
	vm->period_live_statements++;
}
