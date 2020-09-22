/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm.h                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 12:58:05 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/22 11:52:31 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VM_H
# define VM_H

# include "../libft/includes/libft.h"
# include "fcntl.h"
# include "op.h"
# include "asm_op.h"
# include <ncurses.h>
# include "limits.h"

/*
**	Define flags
*/

# define DUMP			1
# define ERROR			2
# define LEAKS			4
# define N				8
# define VISUALIZER		16
# define ADV_VISUALIZER	32
# define START			64
# define LLD_FIX		128
# define AFF_PRINT		256
# define LIVE_PRINT		512

# define BUFFER_SIZE	4096

typedef struct			s_player
{
	char				*filename;
	unsigned char		*code;
	char				*comment;
	char				*name;
	int					id;
	int					player_nbr;
	int					size;
	int					arg_types[3];
	int					last_live_cycle;
	struct s_player		*next;
}						t_player;

typedef struct			s_input
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
	int					statement_owner;
	int					cycles_left;
	int					last_live_cycle;
	int					position;
	int					alive;
	int					bytes_to_jump;
	int					act;
	struct s_carriage	*next;
}						t_carriage;

typedef	struct			s_state
{
	unsigned char		arena[MEM_SIZE];
	int					*cursor_mem;
	int					*changed_mem;
	int					*color_mem;
	int					cycle;
	int					carriage_amount;
	int					cycles_to_die;
	struct s_state		*prev;
	struct s_state		*next;
	int					player1_last_live;
	int					player2_last_live;
	int					player3_last_live;
	int					player4_last_live;
	int					last_live_player;
}						t_state;

typedef struct			s_controls
{
	int					autoplay;
	int					step_size;
}						t_controls;

typedef struct			s_vm
{
	int					flags;
	int					player_amount;
	t_player			*tail;
	t_player			*players;
	t_input				*file;
	unsigned char		arena[MEM_SIZE];
	int					player_last_alive;
	int					cycles_to_die;
	int					cycles;
	int					period_live_statements;
	int					checks_without_change;
	t_carriage			*carriages;
	t_state				*arena_history_head;
	t_state				*cur_state;
	t_controls			controls;
	int					*cursor_mem;
	int					*changed_mem;
	int					*color_mem;
	int					start;
	int					dump_cycle;
	int					*updated_color_mem;
	int					*updated_changed_mem;
	int					cursor_x;
	int					cursor_y;
}						t_vm;

int						get_direct(t_vm *vm, t_carriage *cur, int offset);

int						get_indirect_value_trunc(t_vm *vm, t_carriage *cur,
							int offset, int addition);

int						get_indirect_value(t_vm *vm, t_carriage *cur,
							int offset, int addition);

int						get_indirect_address_trunc(t_vm *vm, t_carriage *cur,
							int offset, int addition);

int						get_indirect_address(t_vm *vm, t_carriage *cur,
							int offset, int addition);

int						get_register(t_vm *vm, t_carriage *cur, int offset);

t_carriage				*get_carriage_by_id(t_vm *vm, int id);

void					op_live(t_vm *vm, t_carriage *cur);

void					op_ld(t_vm *vm, t_carriage *cur);

void					op_lld(t_vm *vm, t_carriage *cur);

int						read_bytes(t_vm *vm, int pos, int amount);

void					write_bytes(t_vm *vm, t_carriage *cur, int pos,
							unsigned int byte);

void					op_ldi(t_vm *vm, t_carriage *cur);

void					op_lldi(t_vm *vm, t_carriage *cur);

void					op_st(t_vm *vm, t_carriage *cur);

void					op_sti(t_vm *vm, t_carriage *cur);

void					op_and(t_vm *vm, t_carriage *cur);

void					op_zjmp(t_vm *vm, t_carriage *cur);

int						get_register_index(t_vm *vm, t_carriage *cur,
							int offset);

void					op_add(t_vm *vm, t_carriage *cur);

void					op_sub(t_vm *vm, t_carriage *cur);

void					op_or(t_vm *vm, t_carriage *cur);

void					op_xor(t_vm *vm, t_carriage *cur);

void					op_aff(t_vm *vm, t_carriage *cur);

t_carriage				*new_carriage(int id, t_carriage *next);

void					copy_carriage(t_vm *vm, t_carriage *cur, int pos);

void					op_fork(t_vm *vm, t_carriage *cur);

void					op_lfork(t_vm *vm, t_carriage *cur);

/*
**	Error management functions.
*/

void					handle_error(char *message);

/*
**	Input parsing functions.
*/

t_player				*save_player(t_vm *vm, char *filename, char *n);
void					check_duplicate_n(t_player *head, t_vm *vm);
void					parse_input(t_vm *vm, int argc, char **argv);
void					sort_players(t_vm *vm);
void					validate_n_flag(char **argv, int i);
void					check_duplicate_n(t_player *head, t_vm *vm);
void					validate_player(t_player *player);

/*
**	remove these!
*/

void					display_list(t_player *head);

int						count_carriages(t_vm *vm);

int						positive_modulo(int n, int mod);

void					print_player_alive(t_vm *vm, int player_num);

int						get_ldi_first_arg(t_vm *vm, t_carriage *cur,
							int *offset);

int						get_sti_first_arg(t_vm *vm, t_carriage *cur,
							int *offset);

int						get_and_first_arg(t_vm *vm, t_carriage *cur,
							int *offset);

int						get_and_second_arg(t_vm *vm, t_carriage *cur,
							int *offset);

void					parse_player(t_vm *vm, char *filename, char *n);

int						count_players(t_vm *vm);

void					check_flags_valid(t_vm *vm);

int						parse_simple_flags(t_vm *vm, char **argv, int i);

int						parse_digit_flags(t_vm *vm, int argc, char **argv,
							int i);

t_player				*swap_nodes(t_player *cur, t_player *nxt);

void					bubble_sort(t_player **head, int count, int swapped,
							int i);

void					reorder_ids(int *tab, t_vm *vm);

void					add_to_tab(t_vm *vm, int count);

void					init_controls(t_vm *vm);

t_vm					*init_vm(void);

void					check_magic_header(int fd);

char					*get_player_name(int fd);

int						get_player_size(int fd);

char					*get_player_comment(int fd);

void					get_player_info(t_player *player);

void					validate_filename(char *filename, char *extension);

void					print_player_code(t_player *player);

void					print_player(t_player *player);

void					manually_create_players(t_vm *vm);

void					load_players(t_vm *vm);

void					init_carriages(t_vm *vm);

void					set_carriage_positions(t_vm *vm);

void					init_arena(t_vm *vm);

void					introduce_contestants(t_vm *vm);

void					disable_dead_carriages(t_vm *vm);

void					perform_check(t_vm *vm);

int						check_carriages_alive(t_vm *vm);

void					get_statement(t_vm *vm, t_carriage *cur);

void					set_statement_codes(t_vm *vm, t_carriage *cur);

void					reduce_cycles(t_vm *vm, t_carriage *cur);

int						check_argument_indirect(t_carriage *cur, int *offset,
							int n);

int						check_argument_registry(t_vm *vm, t_carriage *cur,
							int *offset, int n);

int						check_argument_direct(t_carriage *cur, int *offset,
							int n);

int						check_argument_type_code(t_vm *vm, t_carriage *cur);

int						check_arguments_valid(t_vm *vm, t_carriage *cur);

int						skip_direct_bytes(t_carriage *cur);

void					count_bytes_to_skip(t_carriage *cur);

void					move_carriage_next_statement(t_carriage *cur);

void					execute_statement(t_vm *vm, t_carriage *cur);

void					handle_statement(t_vm *vm, t_carriage *cur);

void					perform_statements(t_vm *vm, t_carriage *cur);

void					get_winner(t_vm *vm);

void					state_get_lives(t_vm *vm, t_state *state);

int						*get_changed_memory(t_vm *vm, t_state *prev);

t_state					*new_state(t_vm *vm, t_state *prev);

void					save_state(t_vm *vm);

void					update_changed_memory(t_vm *vm);

void					dump_memory(t_vm *vm);

void					handle_dump(t_vm *vm);

void					print_cycle_loading(t_vm *vm);

void					handle_visualization(t_vm *vm);

void					save_visualizer_memory(t_vm *vm);

void					handle_cycle_carriages(t_vm *vm);

void					battle_loop(t_vm *vm);

void					print_arena(t_vm *vm);

void					init_visualizer(t_vm *vm);

void					load_player_colors(t_vm *vm, int *cursor_mem);

int						*get_color_mem_old(t_vm *vm, int *prev_mem);

int						*get_cursor_mem_old(t_vm *vm, int *prev);

int						*get_color_mem(t_vm *vm, t_state *prev);

int						*get_cursor_mem(t_vm *vm, t_state *prev);

int						ft_abs(int n);

void					move_if_valid(t_vm *vm, int y, int x);

void					print_if_valid(t_vm *vm, char *str);

void					print_player_last_alive(t_vm *vm, int player_num);

void					print_player_lives(t_vm *vm);

void					print_state_player_lives(t_vm *vm, t_state *state);

void					print_border(t_vm *vm);

void					print_visualizer_info(t_vm *vm);

void					print_controls(t_vm *vm);

void					draw_vertical_borders(t_vm *vm);

void					draw_info_borders(t_vm *vm);

void					draw_borders(t_vm *vm);

void					print_footer(t_vm *vm);

void					visualizer_draw_map(t_vm *vm);

int						visualizer_handle_controls(t_vm *vm);

void					visualize(t_vm *vm);

void					print_visualizer_state_info(t_vm *vm,
							t_state *cur_state);

t_state					*get_start_state(t_vm *vm);

void					visualizer_state_draw_map(t_vm *vm, t_state *cur_state);

void					handle_step_size_controls(t_vm *vm, int key);

int						visualizer_state_handle_controls(t_vm *vm,
							t_state **cur_state);

void					visualize_states(t_vm *vm);

void					visualizer_state_turn_color_on(t_state *cur_state,
							int i);

#endif
