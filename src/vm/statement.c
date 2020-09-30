/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statement.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 09:07:08 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:18:53 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	execute_statement(t_vm *vm, t_carriage *cur)
{
	cur->statement == LIVE ? op_live(vm, cur) : 0;
	cur->statement == LD ? op_ld(vm, cur) : 0;
	cur->statement == ST ? op_st(vm, cur) : 0;
	cur->statement == ADD ? op_add(vm, cur) : 0;
	cur->statement == SUB ? op_sub(vm, cur) : 0;
	cur->statement == AND ? op_and(vm, cur) : 0;
	cur->statement == OR ? op_or(vm, cur) : 0;
	cur->statement == XOR ? op_xor(vm, cur) : 0;
	cur->statement == ZJMP ? op_zjmp(vm, cur) : 0;
	cur->statement == LDI ? op_ldi(vm, cur) : 0;
	cur->statement == STI ? op_sti(vm, cur) : 0;
	cur->statement == FORK ? op_fork(vm, cur) : 0;
	cur->statement == LLD ? op_lld(vm, cur) : 0;
	cur->statement == LLDI ? op_lldi(vm, cur) : 0;
	cur->statement == LFORK ? op_lfork(vm, cur) : 0;
	cur->statement == AFF ? op_aff(vm, cur) : 0;
}

void	set_statement_codes(t_vm *vm, t_carriage *cur)
{
	if (!cur->cycles_left)
		get_statement(vm, cur);
}

void	handle_statement(t_vm *vm, t_carriage *cur)
{
	if (cur->statement > 0 && cur->statement <= OP_CODE_AMOUNT)
	{
		cur->act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
		if (check_arguments_valid(vm, cur))
			execute_statement(vm, cur);
		count_bytes_to_skip(cur);
		move_carriage_next_statement(cur);
	}
	else
		cur->position = (cur->position + 1) % MEM_SIZE;
}

void	perform_statements(t_vm *vm, t_carriage *cur)
{
	if (cur->alive)
	{
		if (!cur->cycles_left)
			handle_statement(vm, cur);
	}
}
