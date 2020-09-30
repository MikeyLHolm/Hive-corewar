/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 09:21:40 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	handle_error(char *message)
{
	ft_fprintf(2, "%s.\n", message);
	system("leaks corewar");
	exit(1);
}

void		validate_filename(char *filename, char *extension)
{
	if (!(ft_strequ(ft_strrchr(filename, '.'), extension)))
		handle_error("File extension not .cor");
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

void	check_magic_header(int fd)
{
	unsigned char	buf[4];
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

t_player	*save_player(t_vm *vm, char *filename, char *n)
{
	t_player	*player;
	static int	id;

	(void)vm;
	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
	player->id = ++id;
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
	print_player_code(player);
}

// void	manually_create_players(t_vm *vm)
// {
// 	t_player *player;

// 	vm->player_amount = 2;
// 	player = save_player(vm, "test.cor", NULL);
// 	player->next = save_player(vm, "test.cor", NULL);
// 	vm->players = player;
// 	vm->player_last_alive = vm->player_amount;
// }

void	load_players(t_vm *vm)
{
	t_player	*cur_player;
	int			offset;
	int			i;

	i = 0;
	cur_player = vm->players;
	offset = MEM_SIZE / vm->player_amount;
	while (i < vm->player_amount)
	{
		ft_memcpy(&vm->arena[i++ * offset], cur_player->code, cur_player->size);
		cur_player = cur_player->next;
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

void	disable_dead_carriages(t_vm *vm)
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

void	reduce_cycles(t_vm *vm, t_carriage *cur)
{
	(void)vm;
	if (cur->cycles_left)
		cur->cycles_left--;
}

int		skip_direct_bytes(t_carriage *cur)
{
	if (g_op_tab[cur->statement - 1].size_t_dir)
		return (2);
	else
		return (4);
}

void	count_bytes_to_skip(t_carriage *cur)
{
	unsigned char	act;
	int				n;
	int				bit;

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
			cur->bytes_to_jump += skip_direct_bytes(cur);
		else if (!((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 1;
		else if (((act >> bit) & 0x01) && ((act >> (bit - 1)) & 0x01))
			cur->bytes_to_jump += 2;
		bit -= 2;
		n++;
	}
}

void	get_winner(t_vm *vm)
{
	t_player *cur_player;

	cur_player = vm->players;
	while (cur_player)
	{
		if (cur_player->id == vm->player_last_alive)
			break ;
		cur_player = cur_player->next;
	}
	ft_printf("Contestant %d, \"%s\", has won !\n",
		cur_player->id, cur_player->name);
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

void	handle_dump(t_vm *vm)
{
	if (!check_carriages_alive(vm))
		get_winner(vm);
	else
		dump_memory(vm);
	exit(0);
}

void	print_cycle_loading(t_vm *vm)
{
	clear();
	print_if_valid(vm, ft_sprintf("Saving cycle: %d\n", vm->cycles));
	refresh();
	clear();
}

void	handle_cycle_carriages(t_vm *vm)
{
	t_carriage *cur;

	cur = vm->carriages;
	while (cur)
	{
		set_statement_codes(vm, cur);
		reduce_cycles(vm, cur);
		perform_statements(vm, cur);
		cur = cur->next;
	}
}

void	battle_loop(t_vm *vm)
{
	save_state(vm);
	while (1)
	{
		if (vm->flags & VISUALIZER)
			save_visualizer_memory(vm);
		vm->cycles++;
		perform_check(vm);
		if (vm->flags & DUMP && vm->cycles - 1 == vm->dump_cycle)
			handle_dump(vm);
		if (!check_carriages_alive(vm))
			break ;
		if (vm->flags & ADV_VISUALIZER)
			print_cycle_loading(vm);
		handle_cycle_carriages(vm);
		handle_visualization(vm);
	}
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

void	print_usage(void)
{
	ft_printf("Usage: /vm [-a -f -l -v -z -x (-dump n) (-s n) (-n n)] ");
	ft_printf("[filename.cor] ([filename.cor] [filename.cor])\n\n");
	ft_printf("-a: Show advanced visualizer\n");
	ft_printf("-f: Fix lld instruction bug\n");
	ft_printf("-l: Print lives to standard output\n");
	ft_printf("-v: Show visualizer\n");
	ft_printf("-z: Print aff instructions to standard output\n");
	ft_printf("-x: Show leaks after program terminates\n");
	ft_printf("-dump n: Dump memory at cycle n\n");
	ft_printf("-s n: Start advanced visualizer at cycle n\n");
	ft_printf("-n n: Set player number\n");
	exit(0);
}

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		print_usage();
	vm = init_vm();
	(void)argv;
	parse_input(vm, argc, argv);
	init_arena(vm);
	introduce_contestants(vm);
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
