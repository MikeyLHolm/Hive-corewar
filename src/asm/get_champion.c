/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_champion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/17 14:00:46 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/29 10:39:12 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Getter for champion name
*/

static char		*get_champion_name(t_asm *assm)
{
	char		*name;

	name = NULL;
	name = ft_strjoin(name, ft_strchr(assm->cur->line, '"') + 1);
	// if (!name)
	// 	handle_error("Champion name invalid");
	assm->cur = assm->cur->next;
	while (!ft_strchr(name, '"') && assm->cur)
	{
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(assm->cur->line));
		assm->cur = assm->cur->next;
		if (ft_strchr(name, '"'))
			break ;
	}
	*ft_strrchr(name, '"') = '\0';
	// if (ft_strlen(name) > PROG_NAME_LENGTH)
	// 	handle_error("Champion name too long");
	return (name);
}

/*
**	Getter for champion comment
*/

static char		*get_champion_comment(t_asm *assm)
{
	char		*comment;

	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(assm->cur->line, '"') + 1);
	// if (!comment)
	// 	handle_error("Champion comment invalid");
	assm->cur = assm->cur->next;
	while (!ft_strchr(comment, '"') && assm->cur)
	{
		comment = ft_strjoinfree(comment, ft_strdup("\n"));
		comment = ft_strjoinfree(comment, ft_strdup(assm->cur->line));
		assm->cur = assm->cur->next;
		if (ft_strchr(comment, '"'))
			break ;
	}
	*ft_strrchr(comment, '"') = '\0';
	// if (ft_strlen(comment) > COMMENT_LENGTH)
	// 	handle_error("Champion comment too long");
	return (comment);
}

/*
**	Used in tokenize_file. Saves champion name and comment to main struct
*/

void			get_name_and_comment(t_asm *assm)
{
	while (assm->cur)
	{
		if (assm->name && assm->comment)
			break ;
		if (ft_strnequ(assm->cur->line, NAME_CMD_STRING,
			ft_strlen(NAME_CMD_STRING)))
		{
			// if (assm->name)
			// 	handle_error("Name already exists");
			assm->name = get_champion_name(assm);
		}
		else if (ft_strnequ(assm->cur->line, COMMENT_CMD_STRING,
			ft_strlen(COMMENT_CMD_STRING)))
		{
			// if (assm->comment)
			// 	handle_error("Comment already exists");
			assm->comment = get_champion_comment(assm);
		}
		else
			assm->cur = assm->cur->next;
	}
}
