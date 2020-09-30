/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 15:15:41 by elindber          #+#    #+#             */
/*   Updated: 2020/09/29 19:30:25 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

void	handle_error(char *msg)
{
	ft_putendl(msg);
	exit(EXIT_FAILURE);
}

/*
** Reads the comment from input file and writes it to output file. Skips
** 4 separator nulls after the comment.
*/

void	write_comment(int input, int output)
{
	unsigned char	byte;
	int				i;

	i = 0;
	write(output, ".comment \"", 10);
	while (i < COMMENT_LENGTH + 4)
	{
		read(input, &byte, 1);
		if (byte)
			write(output, &byte, 1);
		i++;
	}
	write(output, "\"\n\n", 3);
}

/*
** Reads the program name and writes it to output file. Skips 4 separator
** nulls and for bytes that include the size of the champion's code.
*/

void	write_file(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				i;

	i = 0;
	write(output, ".name \"", 7);
	while (i < PROG_NAME_LENGTH + 8)
	{
		read(input, &byte, 1);
		if (i > 3 && byte)
			write(output, &byte, 1);
		i++;
	}
	while (i++ < PROG_NAME_LENGTH + 12)
		read(input, &byte, 1);
	write(output, "\"\n", 2);
	write_comment(input, output);
	write_instructions(input, output, cmnd);
}

void	free_memory(char *name, t_command *cmnd)
{
	if (name)
		free(name);
	if (cmnd)
		free(cmnd);
}

int		main(int ac, char **av)
{
	int				i;
	int				input;
	int				output;
	char			*name;
	t_command		*cmnd;

	i = 0;
	if (ac < 2)
		handle_error("USAGE: ./disasm [file.cor]");
	while (++i < ac)
	{
		input = open(av[i], O_RDONLY);
		if (!(cmnd = (t_command*)malloc(sizeof(t_command))))
			handle_error("Malloc failed.");
		if (!(check_header_and_file_name(input, av[i]))
		|| !(validate_file(input, cmnd)))
			handle_error("Invalid file.");
		input = open(av[i], O_RDONLY);
		*ft_strrchr(av[i], '.') = '\0';
		name = ft_strjoin(av[i], ".s");
		output = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		write_file(input, output, cmnd);
		free_memory(name, cmnd);
	}
	return (0);
}
