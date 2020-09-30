/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:19:58 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 13:00:11 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

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

/*
**	Debug tools, currently not in use.
*/

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

void	manually_create_players(t_vm *vm)
{
	t_player *player;

	vm->player_amount = 2;
	player = save_player(vm, "test.cor", NULL);
	player->next = save_player(vm, "test.cor", NULL);
	vm->players = player;
	vm->player_last_alive = vm->player_amount;
}
