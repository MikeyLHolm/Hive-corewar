/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_players.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 12:52:52 by elindber          #+#    #+#             */
/*   Updated: 2020/09/22 14:23:14 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void	validate_player(t_player *player)
{
	if (ft_strlen(player->name) > PROG_NAME_LENGTH)
		handle_error("Player name over 128 characters");
	else if (ft_strlen(player->comment) > COMMENT_LENGTH)
		handle_error("Player comment over 2048 characters");
}
