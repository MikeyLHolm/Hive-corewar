/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/08/25 08:53:54 by mlindhol         ###   ########.fr       */
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

t_asm	*init_assm(void)
{
	t_asm *assm;

	if (!(assm = (t_asm*)ft_memalloc(sizeof(t_asm))))
		handle_error("Malloc failed");
	return (assm);
}

char	*get_champion_name(t_file *cur)
{
	char	*name;

	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	if (!name)
		handle_error("Champion name invalid");
	cur = cur->next;
	while (!ft_strchr(name, '"') && cur)
	{
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
		cur = cur->next;
	}
	*ft_strrchr(name, '"') = '\0';
	return (name);
}

char	*get_champion_comment(t_file *cur)
{
	char	*name;

	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	if (!name)
		handle_error("Champion name invalid");
	cur = cur->next;
	while (!ft_strchr(name, '"') && cur)
	{
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
		cur = cur->next;
	}
	*ft_strrchr(name, '"') = '\0';
	return (name);
}

void	tokenize_file(t_asm *assm)
{
	t_file *cur;

	cur = assm->file;
	while (cur)
	{
		if (ft_strnequ(cur->line, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
			assm->name = get_champion_name(cur);
		if (ft_strnequ(cur->line, COMMENT_CMD_STRING, ft_strlen(COMMENT_CMD_STRING)))
			assm->comment = get_champion_comment(cur);
		if (assm->name && assm->comment)
			break ;
		cur = cur->next;
	}
	ft_printf("NAME: %s\nCOMMENT: %s\n\n\n", assm->name, assm->comment);
}

int		main(int argc, char **argv)
{
	t_asm	*assm;

	if (argc != 2)
		handle_error("./asm [filename.s]");
	assm = init_assm();
	assm->file = read_file(argv[1]);
	//check_file(assm->file);
	tokenize_file(assm);
	print_file(assm->file);
	handle_writing(assm->file, argv[1]);
	return (0);
}
