/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/21 11:51:09 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	system("leaks corewar");
	exit(1);
}

void		init_controls(t_vm *vm)
{
	vm->controls.step_size = 1;
}

t_vm		*init_vm(void)
{
	t_vm	*vm;

	if (!(vm = (t_vm*)ft_memalloc(sizeof(t_vm))))
		handle_error("Malloc failed at VM init");
	vm->cycles_to_die = CYCLE_TO_DIE;
	if (!(vm->changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	vm->color_mem = NULL;
	vm->dump_cycle = -1;
	if (!(vm->updated_color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	if (!(vm->updated_changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	if (!(vm->cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	init_controls(vm);
	return (vm);
}

// void		save_flag(t_vm *vm, char flag)
// {
// 	if (flag == 'd')
// 		vm->flags = vm->flags | DUMP;
// 	else if (flag == 'e')
// 		vm->flags = vm->flags | ERROR;
// 	else if (flag == 'l')
// 		vm->flags = vm->flags | LEAKS;
// 	else if (flag == 'n')
// 		vm->flags = vm->flags | N;
// 	else if (flag == 'v')
// 		vm->flags = vm->flags | VISUALIZER;
// }

void	check_magic_header(int fd)
{
	unsigned char			buf[4];
	int				magic_num;

	if (read(fd, buf, 4) != 4)
		handle_error("File ended too soon");
	magic_num = COREWAR_EXEC_MAGIC;
	if (buf[0] != ((unsigned char*)&magic_num)[3] ||
	buf[1] != ((unsigned char*)&magic_num)[2] ||
	buf[2] != ((unsigned char*)&magic_num)[1] ||
	buf[3] != ((unsigned char*)&magic_num)[0])
		handle_error("Magic header is incorrect");
}

char	*get_player_name(int fd)
{
	char buf[PROG_NAME_LENGTH + 4];

	if (read(fd, buf, PROG_NAME_LENGTH + 4) != PROG_NAME_LENGTH + 4)
		handle_error("File ended too soon");
	return (ft_strdup(buf));
}

int		get_player_size(int fd)
{
	unsigned char	buf[4];
	int		size;

	if (read(fd, buf, 4) != 4)
		handle_error("File ended too soon");
	size = buf[3];
	size += buf[2] * 16 * 16;
	size += buf[1] * 16 * 16 * 16 * 16;
	size += buf[0] * 16 * 16 * 16 * 16 * 16 * 16;
	if (size > CHAMP_MAX_SIZE)
		handle_error("Player exceeds maximum champion size");
	return (size);
}

char	*get_player_comment(int fd)
{
	char buf[COMMENT_LENGTH + 4];

	if (read(fd, buf, COMMENT_LENGTH + 4) != COMMENT_LENGTH + 4)
		handle_error("File ended too soon");
	return (ft_strdup(buf));
}

unsigned char	*resize_memory(unsigned char *player_code, int size, int size_to_add)
{
	unsigned char *tmp;

	if (!(tmp = malloc(size + size_to_add)))
		handle_error("Malloc failed");
	ft_memcpy(tmp, player_code, size);
	free(player_code);
	return (tmp);
}

unsigned char	*get_player_code(t_player *player, int fd)
{
	char	buf[BUFFER_SIZE];
	unsigned char	*player_code;
	int		amount_read;
	int		total_read;

	amount_read = 0;
	total_read = 0;
	player_code = NULL;
	while ((amount_read = read(fd, buf, BUFFER_SIZE)) > 0)
	{
		if (!player_code)
		{
			if (!(player_code = malloc(amount_read)))
				handle_error("Malloc failed");
			ft_memcpy(player_code, buf, amount_read);
		}
		else
		{
			player_code = resize_memory(player_code, total_read, amount_read);
			ft_memcpy(&player_code[total_read], buf, amount_read);
		}
		total_read += amount_read;
		if (amount_read != BUFFER_SIZE)
			break ;
	}
	if (total_read != player->size)
		handle_error("Player size does not match code size");
	return (player_code);
}

void	get_player_info(t_player *player)
{
	int fd;

	if (((fd = open(player->filename, O_RDONLY)) == -1))
		handle_error("PLayer file could not be opened");
	check_magic_header(fd);
	player->name = get_player_name(fd);
	player->size = get_player_size(fd);
	player->comment = get_player_comment(fd);
	player->code = get_player_code(player, fd);
	validate_player(player);
}

void		validate_filename(char *filename, char *extension)
{
	if (!(ft_strequ(ft_strrchr(filename, '.'), extension)))
		handle_error("File extension not .cor");
}

t_player	*save_player(t_vm *vm, char *filename, char *n)
{
	t_player	*player;
	static int	id;

	(void)vm;
	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
	player->id = ++id;
	//ft_printf("Name is: [%s]\n", filename);
	validate_filename(filename, ".cor");
	player->filename = filename;
	player->player_nbr = n ? ft_atoi(n) : 0;
	get_player_info(player);
	return (player);
}


void	print_player_code(t_player *player)
{
	int i;

	i = 0;
	while (i < player->size)
	{
		ft_printf("%02x", (unsigned char)player->code[i++]);
		if (!(i % 2))
			ft_printf(" ");
		if (!(i % 8))
			ft_printf("\n");
	}
	ft_printf("\n");
}

void	print_player(t_player *player)
{
	ft_printf("PLAYER ID: %d\n", player->id);
	ft_printf("PLAYER NAME: %s\n", player->name);
	ft_printf("PLAYER COMMENT: %s\n", player->comment);
	ft_printf("PLAYER SIZE: %d\n", player->size);
	// ft_printf("PLAYER CODE: \n", player->code);
	print_player_code(player);
}

void	manually_create_players(t_vm *vm)
{
	t_player *player;

	vm->player_amount = 2;
	player = save_player(vm, "test.cor", NULL);
	player->next = save_player(vm, "test.cor", NULL);
	vm->players = player;
	vm->player_last_alive = vm->player_amount;
}

void	load_players(t_vm *vm)
{
	t_player *cur_player;
	int offset;
	int i;

	i = 0;
	cur_player = vm->players;
	offset = MEM_SIZE / vm->player_amount;
	while (i < vm->player_amount)
	{
		ft_memcpy(&vm->arena[i++ * offset], cur_player->code, cur_player->size);
		cur_player = cur_player->next;
	}
}

t_carriage	*new_carriage(int id, t_carriage *next)
{
	t_carriage *carriage;

	if (!(carriage = (t_carriage*)ft_memalloc(sizeof(t_carriage))))
		handle_error("Malloc failed");
	if (REG_NUMBER)
		carriage->reg[0] = id * -1;
	carriage->id = id;
	carriage->alive = 1;
	carriage->next = next;
	return (carriage);
}

void	init_carriages(t_vm *vm)
{
	t_carriage	*head;
	t_player	*cur_player;

	head = NULL;
	cur_player = vm->players;
	while (cur_player)
	{
		head = new_carriage(cur_player->id, head);
		cur_player = cur_player->next;
	}
	vm->carriages = head;
}

void	set_carriage_positions(t_vm *vm)
{
	t_carriage	*cur_carriage;
	int			offset;
	int			i;

	i = vm->player_amount - 1;
	offset = MEM_SIZE / vm->player_amount;
	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		cur_carriage->position = i-- * offset;
		cur_carriage = cur_carriage->next;
	}
}

void	init_arena(t_vm *vm)
{
	load_players(vm);
	init_carriages(vm);
	set_carriage_positions(vm);
}

void	introduce_contestants(t_vm *vm)
{
	t_player *cur_player;

	cur_player = vm->players;
	ft_printf("Introducing contestants...\n");
	while (cur_player)
	{
		ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\") !\n",
		cur_player->id, cur_player->size,
		cur_player->name, cur_player->comment);
		cur_player = cur_player->next;
	}
}

void	disable_dead_carriages(t_vm *vm) //memory is not freed
{
	t_carriage *cur_carriage;
	t_carriage *prev_carriage;
	t_carriage *next_carriage;

	cur_carriage = vm->carriages;
	prev_carriage = NULL;
	while (cur_carriage)
	{
		next_carriage = cur_carriage->next;
		if (cur_carriage->last_live_cycle < vm->cycles - vm->cycles_to_die)
		{
			cur_carriage->alive = 0;
			if (prev_carriage)
				prev_carriage->next = cur_carriage->next;
			else
				vm->carriages = cur_carriage->next;
			free(cur_carriage);
		}
		else
			prev_carriage = cur_carriage;
		//}
		cur_carriage = next_carriage;
	}
}

void	perform_check(t_vm *vm)
{
	static int cycle = -1;

	cycle++;
	if (cycle >= vm->cycles_to_die)
	{
		disable_dead_carriages(vm);
		if (vm->period_live_statements >= NBR_LIVE)
		{
			vm->cycles_to_die -= CYCLE_DELTA;
			vm->checks_without_change = 0;
		}
		else
		{
			vm->checks_without_change++;
			if (vm->checks_without_change >= MAX_CHECKS)
			{
				vm->cycles_to_die -= CYCLE_DELTA;
				vm->checks_without_change = 0;
			}
		}
		vm->period_live_statements = 0;
		cycle = 0;
	}
}

int		check_carriages_alive(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->alive)
			return (1);
		cur_carriage = cur_carriage->next;
	}
	return (0);
}

void	get_statement(t_vm *vm, t_carriage *cur)
{
	cur->statement = vm->arena[cur->position];
	if (cur->statement > 0 && cur->statement <= OP_CODE_AMOUNT)
		cur->cycles_left = g_op_tab[cur->statement - 1].cycles;
	// if (vm->flags & ADV_VISUALIZER)
	// 	cur->statement_owner = vm->cur_state->cursor_mem[cur->position];
	// if (vm->flags & VISUALIZER)
	// 	cur->statement_owner = vm->cursor_mem[cur->position];
}

void	set_statement_codes(t_vm *vm, t_carriage *cur)
{
	if (!cur->cycles_left)
		get_statement(vm, cur);
}

void	reduce_cycles(t_vm *vm, t_carriage *cur)
{
	(void)vm;
	if (cur->cycles_left)
		cur->cycles_left--;
}

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
	unsigned char	act;
	int				offset;
	int				n;
	int				bit;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	offset = 2;
	n = 0;
	bit = 7;
	while (n < g_op_tab[cur->statement - 1].args_n)
	{
		if (((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
		{
			if (!check_argument_indirect(cur, &offset, n))
				return (0);
		}
		else if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
		{
			if (!check_argument_registry(vm, cur, &offset, n))
				return (0);
		}
		else if (((act >> bit) & 0x01) && !((act >> (bit - 1)) & 0x01))
		{
			if (!check_argument_direct(cur, &offset, n))
				return (0);
		}
		else
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

void	count_bytes_to_skip(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				n;
	int				bit;

	(void)vm;
	act = cur->act;
	n = 0;
	bit = 7;
	if (g_op_tab[cur->statement - 1].args_type_code)
		cur->bytes_to_jump = 2;
	else
	{
		cur->bytes_to_jump = g_op_tab[cur->statement - 1].size_t_dir ? 3 : 5;
		return ;
	}
	while (n < g_op_tab[cur->statement - 1].args_n)
	{
		if (((act >> bit) & 0x01) && !((act >> (bit - 1)) & 0x01))
		{
			if (g_op_tab[cur->statement - 1].size_t_dir)
				cur->bytes_to_jump += 2;
			else
				cur->bytes_to_jump += 4;
		}
		else if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 1;
		else if (((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 2;
		bit -= 2;
		n++;
	}
}

void	move_carriage_next_statement(t_carriage *cur)
{
	if (cur->statement != 9 || !cur->carry)
		cur->position = (cur->position + cur->bytes_to_jump) % MEM_SIZE;
	cur->bytes_to_jump = 0;
}

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

void	handle_statement(t_vm *vm, t_carriage *cur)
{
	if (cur->statement > 0 && cur->statement <= OP_CODE_AMOUNT)
	{
		cur->act = (vm->arena[(cur->position + 1) % MEM_SIZE]); //quick fix for act getting overwritten before bytes to skip is calculated
		if (check_arguments_valid(vm, cur))
			execute_statement(vm, cur);
		count_bytes_to_skip(vm, cur);
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

void	get_winner(t_vm *vm)
{
	t_player *cur_player;

	cur_player = vm->players;
	while (cur_player)
	{
		if (cur_player->id == vm->player_last_alive)
			break;
		cur_player = cur_player->next;
	}
	ft_printf("Contestant %d, \"%s\", has won !\n", cur_player->id, cur_player->name);
}

void	load_player_colors(t_vm *vm, int *cursor_mem)
{
	t_player *cur_player;
	int offset;
	int i;

	i = 0;
	cur_player = vm->players;
	offset = MEM_SIZE / vm->player_amount;
	while (i < vm->player_amount)
	{
		ft_memset(&cursor_mem[i++ * offset], cur_player->id + 4, cur_player->size * 4);
		cur_player = cur_player->next;
	}
}

int		*get_color_mem_old(t_vm *vm, int *prev_mem)
{
	t_carriage	*cur;
	int			*color_mem;
	int			i;

	if (!(color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	if (!prev_mem)
		load_player_colors(vm, color_mem);
	else
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_color_mem[i])
			{
				color_mem[i] = vm->updated_color_mem[i];
			}
			else if (prev_mem[i] > vm->player_amount && prev_mem[i] != 9)
				color_mem[i] = prev_mem[i];
			i++;
		}
	}
	ft_bzero(vm->updated_color_mem, MEM_SIZE * sizeof(int));
	free(prev_mem);
	return (color_mem);
}

int		*get_cursor_mem_old(t_vm *vm, int *prev)
{
	t_carriage	*cur;
	int			*cursor_mem;

	if (!(cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	(void)prev;
	while (cur)
	{
		cursor_mem[cur->position] = 1;
		cur = cur->next;
	}
	free(prev);
	return (cursor_mem);
}

int		*get_color_mem(t_vm *vm, t_state *prev)
{
	t_carriage	*cur;
	int			*color_mem;
	int			i;

	if (!(color_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	if (!prev)
		load_player_colors(vm, color_mem);
	else
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_color_mem[i])
			{
				color_mem[i] = vm->updated_color_mem[i];
			}
			else if (prev->color_mem[i] > vm->player_amount && prev->color_mem[i] != 9)
				color_mem[i] = prev->color_mem[i];
			i++;
		}
	}
	ft_bzero(vm->updated_color_mem, MEM_SIZE * sizeof(int));
	return (color_mem);
}

int		*get_cursor_mem(t_vm *vm, t_state *prev)
{
	t_carriage	*cur;
	int			*cursor_mem;

	if (!(cursor_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	cur = vm->carriages;
	(void)prev;
	// if (!prev)
	// 	load_player_colors(vm, cursor_mem);
	// else
	// {
	// 	i = 0;
	// 	while (i < MEM_SIZE)
	// 	{
	// 		if (prev->cursor_mem[i] > vm->player_amount && prev->cursor_mem[i] != 9)
	// 			cursor_mem[i] = prev->cursor_mem[i];
	// 		else if (prev->cursor_mem[i] < 0)
	// 			cursor_mem[i] = -prev->cursor_mem[i] + 4;
	// 		i++;
	// 	}
	// }
	while (cur)
	{
		cursor_mem[cur->position] = 1;
		// if (cursor_mem[cur->position % MEM_SIZE] < 0 || cursor_mem[cur->position % MEM_SIZE] == 9)
		// 	(void)cursor_mem;
		// else if (cursor_mem[cur->position % MEM_SIZE])
		// 	cursor_mem[cur->position % MEM_SIZE] = (cursor_mem[cur->position % MEM_SIZE] - 4) * -1;
		// else
		// 	cursor_mem[cur->position % MEM_SIZE] = 9;
		cur = cur->next;
	}
	return (cursor_mem);
}

int		ft_abs(int n)
{
	return (n > 0 ? n : -n);
}

void	move_if_valid(t_vm *vm, int y, int x)
{
	int max_x;
	int max_y;

	getmaxyx(stdscr, max_y, max_x);
	//if (y <= max_y && x <= max_x)
	move(y, x);
	vm->cursor_y = y;
	vm->cursor_x = x;
}

void	print_if_valid(t_vm *vm, char *str)
{
	int max_x;
	int max_y;

	getmaxyx(stdscr, max_y, max_x);
	if (vm->cursor_y < max_y && vm->cursor_x < max_x)
	{
		if ((int)ft_strlen(str) > max_x - vm->cursor_x)
			str[max_x - vm->cursor_x] = '\0';
		printw(str);
		vm->cursor_x += ft_strlen(str);
		if (ft_strchr(str, '\n'))
			vm->cursor_x = 0;
	}
	free(str);
}

void	print_player_last_alive(t_vm *vm, int player_num)
{
	t_player *cur;

	move_if_valid(vm, 25, 64 * 3 + 18);
	print_if_valid(vm, ft_sprintf("PLAYER WITH LAST LIVE"));
	cur = vm->players;
	while (cur)
	{
		if (cur->id == player_num)
			break ;
		cur = cur->next;
	}
	move_if_valid(vm, 27, 64 * 3 + 7);
	attron(COLOR_PAIR((unsigned char)cur->id + 4));
	print_if_valid(vm, ft_sprintf("PLAYER %d %34s", player_num, cur->name));
	attroff(COLOR_PAIR((unsigned char)cur->id + 4));
}

void	print_player_lives(t_vm *vm)
{
	t_player	*cur;
	int			offset;

	cur = vm->players;
	move_if_valid(vm, 31, 64 * 3 + 21);
	print_if_valid(vm, ft_sprintf("LAST LIVE CYCLES"));
	offset = 34;
	while (cur)
	{
		move_if_valid(vm, offset, 64 * 3 + 7);
		attron(COLOR_PAIR((unsigned char)cur->id + 4));
		print_if_valid(vm, ft_sprintf("PLAYER %d (%s):", cur->id, cur->name));
		attroff(COLOR_PAIR((unsigned char)cur->id + 4));
		move_if_valid(vm, offset, 64 * 3 + 47);
		print_if_valid(vm, ft_sprintf("%d", cur->last_live_cycle));
		cur = cur->next;
		offset += 2;
	}
}

void	print_state_player_lives(t_vm *vm, t_state *state)
{
	t_player	*cur;
	int			offset;
	int			pointer_offset;

	cur = vm->players;
	move_if_valid(vm, 31, 64 * 3 + 21);
	print_if_valid(vm, ft_sprintf("LAST LIVE CYCLES"));
	offset = 34;
	pointer_offset = 0;
	while (cur)
	{
		move_if_valid(vm, offset, 64 * 3 + 7);
		attron(COLOR_PAIR((unsigned char)cur->id + 4));
		print_if_valid(vm, ft_sprintf("PLAYER %d (%s):", cur->id, cur->name));
		attroff(COLOR_PAIR((unsigned char)cur->id + 4));
		move_if_valid(vm, offset, 64 * 3 + 47);
		print_if_valid(vm, ft_sprintf("%d", *(&state->player1_last_live + pointer_offset)));
		cur = cur->next;
		offset += 2;
		pointer_offset += 1;
	}
}

void	print_border(t_vm *vm)
{
	int i;
	int size;

	i = 0;
	size = 64 * 3;
	print_if_valid(vm, ft_sprintf("\n"));
	while (i < size)
	{
		print_if_valid(vm, ft_sprintf("-"));
		i++;
	}
	print_if_valid(vm, ft_sprintf("\n"));
}

void	print_visualizer_info(t_vm *vm)
{
	int			i;
	t_carriage *cur;

	move_if_valid(vm, 2, 64 * 3 + 20);
	print_if_valid(vm, ft_sprintf("BATTLE INFORMATION"));
	move_if_valid(vm, 6, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLE:"));
	move_if_valid(vm, 6, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->cycles));
	move_if_valid(vm, 8, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLES_TO_DIE:"));
	move_if_valid(vm, 8, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->cycles_to_die));
	move_if_valid(vm, 71, 4);
	i = 0;
	cur = vm->carriages;
	while (cur)
	{
		i++;
		cur = cur->next;
	}
	move_if_valid(vm, 10, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CARRIAGES AMOUNT:"));
	move_if_valid(vm, 10, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", i));
	move_if_valid(vm, 18, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("AUTOPLAY:"));
	move_if_valid(vm, 18, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%s", vm->controls.autoplay ? "ON" : "OFF"));
	move_if_valid(vm, 20, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("STEP SIZE:"));
	move_if_valid(vm, 20, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->controls.step_size));
	print_player_last_alive(vm, vm->player_last_alive);
	print_player_lives(vm);
}

void	print_controls(t_vm *vm)
{
	(void)vm;

	move_if_valid(vm, 46, 64 * 3 + 24);
	print_if_valid(vm, ft_sprintf("CONTROLS:"));
	move_if_valid(vm, 50, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Space:"));
	move_if_valid(vm, 50, 64 * 3 + 35);
	print_if_valid(vm, ft_sprintf("Toggle autoplay"));
	move_if_valid(vm, 52, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Left/Right:"));
	move_if_valid(vm, 52, 64 * 3 + 27);
	print_if_valid(vm, ft_sprintf("Move backwards/forwards"));
	move_if_valid(vm, 54, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Up/Down:"));
	move_if_valid(vm, 54, 64 * 3 + 23);
	print_if_valid(vm, ft_sprintf("Increace/Decrease step size"));
	move_if_valid(vm, 56, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("Q:"));
	move_if_valid(vm, 56, 64 * 3 + 46);
	print_if_valid(vm, ft_sprintf("Quit"));
}

void	draw_borders(t_vm *vm)
{
	int i;
	int size;

	(void)vm;
	attron(COLOR_PAIR(9));
	move_if_valid(vm, 0, 0);
	i = 0;
	size = 64 * 3 + 2 + 50;
	while (i < size + 1)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 64 + 3, 0);
	i = 0;
	while (i < size + 1)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 23, size - 49);
	i = 0;
	while (i < 50)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 29, size - 49);
	i = 0;
	while (i < 50)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 43, size - 49);
	i = 0;
	while (i < 50)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 60, size - 49);
	i = 0;
	while (i < 50)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	i = 0;
	while (i < 68)
	{
		move_if_valid(vm, i++, 0);
		print_if_valid(vm, ft_sprintf(".."));
	}
	i = 0;
	while (i < 68)
	{
		move_if_valid(vm, i++, size + 1);
		print_if_valid(vm, ft_sprintf(".."));
	}
	i = 0;
	while (i < 68)
	{
		move_if_valid(vm, i++, size - 49);
		print_if_valid(vm, ft_sprintf(".."));
	}
	attroff((COLOR_PAIR(9)));
}

void	draw_gui_boxes(t_vm *vm)
{
	int i;
	int size;

	(void)vm;
	attron(COLOR_PAIR(9));
	move_if_valid(vm, 75, 0);
	i = 0;
	size = 64 * 3 + 2;
	while (i < size + 1)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	move_if_valid(vm, 81, 0);
	i = 0;
	while (i < size + 1)
	{
		print_if_valid(vm, ft_sprintf("."));
		i++;
	}
	i = 68;
	while (i < 82)
	{
		move_if_valid(vm, i++, 0);
		print_if_valid(vm, ft_sprintf(".."));
	}
	i = 68;
	while (i < 82)
	{
		move_if_valid(vm, i++, size + 1);
		print_if_valid(vm, ft_sprintf(".."));
	}
	attroff((COLOR_PAIR(9)));
}

void	print_footer(t_vm *vm)
{
	move_if_valid(vm, 62, 64 * 3 + 25);
	print_if_valid(vm, ft_sprintf("Made by: "));
	move_if_valid(vm, 65, 64 * 3 + 15);
	attron(COLOR_PAIR(5));
	print_if_valid(vm, ft_sprintf("Elindber"));
	attroff((COLOR_PAIR(5)));
	print_if_valid(vm, ft_sprintf(", "));
	attron(COLOR_PAIR(6));
	print_if_valid(vm, ft_sprintf("Mlindhol "));
	attroff((COLOR_PAIR(6)));
	print_if_valid(vm, ft_sprintf("and "));
	attron(COLOR_PAIR(7));
	print_if_valid(vm, ft_sprintf("Sadawi"));
	attroff((COLOR_PAIR(7)));
	print_if_valid(vm, ft_sprintf("."));
}

void	visualize(t_vm *vm)
{
	int	*cursor_mem;
	int	key;
	int i;

	while (1)
	{
		erase();

		i = 0;
		cursor_mem = vm->cursor_mem;
		draw_borders(vm);
		int row;
		row = 2;
		move_if_valid(vm, row++, 3);
		while (i < MEM_SIZE)
		{
			if (cursor_mem[i])
				attron(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i] ? vm->color_mem[i] - 4 : 9)));
			else if (vm->color_mem[i])
			{
				attron(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i] + (vm->changed_mem[i] ? 5 : 0))));
			}
			print_if_valid(vm, ft_sprintf("%02x", (unsigned char)vm->arena[i]));
			if (cursor_mem[i])
				attroff(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i] - 4)));
			else
				attroff(COLOR_PAIR((unsigned char)ft_abs(vm->color_mem[i] + (vm->changed_mem[i] ? 5 : 0))));
			print_if_valid(vm, ft_sprintf(" "));
			i++;
			if (!(i % 64))
				move_if_valid(vm, row++, 3); //print_if_valid(vm, ft_sprintf("\n");
		}
		//print_border(vm);
		move_if_valid(vm, 68, 0);
		print_visualizer_info(vm);
		print_controls(vm);
		print_footer(vm);
		//draw_gui_boxes(vm);
		key = getch();
		if (key == ' ')
		{
			vm->controls.autoplay = !vm->controls.autoplay;
			timeout(vm->controls.autoplay ? 1 : -1);
			break;
		}
		if (key == 'q')
		{
			endwin();
			exit(0);
		}
		if (key == KEY_RIGHT)
			break;
		if (vm->controls.autoplay)
			break;
		refresh();
	}
}

void	print_visualizer_state_info(t_vm *vm, t_state *cur_state)
{

	move_if_valid(vm, 2, 64 * 3 + 20);
	print_if_valid(vm, ft_sprintf("BATTLE INFORMATION"));
	move_if_valid(vm, 6, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLE:"));
	move_if_valid(vm, 6, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->cycle));
	move_if_valid(vm, 8, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CYCLES_TO_DIE:"));
	move_if_valid(vm, 8, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->cycles_to_die));
	move_if_valid(vm, 71, 4);
	move_if_valid(vm, 10, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("CARRIAGES AMOUNT:"));
	move_if_valid(vm, 10, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", cur_state->carriage_amount));
	move_if_valid(vm, 18, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("AUTOPLAY:"));
	move_if_valid(vm, 18, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%s", vm->controls.autoplay ? "ON" : "OFF"));
	move_if_valid(vm, 20, 64 * 3 + 7);
	print_if_valid(vm, ft_sprintf("STEP SIZE:"));
	move_if_valid(vm, 20, 64 * 3 + 45);
	print_if_valid(vm, ft_sprintf("%d", vm->controls.step_size));
	print_player_last_alive(vm, cur_state->last_live_player);
	print_state_player_lives(vm, cur_state);
}

void	visualize_states(t_vm *vm)
{
	t_state	*cur_state;
	int		i;
	int		key;

	cur_state = vm->arena_history_head;
	for (int i = 0; i < vm->start; i++)
		cur_state = cur_state->next;
	while (1)
	{
		erase();
		draw_borders(vm);
		int row;
		row = 2;
		move_if_valid(vm, row++, 3);
		i = 0;
		while (i < MEM_SIZE)
		{
			if (cur_state->cursor_mem[i])
				attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i] ? cur_state->color_mem[i] - 4 : 9)));
			else if (cur_state->color_mem[i])
			{
				// if (cur_state->cursor_mem[i] < 0)
				// 	attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->cursor_mem[i])));
				// else
				// 	attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->cursor_mem[i] + (cur_state->changed_mem[i] && cur_state->changed_mem[i] < 50 ? 5 : 0))));
				attron(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i] + (cur_state->changed_mem[i] ? 5 : 0))));
			}
			print_if_valid(vm, ft_sprintf("%02x", (unsigned char)cur_state->arena[i]));
			// if (cur_state->cursor_mem[i] < 0)
			// 	attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->cursor_mem[i])));
			// else
			// 	attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->cursor_mem[i] + (cur_state->changed_mem[i] && cur_state->changed_mem[i] < 50 ? 5 : 0))));
			if (cur_state->cursor_mem[i])
				attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i] - 4)));
			else
				attroff(COLOR_PAIR((unsigned char)ft_abs(cur_state->color_mem[i] + (cur_state->changed_mem[i] ? 5 : 0))));
			print_if_valid(vm, ft_sprintf(" "));
			i++;
			if (!(i % 64))
				move_if_valid(vm, row++, 3);//print_if_valid(vm, ft_sprintf("\n");
		}
		move_if_valid(vm, 68, 0);
		print_visualizer_state_info(vm, cur_state);
		print_controls(vm);
		print_footer(vm);
		//draw_gui_boxes(vm);
		refresh();
		key = getch();
		if (vm->controls.autoplay && key == ERR)
			key = KEY_RIGHT;
		i = 0;
		if (key == KEY_LEFT)
		{
			while (i++ < vm->controls.step_size)
				cur_state = cur_state->prev ? cur_state->prev : cur_state;
		}
		if (key == KEY_RIGHT)
		{
			while (i++ < vm->controls.step_size)
				cur_state = cur_state->next ? cur_state->next : cur_state;
		}
		if (key == KEY_UP)
			vm->controls.step_size++;
		if (key == KEY_DOWN)
			vm->controls.step_size > 1 ? vm->controls.step_size-- : (void)vm;
		if (key == 'q')
			break ;
		if (key == ' ')
		{
			vm->controls.autoplay = !vm->controls.autoplay;
			timeout(vm->controls.autoplay ? 1 : -1);
		}
		if (!cur_state->next)
			vm->controls.autoplay = 0;
	}
}

void	state_get_lives(t_vm *vm, t_state *state)
{
	t_player *cur_player;

	cur_player = vm->players;
	state->player1_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player2_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player3_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->player4_last_live = cur_player ? cur_player->last_live_cycle : 0;
	cur_player = cur_player ? cur_player->next : cur_player;
	state->last_live_player = vm->player_last_alive;
}

t_state	*new_state(t_vm *vm, t_state *prev)
{
	t_state	*state;
	int		i;

	if (!(state = (t_state*)ft_memalloc(sizeof(t_state))))
		handle_error("Malloc failed");
	i = 0;
	while (i < MEM_SIZE)
	{
		state->arena[i] = vm->arena[i];
		i++;
	}
	state->cursor_mem = get_cursor_mem(vm, prev);
	state->color_mem = get_color_mem(vm, prev);
	if (!(state->changed_mem = (int*)ft_memalloc(sizeof(int) * MEM_SIZE)))
		handle_error("Malloc failed");
	if (prev)
	{
		i = 0;
		while (i < MEM_SIZE)
		{
			if (vm->updated_changed_mem[i])
				state->changed_mem[i] = vm->updated_changed_mem[i];
			else
				state->changed_mem[i] = prev->changed_mem[i] - 1 > 0 ? prev->changed_mem[i] - 1 : 0;
			i++;
		}
	}
	ft_bzero(vm->updated_changed_mem, MEM_SIZE * sizeof(int));
	int j = 0;
	for (t_carriage *cur = vm->carriages; cur; cur = cur->next)
	{
		j++;
	}
	state->carriage_amount = j;
	state->cycle = vm->cycles;
	state->cycles_to_die = vm->cycles_to_die;
	state->prev = prev;
	state_get_lives(vm, state);
	return (state);
}

void	save_state(t_vm *vm)
{
	if (!vm->arena_history_head)
	{
		vm->arena_history_head = new_state(vm, NULL);
		vm->cur_state = vm->arena_history_head;
	}
	else
	{
		vm->cur_state->next = new_state(vm, vm->cur_state);
		vm->cur_state = vm->cur_state->next;
	}
}

void	update_changed_memory(t_vm *vm)
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

void	dump_memory(t_vm *vm)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		if (!(i % 64))
			ft_printf("0x%04x : ", i);
		ft_printf("%02x ", (unsigned char)vm->arena[i++]);
		if (!(i % 64))
			ft_printf("\n");
	}
}

void	battle_loop(t_vm *vm)
{
	t_carriage *cur;

	save_state(vm);
	while (1)
	{
		if (vm->flags & VISUALIZER)
		{
			vm->color_mem = get_color_mem_old(vm, vm->color_mem);
			vm->cursor_mem = get_cursor_mem_old(vm, vm->cursor_mem);
			update_changed_memory(vm);
		}
		vm->cycles++;
		perform_check(vm);
		if (vm->flags & DUMP && vm->cycles - 1 == vm->dump_cycle)
		{
			if (!check_carriages_alive(vm))
				get_winner(vm);
			else
				dump_memory(vm);
			exit(0);
		}
		if (!check_carriages_alive(vm))
			break;
		if (vm->flags & ADV_VISUALIZER) //tmp debug
		{
			clear();
			print_if_valid(vm, ft_sprintf("Saving cycle: %d\n", vm->cycles));
			refresh();
			clear();
		}
		cur = vm->carriages;
		while (cur)
		{
			set_statement_codes(vm, cur);
			reduce_cycles(vm, cur);
			perform_statements(vm, cur);
			cur = cur->next;
		}
		if (vm->flags & VISUALIZER)
			visualize(vm);
		if (vm->flags & ADV_VISUALIZER)
			save_state(vm);
	}
	//if (!(vm->flags & DUMP))
		get_winner(vm);
}

void	print_arena(t_vm *vm)
{
	int i;

	i = 0;
	while (i < MEM_SIZE)
	{
		ft_printf("%02x ", (unsigned char)vm->arena[i++]);
		if (!(i % 64))
			ft_printf("\n");
	}
}

void	init_visualizer(t_vm *vm)
{
	(void)vm;
	initscr();
	noecho();
	curs_set(0);
	keypad(stdscr, true);
	if (has_colors())
	{
		use_default_colors();
		start_color();
		init_pair(1, -1, COLOR_BLUE);
		init_pair(2, -1, COLOR_GREEN);
		init_pair(3, -1, COLOR_RED);
		init_pair(4, -1, COLOR_YELLOW);
		init_pair(5, COLOR_BLUE, -1);
		init_pair(6, COLOR_GREEN, -1);
		init_pair(7, COLOR_RED, -1);
		init_pair(8, COLOR_YELLOW, -1);
		init_pair(9, COLOR_WHITE, COLOR_WHITE);
		init_pair(10, COLOR_BLUE, COLOR_WHITE);
		init_pair(11, COLOR_GREEN, COLOR_WHITE);
		init_pair(12, COLOR_RED, COLOR_WHITE);
		init_pair(13, COLOR_YELLOW, COLOR_WHITE);
	}
}

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	(void)argv;
	parse_input(vm, argc, argv);
	//read_input();
	//validate(vm);
	//manually_create_players(vm); //used to create players before argument parsing is functional
	init_arena(vm);
	introduce_contestants(vm);
	//print_arena(vm);
	if (vm->flags & (ADV_VISUALIZER | VISUALIZER))
		init_visualizer(vm);
	battle_loop(vm);
	if (vm->flags & ADV_VISUALIZER)
		visualize_states(vm);
	endwin();
	if (vm->flags & LEAKS)
		system("leaks corewar");
	exit(0);
	return (0);
}
