/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/01 15:09:51 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}

t_vm		*init_vm(void)
{
	t_vm	*vm;

	if (!(vm = (t_vm*)ft_memalloc(sizeof(t_vm))))
		handle_error("Malloc failed at VM init");
	vm->cycles_to_die = CYCLE_TO_DIE;
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
/*
void		sort_players(t_player *players)
{
	while (players)
	{
		ft_printf("id = %d || n = %d\n", players->id, players->n);
		players = players->next;
	}
}

*/

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
}

t_player	*save_player(t_vm *vm, char *filename, char *n)
{
	t_player	*player;
	static int	id;

	(void)vm;
	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
	player->id = ++id;
	player->filename = filename;
	player->n = n ? ft_atoi(n) : 0;
	get_player_info(player);
	return (player);
}

/*
void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;
	t_player	*players;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
			vm->flags = vm->flags | DUMP;
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;
			if (argv[i + 1] && argv[i + 2])
				players = save_player(vm, argv[i + 2], argv[i + 1]);
			else
				handle_error("-n flag in wrong place");
			i += 2;
		}
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else
			players = save_player(vm, argv[i], NULL);
		players = players->next;
	}
	players = NULL;
	sort_players(players);
}
*/

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

	i = 0;
	offset = MEM_SIZE / vm->player_amount;
	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		cur_carriage->position = i++ * offset;
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
	ft_printf("Introducing contestants ...\n");
	while (cur_player)
	{
		ft_printf("* Player %d, weighing %d bytes, \"%s\" (\"%s\")!\n",
		cur_player->id, cur_player->size,
		cur_player->name, cur_player->comment);
		cur_player = cur_player->next;
	}
}

void	disable_dead_carriages(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->last_live_cycle <= vm->cycles - vm->cycles_to_die)
			cur_carriage->alive = 0;
		cur_carriage = cur_carriage->next;
	}
}

void	perform_check(t_vm *vm)
{
	static int cycle;

	if (cycle++ >= vm->cycles_to_die)
	{
		disable_dead_carriages(vm);
		if (vm->period_live_statements >= NBR_LIVE)
			vm->cycles_to_die -= CYCLE_DELTA;
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
	if (cur->statement > 0 && cur->statement < OP_CODE_AMOUNT)
		cur->cycles_left = g_op_tab[cur->statement + 1].cycles;
}

void	set_statement_codes(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (!cur_carriage->cycles_left)
			get_statement(vm, cur_carriage);
		cur_carriage = cur_carriage->next;
	}
}

void	reduce_cycles(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->cycles_left)
			cur_carriage->cycles_left--;
		cur_carriage = cur_carriage->next;
	}
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
			if (!check_argument_indirect(cur, &offset, n))
				return (0);
		if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			if (!check_argument_registry(vm, cur, &offset, n))
				return (0);
		if (((act >> bit) & 0x01) && !((act >> (bit - 1)) & 0x01))
			if (!check_argument_direct(cur, &offset, n))
				return (0);
		bit -= 2;
		n++;
	}
	cur->bytes_to_jump = offset - 1;
	return (1);
}

int		check_arguments_valid(t_vm *vm, t_carriage *cur)
{
	if (g_op_tab[cur->statement - 1].args_type_code)
		if (!check_argument_type_code(vm, cur))
			return (0);
	cur->bytes_to_jump += 1;
	return (1);
}

void	count_bytes_to_skip(t_vm *vm, t_carriage *cur)
{
	unsigned char	act;
	int				n;
	int				bit;

	act = (vm->arena[(cur->position + 1) % MEM_SIZE]);
	n = 0;
	bit = 7;
	cur->bytes_to_jump = 1;
	while (n < g_op_tab[cur->statement - 1].args_n)
	{
		if (((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
		{
			if (g_op_tab[cur->statement - 1].size_t_dir)
				cur->bytes_to_jump += 2;
			else
				cur->bytes_to_jump += 4;
		}
		else if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 1;
		else if (((act >> bit) & 0x01) && !((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 2;
		bit -= 2;
		n++;
	}
}

void	move_carriage_next_statement(t_carriage *cur)
{
	cur->position = (cur->position + cur->bytes_to_jump) % MEM_SIZE;
	cur->bytes_to_jump = 0;
}

void	execute_statement(t_vm *vm, t_carriage *cur)
{
	cur->statement == 1 ? op_live(vm, cur) : 0;
	cur->statement == 2 ? op_ld(vm, cur) : 0;
	cur->statement == 3 ? op_st(vm, cur) : 0;
	//cur->statement == 4 ? op_add(vm, cur) : 0;
	//cur->statement == 5 ? op_sub(vm, cur) : 0;
	cur->statement == 6 ? op_and(vm, cur) : 0;
	//cur->statement == 7 ? op_or(vm, cur) : 0;
	//cur->statement == 8 ? op_xor(vm, cur) : 0;
	cur->statement == 9 ? op_zjmp(vm, cur) : 0;
	cur->statement == 10 ? op_ldi(vm, cur) : 0;
	cur->statement == 11 ? op_sti(vm, cur) : 0;
	//cur->statement == 12 ? op_fork(vm, cur) : 0;
	cur->statement == 13 ? op_lld(vm, cur) : 0;
	cur->statement == 14 ? op_lldi(vm, cur) : 0;
	//cur->statement == 15 ? op_lfork(vm, cur) : 0;
	//cur->statement == 16 ? op_aff(vm, cur) : 0;
}

void	handle_statement(t_vm *vm, t_carriage *cur)
{
	if (cur->statement > 0 && cur->statement < OP_CODE_AMOUNT)
	{
		if (check_arguments_valid(vm, cur))
			execute_statement(vm, cur);
		else
			count_bytes_to_skip(vm, cur);
		move_carriage_next_statement(cur);
	}
	else
		cur->position = (cur->position + 1) % MEM_SIZE;
}

void	perform_statements(t_vm *vm)
{
	t_carriage *cur_carriage;

	cur_carriage = vm->carriages;
	while (cur_carriage)
	{
		if (cur_carriage->alive)
		{
			if (!cur_carriage->cycles_left)
				handle_statement(vm, cur_carriage);
		}
		cur_carriage = cur_carriage->next;
	}
}

void	battle_loop(t_vm *vm)
{
	while (1)
	{
		perform_check(vm);
		if (!check_carriages_alive(vm))
			break;
		//ft_printf("CYCLE: %d\n", vm->cycles);
		set_statement_codes(vm);
		reduce_cycles(vm);
		perform_statements(vm);
		vm->cycles++;
	}
	//get_winner(vm);
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
	ft_printf("\n");
}

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	(void)argv;
	// parse_input(vm, argc, argv);
	//read_input();
	//validate();
	manually_create_players(vm); //used to create players before argument parsing is functional
	init_arena(vm);
	introduce_contestants(vm);
	//print_arena(vm);
	battle_loop(vm);
	// if (vm->flags & LEAKS)
	//system("leaks corewar");
	return (0);
}
