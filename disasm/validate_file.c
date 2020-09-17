/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:37:52 by elindber          #+#    #+#             */
/*   Updated: 2020/09/10 15:49:16 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

int		validate_file(int input, t_command *cmnd, int name_len, int comment_len)
{
	int				i;
	unsigned char	code;

	i = -1;
	while (++i < PROG_NAME_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code) || code == '\n' || code == '\t')
			name_len++;
	}
	i = -1;
	while (++i < COMMENT_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code) || code == '\n' || code == '\t')
			comment_len++;
	}
	i = 0;
	while (read(input, &code, 1))
		i++;
	cmnd->champion_size = i - 4;
	close(input);
	return (name_len <= PROG_NAME_LENGTH
	&& comment_len <= COMMENT_LENGTH && i - 4 <= CHAMP_MAX_SIZE);
}

int		check_header(int fd)
{
	int				i;
	int				magic_header[4];
	unsigned char	byte;

	i = 0;
	magic_header[0] = 0x00;
	magic_header[1] = 0xea;
	magic_header[2] = 0x83;
	magic_header[3] = 0xf3;
	while (i < 4)
	{
		read(fd, &byte, 1);
		if (byte != magic_header[i])
			return (0);
		i++;
	}
	return (1);
}
