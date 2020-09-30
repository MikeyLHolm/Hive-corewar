/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator_header.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/14 14:47:54 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/30 12:57:02 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

/*
**	Return 1 if there is only whitespace and '\0' in the line. AKA empty line
*/

static int			empty_line(char *line)
{
	int				i;

	i = 0;
	if (!line)
		return (1);
	while (ft_isspace(line[i]))
		i++;
	if (line[i])
		return (0);
	else
		return (1);
}

/*
**	Validates input from line[0] until the first "
*/

static int			validate_to_quote(char *line, char *str)
{
	int				i;

	i = 0;
	while (line[i] == str[i])
		i++;
	while (ft_isspace(line[i]))
		i++;
	if (line[i] == '"')
		return (1);
	else
		return (0);
}

/*
**	Name validation
*/

static t_file		*validate_name(t_file *cur, t_validator *vd)
{
	char			*name;

	if (vd->data & HEADER_NAME)
		validation_error("Duplicate name", vd->row);
	vd->data = vd->data | HEADER_NAME;
	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	while (!ft_strchr(name, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		if (!cur)
			validation_error("EOF, closing quote not found", vd->row);
		name = ft_strjoinfree(name, ft_strdup("\n"));
		name = ft_strjoinfree(name, ft_strdup(cur->line));
		if (ft_strchr(name, '"'))
			break ;
	}
	if (!(*(ft_strrchr(name, '"') + 1) == '\0'))
		validation_error("Extra chars after name", vd->row);
	if (ft_strlen(name) > PROG_NAME_LENGTH)
		validation_error("Champion name too long", vd->row);
	free(name);
	return (increment_validator(cur, vd));
}

/*
**	Comment validation
*/

static t_file		*validate_comment(t_file *cur, t_validator *vd)
{
	char			*comment;

	if (vd->data & HEADER_COMMENT)
		validation_error("Duplicate comment", vd->row);
	vd->data = vd->data | HEADER_COMMENT;
	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(cur->line, '"') + 1);
	while (!ft_strchr(comment, '"') && cur)
	{
		cur = increment_validator(cur, vd);
		if (!cur)
			validation_error("EOF, closing quote not found", vd->row);
		comment = ft_strjoinfree(comment, ft_strdup("\n"));
		comment = ft_strjoinfree(comment, ft_strdup(cur->line));
		if (ft_strchr(comment, '"'))
			break ;
	}
	if (!(*(ft_strrchr(comment, '"') + 1) == '\0'))
		validation_error("Extra chars after comment", vd->row);
	if (ft_strlen(comment) > COMMENT_LENGTH)
		validation_error("Champion comment too long", vd->row);
	free(comment);
	return (increment_validator(cur, vd));
}

/*
**	Main validation tool for header. Moves forward when
**	both name and comment have been found.
*/

t_file				*validate_header(t_file *cur, t_validator *vd)
{
	while (cur)
	{
		if (validate_to_quote(cur->line, NAME_CMD_STRING))
			cur = validate_name(cur, vd);
		else if (validate_to_quote(cur->line, COMMENT_CMD_STRING))
			cur = validate_comment(cur, vd);
		else if (cur->line[0] == '.')
			validation_error("Str not NAME/COMMENT_CMD_STRING", vd->row);
		if (!cur)
			validation_error("Header incomplete, !cur", vd->row);
		if (vd->data == 3 && empty_line(cur->line))
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
