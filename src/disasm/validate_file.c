/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_file.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/10 15:37:52 by elindber          #+#    #+#             */
/*   Updated: 2020/09/29 19:29:38 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

/*
** Reads PROG_NAME_LENGTH of bytes and 4 bytes that should be nulls. Then reads
** 4 bytes that contain the length of the champion code.
*/

int		validate_name(int input)
{
	int				i;
	int				name_len;
	unsigned char	byte;

	i = -1;
	name_len = 0;
	while (++i < PROG_NAME_LENGTH + 4)
	{
		read(input, &byte, 1);
		if (ft_isprint((int)byte) || byte == '\n' || byte == '\t')
			name_len++;
	}
	while (++i < PROG_NAME_LENGTH + 8)
		read(input, &byte, 1);
	return (name_len);
}

/*
** Reads COMMENT_LENGHT of bytes and 4 bytes that should be nulls. Returns the
** amount of printable characters (and \n \t) within those bytes.
*/

int		validate_comment(int input)
{
	int				i;
	int				comment_len;
	unsigned char	byte;

	i = -1;
	comment_len = 0;
	while (++i < COMMENT_LENGTH + 4)
	{
		read(input, &byte, 1);
		if (ft_isprint((int)byte) || byte == '\n' || byte == '\t')
			comment_len++;
	}
	return (comment_len);
}

/*
** Reads the file except the magic header and returns 1 if the length
** of the name, comment and code of the champion are valid.
*/

int		validate_file(int input, t_command *cmnd)
{
	int				i;
	int				name_len;
	int				comment_len;
	unsigned char	byte;

	i = 0;
	name_len = validate_name(input);
	comment_len = validate_comment(input);
	while (read(input, &byte, 1))
		i++;
	cmnd->champion_size = i - 1;
	close(input);
	return (name_len <= PROG_NAME_LENGTH
	&& comment_len <= COMMENT_LENGTH && i - 1 <= CHAMP_MAX_SIZE);
}

/*
** Checks that the file has the correct magic header and
** the file name extension is .cor.
*/

int		check_header_and_file_name(int fd, char *name)
{
	int				i;
	int				value;
	char			*cmp;
	unsigned char	byte;

	i = 0;
	value = 0;
	read(fd, &byte, 1);
	value += byte * 256 * 256 * 256;
	read(fd, &byte, 1);
	value += byte * 256 * 256;
	read(fd, &byte, 1);
	value += byte * 256;
	read(fd, &byte, 1);
	value += byte;
	cmp = ft_strrchr(name, '.');
	return (value == COREWAR_EXEC_MAGIC && (ft_strequ(cmp, ".cor")));
}
