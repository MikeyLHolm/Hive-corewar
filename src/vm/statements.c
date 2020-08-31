/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   statements.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/31 16:04:19 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/31 18:54:35 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		get_direct(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = 0;
	if (g_op_tab[cur->statement - 1].size_t_dir)
	{
		arg += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
		arg += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
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

int		get_indirect_value_trunc(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	int relative_address;
	int	arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	arg = 0;
	arg += vm->arena[(cur->position + relative_address) % MEM_SIZE] * 256 * 256 * 256;
	arg += vm->arena[(cur->position + relative_address + 1) % MEM_SIZE] * 256 * 256;
	arg += vm->arena[(cur->position + relative_address + 2) % MEM_SIZE] * 256;
	arg += vm->arena[(cur->position + relative_address + 3) % MEM_SIZE];
	return (arg);
}

int		get_indirect_value(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	int relative_address;
	int	arg;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	arg = 0;
	arg += vm->arena[(cur->position + relative_address) % MEM_SIZE] * 256 * 256 * 256;
	arg += vm->arena[(cur->position + relative_address + 1) % MEM_SIZE] * 256 * 256;
	arg += vm->arena[(cur->position + relative_address + 2) % MEM_SIZE] * 256;
	arg += vm->arena[(cur->position + relative_address + 3) % MEM_SIZE];
	return (arg);
}

int		get_indirect_address_trunc(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	int relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	relative_address %= IDX_MOD;
	return (relative_address);
}

int		get_indirect_address(t_vm *vm, t_carriage *cur, int offset, int addition)
{
	int relative_address;

	relative_address = 0;
	relative_address += vm->arena[(cur->position + offset) % MEM_SIZE] * 256;
	relative_address += vm->arena[(cur->position + offset + 1) % MEM_SIZE];
	relative_address += addition;
	return (relative_address);
}

int		get_register(t_vm *vm, t_carriage *cur, int offset)
{
	int arg;

	arg = vm->arena[cur->position + offset];
	return (arg);
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

void	op_live(t_vm *vm, t_carriage *cur)
{
	int			arg;
	t_carriage	*carriage_to_update;

	arg = get_direct(vm, cur, 1);
	arg *= -1;
	if (arg > 0 && arg < vm->player_amount)
	{
		vm->player_last_alive = arg;
		carriage_to_update = get_carriage_by_id(vm, arg);
		carriage_to_update->last_live_cycle = vm->cycles;
	}
	vm->period_live_statements++;
}

void	op_ld(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				reg_num;

	act = (cur->position + 1) % MEM_SIZE;
	if (((act >> 6) & 0x00))
		num = get_direct(vm, cur, 2);
	else
		num = get_indirect_value_trunc(vm, cur, 2, 0);
	reg_num = get_register(vm, cur, 6);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

void	op_lld(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				reg_num;

	act = (cur->position + 1) % MEM_SIZE;
	if (((act >> 6) & 0x00))
		num = get_direct(vm, cur, 2);
	else
		num = get_indirect_value(vm, cur, 2, 0);
	reg_num = get_register(vm, cur, 6);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

int		read_bytes(t_vm *vm, int pos, int amount)
{
	int arg;

	arg = 0;
	if (amount > 0)
		arg += vm->arena[pos % MEM_SIZE] * 256 * 256 * 256;
	if (amount > 1)
		arg += vm->arena[(pos + 1) % MEM_SIZE] * 256 * 256;
	if (amount > 2)
		arg += vm->arena[(pos + 2) % MEM_SIZE] * 256;
	if (amount > 3)
		arg += vm->arena[(pos + 3) % MEM_SIZE];
	return (arg);
}

void	write_bytes(t_vm *vm, int pos, int amount, int byte)
{

	if (amount > 0)
		vm->arena[pos % MEM_SIZE] = byte % 256;
	if (amount > 1)
		vm->arena[(pos + 1) % MEM_SIZE] = byte / 256 % 256;
	if (amount > 2)
		vm->arena[(pos + 2) % MEM_SIZE] = byte / 256 / 256 % 256;
	if (amount > 3)
		vm->arena[(pos + 3) % MEM_SIZE] = byte / 256 / 256 / 256 % 256;
}

void	op_ldi(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	act = (cur->position + 1) % MEM_SIZE;
	offset = 2;
	if (((act >> 7) & 0x01) && ((act >> 6) & 0x00))
	{
		arg1 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else if (((act >> 7) & 0x01) && ((act >> 6) & 0x01))
	{
		arg1 = get_indirect_value_trunc(vm, cur, offset, 0);
		offset += 2;
	}
	else
	{
		arg1 = get_register(vm, cur, offset);
		offset += 1;
	}
	if (((act >> 5) & 0x01) && ((act >> 4) & 0x00))
	{
		arg2 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else
	{
		arg2 = get_register(vm, cur, offset);
		offset += 1;
	}
	num = read_bytes(vm, cur->position + (arg1 + arg2) % IDX_MOD, 4);
	reg_num = get_register(vm, cur, offset);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

void	op_lldi(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				num;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	act = (cur->position + 1) % MEM_SIZE;
	offset = 2;
	if (((act >> 7) & 0x01) && ((act >> 6) & 0x00))
	{
		arg1 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else if (((act >> 7) & 0x01) && ((act >> 6) & 0x01))
	{
		arg1 = get_indirect_value(vm, cur, offset, 0);
		offset += 2;
	}
	else
	{
		arg1 = get_register(vm, cur, offset);
		offset += 1;
	}
	if (((act >> 5) & 0x01) && ((act >> 4) & 0x00))
	{
		arg2 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else
	{
		arg2 = get_register(vm, cur, offset);
		offset += 1;
	}
	num = read_bytes(vm, cur->position + arg1 + arg2, 4);
	reg_num = get_register(vm, cur, offset);
	cur->reg[reg_num] = num;
	cur->carry = !(num);
}

void	op_st(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				arg1;
	int				arg2;

	act = (cur->position + 1) % MEM_SIZE;
	arg1 = get_register(vm, cur, 2);
	if (((act >> 5) & 0x00))
	{
		arg2 = get_register(vm, cur, 3);
		cur->reg[arg2] = cur->reg[arg1];
	}
	else
	{
		arg2 = get_indirect_address_trunc(vm, cur, 3, 0);
		write_bytes(vm, cur->position + arg2, 4, cur->reg[arg1]);
	}
}

void	op_sti(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				arg1;
	int				arg2;
	int				reg_num;
	int				offset;

	act = (cur->position + 1) % MEM_SIZE;
	reg_num = get_register(vm, cur, 2);
	offset = 3;
	if (((act >> 5) & 0x01) && ((act >> 4) & 0x00))
	{
		arg1 = get_direct(vm, cur, offset);
		offset += 2;
	}
	else if (((act >> 5) & 0x01) && ((act >> 4) & 0x01))
	{
		arg1 = get_indirect_address_trunc(vm, cur, offset, 0);
		offset += 2;
	}
	else
	{
		arg1 = get_register(vm, cur, offset);
		arg1 = cur->reg[arg1];
		offset += 1;
	}
	if (((act >> 3) & 0x00))
	{
		arg2 = get_register(vm, cur, offset);
		arg2 = cur->reg[arg2];
	}
	else
		arg2 = get_direct(vm, cur, offset);
	write_bytes(vm, cur->position + (arg1 + arg2) % IDX_MOD, 4, cur->reg[reg_num]);
}
