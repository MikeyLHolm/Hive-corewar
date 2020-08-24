/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sadawi <sadawi@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/24 19:56:57 by sadawi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void	handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}

t_file	*new_file_link(char *line)
{
	t_file *new;

	if (!(new = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed");
	new->line = line;
	return (new);
}

t_file	*read_file(char *filename)
{
	char	*line;
	t_file	*head;
	t_file	*cur;
	int		fd;

	head = NULL;
	if (((fd = open(filename, O_RDONLY)) == -1))
		handle_error("File could not be opened");
	while (get_next_line(fd, &line) > 0)
	{
		if (!head)
		{
			cur = new_file_link(line);
			head = cur;
		}
		else
		{
			cur->next = new_file_link(line);
			cur = cur->next;
		}
	}
	return (head);
}

void	print_file(t_file *head)
{
	t_file *cur;

	cur = head;
	while (cur)
	{
		ft_printf("%s\n", cur->line);
		cur = cur->next;
	}
}

char	*get_output_filename(char *input_filename)
{
	*ft_strrchr(input_filename, '.') = '\0';
	return (ft_strjoin(input_filename, ".cor"));
}

void	write_header(int fd)
{
	int		magic_number;
	char 	buf[4];
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

void	handle_writing(t_file *file, char *input_filename)
{
	int		fd;
	char	*output_filename;

	(void)file;
	output_filename = get_output_filename(input_filename);
	fd = open(output_filename, O_WRONLY|O_CREAT|O_TRUNC, 0666);
	write_header(fd);
}

int		main(int argc, char **argv)
{
	t_file *file;

	if (argc != 2)
		handle_error("./asm [filename.s]");
	file = read_file(argv[1]);
	print_file(file);
	handle_writing(file, argv[1]);
	return (0);
}
