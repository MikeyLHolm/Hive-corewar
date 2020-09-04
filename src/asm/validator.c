/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/04 17:05:17 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		validation_error(char *message, int row, int col)
{
	ft_printf("Error at [%03d:%03d]: %s.\n", row, col, message);
	exit(1);
}

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
**		double name, length > PROG_NAME_LENGTH, bad characters
**		multi-line name, no quotes
*/

void		validate_name(t_file *cur, t_validator *vd)
{
	if (vd->data & HEADER_NAME)
		validation_error(ft_strjoin("Duplicate ", NAME_CMD_STRING), vd->row, 1);
	ft_printf("LINE:: %s\n", cur->line);
	vd->data = vd->data | HEADER_NAME;

}

/*
**	Comment validation:
**		double comment, length > COMMENT_LENGTH, bad characters
**		multi-line comment, no quotes
*/

void		validate_comment(t_file *cur, t_validator *vd)
{
	if (vd->data & HEADER_COMMENT)
		validation_error(ft_strjoin("Duplicate ", COMMENT_CMD_STRING), vd->row, 1);
	ft_printf("LINE:: %s\n", cur->line);
	vd->data = vd->data | HEADER_COMMENT;

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
		// Check that line starting with . is either .name or .comment
		ft_printf("VAL LINE:: [%s]\n", cur->line);
		// ADD LENGTH / DOUBLE VALIDATION  !!!!
		if (!ft_strncmp(cur->line, ".name ", 6))
			validate_name(cur, vd);
		else if (!ft_strncmp(cur->line, ".comment ", 9))
			validate_comment(cur, vd);
		else if (cur->line[0] == '.')
			validation_error("Header str not .name or .comment", vd->row, 1);
		// Add validation for multi-string name or comment. Opening and closing "

		// check for other whitespace?
		if (cur->line[0] == '\0' && vd->data == 3)
			return (cur);
		else if (cur->line[0] != '\0' && cur->line[0] != '.' &&
				cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
			validation_error("Bad string in header.", vd->row, 1);
		else
		{
			cur = cur->next;
			vd->row++;
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

void		validate_instructions(t_file *cur, t_validator *vd)
{
	while (cur)
	{
		if ()

		else if (cur->line[0] != '\0')
			validate_statement()
		cur = cur->next;
	}
}

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
	cur = cur->next;
	++vd->row;
	ft_putendl("Header validated, moving to instructions!\n");
	ft_printf("Post header LINE:: [%s]\n", cur->line);
	// save labels
	//save_labels(file->label);
	validate_instructions(cur, vd);
	// while (cur)
	// {
	// 	validate_line(cur->line, col);
	// 	col = 0;
	// 	row++;
	// 	cur = cur->next;
	// }
	free(vd);
}
