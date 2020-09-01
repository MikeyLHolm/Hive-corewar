/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/01 12:11:49 by mlindhol         ###   ########.fr       */
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

void	display_list(t_player *head)
{
	if (head != NULL)
	{
		ft_printf("\nSTATS START:: id = %d || n = %d\n", head->id, head->n);
		display_list(head->next);
	}
}

t_player	*swap_nodes(t_player *cur, t_player *nxt)
{
	t_player	*tmp;

	tmp = nxt->next;
	nxt->next = cur;
	cur->next = tmp;
	return (nxt);
}

void		bubble_sort(t_player **head, int count, int swapped, int i)
{
	int			j;
	t_player	**h;
	t_player	*p1;
	t_player	*p2;

	while (++i < (count - 1))
	{
		h = head;
		swapped = 0;
		j = -1;
		while (++j < (count - i - 1))
		{
			p1 = *h;
			p2 = p1->next;
			if (p2 && p1->id > p2->id)
			{
				*h = swap_nodes(p1, p2);
				swapped = 1;
			}
			h = &(*h)->next;
		}
		if (swapped == 0)
			break;
	}
}

void		check_duplicate_n(t_player *head, t_vm *vm)
{
	t_player	*tmp;

	while (head)
	{
		tmp = head->next;
		while (tmp)
		{
			if (head->n == tmp->n && head->n != 0)
				handle_error("duplicate -n value");
			if (head->n > vm->player_amount || tmp->n > vm->player_amount)
				handle_error("-n value > players_amount");
			tmp = tmp->next;
		}
		head = head->next;
	}
}

void		sort_players(t_vm *vm)
{
	int				is_n;

	t_player		*cur;

	cur = vm->players;
	is_n = 0;
	while (cur)
	{
		ft_printf("\nSTATS START:: id = %d || n = %d\n", cur->id, cur->n);
		if (cur->n != 0)
		{
			cur->id = cur->n;
			is_n = 1;
		}
		ft_printf("STATS END:: id = %d || n = %d\n", cur->id, cur->n);
		cur = cur->next;
		vm->player_amount++;
	}
	ft_printf("\nNR of players: %d\n", vm->player_amount);
	cur = vm->players;
	check_duplicate_n(cur, vm);
	// update id values. ++id if 2 same and n = different
	if ((vm->players && vm->players->next) && is_n != 0)
	{
		// change id values();
		bubble_sort(&vm->players, vm->player_amount, 0, -1);
	}
	display_list(vm->players);
}

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
	//get_player_info(player);
	return (player);
}

void		parse_player(t_vm *vm, char *filename, char *n)
{
	if (!vm->players)
	{
		vm->tail = save_player(vm, filename, n);
		vm->players = vm->tail;
	}
	else
	{
		vm->tail->next = save_player(vm, filename, n);
		vm->tail = vm->tail->next;
	}
	// vm->tail->next = NULL;
}

void		validate_n_flag(char **argv, int i)
{
	if (!(argv[i + 1] && argv[i + 2]))
		handle_error("EOS, check flag syntax!");
	if (ft_atoi(argv[i + 1]) < 1)
		handle_error("-n value < 1. Insert 1 <= x <= players_amount");
}

void		parse_input(t_vm *vm, int argc, char **argv)
{
	int			i;

	i = 0;
	while (++i < argc)
	{
		if (ft_strequ(argv[i], "-dump"))
			vm->flags = vm->flags | DUMP;
		else if (ft_strequ(argv[i], "-e"))
			vm->flags = vm->flags | ERROR;
		else if (ft_strequ(argv[i], "-x"))
			vm->flags = vm->flags | LEAKS;
		else if (ft_strequ(argv[i], "-v"))
			vm->flags = vm->flags | VISUALIZER;
		else if (ft_strequ(argv[i], "-n"))
		{
			vm->flags = vm->flags | N;
			validate_n_flag(argv, i);
			parse_player(vm, argv[i + 2], argv[i + 1]);
			i += 2;
		}
		else
		{
			parse_player(vm, argv[i], 0);
		}
	}
	sort_players(vm);
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
		carriage->reg[0] = id;
	carriage->next = next;
	return (carriage);
}

void	init_carriages(t_vm *vm)
{
	t_carriage *head;
	t_player *cur_player;

	head = NULL;
	cur_player = vm->players;
	while (cur_player)
	{
		head = new_carriage(cur_player->id, head);
		cur_player = cur_player->next;
	}
	vm->carriages = head;
}

void	init_arena(t_vm *vm)
{
	load_players(vm);
	init_carriages(vm);
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

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	(void)argv;
	parse_input(vm, argc, argv);
	//read_input();
	//validate();
	// manually_create_players(vm); //used to create players before argument parsing is functional
	// init_arena(vm);
	// introduce_contestants(vm);
	//fight();
	// if (vm->flags & LEAKS)
	//system("leaks corewar");
	return (0);
}
