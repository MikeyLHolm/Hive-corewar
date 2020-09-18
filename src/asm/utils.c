/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 12:55:39 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/18 15:19:33 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Is char space/tab
*/

int			ft_isspace(int c)
{
	return (c == ' ' || (8 < c && c < 14));
	// return (c == ' ' || c == '\t');
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
**	If line has only spaces/tabs before comment return 1
*/

int			empty_till_comment(char *line)
{
	int			i;

	i = 0;
	while (ft_isspace(line[i]))
		i++;
	if (line[i] == COMMENT_CHAR || line[i] == ALT_COMMENT_CHAR)
		return (1);
	else
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
		if (line_contains_instruction(cur) || empty_till_comment(cur->line))
		{
			if (ft_strchr(cur->line, COMMENT_CHAR))
				*ft_strchr(cur->line, COMMENT_CHAR) = '\0';
			if (ft_strchr(cur->line, ALT_COMMENT_CHAR))
				*ft_strchr(cur->line, ALT_COMMENT_CHAR) = '\0';
		}
		cur = cur->next;
	}
}
