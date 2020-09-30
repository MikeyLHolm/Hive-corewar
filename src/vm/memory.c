/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 09:14:40 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 09:21:55 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

unsigned char	*resize_memory(unsigned char *player_code, int size,
	int size_to_add)
{
	unsigned char *tmp;

	if (!(tmp = malloc(size + size_to_add)))
		handle_error("Malloc failed");
	ft_memcpy(tmp, player_code, size);
	free(player_code);
	return (tmp);
}

int				*get_changed_memory(t_vm *vm, t_state *prev)
{
	int	i;
	int	*changed_mem;

	if (!(changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	i = 0;
	if (prev)
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_changed_mem[i])
				changed_mem[i] = vm->updated_changed_mem[i];
			else
				changed_mem[i] = prev->changed_mem[i] - 1 > 0 ?
					prev->changed_mem[i] - 1 : 0;
			i++;
		}
	}
	ft_bzero(vm->updated_changed_mem, MEM_SIZE * sizeof(int));
	return (changed_mem);
}

void			update_changed_memory(t_vm *vm)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (vm->changed_mem[i] > 0)
			vm->changed_mem[i]--;
		i++;
	}
}
