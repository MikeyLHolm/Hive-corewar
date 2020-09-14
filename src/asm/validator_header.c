/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_header.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 14:47:54 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/14 15:04:27 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Comment validation:
**		[x] double name
**		[x] length > PROG_NAME_LENGTH
**		[] bad characters
**		[x] multi-line name
**		[] no quotes
**		[] open quote
*/

t_file		*validate_name(t_file *cur, t_validator *vd)
{
	char	*name;

	if (vd->data & HEADER_NAME)
		validation_error(ft_strjoin("Duplicate ", NAME_CMD_STRING), vd->row);
	vd->data = vd->data | HEADER_NAME;
	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!name)
		validation_error("Champion name invalid", vd->row);
	while (!ft_strchr(name, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
	}
	*ft_strrchr(name, '"') = '\0';
	if (ft_strlen(name) > PROG_NAME_LENGTH)
		validation_error("Champion name too long", vd->row);
	free(name);
	return (increment_validator(cur, vd));
}

/*
**	Comment validation:
**		[x] double comment
**		[x] length > COMMENT_LENGTH
**		[] bad characters
**		[] multi-line comment
**		[] no quotes
*/

t_file		*validate_comment(t_file *cur, t_validator *vd)
{
	char		*comment;

	if (vd->data & HEADER_COMMENT)
		validation_error("Duplicate comment", vd->row);
	vd->data = vd->data | HEADER_COMMENT;
	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!comment)
		validation_error("Champion comment invalid", vd->row);
	while (!ft_strchr(comment, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		comment = ft_strjoinfree(comment, ft_strdup("\n"));
		comment = ft_strjoinfree(comment, ft_strdup(cur->line));
		if (ft_strchr(comment, '"'))
			break ;
	}
	*ft_strrchr(comment, '"') = '\0';
	if (ft_strlen(comment) > COMMENT_LENGTH)
		validation_error("Champion comment too long", vd->row);
	free(comment);
	return (increment_validator(cur, vd));
}

/*
**	[] strjoin newline if no closing ".
**	[] if starts with . and not ".name" ".comment"
**	[] if line starts with anything but .#;\n = instruction time
**	[] check for other whitespaces?
**	[] exit validate_header after first found nl after .name & .comment =found.
*/

t_file		*validate_header(t_file *cur, t_validator *vd)
{
	while (cur)
	{
		if (!ft_strncmp(cur->line, ft_strjoin(NAME_CMD_STRING, " "),
			ft_strlen(NAME_CMD_STRING) + 1))
			cur = validate_name(cur, vd);
		else if (!ft_strncmp(cur->line, ft_strjoin(COMMENT_CMD_STRING, " "),
			ft_strlen(COMMENT_CMD_STRING) + 1))
			cur = validate_comment(cur, vd);
		else if (cur->line[0] == '.')
			validation_error("Str not NAME/COMMENT_CMD_STRING", vd->row);
		if (cur->line[0] == '\0' && vd->data == 3)
			return (cur);
		else if (cur->line[0] != '\0' && cur->line[0] != '.' &&
			cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
			validation_error("Bad string in header", vd->row);
		else if (cur->line[0] == '\0' || cur->line[0] == ALT_COMMENT_CHAR ||
			cur->line[0] == COMMENT_CHAR)
			cur = increment_validator(cur, vd);
	}
	validation_error("Header incomplete, no '.name'/'.comment'", vd->row);
	return (cur);
}
