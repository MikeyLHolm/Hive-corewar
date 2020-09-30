/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vm_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/30 10:26:43 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 10:46:27 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vm.h"

void		handle_error(char *message)
{
	ft_fprintf(2, "%s.\n", message);
	//system("leaks corewar");
	exit(1);
}

void		validate_filename(char *filename, char *extension)
{
	if (!(ft_strequ(ft_strrchr(filename, '.'), extension)))
		handle_error("File extension not .cor");
}

void		check_magic_header(int fd)
{
	unsigned char	buf[4];
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
