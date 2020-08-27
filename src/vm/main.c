/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/26 13:26:04 by mlindhol          #+#    #+#             */
/*   Updated: 2020/08/27 17:50:12 by sadawi           ###   ########.fr       */
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

	(void)vm;
	if (!(player = (t_player*)ft_memalloc(sizeof(t_player))))
		handle_error("Malloc failed at save_player.");
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

void	test_player(t_vm *vm)
{
	t_player *player;
	int		i;

	player = save_player(vm, "42.cor", NULL);
	ft_printf("PLAYER NAME: %s\n", player->name);
	ft_printf("PLAYER COMMENT: %s\n", player->comment);
	ft_printf("PLAYER SIZE: %d\n", player->size);
	ft_printf("PLAYER CODE: \n", player->code);
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

int			main(int argc, char **argv)
{
	t_vm	*vm;

	if (argc < 2)
		handle_error("./vm [filename.cor] ([filename.cor] [filename.cor])");
	vm = init_vm();
	(void)argv;
	test_player(vm);
	// parse_input(vm, argc, argv);
	//read_input();
	//validate();
	//init_arena();
	//introduce_contestants();
	//fight();
	// if (vm->flags & LEAKS)
	// 	system("leaks corewar");
	return (0);
}
