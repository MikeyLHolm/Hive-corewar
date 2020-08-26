/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   asm_op.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/25 14:29:20 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/26 08:47:58 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ASM_OP_H
# define ASM_OP_H

# define T_REG					1
# define T_DIR					2
# define T_IND					4

typedef struct 		s_op_tab
{
	int				args_n;
	int				args_type[3];
	int				args_type_code;
	int				op;
	int				op_code;
	char			*op_name;
	int				size_t_dir;
}					t_op_tab;

/*
**	args_type_code: 0 = N, 1 = Y
**	size_t_dir: 0 = 4 bytes, 1 = 2 bytes
*/

static const	t_op_tab		g_op_tab[16] =
{
	{
		.op = 1,
		.op_code = 0x01,
		.op_name = "live",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 0
	},
	{
		.op = 2,
		.op_code = 0x02,
		.op_name = "ld",
		.args_n = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 3,
		.op_code = 0x03,
		.op_name = "st",
		.args_n = 2,
		.args_type = {T_REG, T_IND | T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 4,
		.op_code = 0x04,
		.op_name = "add",
		.args_n = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 5,
		.op_code = 0x05,
		.op_name = "sub",
		.args_n = 3,
		.args_type = {T_REG, T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 6,
		.op_code = 0x06,
		.op_name = "and",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 7,
		.op_code = 0x07,
		.op_name = "or",
		.args_n = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 8,
		.op_code = 0x08,
		.op_name = "xor",
		.args_n = 3,
		.args_type = {T_REG | T_IND | T_DIR, T_REG | T_IND | T_DIR, T_REG},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 9,
		.op_code = 0x09,
		.op_name = "zjmp",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 1
	},
	{
		.op = 10,
		.op_code = 0x0a,
		.op_name = "ldi",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 1
	},
	{
		.op = 11,
		.op_code = 0x0b,
		.op_name = "sti",
		.args_n = 3,
		.args_type = {T_REG, T_REG | T_DIR | T_IND, T_DIR | T_REG},
		.args_type_code = 1,
		.size_t_dir = 1
	},
	{
		.op = 12,
		.op_code = 0x0c,
		.op_name = "fork",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 2
	},
	{
		.op = 13,
		.op_code = 0x0d,
		.op_name = "lld",
		.args_n = 2,
		.args_type = {T_DIR | T_IND, T_REG, 0},
		.args_type_code = 1,
		.size_t_dir = 0
	},
	{
		.op = 14,
		.op_code = 0x0e,
		.op_name = "lldi",
		.args_n = 3,
		.args_type = {T_REG | T_DIR | T_IND, T_DIR | T_REG, T_REG},
		.args_type_code = 1,
		.size_t_dir = 1
	},
	{
		.op = 15,
		.op_code = 0x0f,
		.op_name = "lfork",
		.args_n = 1,
		.args_type = {T_DIR, 0, 0},
		.args_type_code = 0,
		.size_t_dir = 1
	},
	{
		.op = 16,
		.op_code = 0x10,
		.op_name = "aff",
		.args_n = 1,
		.args_type = {T_REG, 0, 0},
		.args_type_code = 1,
		.size_t_dir = 0
	}
};

#endif