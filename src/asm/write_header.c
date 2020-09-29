/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_header.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 16:39:49 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 16:43:39 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	write_header(int fd)
{
	int		magic_number;
	char	buf[4];
	int		i;

	magic_number = COREWAR_EXEC_MAGIC;
	i = 0;
	while (i < 4)
	{
		buf[i++] = magic_number % 256;
		magic_number /= 256;
	}
	while (--i >= 0)
		write(fd, &buf[i], 1);
}

void	write_name(t_asm *assm, int fd)
{
	char	buf[PROG_NAME_LENGTH];
	size_t	i;

	ft_bzero(buf, PROG_NAME_LENGTH);
	i = 0;
	while (i < ft_strlen(assm->name))
	{
		buf[i] = assm->name[i];
		i++;
	}
	i = 0;
	write(fd, buf, PROG_NAME_LENGTH);
	write(fd, "\0\0\0\0", 4);
}

void	write_comment(t_asm *assm, int fd)
{
	char	buf[COMMENT_LENGTH];
	size_t	i;

	ft_bzero(buf, COMMENT_LENGTH);
	i = 0;
	while (i < ft_strlen(assm->comment))
	{
		buf[i] = assm->comment[i];
		i++;
	}
	i = 0;
	write(fd, buf, COMMENT_LENGTH);
	write(fd, "\0\0\0\0", 4);
}
