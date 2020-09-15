/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elindber <elindber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/07 15:15:41 by elindber          #+#    #+#             */
/*   Updated: 2020/09/10 16:41:41 by elindber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disasm.h"

void	write_name(int input, int output)
{
	unsigned char	code;
	int				i;
	int				len;

	i = 0;
	len = 0;
	write(output, ".name \"", 7);
	while (i < PROG_NAME_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code))
		{
			write(output, &code, 1);
			len++;
		}
		i++;
	}
	while (i++ < PROG_NAME_LENGTH + 8)
		read(input, &code, 1);
	write(output, "\"\n", 2);
}

void	write_comment(int input, int output)
{
	unsigned char	code;
	int				i;
	int				len;

	i = 0;
	len = 0;
	write(output, ".comment \"", 10);
	while (i < COMMENT_LENGTH + 4)
	{
		read(input, &code, 1);
		if (ft_isprint((int)code))
		{
			write(output, &code, 1);
			len++;
		}
		i++;
	}
	write(output, "\"\n\n", 3);
	read(input, &code, 4);
}

int		main(int ac, char **av)
{
	int				input;
	int				output;
	char			*name;
	t_command		*cmnd;

	input = open(av[1], O_RDONLY);
	if (!(cmnd = (t_command*)malloc(sizeof(t_command))))
		return (0);
	cmnd->bytes_read = 0;
	if (!(check_header(input)) || !(validate_file(input, cmnd, 0, 0)))
		return (0);
	input = open(av[1], O_RDONLY);
	name = ft_strjoin(av[1], ".EINO");
	output = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	free(name);
	write_name(input, output);
	write_comment(input, output);
	write_instructions(input, output, cmnd);
	close(output);
	close(input);
	return (0);
}
