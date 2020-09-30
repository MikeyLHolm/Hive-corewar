/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   battle_loop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:30:24 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:45:37 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_cycle_carriages(t_vm *vm)
{
	t_carriage *cur;

	cur = vm->carriages;
	while (cur)
	{
		set_statement_codes(vm, cur);
		reduce_cycles(vm, cur);
		perform_statements(vm, cur);
		cur = cur->next;
	}
}

void	reduce_cycles(t_vm *vm, t_carriage *cur)
{
	(void)vm;
	if (cur->cycles_left)
		cur->cycles_left--;
}

void	disable_dead_carriages(t_vm *vm)
{
	t_carriage *cur_carriage;
	t_carriage *prev_carriage;
	t_carriage *next_carriage;

	cur_carriage = vm->carriages;
	prev_carriage = NULL;
	while (cur_carriage)
	{
		next_carriage = cur_carriage->next;
		if (cur_carriage->last_live_cycle < vm->cycles - vm->cycles_to_die)
		{
			cur_carriage->alive = 0;
			if (prev_carriage)
				prev_carriage->next = cur_carriage->next;
			else
				vm->carriages = cur_carriage->next;
			free(cur_carriage);
		}
		else
			prev_carriage = cur_carriage;
		cur_carriage = next_carriage;
	}
}

void	perform_check(t_vm *vm)
{
	static int cycle = -1;

	cycle++;
	if (cycle >= vm->cycles_to_die)
	{
		disable_dead_carriages(vm);
		if (vm->period_live_statements >= NBR_LIVE)
		{
			vm->cycles_to_die -= CYCLE_DELTA;
			vm->checks_without_change = 0;
		}
		else
		{
			vm->checks_without_change++;
			if (vm->checks_without_change >= MAX_CHECKS)
			{
				vm->cycles_to_die -= CYCLE_DELTA;
				vm->checks_without_change = 0;
			}
		}
		vm->period_live_statements = 0;
		cycle = 0;
	}
}

void	battle_loop(t_vm *vm)
{
	save_state(vm);
	while (1)
	{
		if (vm->flags & VISUALIZER)
			save_visualizer_memory(vm);
		vm->cycles++;
		perform_check(vm);
		if (vm->flags & DUMP && vm->cycles - 1 == vm->dump_cycle)
			handle_dump(vm);
		if (!check_carriages_alive(vm))
			break ;
		if (vm->flags & ADV_VISUALIZER)
			print_cycle_loading(vm);
		handle_cycle_carriages(vm);
		handle_visualization(vm);
	}
	get_winner(vm);
}
