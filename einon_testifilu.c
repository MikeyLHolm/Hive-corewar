/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   einon_testifilu.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 14:54:04 by elindber          #+#    #+#             */
/*   Updated: 2020/09/09 17:51:01 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <unistd.h>
#include "libft/includes/libft.h"

int		main(void)
{
	char c;

	c = 0x41;
	write(1, &c, 1);
	write(1, "\n", 1);
	write(1, "\"", 1);
	write(1, "\n", 1);
	printf("%lu\n", sizeof(unsigned int));
	return (0);
}