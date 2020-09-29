/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aff.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 13:11:34 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 13:11:38 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	op_aff(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_register_index(vm, cur, 2);
	arg = cur->reg[arg];
	if (vm->flags & AFF_PRINT)
		ft_printf("AFF: %c\n", (char)arg);
}
