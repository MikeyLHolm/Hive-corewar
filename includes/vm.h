/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 12:58:05 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/02 16:27:07 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"
# include "asm_op.h"

/*
**	Define flags
*/

# define DUMP			1
# define ERROR			2
# define LEAKS			4
# define N				8
# define VISUALIZER		16

# define BUFFER_SIZE	4096

typedef struct 			s_player
{
	char				*filename;
	unsigned char		*code;
	char				*comment;
	char				*name;
	int					id;
	int					n;
	int					size;
	struct s_player		*next;
}						t_player;

typedef struct 			s_input
{
	char				*line;
	struct s_input		*next;
}						t_input;

typedef struct			s_carriage
{
	int					id;
	int					carry;
	int					reg[REG_NUMBER];
	int					statement;
	int					cycles_left;
	int					last_live_cycle;
	int					position;
	int					alive;
	int					bytes_to_jump;
	struct s_carriage	*next;
}						t_carriage;

typedef struct 			s_vm
{
	int					flags;
	int					player_n;
	int					player_amount;
	t_player			*players;
	t_input				*file;
	unsigned char		arena[MEM_SIZE];
	int					player_last_alive;
	int					cycles_to_die;
	int					cycles;
	int					period_live_statements;
	int					checks_without_change;
	t_carriage			*carriages;
}						t_vm;

int						get_direct(t_vm *vm, t_carriage *cur, int offset);

int						get_indirect_value_trunc(t_vm *vm, t_carriage *cur, int offset, int addition);

int						get_indirect_value(t_vm *vm, t_carriage *cur, int offset, int addition);

int						get_indirect_address_trunc(t_vm *vm, t_carriage *cur, int offset, int addition);

int						get_indirect_address(t_vm *vm, t_carriage *cur, int offset, int addition);

int						get_register(t_vm *vm, t_carriage *cur, int offset);

t_carriage				*get_carriage_by_id(t_vm *vm, int id);

void					op_live(t_vm *vm, t_carriage *cur);

void					op_ld(t_vm *vm, t_carriage *cur);

void					op_lld(t_vm *vm, t_carriage *cur);

int						read_bytes(t_vm *vm, int pos, int amount);

void					write_bytes(t_vm *vm, int pos, int amount, unsigned int byte);

void					op_ldi(t_vm *vm, t_carriage *cur);

void					op_lldi(t_vm *vm, t_carriage *cur);

void					op_st(t_vm *vm, t_carriage *cur);

void					op_sti(t_vm *vm, t_carriage *cur);

void					op_and(t_vm *vm, t_carriage *cur);

void					op_zjmp(t_vm *vm, t_carriage *cur);

int						get_register_index(t_vm *vm, t_carriage *cur, int offset);

void					op_add(t_vm *vm, t_carriage *cur);

void					op_sub(t_vm *vm, t_carriage *cur);

void					op_or(t_vm *vm, t_carriage *cur);

void					op_xor(t_vm *vm, t_carriage *cur);

void					op_aff(t_vm *vm, t_carriage *cur);

t_carriage				*new_carriage(int id, t_carriage *next);

void					copy_carriage(t_vm *vm, t_carriage *cur, int pos);

void					op_fork(t_vm *vm, t_carriage *cur);

void					op_lfork(t_vm *vm, t_carriage *cur);

#endif