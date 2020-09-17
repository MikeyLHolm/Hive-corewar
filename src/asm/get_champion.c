/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_champion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:00:46 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/17 16:16:03 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

static char		*get_champion_name(t_file *cur)
{
	char		*name;

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
	if (ft_strlen(name) > PROG_NAME_LENGTH)
		handle_error("Champion name too long");
	return (name);
}

static char		*get_champion_comment(t_file *cur)
{
	char		*comment;

	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(cur->line, '"') + 1);
	if (!comment)
		handle_error("Champion comment invalid");
	cur = cur->next;
	while (!ft_strchr(comment, '"') && cur)
	{
		comment = ft_strjoinfree(comment, ft_strdup("\n"));
		comment = ft_strjoinfree(comment, ft_strdup(cur->line));
		if (ft_strchr(comment, '"'))
			break ;
		cur = cur->next;
	}
	*ft_strrchr(comment, '"') = '\0';
	if (ft_strlen(comment) > COMMENT_LENGTH)
		handle_error("Champion comment too long");
	return (comment);
}

/*
**	Used in tokenize_file.
*/

void			get_name_and_comment(t_asm *assm)
{
	t_file		*cur;

	cur = assm->file;
	while (cur)
	{
		if (ft_strnequ(cur->line, NAME_CMD_STRING, ft_strlen(NAME_CMD_STRING)))
		{
			if (assm->name)
				handle_error("Name already exists");
			assm->name = get_champion_name(cur);
		}
		if (ft_strnequ(cur->line, COMMENT_CMD_STRING,
			ft_strlen(COMMENT_CMD_STRING)))
		{
			if (assm->comment)
				handle_error("Comment already exists");
			assm->comment = get_champion_comment(cur);
		}
		if (assm->name && assm->comment)
			break ;
		cur = cur->next;
	}
}
