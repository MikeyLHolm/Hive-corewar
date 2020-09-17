/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_op.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 14:29:20 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/16 12:25:20 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_OP_H
# define ASM_OP_H

# include "op.h"
# include "limits.h"

# define LIVE 1
# define LD 2
# define ST 3
# define ADD 4
# define SUB 5
# define AND 6
# define OR 7
# define XOR 8
# define ZJMP 9
# define LDI 10
# define STI 11
# define FORK 12
# define LLD 13
# define LLDI 14
# define LFORK 15
# define AFF 16

# define OP_CODE_AMOUNT 16

typedef struct		s_op
{
	int				args_n;
	int				args_type[3];
	int				args_type_code;
	int				op_code;
	char			*op_name;
	int				size_t_dir;
	int				cycles;
}					t_op;

/*
**	args_type_code: 0 = N, 1 = Y
**	size_t_dir: 0 = 4 bytes, 1 = 2 bytes
*/

static const		t_op	g_op_tab[OP_CODE_AMOUNT] =
{
	{
		.op_code = 0x01,
		.op_name = "live",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 0,
		.cycles = 10
	},
	{
		.op_code = 0x02,
		.op_name = "ld",
		.args_n = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 5
	},
	{
		.op_code = 0x03,
		.op_name = "st",
		.args_n = 2,
		.args_type = {T_REG, T_IND | T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 5
	},
	{
		.op_code = 0x04,
		.op_name = "add",
		.args_n = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 10
	},
	{
		.op_code = 0x05,
		.op_name = "sub",
		.args_n = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 10
	},
	{
		.op_code = 0x06,
		.op_name = "and",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 6
	},
	{
		.op_code = 0x07,
		.op_name = "or",
		.args_n = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 6
	},
	{
		.op_code = 0x08,
		.op_name = "xor",
		.args_n = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 6
	},
	{
		.op_code = 0x09,
		.op_name = "zjmp",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 1,
		.cycles = 20
	},
	{
		.op_code = 0x0a,
		.op_name = "ldi",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 1,
		.cycles = 25
	},
	{
		.op_code = 0x0b,
		.op_name = "sti",
		.args_n = 3,
		.args_type = {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},
		.args_type_code = 1,
		.size_t_dir = 1,
		.cycles = 25
	},
	{
		.op_code = 0x0c,
		.op_name = "fork",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 1,
		.cycles = 800
	},
	{
		.op_code = 0x0d,
		.op_name = "lld",
		.args_n = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 10
	},
	{
		.op_code = 0x0e,
		.op_name = "lldi",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 1,
		.cycles = 50
	},
	{
		.op_code = 0x0f,
		.op_name = "lfork",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 1,
		.cycles = 1000
	},
	{
		.op_code = 0x10,
		.op_name = "aff",
		.args_n = 1,
		.args_type = {T_REG, 0, 0},
		.args_type_code = 1,
		.size_t_dir = 0,
		.cycles = 2
	}
};

#endif
