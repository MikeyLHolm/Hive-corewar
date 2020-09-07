/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/07 17:17:12 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

t_label		*new_label(char *labelname)
{
	t_label 	*new;

	if (!(new = (t_label*)ft_memalloc(sizeof(t_label))))
		handle_error("Malloc failed while creating new label");
	new->label_name = labelname;
	return (new);
}

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
		validation_error(ft_strjoin("Duplicate ", NAME_CMD_STRING), vd->row, 1);
	vd->data = vd->data | HEADER_NAME;
	name = NULL;
	name = ft_strjoin(name, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!name)
		validation_error("Champion name invalid", vd->row, 1);
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
		validation_error("Champion name too long", vd->row, 1);
	free(name);
	return (increment_validator(cur, vd));
}

/*
**	Comment validation:
**		double comment, length > COMMENT_LENGTH, bad characters
**		multi-line comment, no quotes
*/

t_file		*validate_comment(t_file *cur, t_validator *vd)
{
	char		*comment;

	if (vd->data & HEADER_COMMENT)
		validation_error(ft_strjoin("Duplicate ", COMMENT_CMD_STRING), vd->row, 1);
	vd->data = vd->data | HEADER_COMMENT;
	comment = NULL;
	comment = ft_strjoin(comment, ft_strchr(cur->line, '"') + 1);
	// is condition below needed?
	if (!comment)
		validation_error("Champion comment invalid", vd->row, 1);
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
		validation_error("Champion comment too long", vd->row, 1);
	free(comment);
	return (increment_validator(cur, vd));
}

/*
**	strjoin newline if no closing ".
**	Check up to 128/2048 chars.
**
**	change to COMMENT_CMD_STRING + " " instead ".comment "
*/

t_file		*validate_header(t_file *cur, t_validator *vd)
{

	while (cur)
	{
		//if (!ft_strncmp(cur->line, ".name ", 6))
		if (!ft_strncmp(cur->line, ft_strjoin(NAME_CMD_STRING, " "), ft_strlen(NAME_CMD_STRING) + 1))
			cur = validate_name(cur, vd);
		else if (!ft_strncmp(cur->line, ft_strjoin(COMMENT_CMD_STRING, " "), ft_strlen(COMMENT_CMD_STRING) + 1))
			cur = validate_comment(cur, vd);
		else if (cur->line[0] == '.')
			validation_error("Header str not NAME/COMMENT_CMD_STRING", vd->row, 1);
		//ft_printf("VAL LINE super:: [%s]\n", cur->line);
		// check for other whitespace?
		if (cur->line[0] == '\0' && vd->data == 3)
			return (cur);
		else if (cur->line[0] != '\0' && cur->line[0] != '.' &&
				cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
			validation_error("Bad string in header", vd->row, 1);
		else if (cur->line[0] == '\0' || cur->line[0] == ALT_COMMENT_CHAR || cur->line[0] == COMMENT_CHAR)
		{
			cur = increment_validator(cur, vd);
		}
	}
	validation_error("Header incomplete, no '.name'/'.comment'", vd->row, 1);
	return (cur);
	// if starts with . and not ".name" ".comment"

	// if line starts with anything but .#;\n = instruction time

	// exit validate_header after first found nl after both .name & .comment are found.
}

/*
**	Save labels to verify right label is called?
**	Parse all labels first to be able to validate whole statements?
*/

// void		validate_statement()
// {


// }

// void		validate_instructions(t_file *cur, t_validator *vd)
// {
// 	while (cur)
// 	{
// 		if ()

// 		else if (cur->line[0] != '\0')
// 			validate_statement()
// 		cur = cur->next;
// 	}
// }

void		validator(t_file *file)
{
	t_file			*cur;
	t_validator		*vd;

	if (!(vd = (t_validator*)ft_memalloc(sizeof(t_validator))))
		handle_error("Malloc failed");
	vd->col = 1;
	vd->row = 1;
	cur = file;
	cur = validate_header(cur, vd);
	cur = increment_validator(cur, vd);
	ft_putendl("Header validated, moving to instructions!\n");
	ft_printf("Post header LINE:: [%s]\n", cur->line);
	// save labels
	//save_labels(file->label);
	//validate_instructions(cur, vd);
	free(vd);
}
