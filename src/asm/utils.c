/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:55:39 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 16:14:22 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Is char space/tab
*/

int			ft_isspace(int c)
{
	return (c == ' ' || c == '\t');
}

/*
**	Prints error message before exiting
*/

void		handle_error(char *message)
{
	ft_printf("%s.\n", message);
	exit(1);
}

/*
**	Boolean to check if line has an instruction
*/

int			line_contains_instruction(t_file *cur)
{
	int			i;

	i = 0;
	if (!cur->line)
		return (0);
	while (cur->line[i])
	{
		if (cur->line[i] == '.' || cur->line[i] == COMMENT_CHAR ||
			cur->line[i] == ALT_COMMENT_CHAR)
			return (0);
		if (!ft_isspace(cur->line[i++]))
			return (1);
	}
	return (0);
}

/*
**	Inserts null to the comment position
*/

void		remove_file_comments(t_file *file)
{
	t_file		*cur;

	cur = file;
	while (cur)
	{
		if (line_contains_instruction(cur))
		{
			if (ft_strchr(cur->line, COMMENT_CHAR))
				*ft_strchr(cur->line, COMMENT_CHAR) = '\0';
			if (ft_strchr(cur->line, ALT_COMMENT_CHAR))
				*ft_strchr(cur->line, ALT_COMMENT_CHAR) = '\0';
		}
		cur = cur->next;
	}
}

int			skip_label(char *line)
{
	int			i;

	i = 0;
	while (ft_strchr(LABEL_CHARS, line[i]))
		i++;
	if (line[i] == LABEL_CHAR)
		i++;
	return (i);
}
