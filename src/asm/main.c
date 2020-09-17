/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/24 18:03:47 by sadawi            #+#    #+#             */
/*   Updated: 2020/09/17 14:03:01 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_file		*new_file_link(char *line)
{
	t_file 		*new;

	if (!(new = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed");
	new->line = line;
	return (new);
}

t_file		*read_file(char *filename)
{
	char		*line;
	t_file		*head;
	t_file		*cur;
	int			fd;

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

t_asm		*init_assm(void)
{
	t_asm 		*assm;

	if (!(assm = (t_asm*)ft_memalloc(sizeof(t_asm))))
		handle_error("Malloc failed");
	return (assm);
}

void	convert_argument_label(t_asm *assm, char **arg, int arg_position)
{
	t_token	*cur_token;
	int		label_position;
	char	*converted_arg;

	label_position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (ft_strequ(cur_token->label, ft_strchr(*arg, ':') + 1))
		{
			if ((*arg)[0] == '%')
				converted_arg = ft_strjoinfree(ft_strdup("%"), ft_itoa(label_position - arg_position));
			else
				converted_arg = ft_itoa(label_position - arg_position);
			free(*arg);
			*arg = converted_arg;
			return ;
		}
		label_position += cur_token->size;
		cur_token = cur_token->next;
	}
}

void	convert_labels(t_asm *assm)
{
	t_token *cur_token;
	int		position;

	position = 0;
	cur_token = assm->token;
	while (cur_token)
	{
		if (cur_token->arg1 && ft_strchr(cur_token->arg1, ':'))
			convert_argument_label(assm, &cur_token->arg1, position);
		if (cur_token->arg2 && ft_strchr(cur_token->arg2, ':'))
			convert_argument_label(assm, &cur_token->arg2, position);
		if (cur_token->arg3 && ft_strchr(cur_token->arg3, ':'))
			convert_argument_label(assm, &cur_token->arg3, position);
		position += cur_token->size;
		cur_token = cur_token->next;
	}
}

int		main(int argc, char **argv)
{
	t_asm	*assm;

	if (argc != 2)
		handle_error("./asm [filename.s]");
	assm = init_assm();
	assm->file = read_file(argv[1]);
	validator(assm->file);
	tokenize_file(assm);
	convert_labels(assm);
	handle_writing(assm, argv[1]);
	free_assm(assm);
	exit(0);
	return (0);
}
