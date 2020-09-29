/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_file.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 16:20:34 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 10:29:54 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Creates new nodes for read lines
*/

static t_file		*new_file_link(char *line)
{
	t_file			*new;

	if (!(new = (t_file*)ft_memalloc(sizeof(t_file))))
		handle_error("Malloc failed");
	new->line = line;
	return (new);
}

/*
**	Read .s file and insert each line into a LL as a node
*/

t_file				*read_file(char *filename)
{
	char			*line;
	t_file			*head;
	t_file			*cur;
	int				fd;

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
