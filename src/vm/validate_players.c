/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_players.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 12:52:52 by elindber          #+#    #+#             */
/*   Updated: 2020/09/15 18:58:35 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

int		validate_arg_values(t_player *player, int statement, int tab, int i)
{
	int				j;

	j = tab;
	if (player->arg_types[i] == REG_CODE)
	{
		if (player->code[tab] < 1 && player->code[tab] > REG_NUMBER)
			handle_error(ft_strjoin(player->name, ": Invalid argument value"));
		tab++;
	}
	else if (player->arg_types[i] == DIR_CODE && statement != ZJMP &&
	statement != LDI && statement != STI && statement != FORK
	&& statement != LFORK && statement != LLDI)
		tab += 4;
	else if (player->arg_types[i] == IND_CODE
	|| player->arg_types[i] == DIR_CODE)
		tab += 2;
	return (tab - j);
}

void	validate_arg_type_code(t_player *player, int statement)
{
	if (player->arg_types[0] == 0 || (player->arg_types[1] == 0 &&
	statement != AFF) || (player->arg_types[2] == 0 && (statement != LD
	&& statement != ST && statement != LLD && statement != AFF)))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if (player->arg_types[0] == REG_CODE
	&& (statement == LD || statement == LLD))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if ((player->arg_types[0] == DIR_CODE || player->arg_types[0] == IND_CODE)
	&& (statement == ST || statement == ADD || statement == SUB
	|| statement == STI || statement == AFF))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if (player->arg_types[1] == REG_CODE && statement == AFF)
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if (player->arg_types[1] == DIR_CODE && (statement == LD || statement == ST
	|| statement == ADD || statement == SUB
	|| statement == LLD || statement == AFF))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if (player->arg_types[1] == IND_CODE && (statement != ST && statement
	!= AND && statement != OR && statement != XOR && statement != STI))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	if (player->arg_types[2] == REG_CODE && (statement == LD || statement == ST
	|| statement == LLD || statement == AFF))
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	else if (player->arg_types[2] == DIR_CODE && statement != STI)
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
	else if (player->arg_types[2] == IND_CODE)
		handle_error(ft_strjoin(player->name, ": Wrong argument type"));
}

void	get_arg_type_code(t_player *player, int tab)
{
	unsigned char	byte;
	int				bit;
	int				i;

	byte = player->code[tab];
	bit = 7;
	i = 0;
	while (i < 3)
	{
		if (((byte >> bit) & 0x01) && ((byte >> (bit - 1)) & 0x01))
			player->arg_types[i] = IND_CODE;
		else if (!((byte >> bit) & 0x01) && ((byte >> (bit - 1)) & 0x01))
			player->arg_types[i] = REG_CODE;
		else if (((byte >> bit) & 0x01) && !((byte >> (bit - 1)) & 0x01))
			player->arg_types[i] = DIR_CODE;
		else if (!((byte >> bit) & 0x01) && !((byte >> (bit - 1)) & 0x01))
			player->arg_types[i] = 0;
		bit -= 2;
		i++;
	}
	//ft_printf("arg types [%d][%d][%d]\n", player->arg_types[0], player->arg_types[1], player->arg_types[2]);
}

int		validate_arguments(t_player *player, int statement, int tab)
{
	int		i;

	i = tab;
	player->arg_types[0] = 0;
	player->arg_types[1] = 0;
	player->arg_types[2] = 0;
	if (statement != LIVE && statement != ZJMP
	&& statement != FORK && statement != LFORK)
	{
		get_arg_type_code(player, tab);
		validate_arg_type_code(player, statement);
		tab++;
	}
	else
	{
		player->arg_types[0] = DIR_CODE;
	}
	tab += validate_arg_values(player, statement, tab, 0);
	tab += validate_arg_values(player, statement, tab, 1);
	tab += validate_arg_values(player, statement, tab, 2);
	return (tab - i);
}

int		validate_instruction(t_player *player, int tab)
{
	int		i;
	int		statement;

	i = tab;
	if (player->code[tab] < 1 || player->code[tab] > OP_CODE_AMOUNT)
		handle_error(ft_strjoin(player->name,
		": invalid statement in player code"));
	statement = player->code[tab++];
	ft_printf("statement %d\n", statement);
	tab += validate_arguments(player, statement, tab);
	return (tab - i);
}

void	validate_code(t_player *player)
{
	int				i;

	i = 0;
	while (i < player->size)
		i += validate_instruction(player, i);
	if (i > CHAMP_MAX_SIZE)
		handle_error(ft_strjoin(player->name, ": code size exceeds the limit"));
}

void	validate_player(t_player *player)
{
	if (ft_strlen(player->name) > PROG_NAME_LENGTH)
		handle_error("Player name over 128 characters");
	else if (ft_strlen(player->comment) > COMMENT_LENGTH)
		handle_error("Player comment over 2048 characters");
	validate_code(player);
}
