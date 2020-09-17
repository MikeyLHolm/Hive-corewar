/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:55:39 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 13:15:05 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Is char space/tab
*/

int		ft_isspace(int c)
{
	return (c == ' ' || c == '\t');
}

/*
**	Prints error message before exiting
*/

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}
