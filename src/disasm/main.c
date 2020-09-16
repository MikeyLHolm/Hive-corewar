/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 15:15:41 by elindber          #+#    #+#             */
/*   Updated: 2020/09/16 14:55:51 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

void	handle_error(char *msg)
{
	ft_putendl(msg);
	exit(EXIT_FAILURE);
}

void	write_file(int input, int output, t_command *cmnd)
{
	unsigned char	byte;
	int				i;

	i = 0;
	write(output, ".name \"", 7);
	while (i < PROG_NAME_LENGTH + 8)
	{
		read(input, &byte, 1);
		if (ft_isprint((int)byte))
			write(output, &byte, 1);
		i++;
	}
	while (i++ < PROG_NAME_LENGTH + 12)
		read(input, &byte, 1);
	write(output, "\"\n", 2);
	write_comment(input, output);
	write_instructions(input, output, cmnd);
}

void	write_comment(int input, int output)
{
	unsigned char	byte;
	int				i;
	int				len;

	i = 0;
	len = 0;
	write(output, ".comment \"", 10);
	while (i < COMMENT_LENGTH + 4)
	{
		read(input, &byte, 1);
		if (ft_isprint((int)byte))
		{
			write(output, &byte, 1);
			len++;
		}
		i++;
	}
	write(output, "\"\n\n", 3);
}

int		main(int ac, char **av)
{
	int				i;
	int				input;
	int				output;
	char			*name;
	t_command		*cmnd;

	i = 0;
	while (++i < ac)
	{
		input = open(av[i], O_RDONLY);
		if (!(cmnd = (t_command*)malloc(sizeof(t_command))))
			handle_error("Malloc failed.");
		if (!(check_header_and_file_name(input, av[i]))
		|| !(validate_file(input, cmnd, 0, 0)))
			handle_error("Invalid file.");
		input = open(av[i], O_RDONLY);
		*ft_strchr(av[i], '.') = '\0';
		name = ft_strjoin(av[i], ".s");
		output = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		free(name);
		write_file(input, output, cmnd);
		free(cmnd);
	}
	system("leaks disasm");
	return (0);
}
