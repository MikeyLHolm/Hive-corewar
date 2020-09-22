/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statements.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 16:04:19 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/22 11:20:38 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		positive_modulo(int n, int mod)
{
	if (n < 0)
	{
		n %= mod;
		n += mod;
	}
	else
		n %= mod;
	return (n);
}

int		get_direct(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = 0;
	if (g_op_tab[cur->statement - 1].size_t_dir)
	{
		arg += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
		arg += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
		return ((short)arg);
	}
	else
	{
		arg += vm->arena[(cur->position + offset) % MEM_SIZE] * 256 * 256 * 256;
		arg += vm->arena[(cur->position + offset + 1) % MEM_SIZE] * 256 * 256;
		arg += vm->arena[(cur->position + offset + 2) % MEM_SIZE] * 256;
		arg += vm->arena[(cur->position + offset + 3) % MEM_SIZE];
	}
	return (arg);
}

int		get_indirect_value_trunc(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256 * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)] * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 2, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 3, MEM_SIZE)];
	return (arg);
}

int		get_indirect_value(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256 * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)] * 256 * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 2, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 3, MEM_SIZE)];
	return (arg);
}

int		get_indirect_value_2_bytes(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short	relative_address;
	int		arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	arg = 0;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address, MEM_SIZE)] * 256;
	arg += vm->arena[positive_modulo(cur->position
		+ relative_address + 1, MEM_SIZE)];
	return (arg);
}

int		get_indirect_address_trunc(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	return (relative_address);
}

int		get_indirect_address(t_vm *vm, t_carriage *cur, int offset,
	int addition)
{
	short relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	return (relative_address);
}

int		get_register(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = vm->arena[(cur->position + offset) % MEM_SIZE];
	return (arg);
}

int		get_register_index(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = vm->arena[(cur->position + offset) % MEM_SIZE];
	return (arg - 1);
}

t_carriage	*get_carriage_by_id(t_vm *vm, int id)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->id == id)
			return (cur_carriage);
		cur_carriage = cur_carriage->next;
	}
	return (NULL);
}

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

void	op_ld(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				reg_num;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	if (!((act >> 6) & 0x01))
		num = get_direct(vm, cur, 2);
	else
		num = get_indirect_value_trunc(vm, cur, 2, 0);
	if (!((act >> 6) & 0x01))
		reg_num = get_register_index(vm, cur, 6);
	else
		reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

void	op_lld(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				reg_num;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	if (!((act >> 6) & 0x01))
		num = get_direct(vm, cur, 2);
	else
	{
		if (vm->flags & LLD_FIX)
			num = get_indirect_value(vm, cur, 2, 0);
		else
			num = get_indirect_value_2_bytes(vm, cur, 2, 0);
	}
	if (!((act >> 6) & 0x01))
		reg_num = get_register_index(vm, cur, 6);
	else
		reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

/*
** This way works with all possible byte amounts
*/

int		read_bytes(t_vm *vm, int pos, int amount)
{
	int		arg;

	arg = 0;
	if (amount == 4)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256 * 256 * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE] * 256 * 256;
		arg += vm->arena[(pos + 2) % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 3) % MEM_SIZE];
	}
	else if (amount == 2)
	{
		arg += vm->arena[pos % MEM_SIZE] * 256;
		arg += vm->arena[(pos + 1) % MEM_SIZE];
	}
	else
		arg += vm->arena[pos % MEM_SIZE];
	return (arg);
}

void	update_advanced_visualizer_memory(t_vm *vm, t_carriage *cur, int pos,
	int offset)
{
	if (offset == 0)
	{
		vm->updated_color_mem[pos % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[pos % MEM_SIZE] = 49;
	}
	else if (offset == 1)
	{
		vm->updated_color_mem[(pos + 1) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 1) % MEM_SIZE] = 49;
	}
	else if (offset == 2)
	{
		vm->updated_color_mem[(pos + 2) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 2) % MEM_SIZE] = 49;
	}
	else if (offset == 3)
	{
		vm->updated_color_mem[(pos + 3) % MEM_SIZE] = cur->id + 4;
		vm->updated_changed_mem[(pos + 3) % MEM_SIZE] = 49;
	}
}

void	update_visualizer_memory(t_vm *vm, t_carriage *cur, int pos,
	int offset)
{
	if (offset == 0)
	{
		vm->color_mem[pos % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[pos % MEM_SIZE] = 50;
	}
	else if (offset == 1)
	{
		vm->color_mem[(pos + 1) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 1) % MEM_SIZE] = 50;
	}
	else if (offset == 2)
	{
		vm->color_mem[(pos + 2) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 2) % MEM_SIZE] = 50;
	}
	else if (offset == 3)
	{
		vm->color_mem[(pos + 3) % MEM_SIZE] = cur->id + 4;
		vm->changed_mem[(pos + 3) % MEM_SIZE] = 50;
	}
}

void	write_bytes(t_vm *vm, t_carriage *cur, int pos, unsigned int byte)
{
	vm->arena[pos % MEM_SIZE] = byte / 256 / 256 / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 0);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 0);
	vm->arena[(pos + 1) % MEM_SIZE] = byte / 256 / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 1);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 1);
	vm->arena[(pos + 2) % MEM_SIZE] = byte / 256 % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 2);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 2);
	vm->arena[(pos + 3) % MEM_SIZE] = byte % 256;
	if (vm->flags & ADV_VISUALIZER)
		update_advanced_visualizer_memory(vm, cur, pos, 3);
	else if (vm->flags & VISUALIZER)
		update_visualizer_memory(vm, cur, pos, 3);
}

int		get_ldi_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 7) & 0x01) && !((cur->act >> 6) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 2;
	}
	else if (((cur->act >> 7) & 0x01) && ((cur->act >> 6) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg1 = get_register_index(vm, cur, *offset);
		arg1 = cur->reg[arg1];
		*offset += 1;
	}
	return (arg1);
}

void	op_ldi(t_vm *vm, t_carriage *cur)
{
	int				num;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_ldi_first_arg(vm, cur, &offset);
	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, offset);
		arg2 = cur->reg[arg2];
		offset += 1;
	}
	num = read_bytes(vm, positive_modulo(cur->position + (arg1 + arg2)
		% IDX_MOD, MEM_SIZE), 4);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = num;
}

void	op_lldi(t_vm *vm, t_carriage *cur)
{
	int				num;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_ldi_first_arg(vm, cur, &offset);
	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, offset);
		arg2 = cur->reg[arg2];
		offset += 1;
	}
	num = read_bytes(vm, positive_modulo(cur->position + (arg1 + arg2),
		MEM_SIZE), 4);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

void	op_st(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				arg1;
	int				arg2;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	arg1 = get_register_index(vm, cur, 2);
	if (!((act >> 5) & 0x01))
	{
		arg2 = get_register_index(vm, cur, 3);
		cur->reg[arg2] = cur->reg[arg1];
	}
	else
	{
		arg2 = get_indirect_address_trunc(vm, cur, 3, 0);
		write_bytes(vm, cur, positive_modulo(cur->position + arg2, MEM_SIZE),
			cur->reg[arg1]);
	}
}

int		get_sti_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 2;
	}
	else if (((cur->act >> 5) & 0x01) && ((cur->act >> 4) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg1 = get_register_index(vm, cur, *offset);
		arg1 = cur->reg[arg1];
		*offset += 1;
	}
	return (arg1);
}

void	op_sti(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	reg_num = get_register_index(vm, cur, 2);
	offset = 3;
	arg1 = get_sti_first_arg(vm, cur, &offset);
	if (!((cur->act >> 3) & 0x01))
	{
		arg2 = get_register_index(vm, cur, offset);
		arg2 = cur->reg[arg2];
	}
	else
		arg2 = get_direct(vm, cur, offset);
	write_bytes(vm, cur, positive_modulo(cur->position + (arg1 + arg2)
		% IDX_MOD, MEM_SIZE), cur->reg[reg_num]);
}

int		get_and_first_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg1;

	if (((cur->act >> 7) & 0x01) && !((cur->act >> 6) & 0x01))
	{
		arg1 = get_direct(vm, cur, *offset);
		*offset += 4;
	}
	else if (((cur->act >> 7) & 0x01) && ((cur->act >> 6) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg1 = get_register_index(vm, cur, *offset);
		arg1 = cur->reg[arg1];
		*offset += 1;
	}
	return (arg1);
}

int		get_and_second_arg(t_vm *vm, t_carriage *cur, int *offset)
{
	int arg2;

	if (((cur->act >> 5) & 0x01) && !((cur->act >> 4) & 0x01))
	{
		arg2 = get_direct(vm, cur, *offset);
		*offset += 4;
	}
	else if (((cur->act >> 5) & 0x01) && ((cur->act >> 4) & 0x01))
	{
		arg2 = get_indirect_value_trunc(vm, cur, *offset, 0);
		*offset += 2;
	}
	else
	{
		arg2 = get_register_index(vm, cur, *offset);
		arg2 = cur->reg[arg2];
		*offset += 1;
	}
	return (arg2);
}

void	op_and(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_and_first_arg(vm, cur, &offset);
	arg2 = get_and_second_arg(vm, cur, &offset);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = arg1 & arg2;
	cur->carry = !(arg1 & arg2);
}

void	op_zjmp(t_vm *vm, t_carriage *cur)
{
	int arg1;

	if (!cur->carry)
		return ;
	arg1 = get_direct(vm, cur, 1);
	cur->position = positive_modulo(cur->position + arg1 % IDX_MOD, MEM_SIZE);
}

void	op_add(t_vm *vm, t_carriage *cur)
{
	int	arg1;
	int	arg2;
	int reg_num;

	arg1 = get_register_index(vm, cur, 2);
	arg1 = cur->reg[arg1];
	arg2 = get_register_index(vm, cur, 3);
	arg2 = cur->reg[arg2];
	reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = arg1 + arg2;
	cur->carry = !(arg1 + arg2);
}

void	op_sub(t_vm *vm, t_carriage *cur)
{
	int	arg1;
	int	arg2;
	int reg_num;

	arg1 = get_register_index(vm, cur, 2);
	arg1 = cur->reg[arg1];
	arg2 = get_register_index(vm, cur, 3);
	arg2 = cur->reg[arg2];
	reg_num = get_register_index(vm, cur, 4);
	cur->reg[reg_num] = arg1 - arg2;
	cur->carry = !(arg1 - arg2);
}

void	op_or(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_and_first_arg(vm, cur, &offset);
	arg2 = get_and_second_arg(vm, cur, &offset);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = arg1 | arg2;
	cur->carry = !(arg1 | arg2);
}

void	op_xor(t_vm *vm, t_carriage *cur)
{
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	offset = 2;
	arg1 = get_and_first_arg(vm, cur, &offset);
	arg2 = get_and_second_arg(vm, cur, &offset);
	reg_num = get_register_index(vm, cur, offset);
	cur->reg[reg_num] = arg1 ^ arg2;
	cur->carry = !(arg1 ^ arg2);
}

void	copy_carriage(t_vm *vm, t_carriage *cur, int pos)
{
	int reg_num;

	vm->carriages = new_carriage(cur->id, vm->carriages);
	vm->carriages->carry = cur->carry;
	vm->carriages->last_live_cycle = cur->last_live_cycle;
	reg_num = 0;
	while (reg_num < REG_NUMBER)
	{
		vm->carriages->reg[reg_num] = cur->reg[reg_num];
		reg_num++;
	}
	vm->carriages->position = pos % MEM_SIZE;
}

void	op_fork(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_direct(vm, cur, 1);
	copy_carriage(vm, cur, positive_modulo(cur->position + arg % IDX_MOD,
		MEM_SIZE));
}

void	op_lfork(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_direct(vm, cur, 1);
	copy_carriage(vm, cur, positive_modulo(cur->position + arg, MEM_SIZE));
}

void	op_aff(t_vm *vm, t_carriage *cur)
{
	int arg;

	arg = get_register_index(vm, cur, 2);
	arg = cur->reg[arg];
	if (vm->flags & AFF_PRINT)
		ft_printf("AFF: %c\n", (char)arg);
}
