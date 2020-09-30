/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 08:54:56 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 08:55:34 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		check_argument_indirect(t_carriage *cur, int *offset, int n)
{
	if (!(g_op_tab[cur->statement - 1].args_type[n] & T_IND))
		return (0);
	*offset += 2;
	return (1);
}

int		check_argument_registry(t_vm *vm, t_carriage *cur, int *offset, int n)
{
	if (vm->arena[(cur->position + *offset) % MEM_SIZE] <= 0
	|| vm->arena[(cur->position + *offset) % MEM_SIZE] > REG_NUMBER)
		return (0);
	if (!(g_op_tab[cur->statement - 1].args_type[n] & T_REG))
		return (0);
	*offset += 1;
	return (1);
}

int		check_argument_direct(t_carriage *cur, int *offset, int n)
{
	if (!(g_op_tab[cur->statement - 1].args_type[n] & T_DIR))
		return (0);
	if (g_op_tab[cur->statement - 1].size_t_dir)
		*offset += 2;
	else
		*offset += 4;
	return (1);
}

int		check_argument_type_code(t_vm *vm, t_carriage *cur)
{
	int				offset;
	int				n;
	int				bit;
	int				return_value;

	offset = 2;
	n = 0;
	bit = 7;
	while (n < g_op_tab[cur->statement - 1].args_n)
	{
		if (((cur->act >> bit) & 0x01) && ((cur->act >> (bit - 1)) & 0x01))
			return_value = check_argument_indirect(cur, &offset, n);
		else if (!((cur->act >> bit) & 1) && ((cur->act >> (bit - 1)) & 0x01))
			return_value = check_argument_registry(vm, cur, &offset, n);
		else if (((cur->act >> bit) & 1) && !((cur->act >> (bit - 1)) & 0x01))
			return_value = check_argument_direct(cur, &offset, n);
		else
			return (0);
		if (!return_value)
			return (0);
		bit -= 2;
		n++;
	}
	return (1);
}

int		check_arguments_valid(t_vm *vm, t_carriage *cur)
{
	if (g_op_tab[cur->statement - 1].args_type_code)
		if (!check_argument_type_code(vm, cur))
			return (0);
	return (1);
}
