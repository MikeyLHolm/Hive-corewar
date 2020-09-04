/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validator.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mlindhol <mlindhol@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/04 12:44:33 by mlindhol          #+#    #+#             */
/*   Updated: 2020/09/04 16:37:42 by mlindhol         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

void		validation_error(char *message, int row, int col)
{
	ft_printf("Error at [%03d:%03d]: %s.\n", row, col, message);
	exit(1);
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
	if (vd->data & HEADER_NAME)
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

void		validate_header(t_file *cur, t_validator *vd)
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
			return ;
		else if (cur->line[0] != '\0' && cur->line[0] != '.' &&
				cur->line[0] != ALT_COMMENT_CHAR && cur->line[0] != COMMENT_CHAR)
			validation_error("Wrong character in header.", vd->row, 1);
		else
		{
			cur = cur->next;
			vd->row++;
		}
	}
	validation_error("Header incomplete, no '.name'/'.comment'", vd->row, 1);
	// if starts with . and not ".name" ".comment"

	// if line starts with anything but .#;\n = instruction time

	// exit validate_header after first found nl after both .name & .comment are found.
}

// void		validate_instructions(t_file *cur, t_validator *vd)
// {


	
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

	// validate header
	validate_header(cur, vd);
	++vd->row;
	cur = cur->next;
	ft_putendl("Header validated, moving to instructions!\n");
	// while (cur)
	// {
	// }
	// newline(s) after both name and command have been found.

	// validate instructions
	// while (cur)
	// {
	// 	validate_line(cur->line, col);
	// 	col = 0;
	// 	row++;
	// 	cur = cur->next;
	// }

}
